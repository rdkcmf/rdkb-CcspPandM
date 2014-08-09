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

    module: cosa_bridging_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Li Shi

    -------------------------------------------------------------------

    revision:

        03/16/2011    initial revision.

**************************************************************************/

#include "cosa_bridging_apis.h"

#if defined _COSA_SIM_ 

COSA_PRI_BRG_FULL g_BrgFull[8]=
{
    {
        {2, "alias1", TRUE, FALSE, COSA_DML_BRG_STD_8021D_2004},
        {COSA_DML_BRG_STATUS_Disabled},
        2, 
        {
            {
                {
                    1, "portalias1", TRUE, COSA_DML_BRG_STD_8021D_2004, 
                    COSA_DML_BRG_LINK_TYPE_EthVlan, "eth0", TRUE, 0, 0, 0,
                    COSA_DML_BRG_PORT_AFT_AdmitAll, FALSE, FALSE,
                    COSA_DML_BPORT_TAGGING, TRUE, FALSE
                },
                {"Port1", COSA_DML_IF_STATUS_Up, 1000, COSA_DML_BRG_PORT_STATE_Disabled}
            },
            {
                {
                    2, "portalias2", TRUE, COSA_DML_BRG_STD_8021Q_2005,
                    COSA_DML_BRG_LINK_TYPE_Eth, "eth1", FALSE, 0, 0, 0,
                    COSA_DML_BRG_PORT_AFT_AdmitAll, FALSE, FALSE,
                    COSA_DML_BPORT_TAGGING, FALSE, FALSE
                },
                {"Port2", COSA_DML_IF_STATUS_Down, 100, COSA_DML_BRG_PORT_STATE_Blocking}
            }
        }
    },
    {
        {1, "alias2", TRUE, TRUE, COSA_DML_BRG_STD_8021Q_2005},
        {COSA_DML_BRG_STATUS_Enabled},
        2, 
        {
            {
                {
                    1, "portalias1", TRUE, COSA_DML_BRG_STD_8021D_2004,
                    COSA_DML_BRG_LINK_TYPE_EthVlan, "eth1", TRUE, 0, 0, 0,
                    COSA_DML_BRG_PORT_AFT_AdmitAll, FALSE, FALSE,
                    COSA_DML_BPORT_TAGGING, FALSE, TRUE
                },
                {"Port1", COSA_DML_IF_STATUS_Up, 10000, COSA_DML_BRG_PORT_STATE_Disabled}
            },
            {
                {
                    2, "portalias2", TRUE, COSA_DML_BRG_STD_8021Q_2005, 
                    COSA_DML_BRG_LINK_TYPE_Eth, "eth0", FALSE, 0, 0, 0,
                    COSA_DML_BRG_PORT_AFT_AdmitAll, FALSE, FALSE, 
                    COSA_DML_BPORT_TAGGING, FALSE, TRUE
                },
                {"Port2", COSA_DML_IF_STATUS_Down, 100000, COSA_DML_BRG_PORT_STATE_Blocking}
            }
        }
    },
    {0},
    {0},
    {0},
    {0},
    {0},
    {0}
};

COSA_DML_IF_STATS g_Stats=
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
};

ULONG    g_BrgFullNum = 2;
#endif


/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlBrgInit
            (
                ANSC_HANDLE                 hContext,
            );

    Description:

        Backend implementation to get the number of Ddns servies.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

    Return:       The number of Ddns servies.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
)
{
#if defined _COSA_SIM_ 
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlBrgGetNumberOfEntries
            (
                ANSC_HANDLE                 hContext,
            );

    Description:

        Backend implementation to get the number of Ddns servies.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

    Return:       The number of Ddns servies.

**********************************************************************/

ULONG
CosaDmlBrgGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
#if defined _COSA_SIM_ 
    return g_BrgFullNum;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgGetEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_BRG_FULL          pEntry
            );

    Description:

        Backend implementation to get a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_BRG_FULL          pEntry
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_FULL          pEntry
    )
{
#if defined _COSA_SIM_ 
    pEntry->Cfg.bEnabled = g_BrgFull[ulIndex].Cfg.bEnabled; //According to the data model, this should always return false.
    pEntry->Cfg.InstanceNumber = g_BrgFull[ulIndex].Cfg.InstanceNumber;
    pEntry->Cfg.Std = g_BrgFull[ulIndex].Cfg.Std;
    pEntry->Info.Status = g_BrgFull[ulIndex].Info.Status;
    AnscCopyString(pEntry->Cfg.Alias, g_BrgFull[ulIndex].Cfg.Alias);

    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgSetValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            );

    Description:

        Backend implementation to set back instance number and alias.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
