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

    module: cosa_routing_dml.c

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
#include "dml_tr181_custom_cfg.h"

#ifdef   CONFIG_CISCO_TRUE_STATIC_IP

#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_truestaticip_dml.h"
#include "cosa_x_cisco_com_truestaticip_apis.h"
#include "cosa_x_cisco_com_truestaticip_internal.h"
#include "ansc_string_util.h"

BOOL
TrueStaticIP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_CFG              pTSIP     = (PCOSA_DML_TSIP_CFG  )&pMyObject->TSIPCfg;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pTSIP->Enabled;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ConfigApply", TRUE))
    {
        /* This parameter always returns FALSE */
        *pBool = FALSE;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

ULONG
TrueStaticIP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_CFG              pTSIP     = (PCOSA_DML_TSIP_CFG  )&pMyObject->TSIPCfg;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "IPAddress", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pTSIP->IPAddress);
        return 0;
    }

    if( AnscEqualString(ParamName, "SubnetMask", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pTSIP->SubnetMask);
        return 0;
    }

    if( AnscEqualString(ParamName, "GatewayIPAddress", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pTSIP->GatewayIPAddress);
        return 0;
    }

    if( AnscEqualString(ParamName, "ConfigEncryptKey", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pTSIP->ConfigEncryptKey);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

BOOL
TrueStaticIP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_CFG              pTSIP     = (PCOSA_DML_TSIP_CFG  )&pMyObject->TSIPCfg;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pTSIP->Enabled        = bValue;
        pTSIP->bIPInfoChanged = TRUE;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ConfigApply", TRUE))
    {
        /* save update to backup */
        pTSIP->ConfigApply = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
TrueStaticIP_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_CFG              pTSIP     = (PCOSA_DML_TSIP_CFG  )&pMyObject->TSIPCfg;
    unsigned int mask = 0;

    /* check if pString doesn't hold null or whitespaces */
    if(AnscValidStringCheck(pString) != TRUE)
        return FALSE;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "IPAddress", TRUE))
    {
        if(is_IpAddress(pString))
        {
            /* save update to backup */
            AnscCopyString(pTSIP->IPAddress, pString);
            pTSIP->bIPInfoChanged = TRUE;
            return TRUE;
        }
    }

    if( AnscEqualString(ParamName, "SubnetMask", TRUE))
    {
        /* In binary representation, the value should be consecutive number of 1's and 0's
           eg: 255.255.255.224*/
        mask = ntohl(inet_addr(pString));
        if(0==(mask & (~mask >> 1)))
        {
            /* save update to backup */
            AnscCopyString(pTSIP->SubnetMask, pString);
            pTSIP->bIPInfoChanged = TRUE;
            return TRUE;
        }
    }

    if( AnscEqualString(ParamName, "GatewayIPAddress", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pTSIP->GatewayIPAddress, pString);
        pTSIP->bIPInfoChanged = TRUE;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ConfigEncryptKey", TRUE))
    {
        /* save update to backup */
         AnscCopyString(pTSIP->ConfigEncryptKey, pString);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

static
BOOL
IsValidHexString
    (
        PCHAR                       pValue
    )
{
    ULONG                           length   = 0;
    ULONG                           i        = 0;
    char                            c        = 0;

    if( pValue == NULL)
    {
        return TRUE; /* empty string is fine */
    }

    length = AnscSizeOfString(pValue);

    if( length % 2 != 0)
    {
        return FALSE;
    }

    for( i = 0; i < length ; i ++)
    {
        c = pValue[i];

            if ( AnscIsHexAlphaOrDigit(c) )
        {
            continue;
        }

        return FALSE;
    }

    return TRUE;
}

BOOL
TrueStaticIP_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_CFG              pTSIP     = (PCOSA_DML_TSIP_CFG  )&pMyObject->TSIPCfg;

    if ( 
           pTSIP->ConfigEncryptKey[0] && 
           (AnscSizeOfString(pTSIP->ConfigEncryptKey) < KEY_LENGTH_64BIT ||
           !IsValidHexString(pTSIP->ConfigEncryptKey))
       )
    {
        return FALSE;
    }

    return TRUE;
}

ULONG
TrueStaticIP_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_CFG              pTSIP     = (PCOSA_DML_TSIP_CFG  )&pMyObject->TSIPCfg;

    if ( ANSC_STATUS_SUCCESS != CosaDmlTSIPSetCfg((ANSC_HANDLE)pMyObject, pTSIP) )
    {
        TrueStaticIP_Rollback(NULL);
    }

    if ( pTSIP->ConfigApply == TRUE )
    {
        /* Apply True Static IP config file */
        CosaDmlTSIPApplyConfigFile((ANSC_HANDLE)pMyObject);
        pTSIP->ConfigApply = FALSE;
    }

    return 0;
}

