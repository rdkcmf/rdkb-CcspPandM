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
#include "syscfg/syscfg.h"

#ifndef CONFIG_SYSTEM_MOCA
#define _COSA_SIM_ 1 
#undef _COSA_INTEL_USG_ARM_
#endif

#if  _COSA_INTEL_USG_ARM_

#include "moca_hal.h"

#if 0
#ifdef AnscTraceWarning
#undef AnscTraceWarning
#define AnscTraceWarning(a) printf("%s:%d> ", __FUNCTION__, __LINE__); printf a
#endif
#endif

#define MOCA_INTEFACE_NUMBER    1

#if 0
/* Just fill in USGv2 required parameters */
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
       .FreqCurrentMaskSetting = "0000000002000000",
       .X_CISCO_COM_ChannelScanMask = "0000000001400000",
       .TxPowerLimit = 7,
       .BeaconPowerLimit = 0
    };

/* Just fill in USGv2 required parameters */
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
       .FreqCurrentMaskSetting = "0000000002000000",
       .X_CISCO_COM_ChannelScanMask = "0000000001400000",
       .TxPowerLimit = 7,
       .BeaconPowerLimit = 0
    };
#endif

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

COSA_DML_MOCA_MESH  g_MoCAMesh[2] = 
    {
        {
        .TxNodeID=6602,
        .RxNodeID=6601,
        .TxRate=6001
        },
        {
        .TxNodeID=6609,
        .RxNodeID=6608,
        .TxRate=6002
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

static int is_moca_available = 0;

#define JUDGE_MOCA_HARDWARE_AVAILABLE(RET) \
    if (!is_moca_available) { \
        CcspTraceWarning((" -- Moca hardware is not available.\n")); \
        return RET; \
    }; 

ANSC_STATUS
CosaDmlMocaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
)
{
    PCOSA_DATAMODEL_MOCA  pMyObject    = (PCOSA_DATAMODEL_MOCA)phContext;

    if (CosaDmlGetMocaHardwareStatus() != 1 ) { 
        CcspTraceWarning(("%s -- Moca hardware is not available.\n")); 
        return ANSC_STATUS_FAILURE; 
    } else {
        is_moca_available = 1;
    }

    AnscTraceWarning(("CosaDmlMocaInit -- \n"));

    syscfg_init();

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMocaGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MOCA_CFG          pCfg
    )
{
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

    AnscTraceWarning(("CosaDmlMocaGetCfg -- .\n"));

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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

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
    JUDGE_MOCA_HARDWARE_AVAILABLE(0)

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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

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
        CosaDmlMocaIfGetCfg(hContext, ulInterfaceIndex, &pEntry->Cfg);
        CosaDmlMocaIfGetDinfo(hContext, ulInterfaceIndex, &pEntry->DynamicInfo);
       // moca_IfGetStaticInfo(ulInterfaceIndex, &pEntry->StaticInfo);
       CosaDmlMocaIfGetStaticInfo(hContext,ulInterfaceIndex,&pEntry->StaticInfo);				
		//AnscCopyString(pEntry->StaticInfo.Name, "sw_5");
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

#define kMax_beaconPwrLen               3
#define kMax_TxPowerLimit               3
#define kMax_AutoPowerControlPhyRate    4
#define kMax_FreqCurrentMaskSetting     5
#define kMax_StringValue                20

ANSC_STATUS
CosaDmlMocaIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInterfaceIndex,
        PCOSA_DML_MOCA_IF_CFG       pCfg
    )
{
    char str_value[kMax_StringValue];
    int status;
    int mask;
    int freq;
	moca_cfg_t mocaCfg;

    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

	memset(&mocaCfg, 0, sizeof(moca_cfg_t));

    AnscTraceWarning(("CosaDmlMocaIfSetCfg -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));
    
    if ( !pCfg )
    {
        return ANSC_STATUS_FAILURE;
    }
    
    if ( ulInterfaceIndex == 0 )
    {
         if(pCfg->X_CISCO_COM_Reset == TRUE) {

             AnscTraceWarning(("Resetting MoCA to factory default settings\n"));

			/* Translate the data structures */
			mocaCfg.InstanceNumber 					= pCfg->InstanceNumber;
			strcpy(mocaCfg.Alias, 			  		  pCfg->Alias);
			memcpy(mocaCfg.FreqCurrentMaskSetting,    pCfg->FreqCurrentMaskSetting, 128);
			memcpy(mocaCfg.KeyPassphrase, 			  pCfg->KeyPassphrase, 18);
			mocaCfg.MaxIngressBWThreshold 			= pCfg->MaxIngressBWThreshold;
			mocaCfg.MaxEgressBWThreshold 			= pCfg->MaxEgressBWThreshold;
			mocaCfg.Reset 							= pCfg->X_CISCO_COM_Reset;
			mocaCfg.MixedMode 						= pCfg->X_CISCO_COM_MixedMode;
			mocaCfg.ChannelScanning 				= pCfg->X_CISCO_COM_ChannelScanning;
			memcpy(mocaCfg.NodeTabooMask, 			  pCfg->NodeTabooMask, 128);
			/* Default Values Due to Reset Condition */
			mocaCfg.ChannelScanning 				= TRUE;
			mocaCfg.bPreferredNC 					= TRUE;
			mocaCfg.PrivacyEnabledSetting 			= FALSE;
			mocaCfg.bEnabled 						= TRUE;
			mocaCfg.AutoPowerControlEnable 			= TRUE;
			mocaCfg.EnableTabooBit 					= FALSE;
			mocaCfg.TxPowerLimit 					= 7;
			mocaCfg.BeaconPowerLimit 				= 9;
			mocaCfg.AutoPowerControlPhyRate 		= 235;
            mocaCfg.Reset 							= FALSE;
         } else {
			/* Translate the data structures */
			mocaCfg.InstanceNumber 					= pCfg->InstanceNumber;
			strcpy(mocaCfg.Alias, 					  pCfg->Alias);
			mocaCfg.bEnabled 						= pCfg->bEnabled;
			mocaCfg.bPreferredNC 					= pCfg->bPreferredNC;
			mocaCfg.PrivacyEnabledSetting 			= pCfg->PrivacyEnabledSetting;
			memcpy(mocaCfg.FreqCurrentMaskSetting, 	  pCfg->FreqCurrentMaskSetting, 128);
			memcpy(mocaCfg.KeyPassphrase, 			  pCfg->KeyPassphrase, 18);
			mocaCfg.TxPowerLimit 					= pCfg->TxPowerLimit;
			mocaCfg.BeaconPowerLimit 				= pCfg->BeaconPowerLimit;
			mocaCfg.MaxIngressBWThreshold 			= pCfg->MaxIngressBWThreshold;
			mocaCfg.MaxEgressBWThreshold 			= pCfg->MaxEgressBWThreshold;
			mocaCfg.Reset 							= pCfg->X_CISCO_COM_Reset;
			mocaCfg.MixedMode 						= pCfg->X_CISCO_COM_MixedMode;
			mocaCfg.ChannelScanning 				= pCfg->X_CISCO_COM_ChannelScanning;
			mocaCfg.AutoPowerControlEnable 			= pCfg->X_CISCO_COM_AutoPowerControlEnable;
			mocaCfg.EnableTabooBit 					= pCfg->X_CISCO_COM_EnableTabooBit;
			memcpy(mocaCfg.NodeTabooMask, 			  pCfg->NodeTabooMask, 128);
			mocaCfg.AutoPowerControlPhyRate 		= pCfg->AutoPowerControlPhyRate;
			memcpy(mocaCfg.ChannelScanMask, 		  pCfg->X_CISCO_COM_ChannelScanMask, 128);
		 }

         moca_SetIfConfig(ulInterfaceIndex, &mocaCfg);

         AnscTraceWarning(("pCfg->bEnabled: %d\n", pCfg->bEnabled));

         if(pCfg->bEnabled == TRUE) {

             if (syscfg_set(NULL, "moca_enabled", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {

                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
             }

         } else {

             if (syscfg_set(NULL, "moca_enabled", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {

                 if (syscfg_commit() != 0) {
                     AnscTraceWarning(("syscfg_commit failed\n"));
                 }
             }
         }

         AnscTraceWarning(("pCfg->X_CISCO_COM_EnableTabooBit: %d\n", pCfg->X_CISCO_COM_EnableTabooBit));

         if(pCfg->X_CISCO_COM_EnableTabooBit == TRUE) {

             if (syscfg_set(NULL, "moca_node_taboo_enabled", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {

             	if (syscfg_set(NULL, "moca_node_taboo_mask", pCfg->NodeTabooMask) != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             	} else {

                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
		}
             }

         } else {

             if (syscfg_set(NULL, "moca_node_taboo_enabled", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {

                 if (syscfg_commit() != 0) {
                     AnscTraceWarning(("syscfg_commit failed\n"));
                 }
             }
         }

         AnscTraceWarning(("pCfg->X_CISCO_COM_ChannelScanning: %d\n", pCfg->X_CISCO_COM_ChannelScanning));

         if(pCfg->X_CISCO_COM_ChannelScanning == TRUE) {

             if (syscfg_set(NULL, "moca_scan_enabled", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {

                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
             }

         } else {

             if (syscfg_set(NULL, "moca_scan_enabled", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {

                 if (syscfg_commit() != 0) {
                     AnscTraceWarning(("syscfg_commit failed\n"));
                 }
             }
         }

         AnscTraceWarning(("pCfg->bPreferredNC: %d\n", pCfg->bPreferredNC));

         if(pCfg->bPreferredNC == TRUE) {

             if (syscfg_set(NULL, "moca_preferred_nc", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {

                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
             }

         } else {

             if (syscfg_set(NULL, "moca_preferred_nc", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {

                 if (syscfg_commit() != 0) {
                     AnscTraceWarning(("syscfg_commit failed\n"));
                 }
             }
         }

         AnscTraceWarning(("pCfg->X_CISCO_COM_MixedMode: %d\n", pCfg->X_CISCO_COM_MixedMode));

         if(pCfg->X_CISCO_COM_MixedMode == TRUE) {

             if (syscfg_set(NULL, "moca_mixed_mode", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {

                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
             }

         } else {

             if (syscfg_set(NULL, "moca_mixed_mode", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {

                 if (syscfg_commit() != 0) {
                     AnscTraceWarning(("syscfg_commit failed\n"));
                 }
             }
         }

         AnscTraceWarning(("pCfg->PrivacyEnabledSetting: %d\n", pCfg->PrivacyEnabledSetting));

         if(pCfg->PrivacyEnabledSetting == TRUE) {

             if (syscfg_set(NULL, "moca_enable_privacy", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {

                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
             }

         } else {

             if (syscfg_set(NULL, "moca_enable_privacy", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {

                 if (syscfg_commit() != 0) {
                     AnscTraceWarning(("syscfg_commit failed\n"));
                 }
             }
         }

         AnscTraceWarning(("pCfg->KeyPassphrase: %s\n", pCfg->KeyPassphrase));

         if(pCfg->KeyPassphrase) {

             if (syscfg_set(NULL, "moca_password_seed", pCfg->KeyPassphrase) != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {

                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
             }

         }     

         AnscTraceWarning(("pCfg->BeaconPowerLimit: %d\n", pCfg->BeaconPowerLimit));
         status = snprintf(str_value, kMax_beaconPwrLen, "%d", pCfg->BeaconPowerLimit);

         if(status > 0) {
    
             if (syscfg_set(NULL, "moca_beacon_pwr_level", str_value) != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {
    
                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
             }

         } else {

             AnscTraceWarning(("snprintf failed\n"));
         }

         AnscTraceWarning(("pCfg->TxPowerLimit: %d\n", pCfg->TxPowerLimit));
         status = snprintf(str_value, kMax_TxPowerLimit, "%d", pCfg->TxPowerLimit);

         if(status > 0) {
    
             if (syscfg_set(NULL, "moca_maximum_tx_power", str_value) != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {
    
                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
             }

         } else {

             AnscTraceWarning(("snprintf failed\n"));
         }

         AnscTraceWarning(("pCfg->AutoPowerControlPhyRate: %d\n", pCfg->AutoPowerControlPhyRate));
         status = snprintf(str_value, kMax_AutoPowerControlPhyRate, "%d", pCfg->AutoPowerControlPhyRate);
         
         if(status > 0) {
    
             if (syscfg_set(NULL, "moca_phy_rate", str_value) != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {
    
                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
             }

         } else {

             AnscTraceWarning(("snprintf failed\n"));
         }


         AnscTraceWarning(("pCfg->X_CISCO_COM_AutoPowerControlEnable: %d\n", pCfg->X_CISCO_COM_AutoPowerControlEnable));

         if(pCfg->X_CISCO_COM_AutoPowerControlEnable == TRUE) {

             if (syscfg_set(NULL, "moca_pwr_apc_enable", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {

                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
             }

         } else {

             if (syscfg_set(NULL, "moca_pwr_apc_enable", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {

                 if (syscfg_commit() != 0) {
                     AnscTraceWarning(("syscfg_commit failed\n"));
                 }
             }
         }

         if(pCfg->X_CISCO_COM_ChannelScanning == FALSE) {

             AnscTraceWarning(("pCfg->FreqCurrentMaskSetting: %s\n", pCfg->FreqCurrentMaskSetting));
    
             //sscanf(pCfg->FreqCurrentMaskSetting, "%016x", &mask);
             //freq = moca_FreqMaskToValue(mask); 
             freq = moca_FreqMaskToValue(pCfg->FreqCurrentMaskSetting);
             status = snprintf(str_value, kMax_FreqCurrentMaskSetting, "%d", freq);
             
             AnscTraceWarning(("freq: %s\n", str_value));
    
             if(status > 0) {
        
                 if (syscfg_set(NULL, "moca_freq_plan", str_value) != 0) {
                         AnscTraceWarning(("syscfg_set failed\n"));
                 } else {
        
                        if (syscfg_commit() != 0) {
                                AnscTraceWarning(("syscfg_commit failed\n"));
                        }
                 }
    
             } else {
    
                 AnscTraceWarning(("snprintf failed\n"));
             }
         }
         
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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)
	moca_cfg_t mocaCfg;

    AnscTraceWarning(("CosaDmlMocaIfGetCfg -- ulInterfaceIndex:%lu.\n", uIndex));
    
    if ( !pCfg )
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( uIndex == 0 )
    {
		memset(&mocaCfg, 0, sizeof(moca_cfg_t));
        moca_GetIfConfig(uIndex, &mocaCfg);
		
		/* Translate the data structures */
		pCfg->InstanceNumber 						= mocaCfg.InstanceNumber;
		strcpy(pCfg->Alias, 						  mocaCfg.Alias);
		pCfg->bEnabled 								= mocaCfg.bEnabled;
		pCfg->bPreferredNC 							= mocaCfg.bPreferredNC;
		pCfg->PrivacyEnabledSetting 				= mocaCfg.PrivacyEnabledSetting;
		memcpy(pCfg->FreqCurrentMaskSetting, 	  	  mocaCfg.FreqCurrentMaskSetting, 128);
		memcpy(pCfg->KeyPassphrase, 			  	  mocaCfg.KeyPassphrase, 18);
		pCfg->TxPowerLimit 							= mocaCfg.TxPowerLimit;
		pCfg->PowerCntlPhyTarget 					= mocaCfg.AutoPowerControlPhyRate;
		pCfg->BeaconPowerLimit 						= mocaCfg.BeaconPowerLimit;
		pCfg->MaxIngressBWThreshold 				= mocaCfg.MaxIngressBWThreshold;
		pCfg->MaxEgressBWThreshold 					= mocaCfg.MaxEgressBWThreshold;
		pCfg->X_CISCO_COM_Reset 					= mocaCfg.Reset;
		pCfg->X_CISCO_COM_MixedMode 				= mocaCfg.MixedMode;
		pCfg->X_CISCO_COM_ChannelScanning 			= mocaCfg.ChannelScanning;
		pCfg->X_CISCO_COM_AutoPowerControlEnable 	= mocaCfg.AutoPowerControlEnable;
		pCfg->X_CISCO_COM_EnableTabooBit 			= mocaCfg.EnableTabooBit;
		memcpy(pCfg->NodeTabooMask, 				  mocaCfg.NodeTabooMask, 128);
		pCfg->AutoPowerControlPhyRate 				= mocaCfg.AutoPowerControlPhyRate;
		memcpy(pCfg->X_CISCO_COM_ChannelScanMask, 	  mocaCfg.ChannelScanMask, 128);
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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)
	moca_dynamic_info_t mocaDInfo;

	memset(&mocaDInfo, 0, sizeof(moca_dynamic_info_t));

    AnscTraceWarning(("CosaDmlMocaIfGetDinfo -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));

    if (!pInfo)
    {
        return ANSC_STATUS_FAILURE;
    }

    _ansc_memset(pInfo, 0, sizeof(COSA_DML_MOCA_IF_DINFO));
        
    if (ulInterfaceIndex == 0)
    {
        moca_IfGetDynamicInfo(ulInterfaceIndex, &mocaDInfo);

		/* Translate the Data Structures */
		pInfo->Status 											= mocaDInfo.Status;
		pInfo->LastChange 										= mocaDInfo.LastChange;
		pInfo->MaxIngressBW 									= mocaDInfo.MaxIngressBW;
		pInfo->MaxEgressBW 										= mocaDInfo.MaxEgressBW;
		memcpy(pInfo->CurrentVersion, 					  		  mocaDInfo.CurrentVersion, 64);
		pInfo->NetworkCoordinator 								= mocaDInfo.NetworkCoordinator;
		pInfo->NodeID 											= mocaDInfo.NodeID;
		pInfo->MaxNodes 										= 16;
		pInfo->BackupNC 										= mocaDInfo.BackupNC;
		pInfo->PrivacyEnabled 									= mocaDInfo.PrivacyEnabled;
		memcpy(pInfo->FreqCurrentMask, 					  		  mocaDInfo.FreqCurrentMask, 8);
		pInfo->CurrentOperFreq 									= mocaDInfo.CurrentOperFreq;
		pInfo->LastOperFreq 									= mocaDInfo.LastOperFreq;
		pInfo->TxBcastRate 										= mocaDInfo.TxBcastRate;
		pInfo->MaxIngressBWThresholdReached 					= mocaDInfo.MaxIngressBWThresholdReached;
		pInfo->MaxEgressBWThresholdReached 						= mocaDInfo.MaxEgressBWThresholdReached;
		pInfo->X_CISCO_COM_NumberOfConnectedClients 			= mocaDInfo.NumberOfConnectedClients;
		memcpy(pInfo->X_CISCO_NetworkCoordinatorMACAddress, 	  mocaDInfo.NetworkCoordinatorMACAddress, 18);
		pInfo->X_CISCO_COM_BestNetworkCoordinatorID 			= mocaDInfo.NetworkCoordinator;
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }
        
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMocaIfGetStaticInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       uIndex,
        PCOSA_DML_MOCA_IF_SINFO       pSInfo
    )
{
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)
	moca_static_info_t mocaStaticCfg;

    AnscTraceWarning(("CosaDmlMocaIfGetStaticInfo -- ulInterfaceIndex:%lu.\n", uIndex));
    
    if ( !pSInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( uIndex == 0 )
    {
		memset(&mocaStaticCfg, 0, sizeof(moca_static_info_t));
        moca_IfGetStaticInfo(uIndex, &mocaStaticCfg);
		
		/* Translate the data structures */
		AnscCopyString(pSInfo->Name, "sw_5");
		memcpy(pSInfo->MacAddress, 	mocaStaticCfg.MacAddress, 18);
		memcpy(pSInfo->FirmwareVersion, mocaStaticCfg.FirmwareVersion, 64);
		pSInfo->MaxBitRate  = mocaStaticCfg.MaxBitRate;
		memcpy(pSInfo->HighestVersion, mocaStaticCfg.HighestVersion, 64);
		memcpy(pSInfo->FreqCapabilityMask, mocaStaticCfg.FreqCapabilityMask, 8);
		memcpy(pSInfo->NetworkTabooMask, mocaStaticCfg.NetworkTabooMask, 128);
		pSInfo->TxBcastPowerReduction = mocaStaticCfg.TxBcastPowerReduction;
		pSInfo->QAM256Capable = mocaStaticCfg.QAM256Capable;
		pSInfo->PacketAggregationCapability = mocaStaticCfg.PacketAggregationCapability;
		
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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)
	moca_stats_t mocaStats;

	memset(&mocaStats, 0, sizeof(moca_stats_t));

    AnscTraceWarning(("CosaDmlMocaIfGetStats -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));

    if ( !pStats )
    {
        return ANSC_STATUS_FAILURE;
    }

    _ansc_memset(pStats, 0, sizeof(COSA_DML_MOCA_STATS));

    if ( ulInterfaceIndex == 1 )
    { 
        moca_IfGetStats(ulInterfaceIndex, &mocaStats);

		/* Translate the Data Structures */
		pStats->BytesSent 						= mocaStats.BytesSent;
		pStats->BytesReceived 					= mocaStats.BytesReceived;
		pStats->PacketsSent						= mocaStats.PacketsSent;
		pStats->PacketsReceived 				= mocaStats.PacketsReceived;
		pStats->ErrorsSent						= mocaStats.ErrorsSent;
		pStats->ErrorsReceived					= mocaStats.ErrorsReceived;
		pStats->UnicastPacketsSent				= mocaStats.UnicastPacketsSent;
		pStats->UnicastPacketsReceived  		= mocaStats.UnicastPacketsReceived;
		pStats->DiscardPacketsSent				= mocaStats.DiscardPacketsSent;
		pStats->DiscardPacketsReceived  		= mocaStats.DiscardPacketsReceived;
		pStats->MulticastPacketsSent			= mocaStats.MulticastPacketsSent;
		pStats->MulticastPacketsReceived 		= mocaStats.MulticastPacketsReceived;
		pStats->BroadcastPacketsSent			= mocaStats.BroadcastPacketsSent;
		pStats->BroadcastPacketsReceived    	= mocaStats.BroadcastPacketsReceived;
		pStats->UnknownProtoPacketsReceived		= mocaStats.UnknownProtoPacketsReceived;
		pStats->X_CISCO_COM_ExtAggrAverageTx 	= mocaStats.ExtAggrAverageTx;
		pStats->X_CISCO_COM_ExtAggrAverageRx 	= mocaStats.ExtAggrAverageRx;
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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

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
    ULONG ulCount = 0;

    JUDGE_MOCA_HARDWARE_AVAILABLE(0)

    AnscTraceWarning(("CosaDmlMocaIfExtCounterGetNumber -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));

    if ( ulInterfaceIndex == 0 )
    {
		moca_GetNumAssociatedDevices(ulInterfaceIndex, &ulCount);
        return ulCount;
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
    ULONG ulCount = 0;
	moca_mac_counters_t mocaMacStats;

    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

	memset(&mocaMacStats, 0, sizeof(moca_mac_counters_t));

    AnscTraceWarning(("CosaDmlMocaIfExtCounterGetEntry -- ulInterfaceIndex:%lu, ulIndex:%lu\n", ulInterfaceIndex, ulIndex));

    if ( ulInterfaceIndex == 0 )
    {
		moca_GetNumAssociatedDevices(ulInterfaceIndex, &ulCount);

        if (ulIndex >=ulCount )
        {
            AnscTraceWarning(("CosaDmlMocaIfExtCounterGetEntry -- Error, not such Entry\n"));
            return ANSC_STATUS_FAILURE;
        }
        
        moca_IfGetExtCounter(ulIndex, &mocaMacStats);

		/* Translate the Data Structures */
		pConf->Map 			= mocaMacStats.Map;
		pConf->Rsrv			= mocaMacStats.Rsrv;
		pConf->Lc			= mocaMacStats.Lc;
		pConf->Adm			= mocaMacStats.Adm;
		pConf->Probe		= mocaMacStats.Probe;
		pConf->Async		= mocaMacStats.Async;

        //*pConf = g_MoCAExtCounter[ulIndex];
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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

    AnscTraceWarning(("CosaDmlMocaIfExtAggrCounterGetNumber -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));

    if ( ulInterfaceIndex == 0 ) {

        // Default to 1 since Intel's MoCA API does not support PCOSA_DML_MOCA_EXTAGGRCOUNTER for other
        // nodes in the MoCA network
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
	moca_aggregate_counters_t mocaCounters;

    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

	memset(&mocaCounters, 0, sizeof(moca_aggregate_counters_t));

    AnscTraceWarning(("CosaDmlMocaIfExtAggrCounterGetEntry -- ulInterfaceIndex:%lu, ulIndex:%lu\n", ulInterfaceIndex, ulIndex));

    if ( ulInterfaceIndex == 0 )
    {
        if (ulIndex >=1 )
        {
            AnscTraceWarning(("CosaDmlMocaIfExtAggrCounterGetEntry -- Error, not such Entry\n"));
            return ANSC_STATUS_FAILURE;
        }

        moca_IfGetExtAggrCounter(ulIndex, &mocaCounters);

		pConf->Tx		= mocaCounters.Tx;
		pConf->Rx		= mocaCounters.Rx;

        //*pConf = g_MoCAExtAggrCounter[0];
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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

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
    This function is used to get total Mesh tables.
    The returned memory should be allocated by AnscAllocateMemory. Or else there is leaking.
*/
ANSC_STATUS
CosaDmlMocaIfMeshTableGetTable
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex,
        PCOSA_DML_MOCA_MESH             *ppConf,
        PULONG                           pCount
    )
{
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

    AnscTraceWarning(("CosaDmlMocaIfFlowTableGetTable -- ulInterfaceIndex:%lu, ppConf:%x\n", ulInterfaceIndex, (UINT)ppConf));

    if ( !pCount || !ppConf || ulInterfaceIndex != 0)
    {
        return ANSC_STATUS_FAILURE;
    }

    *pCount = kMoca_MaxMocaNodes;

    *ppConf = (PCOSA_DML_MOCA_MESH)AnscAllocateMemory(sizeof(COSA_DML_MOCA_MESH) * kMoca_MaxMocaNodes);
    if ( !*ppConf )
    {
        *pCount = 0;
        return ANSC_STATUS_FAILURE;
    }
    
    if (moca_GetFullMeshRates(ulInterfaceIndex, *ppConf, pCount) != STATUS_SUCCESS)
    {
        AnscFreeMemory(*ppConf);
        *ppConf = NULL;
        *pCount = 0;
        AnscTraceError(("%s: fail to get MoCA associated device\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

    AnscTraceWarning(("CosaDmlMocaIfFlowTableGetTable -- ulInterfaceIndex:%lu, ppConf:%x\n", ulInterfaceIndex, (UINT)ppConf));

    if ( !pCount || !ppConf || ulInterfaceIndex != 0)
    {
        return ANSC_STATUS_FAILURE;
    }

    *pCount = kMoca_MaxMocaNodes;
    *ppConf = (PCOSA_DML_MOCA_FLOW)AnscAllocateMemory(sizeof(COSA_DML_MOCA_FLOW) * kMoca_MaxMocaNodes);

    if ( !*ppConf )
    {
        *pCount = 0;
        return ANSC_STATUS_FAILURE;
    }
    
    if (moca_GetFlowStatistics(ulInterfaceIndex, *ppConf, pCount) != STATUS_SUCCESS)
    {
        AnscFreeMemory(*ppConf);
        *ppConf = NULL;
        *pCount = 0;
        AnscTraceError(("%s: fail to get MoCA flow table\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    
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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

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
    JUDGE_MOCA_HARDWARE_AVAILABLE(ANSC_STATUS_FAILURE)

    ULONG ulSize = 0;

    if ( !pulCount || !ppDeviceArray )
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( ulInterfaceIndex == 0 )
    {
	moca_cpe_t cpes[kMoca_MaxCpeList];
        int        pnum_cpes     = 0,
                   iReturnStatus = STATUS_SUCCESS;

       iReturnStatus =  moca_GetMocaCPEs(ulInterfaceIndex, cpes, &pnum_cpes);

       AnscTraceWarning(("pnum_cpes: %u\n", pnum_cpes));

       if( ( iReturnStatus == STATUS_SUCCESS ) && \
           ( 0 < pnum_cpes )
         )
        {
		moca_GetNumAssociatedDevices(ulInterfaceIndex, pulCount);

        AnscTraceWarning(("*pulCount: %lu\n", *pulCount));

        if ( *pulCount )
        {
			moca_associated_device_t*       pdevice_array  = NULL;
			int                             i;

            ulSize = sizeof(COSA_DML_MOCA_ASSOC_DEVICE) * (*pulCount);
                
            *ppDeviceArray = (PCOSA_DML_MOCA_ASSOC_DEVICE)AnscAllocateMemory(ulSize);

		    pdevice_array = (moca_associated_device_t *)
                AnscAllocateMemory
                    (
                        sizeof(moca_associated_device_t) * (*pulCount + COSA_DML_MOCA_AssocDeviceSafeguard)
                    );
			    
            if ( *ppDeviceArray && pdevice_array )
            {
                INT                 iReturnStatus   = STATUS_SUCCESS;
                PCOSA_DML_MOCA_ASSOC_DEVICE pDeviceArray = *ppDeviceArray;

                iReturnStatus = moca_GetAssociatedDevices(ulInterfaceIndex, &pdevice_array);

                if ( iReturnStatus == STATUS_SUCCESS )
                {
        			/* Translate the Data Structures */
    				for (i = 0; i < *pulCount; i++)
    				{
    					memcpy(pDeviceArray->MACAddress, 		  		  pdevice_array[i].MACAddress, 18);
    					pDeviceArray->NodeID 							= pdevice_array[i].NodeID;
    					pDeviceArray->PreferredNC 						= pdevice_array[i].PreferredNC;
    					memcpy(pDeviceArray->HighestVersion, 	  		  pdevice_array[i].HighestVersion, 64);
    					pDeviceArray->PHYTxRate 						= pdevice_array[i].PHYTxRate;
    					pDeviceArray->PHYRxRate 						= pdevice_array[i].PHYRxRate;
    					pDeviceArray->TxPowerControlReduction 			= pdevice_array[i].TxPowerControlReduction;
    					pDeviceArray->RxPowerLevel 						= pdevice_array[i].RxPowerLevel;
    					pDeviceArray->TxBcastRate 						= pdevice_array[i].TxBcastRate;
    					pDeviceArray->RxBcastPowerLevel					= pdevice_array[i].RxBcastPowerLevel;
    					pDeviceArray->TxPackets							= pdevice_array[i].TxPackets;
    					pDeviceArray->RxPackets							= pdevice_array[i].RxPackets;
    					pDeviceArray->RxErroredAndMissedPackets			= pdevice_array[i].RxErroredAndMissedPackets;
    					pDeviceArray->QAM256Capable						= pdevice_array[i].QAM256Capable;
    					pDeviceArray->PacketAggregationCapability 		= pdevice_array[i].PacketAggregationCapability;
    					pDeviceArray->RxSNR								= pdevice_array[i].RxSNR;
    					pDeviceArray->Active							= pdevice_array[i].Active;
    					pDeviceArray->X_CISCO_COM_RxBcastRate			= pdevice_array[i].RxBcastRate;
    					pDeviceArray->X_CISCO_COM_NumberOfClients		= pdevice_array[i].NumberOfClients;
                        ++pDeviceArray;  
    				}
                }

                AnscFreeMemory(pdevice_array);

                return  ANSC_STATUS_SUCCESS;
            }
			else
			{
    			AnscTraceWarning(("CosaDmlMocaIfGetAssocDevices -- Memory Allocation Failure "
									"ulInterfaceIndex:%lu, pulCount:%lu\n", ulInterfaceIndex, *pulCount));

                if ( pdevice_array )
                {
                    AnscFreeMemory(pdevice_array);
                }
                if ( *ppDeviceArray )
                {
                    AnscFreeMemory(*ppDeviceArray);
                    *ppDeviceArray = NULL;
                }

			return  ANSC_STATUS_RESOURCES;
		    }
		}
        }
    }

    AnscTraceWarning(("CosaDmlMocaIfGetAssocDevices -- ulInterfaceIndex:%lu, pulCount:%lu\n", ulInterfaceIndex, *pulCount));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMocaGetResetCount
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
	moca_GetResetCount(pValue);	
    return ANSC_STATUS_SUCCESS;
}

#elif (_COSA_DRG_TPG_)

#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
/*#include "poam_cosa_moca_dm_interface.h"*/

#define MOCA_INTEFACE_NUMBER        1

COSA_DML_MOCA_IF_CFG                g_MoCAIfCfg1 = {1, "Interface1", TRUE};
COSA_DML_MOCA_IF_CFG                g_MoCAIfCfg2 = {2, "Interface2", TRUE};

ANSC_STATUS
CosaDmlMocaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
)
{
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlMocaGetNumberOfIfs
    (
        ANSC_HANDLE                 hContext
    )
{
    return MOCA_INTEFACE_NUMBER;
}


ANSC_STATUS
CosaDmlMocaIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_MOCA_IF_FULL      pEntry
    )
{
    UtopiaContext pCtx;
    int rc = -1;

    if (!Utopia_Init(&pCtx))
    {
        AnscTraceWarning(("MoCA: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    if (pEntry)
        _ansc_memset(pEntry, 0, sizeof(COSA_DML_MOCA_IF_FULL));
    else
    {
        Utopia_Free(&pCtx,0);
        return ANSC_STATUS_FAILURE;
    }
   
    if(CosaDmlMocaIfGetCfg(hContext, &pEntry->Cfg) != ANSC_STATUS_SUCCESS)
    {
        AnscTraceWarning(("MoCA: GetMocaIntf_Cfg Failed!!! \n" ));
        Utopia_Free(&pCtx,0);
        return ANSC_STATUS_FAILURE;
    }
    
    rc = Utopia_GetMocaIntf_Static((void*)&pEntry->StaticInfo);
    if(rc != 0)
    {
        AnscTraceWarning(("MoCA: GetMocaIntf_Static Failed !!! \n" ));
        Utopia_Free(&pCtx, 0);
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        Utopia_Free(&pCtx, 0);
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlMocaIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
)
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMocaIfGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_MOCA_STATS        pStats
    )
{
    if ( !pStats )
    {
        return ANSC_STATUS_FAILURE;
    }

    _ansc_memset(pStats, 0, sizeof(COSA_DML_MOCA_STATS));
    
    if ( ulInstanceNumber == 1 )
    {
        pStats->BytesSent = 11;
    }
    else if ( ulInstanceNumber == 2 )
    {
        pStats->BytesSent = 22;
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMocaIfGetAssocDevices
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PULONG                      pulCount,
        PCOSA_DML_MOCA_ASSOC_DEVICE *ppDeviceArray,
        PULONG                      *ppMeshRateArray
    )
{
    /*PPOAM_COSAMOCADM_OBJECT         pPoamMoCADm    = (PPOAM_COSAMOCADM_OBJECT       )hContext;*/
    ULONG                           ulSize         = 0;
    int                    devNo       = 0;
    int                    cnt        = 0;
    
    if ( /*pPoamMoCADm*/FALSE )
    {
        return 0;
/*
            pPoamMoCADm->CosaDmlMocaIfGetAssocDevices
                (
                    (ANSC_HANDLE)pPoamMoCADm,
                    ulInstanceNumber,
                    pulCount,
                    ppDeviceArray,
                    ppMeshRateArray
                );
*/
    }
    else
    {
        if ( !pulCount || !ppDeviceArray )
    {
        return ANSC_STATUS_FAILURE;
    }
         
    if(Utopia_Count_AssociateDeviceEntry(pulCount) != 0){
            AnscTraceWarning(("MoCA: Count Associate device failed !!! \n"));
            return ANSC_STATUS_FAILURE;
    }
    if(*pulCount == 0)
    {
            return ANSC_STATUS_FAILURE;
    }

        cnt = *pulCount ;
        *ppDeviceArray = AnscAllocateMemory( cnt * sizeof(COSA_DML_MOCA_ASSOC_DEVICE)); 
        
    for(devNo = 0; devNo < cnt; devNo++)
        {
        if(Utopia_GetMocaIntf_AssociateDevice((void *)(*ppDeviceArray + devNo), devNo) != 0)
            {
                AnscTraceWarning(("MoCA: GetMocaIntf_AssociatedDevice Failed for device - %d!!! \n", devNo));
                *pulCount = devNo;
                return ANSC_STATUS_FAILURE;
            }
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMocaIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MOCA_IF_CFG       pCfg
    )
{
    UtopiaContext pCtx;
    int rc = -1;
    if ( !pCfg )
        return ANSC_STATUS_FAILURE;

    if (!Utopia_Init(&pCtx))
    {
        AnscTraceWarning(("MoCA: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    rc = Utopia_SetMocaIntf_Cfg(&pCtx, (void*)pCfg);
    if(rc != 0)
    {
        AnscTraceWarning(("MoCA: SetMocaIntf_Cfg Failed !!! \n" ));
        Utopia_Free(&pCtx, !rc);
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        Utopia_Free(&pCtx, !rc);
        return ANSC_STATUS_SUCCESS;
    }

    Utopia_Free(&pCtx,!rc);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlMocaIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MOCA_IF_CFG       pCfg
    )
{
    UtopiaContext pCtx;
    int rc = -1;
    if ( !pCfg)
    {
        AnscTraceWarning(("MoCA: Memory Alloc Error !!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    if (!Utopia_Init(&pCtx))
    {
        AnscTraceWarning(("MoCA: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
       
    rc = Utopia_GetMocaIntf_Cfg(&pCtx, (void*)pCfg);
    if(rc != 0)
    {
        AnscTraceWarning(("MoCA: GetMocaIntf_Cfg Failed !!! \n" ));
        Utopia_Free(&pCtx, 0);
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        Utopia_Free(&pCtx, 0);
        return ANSC_STATUS_SUCCESS;
    }

    Utopia_Free(&pCtx, !rc);
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMocaIfGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_MOCA_IF_DINFO     pInfo
    )
{
    if (!pInfo)
    {
        AnscTraceWarning(("MoCA: Memory Alloc Error !!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    if(Utopia_GetMocaIntf_Dyn((void*)pInfo)!= 0)
    {
        AnscTraceWarning(("MoCA: GetMocaIntf_Dyn Failed !!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_FAILURE;
}

#elif ( _COSA_SIM_ )


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




    return 0;
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

    if ( !pEntry)
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




    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMocaIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInterfaceIndex,
        PCOSA_DML_MOCA_IF_CFG       pCfg
    )
{
    AnscTraceWarning(("CosaDmlMocaIfGetCfg -- ulInterfaceIndex:%lu.\n", ulInterfaceIndex));
    
    if ( !pCfg )
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


    return 0;
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


    return 0;
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




    *ppConf = NULL;
    *pCount = 0;

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


    
    *ppConf = NULL;
    *pCount = 0;

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

/*
    ppMeshRateArray is hardcoded to be NULL currently.
    
*/
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
    if ( !pulCount || !ppDeviceArray )
    {
        return ANSC_STATUS_FAILURE;
    }


    *pulCount      = 0;
    *ppDeviceArray = NULL;

    AnscTraceWarning(("CosaDmlMocaIfGetAssocDevices -- ulInterfaceIndex:%lu, pulCount:%lu\n", ulInterfaceIndex, *pulCount));

    return ANSC_STATUS_SUCCESS;
}

#endif