#if defined _COSA_SIM_ 
    g_BrgFull[ulIndex].Cfg.InstanceNumber = ulInstanceNumber;
    AnscCopyString(g_BrgFull[ulIndex].Cfg.Alias, pAlias);

    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgAddEntry
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_CFG           pEntry
            );

    Description:

        Backend implementation to add a bridge service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_CFG           pEntry
                  Pointer of the service to be added.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pEntry
    )
{
#if defined _COSA_SIM_     
    if ( g_BrgFullNum >= 10 )
    {
        return ANSC_STATUS_FAILURE;
    }

    g_BrgFull[g_BrgFullNum].Cfg.bEnabled = pEntry->bEnabled;
    g_BrgFull[g_BrgFullNum].Cfg.InstanceNumber = pEntry->InstanceNumber;
    g_BrgFull[g_BrgFullNum].Cfg.Std = pEntry->Std;
    AnscCopyString(g_BrgFull[g_BrgFullNum].Cfg.Alias, pEntry->Alias);
    g_BrgFull[g_BrgFullNum].Info.Status = COSA_DML_BRG_STATUS_Disabled;

    g_BrgFullNum++;

    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgDelEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  ULONG                      ulInstanceNumber
                  Pointer of the service to be deleted.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
#if defined _COSA_SIM_   
    ULONG                           i = 0;
    ULONG                           j = 0;

    for ( i = 0; i < g_BrgFullNum; i++ )
    {
        if ( g_BrgFull[i].Cfg.InstanceNumber == ulInstanceNumber )
        {
            for ( j = i; j < g_BrgFullNum; j++ )
            {
                g_BrgFull[j].Cfg.bEnabled = g_BrgFull[j + 1].Cfg.bEnabled;
                g_BrgFull[j].Cfg.InstanceNumber = g_BrgFull[j + 1].Cfg.InstanceNumber;
                g_BrgFull[j].Cfg.Std = g_BrgFull[j + 1].Cfg.Std;
                AnscCopyString(g_BrgFull[j].Cfg.Alias, g_BrgFull[j + 1].Cfg.Alias);
                g_BrgFull[j].Info.Status = g_BrgFull[j + 1].Info.Status;
            }

            g_BrgFullNum--;

            break;
        }
    }

    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgSetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_CFG           pCfg
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  PCOSA_DML_BRG_CFG          pCfg
                  Pointer of the new service entry.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pCfg        /* Identified by InstanceNumber */
    )
{
#if defined _COSA_SIM_   
    ULONG                           i = 0;

    for ( i = 0; i < g_BrgFullNum; i++)
    {
        if ( g_BrgFull[i].Cfg.InstanceNumber == pCfg->InstanceNumber )
        {
            g_BrgFull[i].Cfg.bEnabled = pCfg->bEnabled;
            g_BrgFull[i].Cfg.Std = pCfg->Std;
            AnscCopyString(g_BrgFull[i].Cfg.Alias, pCfg->Alias);

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;    
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgGetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_CFG           pCfg
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_CFG           pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pCfg        /* Identified by InstanceNumber */
    )
{
#if defined _COSA_SIM_ 
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgGetInfo
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_INFO          pInfo
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_INFO          pInfo
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_BRG_INFO          pInfo
)
{
#if defined _COSA_SIM_ 
    if (!pInfo)
    {
        return ANSC_STATUS_FAILURE;
    }

    _ansc_memset(pInfo, 0, sizeof(COSA_DML_BRG_INFO));
    
    if (ulInstanceNumber == 1)
    {
        pInfo->Status = COSA_DML_BRG_STATUS_Disabled;
    }
    else if (ulInstanceNumber == 2)
    {
        pInfo->Status = COSA_DML_BRG_STATUS_Enabled;
    }
    else
    {
        pInfo->Status = COSA_DML_BRG_STATUS_Error;
    }

    return ANSC_STATUS_SUCCESS;
#endif
}

int CosaDmlBrgGetVLANID
    (
        ULONG                       ulInstanceNumber
    )
{
    return   0;
}

char * CosaDmlBrgGetName
    (
        ULONG                       ulInstanceNumber
    )
{
    /* DH  Temp fix for pc build */
    return  "brgtest";
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlBrgPortGetNumberOfEntries
            (
                ANSC_HANDLE                 hContext,
            );

    Description:

        Backend implementation to get the number of Ddns servies.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

    Return:       The number of Ddns servies.

**********************************************************************/

ULONG
CosaDmlBrgPortGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber
    )
{
#if defined _COSA_SIM_ 
    ULONG                           i = 0;

    for ( i = 0; i < g_BrgFullNum; i++ )
    {
        if ( g_BrgFull[i].Cfg.InstanceNumber == ulBrgInstanceNumber )
        {
            return g_BrgFull[i].ulNumOfPort;
        }
    }

    return 0;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortGetEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_BRG_PORT_FULL     pEntry
            );

    Description:

        Backend implementation to get a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_PORT_FULL     pEntry
    )
{
#if defined _COSA_SIM_ 
    ULONG                           i = 0;

    for ( i = 0; i < g_BrgFullNum; i++ )
    {
        if ( g_BrgFull[i].Cfg.InstanceNumber == ulBrgInstanceNumber )
        {
            pEntry->Cfg.AcceptableFrameTypes = g_BrgFull[i].PortList[ulIndex].Cfg.AcceptableFrameTypes;
            pEntry->Cfg.bEnabled = g_BrgFull[i].PortList[ulIndex].Cfg.bEnabled;
            pEntry->Cfg.bIngressFiltering = g_BrgFull[i].PortList[ulIndex].Cfg.bIngressFiltering;
            pEntry->Cfg.bManagementPort = g_BrgFull[i].PortList[ulIndex].Cfg.bManagementPort;
            pEntry->Cfg.bPriorityTagging = g_BrgFull[i].PortList[ulIndex].Cfg.bPriorityTagging;
            pEntry->Cfg.DftUserPriority = g_BrgFull[i].PortList[ulIndex].Cfg.DftUserPriority;
            pEntry->Cfg.InstanceNumber = g_BrgFull[i].PortList[ulIndex].Cfg.InstanceNumber;
            pEntry->Cfg.LinkType = g_BrgFull[i].PortList[ulIndex].Cfg.LinkType;
            pEntry->Cfg.PVID = g_BrgFull[i].PortList[ulIndex].Cfg.PVID;
            pEntry->Cfg.Std = g_BrgFull[i].PortList[ulIndex].Cfg.Std;

            pEntry->Cfg.mode = g_BrgFull[i].PortList[ulIndex].Cfg.mode;
            printf("%s: bridge=%d, port=%d, mode=%d\n", __FUNCTION__, i, ulIndex, pEntry->Cfg.mode);

            pEntry->Info.LastChange = g_BrgFull[i].PortList[ulIndex].Info.LastChange;
            pEntry->Info.PortState = g_BrgFull[i].PortList[ulIndex].Info.PortState;
            pEntry->Info.Status = g_BrgFull[i].PortList[ulIndex].Info.Status;

            AnscCopyString(pEntry->Cfg.PriorityRegeneration, g_BrgFull[i].PortList[ulIndex].Cfg.PriorityRegeneration);
            AnscCopyString(pEntry->Cfg.LinkName, g_BrgFull[i].PortList[ulIndex].Cfg.LinkName);
            AnscCopyString(pEntry->Cfg.Alias, g_BrgFull[i].PortList[ulIndex].Cfg.Alias);
            AnscCopyString(pEntry->Info.Name, g_BrgFull[i].PortList[ulIndex].Info.Name);

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortSetValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            );

    Description:

        Backend implementation to set back instance number and alias.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
#if defined _COSA_SIM_ 
    ULONG                           i = 0;

    for ( i = 0; i < g_BrgFullNum; i++ )
    {
        if ( g_BrgFull[i].Cfg.InstanceNumber == ulBrgInstanceNumber )
        {
            g_BrgFull[i].PortList[ulIndex].Cfg.InstanceNumber = ulInstanceNumber;
            AnscCopyString(g_BrgFull[i].PortList[ulIndex].Cfg.Alias, pAlias);

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortAddEntry
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_PORT_CFG      pEntry
            );

    Description:

        Backend implementation to add a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_PORT_CFG      pEntry
                  Pointer of the service to be added.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pEntry
    )
{
#if defined _COSA_SIM_ 
    ULONG                           i = 0;

    for ( i = 0; i < g_BrgFullNum; i++ )
    {
        if ( g_BrgFull[i].Cfg.InstanceNumber == ulBrgInstanceNumber )
        {
            if ( g_BrgFull[i].ulNumOfPort >= 10 )
            {
                return ANSC_STATUS_FAILURE;
            }

            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.AcceptableFrameTypes = pEntry->AcceptableFrameTypes;
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.bEnabled = pEntry->bEnabled;
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.bIngressFiltering = pEntry->bIngressFiltering;
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.bManagementPort = pEntry->bManagementPort;
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.bPriorityTagging = pEntry->bPriorityTagging;
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.DftUserPriority = pEntry->DftUserPriority;
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.InstanceNumber = pEntry->InstanceNumber;
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.LinkType = pEntry->LinkType;
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.PVID = pEntry->PVID;
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.Std = pEntry->Std;
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.mode = pEntry->mode;
            printf("%s: bridge=%d, port=%d, mode=%d\n", __FUNCTION__, i, g_BrgFull[i].ulNumOfPort, pEntry->mode);
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Info.LastChange = AnscGetTickInSeconds();
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Info.PortState = COSA_DML_BRG_PORT_STATE_Disabled;
            g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Info.Status = COSA_DML_IF_STATUS_Up;

            AnscCopyString(g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.PriorityRegeneration, pEntry->PriorityRegeneration);
            AnscCopyString(g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.LinkName, pEntry->LinkName);
            AnscCopyString(g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Cfg.Alias, pEntry->Alias);
            AnscCopyString(g_BrgFull[i].PortList[g_BrgFull[i].ulNumOfPort].Info.Name, "infox");

            g_BrgFull[i].ulNumOfPort++;

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortDelEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  ULONG                      ulInstanceNumber
                  Pointer of the service to be deleted.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{
#if defined _COSA_SIM_ 
    ULONG                           i = 0;
    ULONG                           j = 0;
    ULONG                           k = 0;

    for ( k = 0; k < g_BrgFullNum; k++ )
    {
        if ( g_BrgFull[k].Cfg.InstanceNumber == ulBrgInstanceNumber )
        {
            for ( i = 0; i < g_BrgFull[k].ulNumOfPort; i++ )
            {
                if ( g_BrgFull[k].PortList[i].Cfg.InstanceNumber == ulInstanceNumber )
                {
                    for ( j = i; j < g_BrgFull[k].ulNumOfPort; j++ )
                    {
                        g_BrgFull[k].PortList[j].Cfg.AcceptableFrameTypes = g_BrgFull[k].PortList[j + 1].Cfg.AcceptableFrameTypes;
                        g_BrgFull[k].PortList[j].Cfg.bEnabled = g_BrgFull[k].PortList[j + 1].Cfg.bEnabled;
                        g_BrgFull[k].PortList[j].Cfg.bIngressFiltering = g_BrgFull[k].PortList[j + 1].Cfg.bIngressFiltering;
                        g_BrgFull[k].PortList[j].Cfg.bManagementPort = g_BrgFull[k].PortList[j + 1].Cfg.bManagementPort;
                        g_BrgFull[k].PortList[j].Cfg.bPriorityTagging = g_BrgFull[k].PortList[j + 1].Cfg.bPriorityTagging;
                        g_BrgFull[k].PortList[j].Cfg.DftUserPriority = g_BrgFull[k].PortList[j + 1].Cfg.DftUserPriority;
                        g_BrgFull[k].PortList[j].Cfg.InstanceNumber = g_BrgFull[k].PortList[j + 1].Cfg.InstanceNumber;
                        g_BrgFull[k].PortList[j].Cfg.LinkType = g_BrgFull[k].PortList[j + 1].Cfg.LinkType;
                        g_BrgFull[k].PortList[j].Cfg.PVID = g_BrgFull[k].PortList[j + 1].Cfg.PVID;
                        g_BrgFull[k].PortList[j].Cfg.Std = g_BrgFull[k].PortList[j + 1].Cfg.Std;
                        g_BrgFull[k].PortList[j].Cfg.mode = g_BrgFull[k].PortList[j + 1].Cfg.mode;
                        g_BrgFull[k].PortList[j].Info.LastChange = g_BrgFull[k].PortList[j + 1].Info.LastChange;
                        g_BrgFull[k].PortList[j].Info.PortState = g_BrgFull[k].PortList[j + 1].Info.PortState;
                        g_BrgFull[k].PortList[j].Info.Status = g_BrgFull[k].PortList[j + 1].Info.Status;

                        AnscCopyString(g_BrgFull[k].PortList[j].Cfg.PriorityRegeneration, g_BrgFull[k].PortList[j + 1].Cfg.PriorityRegeneration);
                        AnscCopyString(g_BrgFull[k].PortList[j].Cfg.LinkName, g_BrgFull[k].PortList[j + 1].Cfg.LinkName);
                        AnscCopyString(g_BrgFull[k].PortList[j].Cfg.Alias, g_BrgFull[k].PortList[j + 1].Cfg.Alias);
                        AnscCopyString(g_BrgFull[k].PortList[j].Info.Name, g_BrgFull[k].PortList[j + 1].Info.Name);
                    }

                    g_BrgFull[k].ulNumOfPort--;

                    return ANSC_STATUS_SUCCESS;
                }
            }
        }
    }

    return ANSC_STATUS_CANT_FIND;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortSetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_PORT_CFG      pCfg
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  PCOSA_DML_BRG_CFG          pCfg
                  Pointer of the new service entry.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortSetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    )
{
#if defined _COSA_SIM_ 
    ULONG                           i = 0;
    ULONG                           k = 0;

    for ( k = 0; k < g_BrgFullNum; k++ )
    {
        if ( g_BrgFull[k].Cfg.InstanceNumber == ulBrgInstanceNumber )
        {
            for ( i = 0; i < g_BrgFull[k].ulNumOfPort; i++)
            {
                if ( g_BrgFull[k].PortList[i].Cfg.InstanceNumber == pCfg->InstanceNumber )
                {
                    g_BrgFull[k].PortList[i].Cfg.AcceptableFrameTypes = pCfg->AcceptableFrameTypes;
                    g_BrgFull[k].PortList[i].Cfg.bEnabled = pCfg->bEnabled;
                    g_BrgFull[k].PortList[i].Cfg.bIngressFiltering = pCfg->bIngressFiltering;
                    g_BrgFull[k].PortList[i].Cfg.bManagementPort = pCfg->bManagementPort;
                    g_BrgFull[k].PortList[i].Cfg.bPriorityTagging = pCfg->bPriorityTagging;
                    g_BrgFull[k].PortList[i].Cfg.DftUserPriority = pCfg->DftUserPriority;
                    g_BrgFull[k].PortList[i].Cfg.InstanceNumber = pCfg->InstanceNumber;
                    g_BrgFull[k].PortList[i].Cfg.LinkType = pCfg->LinkType;
                    g_BrgFull[k].PortList[i].Cfg.PVID = pCfg->PVID;
                    g_BrgFull[k].PortList[i].Cfg.Std = pCfg->Std;
                    g_BrgFull[k].PortList[i].Cfg.mode = pCfg->mode;

                    AnscCopyString(g_BrgFull[k].PortList[i].Cfg.PriorityRegeneration, pCfg->PriorityRegeneration);
                    AnscCopyString(g_BrgFull[k].PortList[i].Cfg.LinkName, pCfg->LinkName);
                    AnscCopyString(g_BrgFull[k].PortList[i].Cfg.Alias, pCfg->Alias);

                    return ANSC_STATUS_SUCCESS;
                }
            }
        }
    }

    return ANSC_STATUS_CANT_FIND;  
#endif  
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortGetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_PORT_CFG      pCfg
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_PORT_CFG      pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

#ifdef _COSA_SIM_
///Maps a bridge instance number to a backend object
PCOSA_PRI_BRG_FULL getBridge(ULONG instanceNumber) 
{
    int i;
    for (i=0; i<g_BrgFullNum; i++)
        if (g_BrgFull[i].Cfg.InstanceNumber == instanceNumber)
            return &(g_BrgFull[i]);
    if (i == g_BrgFullNum) return NULL;
}

///Maps a bridge port instance to a backend object
PCOSA_DML_BRG_PORT_FULL getBPort(PCOSA_PRI_BRG_FULL pBridge, ULONG ulInstanceNumber)
{
    int i;
    for (i=0; i<pBridge->ulNumOfPort; i++)
        if (pBridge->PortList[i].Cfg.InstanceNumber == ulInstanceNumber)
            return &(pBridge->PortList[i]);
    if (i == pBridge->ulNumOfPort) return NULL;
}
#endif

ANSC_STATUS
CosaDmlBrgPortGetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    )
{
#if defined _COSA_SIM_ 

    PCOSA_PRI_BRG_FULL pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }

    PCOSA_DML_BRG_PORT_FULL pBPort = getBPort(pBridge, pCfg->InstanceNumber);
    if (pBPort == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }

    memcpy(pCfg, &(pBPort->Cfg), sizeof(COSA_DML_BRG_PORT_CFG)); 
    //    printf("%s: bridge=%d, port=%d, mode=%d\n", __FUNCTION__, 
    //           ulBrgInstanceNumber, pCfg->InstanceNumber, pCfg->mode); // pBPort->Cfg.mode);    
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortGetInfo
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_PORT_INFO     pInfo
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_PORT_INFO     pInfo
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_BRG_PORT_INFO     pInfo
    )
{
#if defined _COSA_SIM_ 
    if (!pInfo)
    {
        return ANSC_STATUS_FAILURE;
    }

    _ansc_memset(pInfo, 0, sizeof(COSA_DML_BRG_PORT_INFO));
    
    if (ulInstanceNumber == 1)
    {
        pInfo->Status = COSA_DML_IF_STATUS_Down;
        pInfo->LastChange = 11;
        pInfo->PortState = COSA_DML_BRG_PORT_STATE_Disabled;
    }
    else if (ulInstanceNumber == 2)
    {
        pInfo->Status = COSA_DML_IF_STATUS_Up;
        pInfo->LastChange = 22;
        pInfo->PortState = COSA_DML_BRG_PORT_STATE_Blocking;
    }
    else
    {
        pInfo->Status = COSA_DML_IF_STATUS_Unknown;
        pInfo->LastChange = 33;
        pInfo->PortState = COSA_DML_BRG_PORT_STATE_Listening;
    }

    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortGetStats
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_IF_STATS          pInfo
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_IF_STATS          pInfo
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IF_STATS          pStats
    )
{
#if defined _COSA_SIM_
    pStats->BroadcastPacketsReceived = g_Stats.BroadcastPacketsReceived;
    pStats->BroadcastPacketsSent = g_Stats.BroadcastPacketsSent;
    pStats->BytesReceived = g_Stats.BytesReceived;
    pStats->BytesSent = g_Stats.BytesSent;
    pStats->DiscardPacketsReceived = g_Stats.DiscardPacketsReceived;
    pStats->DiscardPacketsSent = g_Stats.DiscardPacketsSent;
    pStats->ErrorsReceived = g_Stats.ErrorsReceived;
    pStats->ErrorsSent = g_Stats.ErrorsSent;
    pStats->MulticastPacketsReceived = g_Stats.MulticastPacketsReceived;
    pStats->MulticastPacketsSent = g_Stats.MulticastPacketsSent;
    pStats->PacketsReceived = g_Stats.PacketsReceived;
    pStats->PacketsSent = g_Stats.PacketsSent;
    pStats->UnicastPacketsReceived = g_Stats.UnicastPacketsReceived;
    pStats->UnicastPacketsSent = g_Stats.UnicastPacketsSent;
    pStats->UnknownProtoPacketsReceived = g_Stats.UnknownProtoPacketsReceived;

    return ANSC_STATUS_SUCCESS;
#endif
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgVlanAddEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulBrgInstanceNumber,
                int                         VLANID
            );

    Description:

        Backend implementation to add a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulBrgInstanceNumber
                  Instance number of bridge

                  PCOSA_DML_BRG_VLAN_FULL     pVLAN
                  pointer to VLAN data structure

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgVlanAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_VLAN_FULL     pVLAN 
    )
{
#if defined _COSA_SIM_ 
    return ANSC_STATUS_CANT_FIND;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgVlanDelEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulBrgInstanceNumber,
                int                         VLANID
            );

    Description:

        Backend implementation to delete a bridge vlan entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  ULONG                      ulBrgInstanceNumber
                  Instance number of the bridge

                  int                        VLANID
                  VLANID

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgVlanDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        int                         VLANID
    )
{
#if defined _COSA_SIM_ 
    return ANSC_STATUS_CANT_FIND;
#endif
}


ULONG
CosaDmlBrgVlanGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber
    )
{
    return  0;
}


ANSC_STATUS
CosaDmlBrgVlanSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlBrgVlanGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_VLAN_FULL     pEntry
    )
{
    return  ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlBrgVlanSetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_VLAN_CFG      pCfg
    )
{
    return  ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgVlanPortAddEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulBrgInstanceNumber,
                int                         VLANID,
                ULONG                       ulPortInstanceNumber,
                BOOLEAN                     bUntagged
            );

    Description:

        Backend implementation to add a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulBrgInstanceNumber
                  Instance number of bridge

                  int                         VLANID

                  ULONG                       ulPortInstanceNumber
                  Instance number of bridge port

                  BOOLEAN                     bUntagged
                  indicated egress packets will be untagged

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgVlanPortAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        int                         VLANID,
        ULONG                       ulPortInstanceNumber,
        BOOLEAN                     bUntagged
    )
{
#if defined _COSA_SIM_ 
    return ANSC_STATUS_CANT_FIND;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgVlanPortDelEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulBrgInstanceNumber,
                int                         VLANID,
                ULONG                       ulPortInstanceNumber,
            );

    Description:

        Backend implementation to delete a bridge vlan entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  ULONG                      ulBrgInstanceNumber
                  Instance number of the bridge

                  ULONG                      ulInstanceNumber
                  Pointer of the bridge vlan to be deleted.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgVlanPortDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        int                         VLANID,
        ULONG                       ulPortInstanceNumber
    )
{
#if defined _COSA_SIM_ 
    return ANSC_STATUS_CANT_FIND;
#endif
}

