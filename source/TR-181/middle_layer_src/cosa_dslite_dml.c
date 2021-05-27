/**********************************************************************
 * Copyright 2017-2019 ARRIS Enterprises, LLC.
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
**********************************************************************/
#ifdef DSLITE_FEATURE_SUPPORT
#include "ansc_platform.h"
#include "cosa_dslite_apis.h"
#include "cosa_dslite_dml.h"
#include "plugin_main_apis.h"
#include "cosa_dslite_internal.h"
#include "safec_lib_common.h"
#include <syscfg/syscfg.h>

static const char *UPDATE_RESOLV_CMD = "/bin/sh /etc/utopia/service.d/set_resolv_conf.sh";
//#endif
/***********************************************************************

 APIs for Object:

    DSLite.

    *  DSLite_GetParamBoolValue
    *  DSLite_SetParamBoolValue

    *  InterfaceSetting4_GetEntryCount
    *  InterfaceSetting4_GetEntry
    *  InterfaceSetting4_AddEntry
    *  InterfaceSetting4_DelEntry

    *  InterfaceSetting4_GetParamBoolValue
    *  InterfaceSetting4_GetParamStringValue
    *  InterfaceSetting4_GetParamUlongValue

    *  InterfaceSetting4_SetParamBoolValue
    *  InterfaceSetting4_SetParamStringValue
    *  InterfaceSetting4_SetParamUlongValue

    *  InterfaceSetting4_Validate
    *  InterfaceSetting4_Commit
    *  InterfaceSetting4_Rollback

***********************************************************************/

BOOL
DSLite_GetParamBoolValue
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
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
       BOOL bEnabled = FALSE; 
        /* collect value */
        CosaDmlGetDsliteEnable(NULL, &bEnabled);
        if (TRUE == bEnabled) 
        {
            *pBool = TRUE;
        }
        else
        {
            *pBool = FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL
DSLite_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(bValue);
    int rc = -1;
    /* check the parameter name and set the corresponding value */
    if ( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* save update to backup */
        CosaDmlSetDsliteEnable(NULL, (BOOLEAN)bValue);
        rc = vsystem(UPDATE_RESOLV_CMD);
        return TRUE;
    }
    return FALSE;
}

ULONG
InterfaceSetting4_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DSLITE pDslite = (PCOSA_DATAMODEL_DSLITE)g_pCosaBEManager->hDslite;

    return AnscSListQueryDepth( &pDslite->DsliteList );
}

ANSC_HANDLE
InterfaceSetting4_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(nIndex);
    UNREFERENCED_PARAMETER(pInsNumber);
    PCOSA_DATAMODEL_DSLITE          pDslite           = (PCOSA_DATAMODEL_DSLITE)g_pCosaBEManager->hDslite;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = NULL;

    pSListEntry = AnscSListGetEntryByIndex(&pDslite->DsliteList, nIndex);

    if ( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
        *pInsNumber       = pCxtLink->InstanceNumber;
    }

    return pSListEntry;
}

ANSC_HANDLE
InterfaceSetting4_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pInsNumber);
    PCOSA_DATAMODEL_DSLITE          pDslite           = (PCOSA_DATAMODEL_DSLITE)g_pCosaBEManager->hDslite;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = NULL;
    PCOSA_DML_DSLITE                pDsliteTunnel     = NULL;
    errno_t                         rc                = -1;

    pDsliteTunnel  = (PCOSA_DML_DSLITE)AnscAllocateMemory( sizeof(COSA_DML_DSLITE) );
    if ( !pDsliteTunnel )
    {
        goto EXIT2;
    }

    /* Set default value */
    DSLITE_SET_DEFAULTVALUE(pDsliteTunnel);

    pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
    if ( !pDsliteTunnel )
    {
        goto EXIT1;
    }

    pCxtLink->hContext       = (ANSC_HANDLE)pDsliteTunnel;
    pCxtLink->bNew           = TRUE;

    if ( !++pDslite->maxInstanceOfDslite )
    {
        pDslite->maxInstanceOfDslite   = 1;
    }

    pDsliteTunnel->InstanceNumber    = pDslite->maxInstanceOfDslite;
    pCxtLink->InstanceNumber         = pDsliteTunnel->InstanceNumber;
    *pInsNumber                      = pDsliteTunnel->InstanceNumber;

    rc = sprintf_s( pDsliteTunnel->alias, sizeof(pDsliteTunnel->alias),"Dslite.Tunnel.%lu", pDsliteTunnel->InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pCxtLink);
      goto EXIT1;
    }

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pDslite->DsliteList, pCxtLink);

    /* we recreate the configuration because we has new delay_added entry for dslite */
    CosaDsliteRegSetDsliteInfo(pDslite);

    return (ANSC_HANDLE)pCxtLink;


