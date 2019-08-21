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
#include <utctx_api.h>
#include <utapi.h>
#include "cosa_dslite_apis.h"
#include <utapi/utapi_dslite.h>

ANSC_STATUS
CosaDmlDsliteInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlGetDsliteEnable
    (
        ANSC_HANDLE                 hContext,
        BOOL                       *bEnabled
    )
{
    int rc = -1;
    UtopiaContext ctx;
    boolean_t enabled = FALSE;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_GetDsliteEnable(&ctx, &enabled);

    if (TRUE == enabled)
    {
        *bEnabled = TRUE;
    }
    else
    {
        *bEnabled = FALSE;
    }
    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);
    return rc;
}

ANSC_STATUS
CosaDmlSetDsliteEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    int rc = -1;
    UtopiaContext ctx;
    boolean_t read_dslite_enable;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetDsliteEnable(&ctx, &read_dslite_enable);
    if(read_dslite_enable != bEnabled)
    {
        rc = Utopia_SetDsliteEnable(&ctx, bEnabled);
        Utopia_Free(&ctx,!rc);
        commonSyseventSet("wan-restart", "");
        system("service_dslite restart &");
        return ANSC_STATUS_SUCCESS;
    }

    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDsliteAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DSLITE            pEntry
    )
{
    int rc = -1;
    UtopiaContext ctx;
    DsLiteCfg_t dslite_cfg;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    memset(&dslite_cfg, 0, sizeof(DsLiteCfg_t));
    dslite_cfg.InstanceNumber = pEntry->InstanceNumber;
    dslite_cfg.active = pEntry->active;
    dslite_cfg.status = pEntry->status;
    dslite_cfg.mode = pEntry->mode;
    dslite_cfg.addr_type = pEntry->addr_type;
    dslite_cfg.origin = pEntry->origin;
    dslite_cfg.mss_clamping_enable = pEntry->mss_clamping_enable;
    dslite_cfg.tcpmss = pEntry->tcpmss;
    dslite_cfg.ipv6_frag_enable = pEntry->ipv6_frag_enable;

    memcpy(dslite_cfg.alias, pEntry->alias, 65*sizeof(char));
    memcpy(dslite_cfg.addr_inuse, pEntry->addr_inuse, 257*sizeof(char));
    memcpy(dslite_cfg.addr_fqdn, pEntry->addr_fqdn, 257*sizeof(char));
    memcpy(dslite_cfg.addr_ipv6, pEntry->addr_ipv6, 257*sizeof(char));
    memcpy(dslite_cfg.tunnel_interface, pEntry->tunnel_interface, 257*sizeof(char));
    memcpy(dslite_cfg.tunneled_interface, pEntry->tunneled_interface, 257*sizeof(char));

    rc = Utopia_AddDsliteEntry(&ctx,&dslite_cfg);

    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDsliteDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    int rc = -1;
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_DelDsliteEntry(&ctx,ulInstanceNumber);

    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDsliteSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DSLITE            pEntry
    )
{
    int rc = -1;
    UtopiaContext ctx;
    DsLiteCfg_t dslite_cfg;
    DsLiteCfg_t read_dslite_cfg;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    memset(&read_dslite_cfg, 0, sizeof(DsLiteCfg_t));
    read_dslite_cfg.InstanceNumber = pEntry->InstanceNumber;
    Utopia_GetDsliteCfg(&ctx, &read_dslite_cfg);

    memset(&dslite_cfg, 0, sizeof(DsLiteCfg_t));
    dslite_cfg.InstanceNumber = pEntry->InstanceNumber;
    dslite_cfg.active = pEntry->active;
    dslite_cfg.status = pEntry->status;
    dslite_cfg.mode = pEntry->mode;
    dslite_cfg.addr_type = pEntry->addr_type;
    dslite_cfg.origin = pEntry->origin;
    dslite_cfg.mss_clamping_enable = pEntry->mss_clamping_enable;
    dslite_cfg.tcpmss = pEntry->tcpmss;
    dslite_cfg.ipv6_frag_enable = pEntry->ipv6_frag_enable;

    memcpy(dslite_cfg.alias, pEntry->alias, 65*sizeof(char));
    memcpy(dslite_cfg.addr_inuse, pEntry->addr_inuse, 257*sizeof(char));
    memcpy(dslite_cfg.addr_fqdn, pEntry->addr_fqdn, 257*sizeof(char));
    memcpy(dslite_cfg.addr_ipv6, pEntry->addr_ipv6, 257*sizeof(char));
    memcpy(dslite_cfg.tunnel_interface, pEntry->tunnel_interface, 257*sizeof(char));
    memcpy(dslite_cfg.tunneled_interface, pEntry->tunneled_interface, 257*sizeof(char));

    rc = Utopia_SetDsliteCfg(&ctx,&dslite_cfg);
    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);

    if(read_dslite_cfg.active != pEntry->active)
        goto SET_DSLITE_RESTART;
    else if(read_dslite_cfg.mode != pEntry->mode)
        goto SET_DSLITE_RESTART;
    else if(read_dslite_cfg.addr_type != pEntry->addr_type)
        goto SET_DSLITE_RESTART;
    else if(read_dslite_cfg.mss_clamping_enable != pEntry->mss_clamping_enable)
        goto SET_DSLITE_RESTART;
    else if(read_dslite_cfg.tcpmss != pEntry->tcpmss)
        goto SET_DSLITE_RESTART;
    else if(read_dslite_cfg.ipv6_frag_enable != pEntry->ipv6_frag_enable)
        goto SET_DSLITE_RESTART;
    else if(!AnscEqualString(read_dslite_cfg.addr_fqdn, pEntry->addr_fqdn, TRUE))
        goto SET_DSLITE_RESTART;
    else if(!AnscEqualString(read_dslite_cfg.addr_ipv6, pEntry->addr_ipv6, TRUE))
        goto SET_DSLITE_RESTART;
    else
        goto EXIT;

