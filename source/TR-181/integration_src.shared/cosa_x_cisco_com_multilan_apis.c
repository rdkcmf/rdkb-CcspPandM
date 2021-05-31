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

/**********************************************************************

    File: cosa_x_cisco_com_multilan_apis.c

    For Data Model SBAPI Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        The APIs servicing TR-181 data model X_CISCO_COM_MultiLAN SBAPI
        integration are implemented in this file.

    ---------------------------------------------------------------

    environment:

        Platform Independent, but with multi-LAN support

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        02/28/2013  initial revision.

**********************************************************************/

#include "cosa_apis.h"
#include "cosa_x_cisco_com_multilan_apis.h"
#include "cosa_x_cisco_com_multilan_internal.h"
#include "plugin_main_apis.h"
#include "ccsp_psm_helper.h"
#include "dmsb_tr181_psm_definitions.h"
#include "safec_lib_common.h"


/**********************************************************************
                            HELPER FUNCTIONS
**********************************************************************/

ANSC_STATUS pValue_update(char *pValue, PULONG pValueSize, char *str)
{
    errno_t rc = -1;
    if (! str)
    {
        return ANSC_STATUS_RESOURCES;
    }
    size_t len = strlen(str);
    if ( (len+1) > *pValueSize)
    {
        *pValueSize = len+1;
        return  ANSC_STATUS_MORE_DATA;
    }
    rc = strcpy_s(pValue, *pValueSize, str);
    ERR_CHK(rc);
    *pValueSize = len+1;
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************
                            MAIN ROUTINES
**********************************************************************/

ANSC_STATUS
CosaDmlMlanInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = NULL;
    UNREFERENCED_PARAMETER(hDml);
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pContext = (PCOSA_DATAMODEL_MULTILAN)AnscAllocateMemory(sizeof(COSA_DATAMODEL_MULTILAN));

    if ( pContext )
    {
        COSA_DATAMODEL_MULTILAN_Init(pContext);

        *phContext = (ANSC_HANDLE)pContext;
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }

    /* Need the Unload function to free allocated context */
}


ANSC_STATUS
CosaDmlMlanUnload
    (
        ANSC_HANDLE                 hContext
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;

    if ( pContext )
    {
        COSA_DATAMODEL_MULTILAN_Clean(pContext);
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMlanGetPrimaryLanIpIf
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    ULONG                           ulStrSize       = 0;
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pContext->pPrimaryLanIpIf )
    {
        SlapInitVariable(&SlapValue);

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_PrimaryLan_l3net,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) )
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter " DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_PrimaryLan_l3net
                    ", error code %d, type %d\n",
                    __FUNCTION__,
                    iReturnValue,
                    RecordType
                ));
        }
        else if (RecordType == ccsp_unsignedInt)
        {
            ulStrSize                 = _ansc_strlen(DM_TR181_NAME_IP_IF) + 10;
            pContext->pPrimaryLanIpIf = AnscAllocateMemory(ulStrSize);

            if ( pContext->pPrimaryLanIpIf )
            {
                rc = sprintf_s(pContext->pPrimaryLanIpIf, ulStrSize, "%s%d", DM_TR181_NAME_IP_IF, (int)SlapValue.Variant.varUint32);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }
            }
        }
        else if (RecordType == ccsp_string)
        {
            ulStrSize                 = _ansc_strlen(DM_TR181_NAME_IP_IF) + 10;
            pContext->pPrimaryLanIpIf = AnscAllocateMemory(ulStrSize);

            if ( pContext->pPrimaryLanIpIf )
            {
                rc = sprintf_s(pContext->pPrimaryLanIpIf, ulStrSize, "%s%s", DM_TR181_NAME_IP_IF, SlapValue.Variant.varString);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    return  pValue_update(pValue,pValueSize, pContext->pPrimaryLanIpIf);
}


ANSC_STATUS
CosaDmlMlanGetHomeSecurityIpIf
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    ULONG                           ulStrSize       = 0;
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pContext->pHomeSecurityIpIf )
    {
        SlapInitVariable(&SlapValue);

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_l3net,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) )
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter " DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_l3net
                    ", error code %d, type %d\n",
                    __FUNCTION__,
                    iReturnValue,
                    RecordType
                ));
        }
        else if (RecordType == ccsp_unsignedInt)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_IP_IF) + 10;
            pContext->pHomeSecurityIpIf = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityIpIf )
            {
                rc = sprintf_s(pContext->pHomeSecurityIpIf, ulStrSize, "%s%d", DM_TR181_NAME_IP_IF, (int)SlapValue.Variant.varUint32);
                if(rc < EOK)
                {
                   ERR_CHK(rc);
                }
            }
        }
        else if (RecordType == ccsp_string)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_IP_IF) + 10;
            pContext->pHomeSecurityIpIf = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityIpIf )
            {
                rc = sprintf_s(pContext->pHomeSecurityIpIf, ulStrSize, "%s%s", DM_TR181_NAME_IP_IF, SlapValue.Variant.varString);
                if(rc < EOK)
                {
                   ERR_CHK(rc);
                }
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    return  pValue_update(pValue,pValueSize, pContext->pHomeSecurityIpIf);
}