EXIT1:

    AnscFreeMemory(pDsliteTunnel);

EXIT2:

    return NULL; /* return the handle */
}

ULONG
InterfaceSetting4_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(hInstance);
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DSLITE          pDslite           = (PCOSA_DATAMODEL_DSLITE)g_pCosaBEManager->hDslite;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_DSLITE                pDsliteTunnel     = (PCOSA_DML_DSLITE)pCxtLink->hContext;

    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlDsliteDelEntry(NULL, pDsliteTunnel->InstanceNumber);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }

    /* Firstly we del this link point from our list */
    if (AnscSListPopEntryByLink(&pDslite->DsliteList, &pCxtLink->Linkage))
    {
        CosaDsliteRegSetDsliteInfo(pDslite);

        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }

    return returnStatus;

}

BOOL
InterfaceSetting4_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DSLITE                 pDsliteTunnel     = (PCOSA_DML_DSLITE)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        /* collect value */
        *pBool  =  pDsliteTunnel->active;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_MssClampingEnable", TRUE))
    {
        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        /* collect value */
        *pBool  =  pDsliteTunnel->mss_clamping_enable;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_IPv6FragEnable", TRUE))
    {

        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        *pBool  =  pDsliteTunnel->ipv6_frag_enable;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

ULONG
InterfaceSetting4_GetParamStringValue
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
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DSLITE                pDsliteTunnel     = (PCOSA_DML_DSLITE)pCxtLink->hContext;
    errno_t                         rc                = -1;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        /* collect value */
        if ( AnscSizeOfString(pDsliteTunnel->alias) < *pUlSize)
        {
           rc = strcpy_s(pValue, *pUlSize, pDsliteTunnel->alias);
           if(rc != EOK)
           {
              ERR_CHK(rc);
              return -1;
           }
           return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDsliteTunnel->alias)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "EndpointAddressInUse", TRUE))
    {
        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        /* collect value */
        if ( AnscSizeOfString(pDsliteTunnel->addr_inuse) < *pUlSize)
        {
           rc = strcpy_s(pValue, *pUlSize, pDsliteTunnel->addr_inuse);
           if(rc != EOK)
           {
             ERR_CHK(rc);
             return -1;
           }
           return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDsliteTunnel->addr_inuse)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "EndpointName", TRUE))
    {
        char endPointName[260] = {0};
        char buf[8] = {0};

        syscfg_get(NULL, "dslite_enable", buf, sizeof(buf));

        if (!strncmp(buf,"1",1))
        {
            CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
            if (pDsliteTunnel->mode == 1)
            {
                commonSyseventGet("dslite_dhcpv6_endpointname", endPointName, sizeof(endPointName));
            }
            else
            {
                if ( AnscSizeOfString(pDsliteTunnel->addr_fqdn) < sizeof(endPointName))
                {
                    rc = strcpy_s(endPointName, sizeof(endPointName), pDsliteTunnel->addr_fqdn);
                    if(rc != EOK)
                    {
                       ERR_CHK(rc);
                       return -1;
                    }
                }
            }
            /* collect value */
            if ( AnscSizeOfString(endPointName) < *pUlSize)
            {
                rc = strcpy_s(pValue, *pUlSize, endPointName);
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                   return -1;
                }
                return 0;
            }
            else
            {
                *pUlSize = AnscSizeOfString(endPointName)+1;
                return 1;
            }
        }
        else
        {
            pValue[0] = '\0';
            return 0;
        }
    }

    if( AnscEqualString(ParamName, "EndpointAddress", TRUE))
    {
        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        /* collect value */
        if ( AnscSizeOfString(pDsliteTunnel->addr_ipv6) < *pUlSize)
        {
            rc =  strcpy_s(pValue,*pUlSize, pDsliteTunnel->addr_ipv6);
            if(rc != EOK)
            {
              ERR_CHK(rc);
              return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDsliteTunnel->addr_ipv6)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "TunnelInterface", TRUE))
    {
        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        /* collect value */
        if ( AnscSizeOfString(pDsliteTunnel->tunnel_interface) < *pUlSize)
        {
            rc =  strcpy_s(pValue, *pUlSize, pDsliteTunnel->tunnel_interface);
            if(rc != EOK)
            {
              ERR_CHK(rc);
              return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDsliteTunnel->tunnel_interface)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "TunneledInterface", TRUE))
    {
        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        /* collect value */
        if ( AnscSizeOfString(pDsliteTunnel->tunneled_interface) < *pUlSize)
        {
            rc =  strcpy_s(pValue,*pUlSize, pDsliteTunnel->tunneled_interface);
            if(rc != EOK)
            {
              ERR_CHK(rc);
              return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDsliteTunnel->tunneled_interface)+1;
            return 1;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

BOOL
InterfaceSetting4_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DSLITE                pDsliteTunnel     = (PCOSA_DML_DSLITE)pCxtLink->hContext;
    errno_t                         rc                = -1;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        char buf[8] = {0};
        int status = 2; //status: Enabled(1), Disabled(2), Error(3)
        syscfg_get(NULL, "dslite_enable", buf, sizeof(buf));

        if (!strncmp(buf,"1",1))
        {
            char endPointName[260] = {0};

            CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
            if (pDsliteTunnel->mode == 1)
            {
                commonSyseventGet("dslite_dhcpv6_endpointname", endPointName, sizeof(endPointName));
            }
            else
            {
                if (AnscSizeOfString(pDsliteTunnel->addr_fqdn) < sizeof(endPointName))
                {
                   rc = strcpy_s(endPointName,sizeof(endPointName),pDsliteTunnel->addr_fqdn);
                   if(rc != EOK)
                   {
                      ERR_CHK(rc);
                      *puLong = status;
                      return FALSE;
                   }
                }
            }
            if (AnscSizeOfString(endPointName) > 0)
            {
                status = 1; // Enabled
            }
            else
            {
                status = 3; // Error
            }
        }
        /* collect value */
        *puLong = status;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "EndpointAssignmentPrecedence", TRUE))
    {
        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        /* collect value */
        *puLong = pDsliteTunnel->mode;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "EndpointAddressTypePrecedence", TRUE))
    {
        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        /* collect value */
        *puLong = pDsliteTunnel->addr_type;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Origin", TRUE))
    {
        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        /* collect value */
        *puLong = pDsliteTunnel->origin;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_Tcpmss", TRUE))
    {
        CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        /* collect value */
        *puLong = pDsliteTunnel->tcpmss;

        return TRUE;
    }
    return FALSE;
}

BOOL
InterfaceSetting4_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(bValue);
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DSLITE                pDsliteTunnel     = (PCOSA_DML_DSLITE)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if ( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* save update to backup */
        pDsliteTunnel->active   =  bValue;
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_MssClampingEnable", TRUE) )
    {
        /* save update to backup */
        pDsliteTunnel->mss_clamping_enable   =  bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_IPv6FragEnable", TRUE))
    {
        /* save update to backup */
        pDsliteTunnel->ipv6_frag_enable   =  bValue;

        return TRUE;
    }
    return FALSE;
}

BOOL
InterfaceSetting4_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pString);
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DSLITE                pDsliteTunnel     = (PCOSA_DML_DSLITE)pCxtLink->hContext;
    errno_t                         rc                = -1;

    CcspTraceWarning(("Dslite: set %s to %s\n", ParamName, pString));

    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* save update to backup */
        rc = strcpy_s(pDsliteTunnel->alias, sizeof(pDsliteTunnel->alias), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    if( AnscEqualString(ParamName, "EndpointName", TRUE))
    {
        COSA_DML_DSLITE read_cfg;
        read_cfg.InstanceNumber = pDsliteTunnel->InstanceNumber;
        CosaDmlDsliteGetCfg(NULL, &read_cfg);
        if(read_cfg.mode == 2)//EndpointName is only writable when EndpointAssignmentPrecedence is Static
        {
            /* save update to backup */
            rc = strcpy_s(pDsliteTunnel->addr_fqdn, sizeof(pDsliteTunnel->addr_fqdn),pString);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }
            return TRUE;
        }
        else
            return FALSE;
    }

    if( AnscEqualString(ParamName, "EndpointAddress", TRUE))
    {
        /* save update to backup */
        rc = strcpy_s(pDsliteTunnel->addr_ipv6,sizeof(pDsliteTunnel->addr_ipv6), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    return FALSE;
}

BOOL
InterfaceSetting4_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
UNREFERENCED_PARAMETER(uValue);
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DSLITE                pDsliteTunnel     = (PCOSA_DML_DSLITE)pCxtLink->hContext;

    if( AnscEqualString(ParamName, "EndpointAssignmentPrecedence", TRUE))
    {
        /* save update to backup */
        pDsliteTunnel->mode   =  uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "EndpointAddressTypePrecedence", TRUE))
    {
        /* save update to backup */
        pDsliteTunnel->addr_type   =  uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_Tcpmss", TRUE))
    {
        /* save update to backup */
        pDsliteTunnel->tcpmss   =  uValue;
        return TRUE;
    }
    return FALSE;
}

BOOL
InterfaceSetting4_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(puLength);
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    PCOSA_DATAMODEL_DSLITE          pDslite           = (PCOSA_DATAMODEL_DSLITE)g_pCosaBEManager->hDslite;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DSLITE                pDsliteTunnel     = (PCOSA_DML_DSLITE)pCxtLink->hContext;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DML_DSLITE                pDsliteTunnel2    = NULL;
    BOOL                            bFound            = FALSE;

    if ( pDslite->Alias[0] )
    {
        bFound                = FALSE;
        pSListEntry           = AnscSListGetFirstEntry(&pDslite->DsliteList);
        while( pSListEntry != NULL)
        {
            pCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);

            pDsliteTunnel2 = (PCOSA_DML_DSLITE)pCxtLink->hContext;

            if( AnscEqualString(pDsliteTunnel2->alias, pDsliteTunnel->alias, TRUE) )
            {
                if ( (ANSC_HANDLE)pCxtLink == hInsContext )
                {
                    continue;
                }

                bFound = TRUE;

                break;
            }
        }

        if ( bFound )
        {
#if COSA_DSLITE_ROLLBACK_TEST
            InterfaceSetting4_Rollback(hInsContext);
#endif
            return FALSE;
        }
    }
    return TRUE;
}

