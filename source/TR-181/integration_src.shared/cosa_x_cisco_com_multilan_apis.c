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


/**********************************************************************
                            HELPER FUNCTIONS
**********************************************************************/

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
                _ansc_sprintf(pContext->pPrimaryLanIpIf, "%s%d", DM_TR181_NAME_IP_IF, (int)SlapValue.Variant.varUint32);
            }
        }
        else if (RecordType == ccsp_string)
        {
            ulStrSize                 = _ansc_strlen(DM_TR181_NAME_IP_IF) + 10;
            pContext->pPrimaryLanIpIf = AnscAllocateMemory(ulStrSize);

            if ( pContext->pPrimaryLanIpIf )
            {
                _ansc_sprintf(pContext->pPrimaryLanIpIf, "%s%s", DM_TR181_NAME_IP_IF, SlapValue.Variant.varString);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( pContext->pPrimaryLanIpIf )
    {
        ulStrSize = _ansc_strlen(pContext->pPrimaryLanIpIf);

        if ( ( ulStrSize + 1) <= *pValueSize )
        {
            AnscCopyString(pValue, pContext->pPrimaryLanIpIf);
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_MORE_DATA;
        }
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }
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
                _ansc_sprintf(pContext->pHomeSecurityIpIf, "%s%d", DM_TR181_NAME_IP_IF, (int)SlapValue.Variant.varUint32);
            }
        }
        else if (RecordType == ccsp_string)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_IP_IF) + 10;
            pContext->pHomeSecurityIpIf = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityIpIf )
            {
                _ansc_sprintf(pContext->pHomeSecurityIpIf, "%s%s", DM_TR181_NAME_IP_IF, SlapValue.Variant.varString);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( pContext->pHomeSecurityIpIf )
    {
        ulStrSize = _ansc_strlen(pContext->pHomeSecurityIpIf);

        if ( ( ulStrSize + 1) <= *pValueSize )
        {
            AnscCopyString(pValue, pContext->pHomeSecurityIpIf);
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_MORE_DATA;
        }
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }
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
                _ansc_sprintf(pContext->pPrimaryLanBridge, "%s%d", DM_TR181_NAME_BRIDGE,(int) SlapValue.Variant.varUint32);
            }
        }
        else if (RecordType == ccsp_string)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_BRIDGE) + 10;
            pContext->pPrimaryLanBridge = AnscAllocateMemory(ulStrSize);

            if ( pContext->pPrimaryLanBridge )
            {
                _ansc_sprintf(pContext->pPrimaryLanBridge, "%s%s", DM_TR181_NAME_BRIDGE, SlapValue.Variant.varString);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( pContext->pPrimaryLanBridge )
    {
        ulStrSize = _ansc_strlen(pContext->pPrimaryLanBridge);

        if ( ( ulStrSize + 1) <= *pValueSize )
        {
            AnscCopyString(pValue, pContext->pPrimaryLanBridge);
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_MORE_DATA;
        }
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }
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
            ulStrSize = _ansc_strlen(SlapValue.Variant.varString) + 3;
            pContext->pPrimaryLanBridgeHsPorts = AnscAllocateMemory(ulStrSize);

            if ( pContext->pPrimaryLanBridgeHsPorts )
            {
                AnscCopyString(pContext->pPrimaryLanBridgeHsPorts, SlapValue.Variant.varString);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( pContext->pPrimaryLanBridgeHsPorts )
    {
        ulStrSize = _ansc_strlen(pContext->pPrimaryLanBridgeHsPorts);

        if ( ( ulStrSize + 1) <= *pValueSize )
        {
            AnscCopyString(pValue, pContext->pPrimaryLanBridgeHsPorts);
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_MORE_DATA;
        }
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }
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
                _ansc_sprintf(pContext->pPrimaryLanDhcpv4ServerPool, "%s%d", DM_TR181_NAME_DHCPv4ServerPool, (int)SlapValue.Variant.varUint32);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( pContext->pPrimaryLanDhcpv4ServerPool )
    {
        ulStrSize = _ansc_strlen(pContext->pPrimaryLanDhcpv4ServerPool);

        if ( ( ulStrSize + 1) <= *pValueSize )
        {
            AnscCopyString(pValue, pContext->pPrimaryLanDhcpv4ServerPool);
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_MORE_DATA;
        }
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }
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
                _ansc_sprintf(pContext->pHomeSecurityBridge, "%s%d", DM_TR181_NAME_BRIDGE, (int)SlapValue.Variant.varUint32);
            }
        }
        else if (RecordType == ccsp_string)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_BRIDGE) + 10;
            pContext->pHomeSecurityBridge = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityBridge )
            {
                _ansc_sprintf(pContext->pHomeSecurityBridge, "%s%s", DM_TR181_NAME_BRIDGE, SlapValue.Variant.varString);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( pContext->pHomeSecurityBridge )
    {
        ulStrSize = _ansc_strlen(pContext->pHomeSecurityBridge);

        if ( ( ulStrSize + 1) <= *pValueSize )
        {
            AnscCopyString(pValue, pContext->pHomeSecurityBridge);
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_MORE_DATA;
        }
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }
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
            ulStrSize = _ansc_strlen(SlapValue.Variant.varString) + 3;
            pContext->pHomeSecurityBridgePorts = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityBridgePorts )
            {
                AnscCopyString(pContext->pHomeSecurityBridgePorts, SlapValue.Variant.varString);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( pContext->pHomeSecurityBridgePorts )
    {
        ulStrSize = _ansc_strlen(pContext->pHomeSecurityBridgePorts);

        if ( ( ulStrSize + 1) <= *pValueSize )
        {
            AnscCopyString(pValue, pContext->pHomeSecurityBridgePorts);
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_MORE_DATA;
        }
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }
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
                _ansc_sprintf(pContext->pHomeSecurityDHCPv4ServerPool, "%s%d", DM_TR181_NAME_DHCPv4ServerPool, (int)SlapValue.Variant.varUint32);
            }
        }
        else if (RecordType == ccsp_string)
        {
            ulStrSize = _ansc_strlen(DM_TR181_NAME_DHCPv4ServerPool) + 10;
            pContext->pHomeSecurityDHCPv4ServerPool = AnscAllocateMemory(ulStrSize);

            if ( pContext->pHomeSecurityDHCPv4ServerPool )
            {
                _ansc_sprintf(pContext->pHomeSecurityDHCPv4ServerPool, "%s%s", DM_TR181_NAME_DHCPv4ServerPool, SlapValue.Variant.varString);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( pContext->pHomeSecurityDHCPv4ServerPool )
    {
        ulStrSize = _ansc_strlen(pContext->pHomeSecurityDHCPv4ServerPool);

        if ( ( ulStrSize + 1) <= *pValueSize )
        {
            AnscCopyString(pValue, pContext->pHomeSecurityDHCPv4ServerPool);
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_MORE_DATA;
        }
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }
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
                _ansc_sprintf(pContext->pHomeSecurityWiFiRadio, "%s%d", DM_TR181_NAME_WiFi_Radio,(int)SlapValue.Variant.varUint32);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( pContext->pHomeSecurityWiFiRadio )
    {
        ulStrSize = _ansc_strlen(pContext->pHomeSecurityWiFiRadio);

        if ( ( ulStrSize + 1) <= *pValueSize )
        {
            AnscCopyString(pValue, pContext->pHomeSecurityWiFiRadio);
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_MORE_DATA;
        }
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }
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
                _ansc_sprintf(pContext->pHomeSecurityWiFiSsid, "%s%d", DM_TR181_NAME_WiFi_SSID, (int)SlapValue.Variant.varUint32);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( pContext->pHomeSecurityWiFiSsid )
    {
        ulStrSize = _ansc_strlen(pContext->pHomeSecurityWiFiSsid);

        if ( ( ulStrSize + 1) <= *pValueSize )
        {
            AnscCopyString(pValue, pContext->pHomeSecurityWiFiSsid);
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_MORE_DATA;
        }
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }
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
                _ansc_sprintf(pContext->pHomeSecurityWiFiAp, "%s%d", DM_TR181_NAME_WiFi_AP, (int)SlapValue.Variant.varUint32);
            }
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( pContext->pHomeSecurityWiFiAp )
    {
        ulStrSize = _ansc_strlen(pContext->pHomeSecurityWiFiAp);

        if ( ( ulStrSize + 1) <= *pValueSize )
        {
            AnscCopyString(pValue, pContext->pHomeSecurityWiFiAp);
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            *pValueSize = ulStrSize;

            return  ANSC_STATUS_MORE_DATA;
        }
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }
}

