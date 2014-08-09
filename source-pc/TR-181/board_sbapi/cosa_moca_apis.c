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

    module: cosa_moca_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaMoCACreate
        *  CosaMoCAInitialize
        *  CosaMoCARemove
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "cosa_moca_apis.h"
#include "cosa_moca_internal.h"

/* Just fill in some basic required parameters */
COSA_DML_MOCA_IF_CFG   g_MoCAIfCfg1 = 
    {  
       .bEnabled = FALSE,
       .X_CISCO_COM_Reset = FALSE,
       .X_CISCO_COM_MixedMode = FALSE,
       .X_CISCO_COM_ChannelScanning = FALSE,
       .X_CISCO_COM_AutoPowerControlEnable = FALSE,
       .AutoPowerControlPhyRate = 235,
       .X_CISCO_COM_EnableTabooBit = FALSE,
       .PrivacyEnabledSetting = FALSE,
       .KeyPassphrase = "KeyPassphrase",
       .bPreferredNC = FALSE,
       .FreqCurrentMaskSetting = "D1,D3,D5a,D10a",
       .X_CISCO_COM_ChannelScanMask = "D1,D3,D5a,D10a",
       .TxPowerLimit = -29,
       .BeaconPowerLimit = 0
    };

/* Just fill in some basic required parameters */
COSA_DML_MOCA_IF_CFG   g_MoCAIfCfg2 = 
    {
       .bEnabled = FALSE,
       .X_CISCO_COM_Reset = FALSE,
       .X_CISCO_COM_MixedMode = FALSE,
       .X_CISCO_COM_ChannelScanning = FALSE,
       .X_CISCO_COM_AutoPowerControlEnable = FALSE,
       .AutoPowerControlPhyRate = 235,
       .X_CISCO_COM_EnableTabooBit = FALSE,
       .PrivacyEnabledSetting = FALSE,
       .KeyPassphrase = "KeyPassphrase",
       .bPreferredNC = FALSE,
       .FreqCurrentMaskSetting = "D1,D3,D10,D10a",
       .X_CISCO_COM_ChannelScanMask = "D1,D3,D5a,D10a",
       .TxPowerLimit = -29,
       .BeaconPowerLimit = 0
    };

COSA_DML_MOCA_CFG     g_MoCACfg = 
    { 
        "X_CISCO_COM_ProvisioningFilename",
        "2031:0000:1F1F:0000:0000:0100:11A0:ADDF", 
        MOCA_PROVISIONING_SERVADDR_TYPE_IPV6
    };

COSA_DML_MOCA_ASSOC_DEVICE   g_MoCAAssocDevice[5] = 
    { 
        {   
            .MACAddress= "00:00:0c:9f:f0:01",
            .NodeID = 1, 
            .TxPackets=6601,
            .RxErroredAndMissedPackets = 6602,
            .PHYTxRate = 6603,
            .PHYRxRate = 6604,
            .TxBcastRate = 6605,
            .X_CISCO_COM_RxBcastRate = 6606, 
        },
        {   
            .MACAddress= "00:00:0c:9f:f0:02",
            .NodeID = 2, 
            .TxPackets=6607,
            .RxErroredAndMissedPackets = 6608,
            .PHYTxRate = 6609,
            .PHYRxRate = 6610,
            .TxBcastRate = 6611,
            .X_CISCO_COM_RxBcastRate = 6612, 
        },
        {   
            .MACAddress= "00:00:0c:9f:f0:03",
            .NodeID = 3, 
            .TxPackets=6613,
            .RxErroredAndMissedPackets = 6614,
            .PHYTxRate = 6615,
            .PHYRxRate = 6616,
            .TxBcastRate = 6617,
            .X_CISCO_COM_RxBcastRate = 6618, 
        },
        {   
            .MACAddress= "00:00:0c:9f:f0:04",
            .NodeID = 4, 
            .TxPackets=6619,
            .RxErroredAndMissedPackets = 6620,
            .PHYTxRate = 6621,
            .PHYRxRate = 6622,
            .TxBcastRate = 6623,
            .X_CISCO_COM_RxBcastRate = 6624, 
        },
        {   
            .MACAddress= "00:00:0c:9f:f0:05",
            .NodeID = 5, 
            .TxPackets=6625,
            .RxErroredAndMissedPackets = 6626,
            .PHYTxRate = 6627,
            .PHYRxRate = 6628,
            .TxBcastRate = 6629,
            .X_CISCO_COM_RxBcastRate = 6630, 
        }
    };