ULONG
InterfaceSetting4_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DSLITE                pDsliteTunnel     = (PCOSA_DML_DSLITE)pCxtLink->hContext;
    PCOSA_DATAMODEL_DSLITE          pDslite           = (PCOSA_DATAMODEL_DSLITE)g_pCosaBEManager->hDslite;
    errno_t                         rc                = -1;

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlDsliteAddEntry(NULL, pDsliteTunnel );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaDsliteRegSetDsliteInfo(pDslite);
        }
        else
        {
            DSLITE_SET_DEFAULTVALUE(pDsliteTunnel);

            if ( pDslite->Alias[0] ){
                rc = strcpy_s(pDsliteTunnel->alias,sizeof(pDsliteTunnel->alias),pDslite->Alias);
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                   return 0;
                }
            }
        }
    }
    else
    {
        returnStatus = CosaDmlDsliteSetCfg(NULL, pDsliteTunnel);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDsliteGetCfg(NULL, pDsliteTunnel);
        }
    }

    AnscZeroMemory( pDslite->Alias, sizeof(pDslite->Alias) );

    return returnStatus;

}

ULONG
InterfaceSetting4_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DSLITE          pDslite           = (PCOSA_DATAMODEL_DSLITE)g_pCosaBEManager->hDslite;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DSLITE                pDsliteTunnel     = (PCOSA_DML_DSLITE)pCxtLink->hContext;
    errno_t                         rc                = -1;

    if ( pDslite->Alias[0] )
    {
       rc = strcpy_s(pDsliteTunnel->alias,sizeof(pDsliteTunnel->alias),pDslite->Alias);
       if(rc != EOK)
       {
         ERR_CHK(rc);
         return 0;
       }
    }
    if ( !pCxtLink->bNew )
    {
        CosaDmlDsliteGetCfg( NULL, pDsliteTunnel );
    }
    else
    {
        DSLITE_SET_DEFAULTVALUE(pDsliteTunnel);
    }

    AnscZeroMemory( pDslite->Alias, sizeof(pDslite->Alias) );

    return returnStatus;

}
#endif