SET_DSLITE_RESTART:
    system("service_dslite restart &");
EXIT:
    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDsliteGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DSLITE            pEntry
    )
{
    int rc = -1;
    UtopiaContext ctx;
    DsLiteCfg_t dslite_cfg;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    dslite_cfg.InstanceNumber = pEntry->InstanceNumber;
    rc = Utopia_GetDsliteCfg(&ctx,&dslite_cfg);
    if(rc == 0)
    {
      pEntry->active = dslite_cfg.active;
      pEntry->status = dslite_cfg.status;
      pEntry->mode = dslite_cfg.mode;
      pEntry->addr_type = dslite_cfg.addr_type;
      pEntry->origin = dslite_cfg.origin;
      pEntry->mss_clamping_enable = dslite_cfg.mss_clamping_enable;
      pEntry->tcpmss = dslite_cfg.tcpmss;
      pEntry->ipv6_frag_enable = dslite_cfg.ipv6_frag_enable;

      memcpy(pEntry->alias, dslite_cfg.alias, 65*sizeof(char));
      memcpy(pEntry->addr_inuse, dslite_cfg.addr_inuse, 257*sizeof(char));
      memcpy(pEntry->addr_fqdn, dslite_cfg.addr_fqdn, 257*sizeof(char));
      memcpy(pEntry->addr_ipv6, dslite_cfg.addr_ipv6, 257*sizeof(char));
      memcpy(pEntry->tunnel_interface, dslite_cfg.tunnel_interface, 257*sizeof(char));
      memcpy(pEntry->tunneled_interface, dslite_cfg.tunneled_interface, 257*sizeof(char));
    }
    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDsliteGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pNum
    )
{
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    Utopia_GetNumOfDsliteEntries(&ctx, pNum);

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDsliteGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DSLITE            pEntry
    )
{
    int rc = -1;
    UtopiaContext ctx;
    DsLiteCfg_t dslite_cfg;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_GetDsliteEntry(&ctx,ulIndex,&dslite_cfg);
    if(rc == 0)
    {
      pEntry->InstanceNumber = dslite_cfg.InstanceNumber;
      pEntry->active = dslite_cfg.active;
      pEntry->status = dslite_cfg.status;
      pEntry->mode = dslite_cfg.mode;
      pEntry->addr_type = dslite_cfg.addr_type;
      pEntry->origin = dslite_cfg.origin;
      pEntry->mss_clamping_enable = dslite_cfg.mss_clamping_enable;
      pEntry->tcpmss = dslite_cfg.tcpmss;
      pEntry->ipv6_frag_enable = dslite_cfg.ipv6_frag_enable;

      memcpy(pEntry->alias, dslite_cfg.alias, 65*sizeof(char));
      memcpy(pEntry->addr_inuse, dslite_cfg.addr_inuse, 257*sizeof(char));
      memcpy(pEntry->addr_fqdn, dslite_cfg.addr_fqdn, 257*sizeof(char));
      memcpy(pEntry->addr_ipv6, dslite_cfg.addr_ipv6, 257*sizeof(char));
      memcpy(pEntry->tunnel_interface, dslite_cfg.tunnel_interface, 257*sizeof(char));
      memcpy(pEntry->tunneled_interface, dslite_cfg.tunneled_interface, 257*sizeof(char));
    }

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDsliteSetInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber
    )
{
    int rc = -1;
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_SetDsliteInsNum(&ctx,ulIndex,ulInstanceNumber);

    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}
#endif