COSA_DML_MOCA_EXTCOUNTER g_MoCAExtCounter[5] = 
    {
        {
            .Type=1,
            .Map=6601,
            .Rsrv=6602,
            .Lc=6603,
            .Adm=6604,
            .Probe=6605,
            .Async=6606
        },
        {
            .Type=2,
            .Map=6607,
            .Rsrv=6608,
            .Lc=6609,
            .Adm=6610,
            .Probe=6611,
            .Async=6612
        },
        {
            .Type=3,
            .Map=6613,
            .Rsrv=6614,
            .Lc=6615,
            .Adm=6616,
            .Probe=6617,
            .Async=6618
        },
        {
            .Type=4,
            .Map=6619,
            .Rsrv=6620,
            .Lc=6621,
            .Adm=6622,
            .Probe=6623,
            .Async=6624
        },
        {
            .Type=5,
            .Map=6625,
            .Rsrv=6626,
            .Lc=6627,
            .Adm=6628,
            .Probe=6629,
            .Async=6630
        }
    };

COSA_DML_MOCA_EXTAGGRCOUNTER g_MoCAExtAggrCounter[2] = 
    {
        {
            .Tx=6601,
            .Rx=6602
        },
        {
            .Tx=6603,
            .Rx=6604
        }
    };


COSA_DML_MOCA_PEER  g_MoCAPeer[2] = 
    {
        {
            6601
        },
        {
            6602
        }
    };

COSA_DML_MOCA_FLOW  g_MoCAFlow[2] = 
    {
        {
            .FlowID=1,
            .IngressNodeID=6601,
            .EgressNodeID=6602,
            .FlowTimeLeft=6603,
            .DestinationMACAddress="01:00:5e:00:00:fc",
            .PacketSize=6604,
            .PeakDataRate=6605,
            .BurstSize=6606,
            .FlowTag=6607
        },
        {
            .FlowID=2,
            .IngressNodeID=6608,
            .EgressNodeID=6609,
            .FlowTimeLeft=6610,
            .DestinationMACAddress="01:00:5e:00:00:fc",
            .PacketSize=6611,
            .PeakDataRate=6612,
            .BurstSize=6613,
            .FlowTag=6614
        }
    };

