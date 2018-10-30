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


/*********************************************************************** 
  
    module: plugin_main_apis.c

        Implement COSA Data Model Library Init and Unload apis.
        This files will hold all data in it.
 
    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Var Record Object.

        *   CosaBackEndManagerCreate
        *   CosaBackEndManagerInitialize
        *   CosaBackEndManagerRemove
    ---------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    ---------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**********************************************************************/

#include "dml_tr181_custom_cfg.h"
#include "plugin_main_apis.h"
#include "cosa_ethernet_apis.h"
//#include "cosa_moca_apis.h"
#include "cosa_time_apis.h"
#include "cosa_userinterface_apis.h"
#include "cosa_nat_apis.h"
#include "cosa_dhcpv4_apis.h"
#include "cosa_users_apis.h"
#include "cosa_deviceinfo_apis.h"
#include "cosa_firewall_internal.h"
#include "cosa_x_cisco_com_ddns_internal.h"
#include "cosa_x_cisco_com_security_internal.h"
#include "cosa_ip_apis.h"
#include "cosa_hosts_apis.h"
#include "cosa_routing_apis.h"
#include "cosa_nat_internal.h"
#include "cosa_dhcpv4_internal.h"
#include "cosa_users_internal.h"
#include "cosa_ip_internal.h"
#include "cosa_hosts_internal.h"
#include "cosa_dns_internal.h"
#include "cosa_routing_internal.h"
#include "cosa_deviceinfo_internal.h"
#include "cosa_ethernet_internal.h"
//#include "cosa_moca_internal.h"
#include "cosa_time_internal.h"
#include "cosa_userinterface_internal.h"
#include "cosa_ppp_internal.h"
#include "cosa_bridging_internal.h"
#include "cosa_upnp_internal.h"
#include "cosa_interfacestack_internal.h"
/*#include "cosa_diagnostic_apis.h"*/
#include "cosa_x_cisco_com_devicecontrol_internal.h"
#include "cosa_ipv6rd_internal.h"
#include "cosa_x_cisco_com_mld_internal.h"
#include "cosa_x_cisco_com_multilan_apis.h"
#if CONFIG_CISCO_TRUE_STATIC_IP
    #include "cosa_x_cisco_com_truestaticip_internal.h"
#endif
#if CONFIG_CISCO_FILE_TRANSFER
    #include "cosa_x_cisco_com_filetransfer_internal.h"
#endif

#if  CFG_USE_Common_Util
#include "cosa_common_util.h"
#endif
#include "cosa_dev_fingerprint_internal.h"
static void CheckAndSetRebootReason();
/*PCOSA_DIAG_PLUGIN_INFO             g_pCosaDiagPluginInfo;*/
COSAGetParamValueByPathNameProc    g_GetParamValueByPathNameProc;
COSASetParamValueByPathNameProc    g_SetParamValueByPathNameProc;
COSAGetParamValueStringProc        g_GetParamValueString;
COSAGetParamValueUlongProc         g_GetParamValueUlong;
COSAGetParamValueIntProc           g_GetParamValueInt;
COSAGetParamValueBoolProc          g_GetParamValueBool;
COSASetParamValueStringProc        g_SetParamValueString;
COSASetParamValueUlongProc         g_SetParamValueUlong;
COSASetParamValueIntProc           g_SetParamValueInt;
COSASetParamValueBoolProc          g_SetParamValueBool;
COSAGetInstanceNumbersProc         g_GetInstanceNumbers;