ANSC_STATUS
CosaDmlMlanGetPrimaryLanBridge
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    ULONG                           ulStrSize       = 0;
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pContext->pPrimaryLanBridge )
    {
        SlapInitVariable(&SlapValue);

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_PrimaryLan_l2net,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) )
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter " DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_PrimaryLan_l2net
                    ", error code %d, type %d\n",
                    __FUNCTION__,
                    iReturnValue,
                    RecordType
                ));
        }
        else if (RecordType == ccsp_unsignedInt)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_BRIDGE) + 10;
            pContext->pPrimaryLanBridge = AnscAllocateMemory(ulStrSize);

            if ( pContext->pPrimaryLanBridge )
            {
                rc = sprintf_s(pContext->pPrimaryLanBridge, ulStrSize, "%s%d", DM_TR181_NAME_BRIDGE,(int) SlapValue.Variant.varUint32);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }
            }
        }
        else if (RecordType == ccsp_string)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_BRIDGE) + 10;
            pContext->pPrimaryLanBridge = AnscAllocateMemory(ulStrSize);

            if ( pContext->pPrimaryLanBridge )
            {
                rc = sprintf_s(pContext->pPrimaryLanBridge, ulStrSize, "%s%s", DM_TR181_NAME_BRIDGE, SlapValue.Variant.varString);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    return  pValue_update(pValue,pValueSize, pContext->pPrimaryLanBridge);
}

ANSC_STATUS
CosaDmlMlanGetPrimaryLanBridgeHsPorts
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    ULONG                           ulStrSize       = 0;
    errno_t                         rc   = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pContext->pPrimaryLanBridgeHsPorts )
    {
        SlapInitVariable(&SlapValue);

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_PrimaryLan_HsPorts,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter " DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_PrimaryLan_HsPorts
                    ", error code %d, type %d\n",
                    __FUNCTION__,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            ulStrSize = _ansc_strlen(SlapValue.Variant.varString) + 1;
            pContext->pPrimaryLanBridgeHsPorts = AnscAllocateMemory(ulStrSize);

            if ( pContext->pPrimaryLanBridgeHsPorts )
            {
                rc = strcpy_s(pContext->pPrimaryLanBridgeHsPorts, ulStrSize, SlapValue.Variant.varString);
                ERR_CHK(rc);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    return  pValue_update(pValue,pValueSize, pContext->pPrimaryLanBridgeHsPorts);
}


ANSC_STATUS
CosaDmlMlanGetPrimaryLanDhcpv4ServerPool
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    ULONG                           ulStrSize       = 0;
    errno_t                         rc      = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pContext->pPrimaryLanDhcpv4ServerPool )
    {
        SlapInitVariable(&SlapValue);

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_PrimaryLANDHCPv4ServerPool,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_unsignedInt))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter " DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_PrimaryLANDHCPv4ServerPool
                    ", error code %d, type %d\n",
                    __FUNCTION__,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_DHCPv4ServerPool) + 10;
            pContext->pPrimaryLanDhcpv4ServerPool = AnscAllocateMemory(ulStrSize);

            if ( pContext->pPrimaryLanDhcpv4ServerPool )
            {
                rc = sprintf_s(pContext->pPrimaryLanDhcpv4ServerPool, ulStrSize, "%s%d", DM_TR181_NAME_DHCPv4ServerPool, (int)SlapValue.Variant.varUint32);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    return  pValue_update(pValue,pValueSize, pContext->pPrimaryLanDhcpv4ServerPool);
}


ANSC_STATUS
CosaDmlMlanGetHomeSecurityBridge
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    ULONG                           ulStrSize       = 0;
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pContext->pHomeSecurityBridge )
    {
        SlapInitVariable(&SlapValue);

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_l2net,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) )
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter " DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_l2net
                    ", error code %d, type %d\n",
                    __FUNCTION__,
                    iReturnValue,
                    RecordType
                ));
        }
        else if (RecordType == ccsp_unsignedInt)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_BRIDGE) + 10;
            pContext->pHomeSecurityBridge = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityBridge )
            {
                rc = sprintf_s(pContext->pHomeSecurityBridge, ulStrSize, "%s%d", DM_TR181_NAME_BRIDGE, (int)SlapValue.Variant.varUint32);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }
            }
        }
        else if (RecordType == ccsp_string)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_BRIDGE) + 10;
            pContext->pHomeSecurityBridge = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityBridge )
            {
                rc = sprintf_s(pContext->pHomeSecurityBridge, ulStrSize, "%s%s", DM_TR181_NAME_BRIDGE, SlapValue.Variant.varString);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    return  pValue_update(pValue,pValueSize, pContext->pHomeSecurityBridge);
}

