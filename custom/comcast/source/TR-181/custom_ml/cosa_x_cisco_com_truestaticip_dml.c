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
#include "safec_lib_common.h"

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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_CFG              pTSIP     = (PCOSA_DML_TSIP_CFG  )&pMyObject->TSIPCfg;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pTSIP->Enabled;
        return TRUE;
    }

    if (strcmp(ParamName, "ConfigApply") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_CFG              pTSIP     = (PCOSA_DML_TSIP_CFG  )&pMyObject->TSIPCfg;
    errno_t                         rc        = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pTSIP->IPAddress);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "SubnetMask") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pTSIP->SubnetMask);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "GatewayIPAddress") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pTSIP->GatewayIPAddress);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "ConfigEncryptKey") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pTSIP->ConfigEncryptKey);
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

BOOL
TrueStaticIP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_CFG              pTSIP     = (PCOSA_DML_TSIP_CFG  )&pMyObject->TSIPCfg;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pTSIP->Enabled        = bValue;
        pTSIP->bIPInfoChanged = TRUE;
        return TRUE;
    }

    if (strcmp(ParamName, "ConfigApply") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_CFG              pTSIP     = (PCOSA_DML_TSIP_CFG  )&pMyObject->TSIPCfg;
    errno_t                         rc        = -1;
    unsigned int mask = 0;

    /* check if pString doesn't hold null or whitespaces */
    if(AnscValidStringCheck(pString) != TRUE)
        return FALSE;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        if(is_IpAddress((unsigned char*)pString))
        {
            /* save update to backup */
            if(!(is_PrivateIp((PUCHAR)pString)))
            {
                rc = STRCPY_S_NOCLOBBER(pTSIP->IPAddress, sizeof(pTSIP->IPAddress), pString);
                if(rc != EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                pTSIP->bIPInfoChanged = TRUE;
                return TRUE;
            }
        }
    }

    if (strcmp(ParamName, "SubnetMask") == 0)
    {
        /* In binary representation, the value should be consecutive number of 1's and 0's
           eg: 255.255.255.224*/
        in_addr_t subnet_addr = inet_addr(pString);
        if (subnet_addr)
        {
            mask = ntohl(subnet_addr);
            if(0==(mask & (~mask >> 1)))
            {
                /* save update to backup */
                rc = STRCPY_S_NOCLOBBER(pTSIP->SubnetMask, sizeof(pTSIP->SubnetMask), pString);
                if(rc != EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                pTSIP->bIPInfoChanged = TRUE;
                return TRUE;
            }
        }
    }

    if (strcmp(ParamName, "GatewayIPAddress") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pTSIP->GatewayIPAddress, sizeof(pTSIP->GatewayIPAddress), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        pTSIP->bIPInfoChanged = TRUE;
        return TRUE;
    }

    if (strcmp(ParamName, "ConfigEncryptKey") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pTSIP->ConfigEncryptKey, sizeof(pTSIP->ConfigEncryptKey), pString);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP       )g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pEntry                  = (PCOSA_DML_TSIP_SUBNET_ENTRY)NULL;
    errno_t                         rc                      = -1;

    pEntry = (PCOSA_DML_TSIP_SUBNET_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_TSIP_SUBNET_ENTRY));
    if (!pEntry)
    {
        return NULL;
    }

    rc = sprintf_s(pEntry->Alias, sizeof(pEntry->Alias), "Subnet%lu", pMyObject->ulSubnetNextInsNum);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

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
    UNREFERENCED_PARAMETER(hInsContext);
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
    if (strcmp(ParamName, "Enable") == 0)
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
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnet      = (PCOSA_DML_TSIP_SUBNET_ENTRY)hInsContext;
    errno_t                         rc           = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pSubnet->IPAddress);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "SubnetMask") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pSubnet->SubnetMask);
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
    if (strcmp(ParamName, "Enable") == 0)
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
    errno_t rc = -1;

    /* check if pString doesn't hold null or whitespaces */
    if(AnscValidStringCheck(pString) != TRUE)
        return FALSE;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        if(is_IpAddress((unsigned char*)pString))
        {
            if(!(is_PrivateIp((PUCHAR)pString)))
            {
                /* save update to backup */
                rc = STRCPY_S_NOCLOBBER(pSubnet->IPAddress, sizeof(pSubnet->IPAddress), pString);
                if(rc != EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                return TRUE;
            }
        }
    }

    if (strcmp(ParamName, "SubnetMask") == 0)
    {
        in_addr_t subnet_addr = inet_addr(pString);
        if((subnet_addr) && (AnscIsValidIpString(pString)))
        {
            mask = ntohl(subnet_addr);
            if(0==(mask & (~mask >> 1)))
            {
                /* save update to backup */
                rc = STRCPY_S_NOCLOBBER(pSubnet->SubnetMask, sizeof(pSubnet->SubnetMask), pString);
                if(rc != EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                return TRUE;
            }
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg    = (PCOSA_DML_TSIP_PORTMANAGEMENT_CFG)&pMyObject->PortManagementCfg;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg    = (PCOSA_DML_TSIP_PORTMANAGEMENT_CFG)&pMyObject->PortManagementCfg;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "RuleType") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg    = (PCOSA_DML_TSIP_PORTMANAGEMENT_CFG)&pMyObject->PortManagementCfg;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TSIP            pMyObject = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg    = (PCOSA_DML_TSIP_PORTMANAGEMENT_CFG)&pMyObject->PortManagementCfg;
    
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "RuleType") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

ULONG
PortManagement_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->RuleList;
    PCOSA_DML_TSIP_RULE_ENTRY       pEntry                  = (PCOSA_DML_TSIP_RULE_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    errno_t                         rc                      = -1;

    pEntry = (PCOSA_DML_TSIP_RULE_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_TSIP_RULE_ENTRY));
    if (!pEntry)
    {
        return NULL;
    }

    rc = sprintf_s(pEntry->Alias, sizeof(pEntry->Alias), "Rule%lu", pMyObject->ulRuleNextInsNum);
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP)g_pCosaBEManager->hTSIP;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_TSIP_RULE_ENTRY       pEntry                  = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER)&pMyObject->RuleList;

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
    if (strcmp(ParamName, "Enable") == 0)
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
    if (strcmp(ParamName, "Protocol") == 0)
    {
        /* collect value */       
        *puLong = pRule->Protocol;
        return TRUE;
    }

    if (strcmp(ParamName, "PortRangeMin") == 0)
    {
        /* collect value */       
        *puLong = pRule->PortRangeMin;
        return TRUE;
    }

    if (strcmp(ParamName, "PortRangeMax") == 0)
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
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pRule        = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;
    errno_t                         rc           = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Name") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pRule->Name);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "IPRangeMin") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pRule->IPRangeMin);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "IPRangeMax") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pRule->IPRangeMax);
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
    if (strcmp(ParamName, "Enable") == 0)
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
    if (strcmp(ParamName, "Protocol") == 0)
    {
        /* save update to backup */
        pRule->Protocol = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "PortRangeMin") == 0)
    {
        /* save update to backup */
        pRule->PortRangeMin = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "PortRangeMax") == 0)
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
    errno_t                         rc           = -1;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Name") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pRule->Name, sizeof(pRule->Name), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "IPRangeMin") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pRule->IPRangeMin, sizeof(pRule->IPRangeMin), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "IPRangeMax") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pRule->IPRangeMax, sizeof(pRule->IPRangeMax), pString);
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

BOOL
Rule_Validate
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

ULONG
Rule_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
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
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pRule        = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;

    CosaDmlTSIPRuleGetCfg(NULL, pRule);

    return 0; 
}

#endif