COSAValidateHierarchyInterfaceProc g_ValidateInterface;
COSAGetHandleProc                  g_GetRegistryRootFolder;
COSAGetInstanceNumberByIndexProc   g_GetInstanceNumberByIndex;
COSAGetInterfaceByNameProc         g_GetInterfaceByName;
COSAGetHandleProc                  g_GetMessageBusHandle;
COSAGetSubsystemPrefixProc         g_GetSubsystemPrefix;
PCCSP_CCD_INTERFACE                g_pPnmCcdIf;
ANSC_HANDLE                        g_MessageBusHandle;
char*                              g_SubsystemPrefix;
COSARegisterCallBackAfterInitDmlProc  g_RegisterCallBackAfterInitDml;
COSARepopulateTableProc            g_COSARepopulateTable;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaBackEndManagerCreate
            (
            );

    description:

        This function constructs cosa datamodel object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaBackEndManagerCreate
    (
        VOID
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_BACKEND_MANAGER_OBJECT    pMyObject    = (PCOSA_BACKEND_MANAGER_OBJECT)NULL;

    /*
        * We create object by first allocating memory for holding the variables and member functions.
        */
    pMyObject = (PCOSA_BACKEND_MANAGER_OBJECT)AnscAllocateMemory(sizeof(COSA_BACKEND_MANAGER_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_BASE_OID;
    pMyObject->Create            = CosaBackEndManagerCreate;
    pMyObject->Remove            = CosaBackEndManagerRemove;
    pMyObject->Initialize        = CosaBackEndManagerInitialize;
printf("-- %s %d\n", __func__, __LINE__);
    CcspTraceWarning(("RDKB_SYSTEM_BOOT_UP_LOG : Entering %s %d\n", __func__, __LINE__));
    /*pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);*/

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaBackEndManagerInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate cosa manager object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaBackEndManagerInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_BACKEND_MANAGER_OBJECT  pMyObject    = (PCOSA_BACKEND_MANAGER_OBJECT)hThisObject;

#ifdef _COSA_SIM_
        pMyObject->has_moca_slap  = 0;
#endif

    AnscTraceWarning(("%s...\n", __FUNCTION__));
    CcspTraceWarning(("RDKB_SYSTEM_BOOT_UP_LOG : PandM DM initialize...\n"));
    printf("PandM DM initialize...\n");

    /* Create all object */
    pMyObject->hNat           = (ANSC_HANDLE)CosaNatCreate();
    AnscTraceWarning(("  CosaNatCreate done!\n"));
    pMyObject->hProcStatus    = (ANSC_HANDLE)CosaProcStatusCreate();    
    AnscTraceWarning(("  CosaProcStatusCreate done!\n"));
    pMyObject->hDeviceInfo    = (ANSC_HANDLE)CosaDeviceInfoCreate();
    AnscTraceWarning(("  CosaDeviceInfoCreate done!\n"));
    pMyObject->hUserinterface = (ANSC_HANDLE)CosaUserinterfaceCreate();
    AnscTraceWarning(("  CosaUserinterfaceCreate done!\n"));
    pMyObject->hEthernet      = (ANSC_HANDLE)CosaEthernetCreate();
    AnscTraceWarning(("  CosaEthernetCreate done!\n"));

    pMyObject->hUsers         = (ANSC_HANDLE)CosaUsersCreate();
    AnscTraceWarning(("  CosaUsersCreate done!\n"));
    pMyObject->hDdns          = (ANSC_HANDLE)CosaDdnsCreate();
    AnscTraceWarning(("  CosaDdnsCreate done!\n"));
    pMyObject->hFirewall      = (ANSC_HANDLE)CosaFirewallCreate();
    AnscTraceWarning(("  CosaFirewallCreate done!\n"));
    pMyObject->hSecurity      = (ANSC_HANDLE)CosaSecurityCreate();
    AnscTraceWarning(("  CosaSecurityCreate done!\n"));
    pMyObject->hIP            = (ANSC_HANDLE)CosaIPCreate();
    AnscTraceWarning(("  CosaIPCreate done!\n"));
    pMyObject->hDhcpv4        = (ANSC_HANDLE)CosaDhcpv4Create();
    AnscTraceWarning(("  CosaDhcpv4Create done!\n"));
    pMyObject->hHosts         = (ANSC_HANDLE)CosaHostsCreate();
    AnscTraceWarning(("  CosaHostsCreate done!\n"));
    pMyObject->hDNS           = (ANSC_HANDLE)CosaDNSCreate();
    AnscTraceWarning(("  CosaDNSCreate done!\n"));
    pMyObject->hRouting       = (ANSC_HANDLE)CosaRoutingCreate();
    AnscTraceWarning(("  CosaRoutingCreate done!\n"));
    pMyObject->hBridging      = (ANSC_HANDLE)CosaBridgingCreate();
    AnscTraceWarning(("  CosaBridgingCreate done!\n"));
    pMyObject->hInterfaceStack = (ANSC_HANDLE)CosaIFStackCreate();
    AnscTraceWarning(("  CosaIFStackCreate done!\n"));
    pMyObject->hPPP           = (ANSC_HANDLE)CosaPPPCreate();
    AnscTraceWarning(("  CosaPPPCreate done!\n"));
    pMyObject->hDhcpv6        = (ANSC_HANDLE)CosaDhcpv6Create();
    AnscTraceWarning(("  CosaDhcpv6Create done!\n"));
    pMyObject->hDeviceControl  = (ANSC_HANDLE)CosaDeviceControlCreate();
    AnscTraceWarning(("  CosaDeviceControlCreate done!\n"));
    pMyObject->hIPv6rd        = (ANSC_HANDLE)CosaIPv6rdCreate();
    AnscTraceWarning(("  CosaIPv6rdCreate done!\n"));
    pMyObject->hRA            = (ANSC_HANDLE)CosaRACreate();
    AnscTraceWarning(("  CosaRACreate done!\n"));
    pMyObject->hNeighdisc     = (ANSC_HANDLE)CosaNeighdiscCreate();
    AnscTraceWarning(("  CosaNeighdiscCreate done!\n"));
    pMyObject->hMld           = (ANSC_HANDLE)CosaMldCreate();
    AnscTraceWarning(("  CosaMldCreate done!\n"));
    /*
#ifdef CONFIG_TI_PACM
    pMyObject->hMTA           = (ANSC_HANDLE)CosaMTACreate();
    AnscTraceWarning(("  CosaMTACreate done!\n"));
#endif
    */

    returnStatus = CosaDmlMlanInit((ANSC_HANDLE)pMyObject, &pMyObject->hMultiLan);
    AnscTraceWarning(("  CosaDmlMlanInit -- status %d!\n", returnStatus));

#if !defined(INTEL_PUMA7) && !defined(_COSA_BCM_MIPS_) && !defined(_COSA_BCM_ARM_)
    printf("pnm-status is renamed to bring-lan and set in PSM for XB3\n");
#else
   printf("**************** sysevent set pnm-status up \n");
   fflush(stdout);
   system("sysevent set pnm-status up");
#endif

    pMyObject->hDiag          = (ANSC_HANDLE)CosaDiagnosticsCreate();
    AnscTraceWarning(("  CosaDiagnosticsCreate done!\n"));
    pMyObject->hTime          = (ANSC_HANDLE)CosaTimeCreate();
    AnscTraceWarning(("  CosaTimeCreate done!\n"));
#if !defined(_PLATFORM_IPQ_)
    //pMyObject->hMoCA          = (ANSC_HANDLE)CosaMoCACreate();
    AnscTraceWarning(("  CosaMoCACreate done!\n"));
    pMyObject->hUpnp          = (ANSC_HANDLE)CosaUpnpCreate();
    AnscTraceWarning(("  CosaUpnpCreate done!\n"));

    pMyObject->hParentalControl = (ANSC_HANDLE)TR181_ParentalControlCreate();
    AnscTraceWarning(("  CosaParentalControlCreate done - hParentalControl = 0x%X!\n", pMyObject->hParentalControl));
#endif
    pMyObject->hRLog          = (ANSC_HANDLE)CosaRLogCreate();
    AnscTraceWarning(("  CosaRLogCreate done!\n"));



#if !defined(HOTSPOT_DISABLE)
//#ifdef CONFIG_CISCO_HOTSPOT
	
    pMyObject->hGRE           = (ANSC_HANDLE)CosaGreCreate();
    AnscTraceWarning(("  CosaGreCreate done!\n"));
	//zqiu>>
	printf("-- %s %d CosaGreTunnelCreate\n", __func__, __LINE__);
    pMyObject->hTGRE       = (ANSC_HANDLE)CosaGreTunnelCreate();
    AnscTraceWarning(("  CosaGreTunnelCreate done!\n"));	
	//zqiu<<
    pMyObject->hCGRE           = (ANSC_HANDLE)CosaCGreCreate();
    AnscTraceWarning(("  CosaCGreCreate done!\n"));
    pMyObject->hHotspot       = (ANSC_HANDLE)CosaHotspotCreate();
    AnscTraceWarning(("  CosaHotspotCreate done!\n"));
	
//#endif
#endif
#if CONFIG_CISCO_FILE_TRANSFER
    pMyObject->hFileTransfer  = (ANSC_HANDLE)CosaFileTransferCreate();
    AnscTraceWarning(("  CosaFileTransferCreate done!\n"));
#endif

#if CONFIG_CISCO_TRUE_STATIC_IP
    pMyObject->hTSIP          = (ANSC_HANDLE)CosaTSIPCreate();
    AnscTraceWarning(("  CosaTSIPCreate done!\n"));
#endif
    
    pMyObject->hDeviceFingerprint          = (ANSC_HANDLE)CosaDeviceFingerprintCreate();
    AnscTraceWarning(("  CosaDeviceFingerprintCreate done!\n"));


#if CFG_USE_Event_Displatch

    /*create a thread to handle the sysevent asynchronously after all of the dmls created
     *
     * when a specified event trigger, the related functions which are registered previously
     * will run step by step.
     */
    EvtDispterHandleEventAsync();
#endif

    printf("PandM DM initialization done!\n");
    CcspTraceWarning(("RDKB_SYSTEM_BOOT_UP_LOG : PandM DM initialization done!\n"));
    //Unknown Reboot Reason 
    
    CheckAndSetRebootReason();

#ifdef PARODUS_ENABLE

    printf("Calling PARODUS!\n");
    CcspTraceWarning(("RDKB_SYSTEM_BOOT_UP_LOG : PARODUS call!\n")); 
    initparodusTask();

#endif    
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaBackEndManagerRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function remove cosa manager object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaBackEndManagerRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_BACKEND_MANAGER_OBJECT  pMyObject    = (PCOSA_BACKEND_MANAGER_OBJECT)hThisObject;

    /* Remove all objects */
    if ( pMyObject->hMultiLan )
    {
        CosaDmlMlanUnload(pMyObject->hMultiLan);
    }

    if ( pMyObject->hNat )
    {
        CosaNatRemove((ANSC_HANDLE)pMyObject->hNat);
    }

    if ( pMyObject->hDiag )
    {
        CosaDiagnosticsRemove((ANSC_HANDLE)pMyObject->hDiag);
    }

    if ( pMyObject->hDeviceInfo )
    {
        CosaDeviceInfoRemove((ANSC_HANDLE)pMyObject->hDeviceInfo);
    }
    
    if ( pMyObject->hTime )
    {
        CosaTimeRemove((ANSC_HANDLE)pMyObject->hTime);
    }

    if ( pMyObject->hUserinterface )
    {
        CosaUserinterfaceRemove((ANSC_HANDLE)pMyObject->hUserinterface);
    }

    if ( pMyObject->hEthernet )
    {
        CosaEthernetRemove((ANSC_HANDLE)pMyObject->hEthernet);
    }
/*
    if ( pMyObject->hMoCA )
    {
        CosaMoCARemove((ANSC_HANDLE)pMyObject->hMoCA);
    }
*/
    if ( pMyObject->hDhcpv4 )
    {
        CosaDhcpv4Remove((ANSC_HANDLE)pMyObject->hDhcpv4);
    }

    if ( pMyObject->hDhcpv6 )
    {
        CosaDhcpv6Remove((ANSC_HANDLE)pMyObject->hDhcpv6);
    }
    
    if ( pMyObject->hUsers )
    {
        CosaUsersRemove((ANSC_HANDLE)pMyObject->hUsers);
    }
    
    if ( pMyObject->hProcStatus )
    {
        COSADmlRemoveProcessInfo((ANSC_HANDLE)pMyObject->hProcStatus);
    }

    if ( pMyObject->hDdns )
    {
        CosaDdnsRemove((ANSC_HANDLE)pMyObject->hDdns);
    }

    if ( pMyObject->hFirewall )
    {
        CosaFirewallRemove((ANSC_HANDLE)pMyObject->hFirewall);
    }

    if ( pMyObject->hSecurity )
    {
        CosaSecurityRemove((ANSC_HANDLE)pMyObject->hSecurity);
    }
    
    if ( pMyObject->hIP )
    {
        CosaIPRemove((ANSC_HANDLE)pMyObject->hIP);
    }
    
    if ( pMyObject->hHosts )
    {
        CosaHostsRemove((ANSC_HANDLE)pMyObject->hHosts);
    }

    if ( pMyObject->hDNS )
    {
        CosaDNSRemove((ANSC_HANDLE)pMyObject->hDNS);
    }

    if( pMyObject->hRouting )
    {
        CosaRoutingRemove((ANSC_HANDLE)pMyObject->hRouting);
    }

    if( pMyObject->hBridging )
    {
        CosaBridgingRemove((ANSC_HANDLE)pMyObject->hBridging);
    }

    if ( pMyObject->hUpnp )
    {
        CosaUpnpRemove((ANSC_HANDLE)pMyObject->hUpnp);
    }
    
    if ( pMyObject->hInterfaceStack )
    {
        CosaIFStackRemove((ANSC_HANDLE)pMyObject->hInterfaceStack);
    }
        
    if ( pMyObject->hPPP )
    {
        CosaPPPRemove((ANSC_HANDLE)pMyObject->hPPP);
    }
        
    if ( pMyObject->hDeviceControl )
    {
        CosaDeviceControlRemove((ANSC_HANDLE)pMyObject->hDeviceControl);
    }

    if ( pMyObject->hIPv6rd )
    {
        CosaIPv6rdRemove((ANSC_HANDLE)pMyObject->hIPv6rd);
    }

    if ( pMyObject->hRA )
    {
        CosaRARemove((ANSC_HANDLE)pMyObject->hRA);
    }

    if ( pMyObject->hNeighdisc )
    {
        CosaNeighdiscRemove((ANSC_HANDLE)pMyObject->hNeighdisc);
    }

    if ( pMyObject->hMld )
    {
        CosaMldRemove((ANSC_HANDLE)pMyObject->hMld);
    }
    /*
#ifdef CONFIG_TI_PACM
    if ( pMyObject->hMTA )
    {
        CosaMTARemove((ANSC_HANDLE)pMyObject->hMTA);
    }
#endif
    */
    if ( pMyObject->hParentalControl )
    {
        TR181_ParentalControlRemove((ANSC_HANDLE)pMyObject->hParentalControl);
    }

#ifdef CONFIG_CISCO_HOTSPOT
    if ( pMyObject->hGRE )
    {
        CosaGreRemove((ANSC_HANDLE)pMyObject->hGRE);
    }

    if ( pMyObject->hCGRE )
    {
        CosaCGreRemove((ANSC_HANDLE)pMyObject->hCGRE);
    }

    if ( pMyObject->hHotspot )
    {
        CosaHotspotRemove((ANSC_HANDLE)pMyObject->hHotspot);
    }
	//zqiu>>
	if ( pMyObject->hTGRE )
    {		
		CosaGreTunnelRemove((ANSC_HANDLE)pMyObject->hTGRE);
    }
	//zqiu<<
#endif

#if CONFIG_CISCO_TRUE_STATIC_IP
    if ( pMyObject->hTSIP )
    {
        CosaTSIPRemove((ANSC_HANDLE)pMyObject->hTSIP);
    }
#endif

#if CONFIG_CISCO_FILE_TRANSFER
    if ( pMyObject->hFileTransfer )
    {
        CosaFileTransferRemove((ANSC_HANDLE)pMyObject->hFileTransfer);
    }
#endif

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

static void CheckAndSetRebootReason()
{
  
    int value = -1;
    if(fopen("/var/tmp/lastrebootreason","r")==NULL)
    {
        char rebootReason[64] = "unknown";
        char BOOT_TIME_LOG_FILE[32] = "/rdklogs/logs/BootTime.log";
        FILE *fpBootLogFile = NULL;

        CcspTraceWarning((" /var/tmp/lastrebootreason File doesn't exist --Create new file\n"));
        system("touch /var/tmp/lastrebootreason");
        //Check for Rebootcounter value--GET & SET
        value = getRebootCounter();
        if(value == -1)
        {
            CcspTraceWarning(("Error to GET Counter Value\n"));
        }
        else
        {
            value = value -1;
            if(value<0)
            {
                //SET unknown as reason 
                if(-1 == setUnknownRebootReason())
                     CcspTraceWarning(("Error to SET unknown reboot reason \n"));
            }
            else
            {
                char buf[64] = {'\0'};
                if(syscfg_get( NULL, "X_RDKCENTRAL-COM_LastRebootReason", buf, sizeof(buf))==0)
                {
                    AnscCopyString(rebootReason,buf);
                }
            }
                // reset counter to 0 for both known and unknown reason
                if(-1 == setRebootCounter())
                    CcspTraceWarning(("Error to SET reboot counter \n"));
        }

        fpBootLogFile = fopen(BOOT_TIME_LOG_FILE, "a+");
        if (NULL != fpBootLogFile)
        {
            fprintf(fpBootLogFile, "Received reboot_reason as:%s\n", rebootReason);
            fclose(fpBootLogFile);
        }
        else
        {
            CcspTraceWarning(("Fail to open BootTime.log to write reboot reason \n"));
        }
           
    }
    else
    {
        CcspTraceWarning(("/var/tmp/lastrebootreason File exists\n"));
    }
}