ANSC_STATUS
CosaDmlMlanGetHomeSecurityBridgePorts
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    ULONG                           ulStrSize       = 0;
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pContext->pHomeSecurityBridgePorts )
    {
        SlapInitVariable(&SlapValue);

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_HsPorts,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter " DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_HsPorts
                    ", error code %d, type %d\n",
                    __FUNCTION__,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            ulStrSize = _ansc_strlen(SlapValue.Variant.varString) + 1;
            pContext->pHomeSecurityBridgePorts = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityBridgePorts )
            {
                rc = strcpy_s(pContext->pHomeSecurityBridgePorts, ulStrSize, SlapValue.Variant.varString);
                ERR_CHK(rc);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    return  pValue_update(pValue,pValueSize, pContext->pHomeSecurityBridgePorts);
}

ANSC_STATUS
CosaDmlMlanGetHomeSecurityDhcpv4ServerPool
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    ULONG                           ulStrSize       = 0;
    errno_t                         rc      = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pContext->pHomeSecurityDHCPv4ServerPool )
    {
        SlapInitVariable(&SlapValue);

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_DHCPv4ServerPool,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) )
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter " DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_DHCPv4ServerPool
                    ", error code %d, type %d\n",
                    __FUNCTION__,
                    iReturnValue,
                    RecordType
                ));
        }
        else if (RecordType == ccsp_unsignedInt)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_DHCPv4ServerPool) + 10;
            pContext->pHomeSecurityDHCPv4ServerPool = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityDHCPv4ServerPool )
            {
                rc = sprintf_s(pContext->pHomeSecurityDHCPv4ServerPool, ulStrSize, "%s%d", DM_TR181_NAME_DHCPv4ServerPool, (int)SlapValue.Variant.varUint32);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }
            }
        }
        else if (RecordType == ccsp_string)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_DHCPv4ServerPool) + 10;
            pContext->pHomeSecurityDHCPv4ServerPool = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityDHCPv4ServerPool )
            {
                rc = sprintf_s(pContext->pHomeSecurityDHCPv4ServerPool, ulStrSize, "%s%s", DM_TR181_NAME_DHCPv4ServerPool, SlapValue.Variant.varString);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    return  pValue_update(pValue,pValueSize, pContext->pHomeSecurityDHCPv4ServerPool);
}


ANSC_STATUS
CosaDmlMlanGetHomeSecurityWiFiRadio
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    ULONG                           ulStrSize       = 0;
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pContext->pHomeSecurityWiFiRadio )
    {
        SlapInitVariable(&SlapValue);

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_WiFiRadio,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_unsignedInt))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter " DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_WiFiRadio
                    ", error code %d, type %d\n",
                    __FUNCTION__,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_WiFi_Radio) + 10;
            pContext->pHomeSecurityWiFiRadio = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityWiFiRadio )
            {
                rc = sprintf_s(pContext->pHomeSecurityWiFiRadio, ulStrSize, "%s%d", DM_TR181_NAME_WiFi_Radio,(int)SlapValue.Variant.varUint32);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    return  pValue_update(pValue,pValueSize, pContext->pHomeSecurityWiFiRadio);
}


ANSC_STATUS
CosaDmlMlanGetHomeSecurityWiFiSsid
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    ULONG                           ulStrSize       = 0;
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pContext->pHomeSecurityWiFiSsid )
    {
        SlapInitVariable(&SlapValue);

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_WiFiSsid,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_unsignedInt))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter " DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_WiFiSsid
                    ", error code %d, type %d\n",
                    __FUNCTION__,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_WiFi_SSID) + 10;
            pContext->pHomeSecurityWiFiSsid = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityWiFiSsid )
            {
                rc = sprintf_s(pContext->pHomeSecurityWiFiSsid, ulStrSize, "%s%d", DM_TR181_NAME_WiFi_SSID, (int)SlapValue.Variant.varUint32);
                if(rc < EOK)
                {
                   ERR_CHK(rc);
                }
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    return  pValue_update(pValue,pValueSize, pContext->pHomeSecurityWiFiSsid);
}


ANSC_STATUS
CosaDmlMlanGetHomeSecurityWiFiAp
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    )
{
    PCOSA_DATAMODEL_MULTILAN        pContext        = (PCOSA_DATAMODEL_MULTILAN)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    ULONG                           ulStrSize       = 0;
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pContext->pHomeSecurityWiFiAp )
    {
        SlapInitVariable(&SlapValue);

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_WiFiAp,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_unsignedInt))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter " DMSB_TR181_PSM_MultiLAN_Root DMSB_TR181_PSM_MultiLAN_HomeSecurity_WiFiAp
                    ", error code %d, type %d\n",
                    __FUNCTION__,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_WiFi_AP) + 10;
            pContext->pHomeSecurityWiFiAp = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityWiFiAp )
            {
                rc = sprintf_s(pContext->pHomeSecurityWiFiAp, ulStrSize,  "%s%d", DM_TR181_NAME_WiFi_AP, (int)SlapValue.Variant.varUint32);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    return  pValue_update(pValue,pValueSize, pContext->pHomeSecurityWiFiAp);
}