ULONG
TrueStaticIP_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{   
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_CFG              pTSIP     = (PCOSA_DML_TSIP_CFG  )&pMyObject->TSIPCfg;

    CosaDmlTSIPGetCfg(NULL, pTSIP);

    return 0; 
}

/* Beautiful cut off */

ULONG
Subnet_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    
    return AnscSListQueryDepth(&pMyObject->SubnetList);
}

ANSC_HANDLE
Subnet_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject    = (PCOSA_DATAMODEL_TSIP       )g_pCosaBEManager->hTSIP;
    PSLIST_HEADER                   pSubnetHead  = (PSLIST_HEADER              )&pMyObject->SubnetList;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pEntry       = (PCOSA_DML_TSIP_SUBNET_ENTRY)NULL;
    PSINGLE_LINK_ENTRY              pLink        = (PSINGLE_LINK_ENTRY         )NULL;

    pLink = AnscSListGetEntryByIndex(pSubnetHead, nIndex);

    if ( pLink )
    {
        pEntry = ACCESS_DMSB_TR181_TSIP_ASN(pLink);
        *pInsNumber = pEntry->InstanceNumber;
    }
    
    return pEntry;
}

ANSC_HANDLE
Subnet_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP       )g_pCosaBEManager->hTSIP;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER              )&pMyObject->SubnetList;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pEntry                  = (PCOSA_DML_TSIP_SUBNET_ENTRY)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY         )NULL;

    pEntry = (PCOSA_DML_TSIP_SUBNET_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_TSIP_SUBNET_ENTRY));
    if (!pEntry)
    {
        return NULL;
    }

    _ansc_sprintf(pEntry->Alias, "Subnet%d", pMyObject->ulSubnetNextInsNum);

    *pInsNumber = pEntry->InstanceNumber = pMyObject->ulSubnetNextInsNum;

    pMyObject->ulSubnetNextInsNum++;

    if ( pMyObject->ulSubnetNextInsNum == 0 )
    {
        pMyObject->ulSubnetNextInsNum = 1;
    }

    AnscSListPushEntryAtBack(&pMyObject->SubnetList, &pEntry->Linkage);

    return pEntry;
}

ULONG
Subnet_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP       )g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pEntry                  = (PCOSA_DML_TSIP_SUBNET_ENTRY)hInstance;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER              )&pMyObject->SubnetList;

    CosaDmlTSIPSubnetDelEntry(pMyObject, pEntry);

    AnscSListPopEntryByLink(pListHead, &pEntry->Linkage);
    AnscFreeMemory(pEntry);
    
    return ANSC_STATUS_SUCCESS;
}

BOOL
Subnet_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnet      = (PCOSA_DML_TSIP_SUBNET_ENTRY)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pSubnet->Enabled;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