ANSC_STATUS
CosaDmlMocaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
)
{
    PCOSA_DATAMODEL_MOCA  pMyObject    = (PCOSA_DATAMODEL_MOCA)phContext;

    AnscTraceWarning(("CosaDmlMocaInit -- \n"));
    

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMocaGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MOCA_CFG          pCfg
    )
{
    AnscTraceWarning(("CosaDmlMocaIfGetCfg -- .\n"));

    *pCfg = g_MoCACfg;
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMocaSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MOCA_CFG          pCfg
    )
{
    AnscTraceWarning(("CosaDmlMocaSetCfg -- %s %s %s.\n", 
        pCfg->X_CISCO_COM_ProvisioningFilename, 
        pCfg->X_CISCO_COM_ProvisioningServerAddress, 
        (pCfg->X_CISCO_COM_ProvisioningServerAddressType==1)?"IPv4":"IPv6"));


    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlMocaGetNumberOfIfs
    (
        ANSC_HANDLE                 hContext
    )
{
    AnscTraceWarning(("CosaDmlMocaGetNumberOfIfs -- Number:%d.\n", MOCA_INTEFACE_NUMBER));

    return MOCA_INTEFACE_NUMBER;
}

ANSC_STATUS
CosaDmlMocaIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInterfaceIndex,
        PCOSA_DML_MOCA_IF_FULL      pEntry
    )
{
    AnscTraceWarning(("CosaDmlMocaIfGetEntry -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));

    if (pEntry)
    {
        _ansc_memset(pEntry, 0, sizeof(COSA_DML_MOCA_IF_FULL));
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }
        
    if (ulInterfaceIndex == 0)
    {
        AnscCopyMemory(&pEntry->Cfg, &g_MoCAIfCfg1, sizeof(COSA_DML_MOCA_IF_CFG));

        /* Just fill in USGv2 required parameters */
        _ansc_strcpy((PCHAR)pEntry->StaticInfo.MacAddress, "00:24:D7:24:AD:20");
        pEntry->DynamicInfo.NodeID  = 6;
        pEntry->StaticInfo.X_CISCO_COM_CycleMaster  = TRUE;
        pEntry->DynamicInfo.BackupNC = 6666;
    }
    else if (ulInterfaceIndex == 1)
    {
        AnscCopyMemory(&pEntry->Cfg, &g_MoCAIfCfg2, sizeof(COSA_DML_MOCA_IF_CFG));

        /* Just fill in USGv2 required parameters */
        _ansc_strcpy((PCHAR)pEntry->StaticInfo.MacAddress, "00:24:D7:24:AD:21");
        pEntry->DynamicInfo.NodeID  = 66;
        pEntry->StaticInfo.X_CISCO_COM_CycleMaster  = FALSE;
        pEntry->DynamicInfo.BackupNC = 6;
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMocaIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInterfaceIndex,
        PCOSA_DML_MOCA_IF_CFG       pCfg
    )
{
    AnscTraceWarning(("CosaDmlMocaIfSetCfg -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));
    
    if ( !pCfg )
    {
        return ANSC_STATUS_FAILURE;
    }
    
    if ( ulInterfaceIndex == 0 )
    {
        AnscCopyMemory(&g_MoCAIfCfg1, pCfg, sizeof(COSA_DML_MOCA_IF_CFG));
    }
    else if ( ulInterfaceIndex == 1 )
    {
        AnscCopyMemory(&g_MoCAIfCfg2, pCfg, sizeof(COSA_DML_MOCA_IF_CFG));
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMocaIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       uIndex,
        PCOSA_DML_MOCA_IF_CFG       pCfg
    )
{
    AnscTraceWarning(("CosaDmlMocaIfGetCfg -- ulInterfaceIndex:%lu.\n", uIndex));
    
    if ( !pCfg )
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( uIndex == 0 )
    {
        AnscCopyMemory(pCfg, &g_MoCAIfCfg1, sizeof(COSA_DML_MOCA_IF_CFG));
    }
    else if ( uIndex == 1 )
    {
        AnscCopyMemory(pCfg, &g_MoCAIfCfg2, sizeof(COSA_DML_MOCA_IF_CFG));
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
    
}


ANSC_STATUS
CosaDmlMocaIfGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInterfaceIndex,
        PCOSA_DML_MOCA_IF_DINFO     pInfo
    )
{
    AnscTraceWarning(("CosaDmlMocaIfGetDinfo -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));

    if (!pInfo)
    {
        return ANSC_STATUS_FAILURE;
    }

    _ansc_memset(pInfo, 0, sizeof(COSA_DML_MOCA_IF_DINFO));
        
    if (ulInterfaceIndex == 0)
    {
        pInfo->LastChange = 11;
        pInfo->Status = COSA_DML_IF_STATUS_Up;
        _ansc_strcpy(pInfo->X_CISCO_NetworkCoordinatorMACAddress, "00:24:D7:24:AD:22");
        pInfo->X_CISCO_COM_BestNetworkCoordinatorID = 666;
    }
    else if (ulInterfaceIndex == 1)
    {
        pInfo->LastChange = 22;
        pInfo->Status = COSA_DML_IF_STATUS_Up;
        _ansc_strcpy(pInfo->X_CISCO_NetworkCoordinatorMACAddress, "00:24:D7:24:AD:23");
        pInfo->X_CISCO_COM_BestNetworkCoordinatorID = 6666;
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }
        
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMocaIfGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInterfaceIndex,
        PCOSA_DML_MOCA_STATS        pStats
    )
{
    AnscTraceWarning(("CosaDmlMocaIfGetDinfo -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));

    if ( !pStats )
    {
        return ANSC_STATUS_FAILURE;
    }

    _ansc_memset(pStats, 0, sizeof(COSA_DML_MOCA_STATS));

    if ( ulInterfaceIndex == 0 )
    {
        pStats->BytesSent = 111;
        pStats->X_CISCO_COM_ExtAggrAverageTx = 6661;
        pStats->X_CISCO_COM_ExtAggrAverageRx = 6662;
    }
    else if ( ulInterfaceIndex == 1 )
    {
        pStats->BytesSent = 222;
        pStats->X_CISCO_COM_ExtAggrAverageTx = 6663;
        pStats->X_CISCO_COM_ExtAggrAverageRx = 6664;
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMocaIfResetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInterfaceIndex
    )
{
    AnscTraceWarning(("CosaDmlMocaIfResetStats -- ulInterfaceIndex:%lu\n", ulInterfaceIndex));


    return ANSC_STATUS_SUCCESS;
}


ULONG
CosaDmlMocaIfExtCounterGetNumber
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex
    )
{
    AnscTraceWarning(("CosaDmlMocaIfExtCounterGetNumber -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));

    if ( ulInterfaceIndex == 0 )
    {
        return 3;
    }
    else if ( ulInterfaceIndex == 1 )
    {
        return 2;
    }
    else
    {
        AnscTraceWarning(("CosaDmlMocaIfExtCounterGetNumber -- Error, not such Entry\n"));
        return 0;
    }

}

/*
    ANSC_HANDLE   :  Hardcoded to NULL currently.
    ulInterfaceIndex : it's the index of Interface.
    ulIndex              : it's the index of this ExtCounter.
    PCOSA_CONTEXT_MOCA_LINK_OBJECT

    return value: if successful, return ANSC_STATUS_SUCCESS.
                        if fail,           return ANSC_STATUS_FAILURE
*/
ANSC_STATUS
CosaDmlMocaIfExtCounterGetEntry
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex,
        ULONG                            ulIndex,
        PCOSA_DML_MOCA_EXTCOUNTER        pConf
    )
{
    AnscTraceWarning(("CosaDmlMocaIfExtCounterGetEntry -- ulInterfaceIndex:%lu, ulIndex:%lu\n", ulInterfaceIndex, ulIndex));

    if ( ulInterfaceIndex == 0 )
    {
        if (ulIndex >=3 )
        {
            AnscTraceWarning(("CosaDmlMocaIfExtCounterGetEntry -- Error, not such Entry\n"));
            return ANSC_STATUS_FAILURE;
        }
        
        *pConf = g_MoCAExtCounter[ulIndex];
    }
    else if ( ulInterfaceIndex == 1 )
    {
        if (ulIndex >=2 )
        {
            AnscTraceWarning(("CosaDmlMocaIfExtCounterGetEntry -- Error, not such Entry\n"));
            return ANSC_STATUS_FAILURE;
        }
        
        *pConf = g_MoCAExtCounter[ulIndex+3];
    }
    else
    {
        AnscTraceWarning(("CosaDmlMocaIfExtCounterGetEntry -- Error, not such Entry\n"));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}


ULONG
CosaDmlMocaIfExtAggrCounterGetNumber
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex
    )
{
    AnscTraceWarning(("CosaDmlMocaIfExtAggrCounterGetNumber -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));

    if ( ulInterfaceIndex == 0 )
    {
        return 1;
    }
    else if ( ulInterfaceIndex == 1 )
    {
        return 1;
    }
    else
    {
        AnscTraceWarning(("CosaDmlMocaIfExtAggrCounterGetNumber -- Error, not such Entry\n"));
        return 0;
    }

}

/*
    ANSC_HANDLE   :  Hardcoded to NULL currently.
    ulInterfaceIndex : it's the index of Interface.
    ulIndex              : it's the index of this ExtCounter.
    PCOSA_CONTEXT_MOCA_LINK_OBJECT

    return value: if successful, return ANSC_STATUS_SUCCESS.
                        if fail,           return ANSC_STATUS_FAILURE
*/
ANSC_STATUS
CosaDmlMocaIfExtAggrCounterGetEntry
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex,
        ULONG                            ulIndex,
        PCOSA_DML_MOCA_EXTAGGRCOUNTER    pConf
    )
{
    AnscTraceWarning(("CosaDmlMocaIfExtAggrCounterGetEntry -- ulInterfaceIndex:%lu, ulIndex:%lu\n", ulInterfaceIndex, ulIndex));

    if ( ulInterfaceIndex == 0 )
    {
        if (ulIndex >=1 )
        {
            AnscTraceWarning(("CosaDmlMocaIfExtAggrCounterGetEntry -- Error, not such Entry\n"));
            return ANSC_STATUS_FAILURE;
        }

        *pConf = g_MoCAExtAggrCounter[0];
    }
    else if ( ulInterfaceIndex == 1 )
    {
        if (ulIndex >=1 )
        {
            AnscTraceWarning(("CosaDmlMocaIfExtAggrCounterGetEntry -- Error, not such Entry\n"));
            return ANSC_STATUS_FAILURE;
        }

        *pConf = g_MoCAExtAggrCounter[1];
    }
    else
    {
        AnscTraceWarning(("CosaDmlMocaIfExtAggrCounterGetEntry -- Error, not such Entry\n"));
        return ANSC_STATUS_FAILURE;
    }
    
    return ANSC_STATUS_SUCCESS;
}

/*
    This function is used to get total Peer tables.
    The returned memory should be allocated by AnscAllocateMemory. Or else there is leaking.
*/
ANSC_STATUS
CosaDmlMocaIfPeerTableGetTable
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex,
        PCOSA_DML_MOCA_PEER             *ppConf,
        PULONG                           pCount
    )
{
    AnscTraceWarning(("CosaDmlMocaIfPeerTableGetTable -- ulInterfaceIndex:%lu, ppConf:%x\n", ulInterfaceIndex, (UINT)ppConf));

    *ppConf = (PCOSA_DML_MOCA_PEER)AnscAllocateMemory(sizeof(g_MoCAPeer));
    if ( !ppConf )
    {
        *ppConf = NULL;
        *pCount = 0;
        return ANSC_STATUS_FAILURE;
    }
    
    AnscCopyMemory( *ppConf, &g_MoCAPeer, sizeof(g_MoCAPeer) );
    *pCount = sizeof(g_MoCAPeer)/sizeof(COSA_DML_MOCA_PEER);

    return ANSC_STATUS_SUCCESS;
}

/*
    This function is used to get total Flow tables.
    The returned memory should be allocated by AnscAllocateMemory. Or else there is leaking.
*/
ANSC_STATUS
CosaDmlMocaIfFlowTableGetTable
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex,
        PCOSA_DML_MOCA_FLOW             *ppConf,
        PULONG                           pCount
    )
{
    AnscTraceWarning(("CosaDmlMocaIfFlowTableGetTable -- ulInterfaceIndex:%lu, ppConf:%x\n", ulInterfaceIndex, (UINT)ppConf));

    *ppConf = (PCOSA_DML_MOCA_FLOW)AnscAllocateMemory(sizeof(g_MoCAFlow));
    if ( !ppConf )
    {
        *ppConf = NULL;
        *pCount = 0;
        return ANSC_STATUS_FAILURE;
    }
    
    AnscCopyMemory( *ppConf, &g_MoCAFlow, sizeof(g_MoCAFlow) );
    *pCount = sizeof(g_MoCAFlow)/sizeof(COSA_DML_MOCA_FLOW);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMocaIfGetQos
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInterfaceIndex,
        PCOSA_DML_MOCA_QOS          pConf
    )
{
    pConf->Enabled = FALSE;


    
    AnscTraceWarning(("CosaDmlMocaIfGetQos -- ulInterfaceIndex:%lu, Enabled:%s\n", ulInterfaceIndex, (pConf->Enabled==TRUE)?"TRUE":"FALSE" ));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMocaIfSetQos
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInterfaceIndex,
        PCOSA_DML_MOCA_QOS          pConf
    )
{
    AnscTraceWarning(("CosaDmlMocaIfSetQos -- ulInterfaceIndex:%lu, Enabled:%s\n", ulInterfaceIndex, (pConf->Enabled==TRUE)?"TRUE":"FALSE" ));



    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMocaIfGetAssocDevices
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInterfaceIndex,
        PULONG                      pulCount,
        PCOSA_DML_MOCA_ASSOC_DEVICE *ppDeviceArray,
        PULONG                      *ppMeshRateArray    /* Not used now */
    )
{
    ULONG ulSize = 0;

    if ( !pulCount || !ppDeviceArray )
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( ulInterfaceIndex == 0 )
    {
        *pulCount = 2;

        ulSize = (*pulCount) * sizeof(COSA_DML_MOCA_ASSOC_DEVICE);
            
        *ppDeviceArray = (PCOSA_DML_MOCA_ASSOC_DEVICE)AnscAllocateMemory(ulSize);

        AnscCopyMemory
            (
                *ppDeviceArray,
                &g_MoCAAssocDevice[0],
                ulSize
                );
    }
    else if ( ulInterfaceIndex == 1 )
    {
        *pulCount = 3;

        ulSize = (*pulCount) * sizeof(COSA_DML_MOCA_ASSOC_DEVICE);
            
        *ppDeviceArray = (PCOSA_DML_MOCA_ASSOC_DEVICE)AnscAllocateMemory(ulSize);

        AnscCopyMemory
            (
                *ppDeviceArray,
                &g_MoCAAssocDevice[2],
                ulSize
                );
    }

    AnscTraceWarning(("CosaDmlMocaIfGetAssocDevices -- ulInterfaceIndex:%lu, pulCount:%lu\n", ulInterfaceIndex, *pulCount));

    return ANSC_STATUS_SUCCESS;
}