ULONG
Subnet_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnet      = (PCOSA_DML_TSIP_SUBNET_ENTRY)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "IPAddress", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pSubnet->IPAddress);
        return 0;
    }

    if( AnscEqualString(ParamName, "SubnetMask", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pSubnet->SubnetMask);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

BOOL
Subnet_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnet      = (PCOSA_DML_TSIP_SUBNET_ENTRY)hInsContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pSubnet->Enabled        = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
Subnet_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnet      = (PCOSA_DML_TSIP_SUBNET_ENTRY)hInsContext;
    unsigned int mask = 0;

    /* check if pString doesn't hold null or whitespaces */
    if(AnscValidStringCheck(pString) != TRUE)
        return FALSE;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "IPAddress", TRUE))
    {
        if(is_IpAddress(pString))
        {
            /* save update to backup */
            AnscCopyString(pSubnet->IPAddress, pString);
            return TRUE;
        }
    }

    if( AnscEqualString(ParamName, "SubnetMask", TRUE))
    {
        mask = ntohl(inet_addr(pString));
        if(0==(mask & (~mask >> 1)))
        {
            /* save update to backup */
            AnscCopyString(pSubnet->SubnetMask, pString);
            return TRUE;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
Subnet_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnet      = (PCOSA_DML_TSIP_SUBNET_ENTRY)hInsContext;

    return TRUE;
}

ULONG
Subnet_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject    = (PCOSA_DATAMODEL_TSIP       )g_pCosaBEManager->hTSIP; 
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnet      = (PCOSA_DML_TSIP_SUBNET_ENTRY)hInsContext;

    CosaDmlTSIPSubnetSetEntry((ANSC_HANDLE)pMyObject, pSubnet);

    return 0;

}

ULONG
Subnet_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{   
    PCOSA_DATAMODEL_TSIP            pMyObject    = (PCOSA_DATAMODEL_TSIP       )g_pCosaBEManager->hTSIP; 
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnet      = (PCOSA_DML_TSIP_SUBNET_ENTRY)hInsContext;

    CosaDmlTSIPSubnetGetCfg((ANSC_HANDLE)pMyObject, pSubnet);

    return 0; 
}

/* Beautiful cut off */

BOOL
PortManagement_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg    = (PCOSA_DML_TSIP_PORTMANAGEMENT_CFG)&pMyObject->PortManagementCfg;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pCfg->Enabled;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
PortManagement_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg    = (PCOSA_DML_TSIP_PORTMANAGEMENT_CFG)&pMyObject->PortManagementCfg;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "RuleType", TRUE))
    {
        /* collect value */       
        *puLong = pCfg->RuleType;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
PortManagement_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg    = (PCOSA_DML_TSIP_PORTMANAGEMENT_CFG)&pMyObject->PortManagementCfg;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pCfg->Enabled = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
PortManagement_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg    = (PCOSA_DML_TSIP_PORTMANAGEMENT_CFG)&pMyObject->PortManagementCfg;
    
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "RuleType", TRUE))
    {
        /* save update to backup */
        pCfg->RuleType = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
PortManagement_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
PortManagement_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg    = (PCOSA_DML_TSIP_PORTMANAGEMENT_CFG)&pMyObject->PortManagementCfg;

    if ( ANSC_STATUS_SUCCESS != CosaDmlTSIPPortManagementSetCfg(NULL, pCfg) )
    {
        PortManagement_Rollback(NULL);
    }

    return 0;
}

ULONG
PortManagement_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{   
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg    = (PCOSA_DML_TSIP_PORTMANAGEMENT_CFG)&pMyObject->PortManagementCfg;

    CosaDmlTSIPPortManagementGetCfg(NULL, pCfg);

    return 0; 
}

/* Beautiful cut off */

ULONG
Rule_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    
    return AnscSListQueryDepth(&pMyObject->RuleList);
}

ANSC_HANDLE
Rule_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject    = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PSLIST_HEADER                   pRuleHead  = (PSLIST_HEADER)&pMyObject->RuleList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pLink        = (PSINGLE_LINK_ENTRY       )NULL;

    pLink = AnscSListGetEntryByIndex(pRuleHead, nIndex);

    if ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pCosaContext->InstanceNumber;
    }
    
    return pCosaContext;
}

ANSC_HANDLE
Rule_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->RuleList;
    PCOSA_DML_TSIP_RULE_ENTRY       pEntry                  = (PCOSA_DML_TSIP_RULE_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY       )NULL;

    pEntry = (PCOSA_DML_TSIP_RULE_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_TSIP_RULE_ENTRY));
    if (!pEntry)
    {
        return NULL;
    }

    _ansc_sprintf(pEntry->Alias, "Rule%d", pMyObject->ulRuleNextInsNum);

    /* Update the cache */
    pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pCosaContext)
    {
        AnscFreeMemory(pEntry);

        return NULL;
    }

    pCosaContext->InstanceNumber = pEntry->InstanceNumber = pMyObject->ulRuleNextInsNum;

    pMyObject->ulRuleNextInsNum++;

    if ( pMyObject->ulRuleNextInsNum == 0 )
    {
        pMyObject->ulRuleNextInsNum = 1;
    }

    pCosaContext->hContext        = (ANSC_HANDLE)pEntry;
    pCosaContext->hParentTable    = NULL;
    pCosaContext->bNew            = TRUE;

    CosaSListPushEntryByInsNum(pListHead, pCosaContext);

    CosaTSIPRuleRegAddInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pCosaContext);

    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext;
}

ULONG
Rule_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2           = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_TSIP_RULE_ENTRY       pEntry                  = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pNewEntry               = (PCOSA_DML_TSIP_RULE_ENTRY)NULL;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER)&pMyObject->RuleList;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = NULL;

    if ( !pCosaContext->bNew )
    {
        if ( ANSC_STATUS_SUCCESS != CosaDmlTSIPRuleDelEntry(NULL, pEntry) )
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    AnscSListPopEntryByLink(pListHead, &pCosaContext->Linkage);

    if ( pCosaContext->bNew )
    {
        CosaTSIPRuleRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    
    AnscFreeMemory(pEntry);
    AnscFreeMemory(pCosaContext);

    return ANSC_STATUS_SUCCESS;
}

BOOL
Rule_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pRule        = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pRule->Enabled;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
Rule_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pRule        = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Protocol", TRUE))
    {
        /* collect value */       
        *puLong = pRule->Protocol;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PortRangeMin", TRUE))
    {
        /* collect value */       
        *puLong = pRule->PortRangeMin;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PortRangeMax", TRUE))
    {
        /* collect value */       
        *puLong = pRule->PortRangeMax;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

ULONG
Rule_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pRule        = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Name", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pRule->Name);
        return 0;
    }

    if( AnscEqualString(ParamName, "IPRangeMin", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pRule->IPRangeMin);
        return 0;
    }

    if( AnscEqualString(ParamName, "IPRangeMax", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pRule->IPRangeMax);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

BOOL
Rule_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pRule        = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pRule->Enabled = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
Rule_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pRule        = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;
    
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Protocol", TRUE))
    {
        /* save update to backup */
        pRule->Protocol = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PortRangeMin", TRUE))
    {
        /* save update to backup */
        pRule->PortRangeMin = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PortRangeMax", TRUE))
    {
        /* save update to backup */
        pRule->PortRangeMax = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
Rule_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pRule        = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Name", TRUE))
    {
        /* save update to backup */
         AnscCopyString(pRule->Name, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPRangeMin", TRUE))
    {
        /* save update to backup */
         AnscCopyString(pRule->IPRangeMin, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPRangeMax", TRUE))
    {
        /* save update to backup */
         AnscCopyString(pRule->IPRangeMax, pString);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
Rule_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
Rule_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject    = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pRule        = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlTSIPRuleAddEntry(NULL, pRule);
        CosaTSIPRuleRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlTSIPRuleSetEntry(NULL, pRule);
    }

    return 0;

}

ULONG
Rule_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{   
    PCOSA_DATAMODEL_TSIP            pMyObject    = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pRule        = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;

    CosaDmlTSIPRuleGetCfg(NULL, pRule);

    return 0; 
}

#endif
