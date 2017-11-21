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

    module: cosa_nat_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlNatGetPortMappingNumber
        *  CosaDmlNatIsUpdatedPortMappings
        *  CosaDmlNatSynchronizePortMappings
        *  CosaDmlNatGetPortMappings
        *  CosaDmlNatAddPortMapping
        *  CosaDmlNatDelPortMapping
        *  CosaDmlNatSetPortMapping
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
#include "cosa_apis.h"
#include "cosa_nat_apis.h"
#include "cosa_nat_internal.h"
#include "plugin_main_apis.h"
#include "dmsb_tr181_psm_definitions.h"

PFN_COSA_DML_NAT_GEN   g_nat_pportmapping_callback = NULL;
int g_count=0;//RDKB_EMULATOR 
int pt_count=0;//RDKB_EMULATOR 
static BOOL g_NatPFEnable  = FALSE;//RDKB_EMULATOR 
#if 1
// for PSM access
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
// PSM access MACRO FOR PF & PT
#define _PSM_WRITE_PARAM(_PARAM_NAME) { \
        _ansc_sprintf(param_name, _PARAM_NAME, instancenum); \
        retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value); \
        if (retPsmSet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d writing %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value));\
        } \
        else \
        { \
            AnscTraceFlow(("%s: retPsmSet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value)); \
           printf("%s: retPsmSet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value); \
        } \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_memset(param_value, 0, sizeof(param_value)); \
    }
#define _PSM_READ_PARAM(_PARAM_NAME) { \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_sprintf(param_name, _PARAM_NAME, instancenum); \
        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d reading %s %s\n", __FUNCTION__, retPsmGet, param_name, param_value));\
        } \
        else { \
            AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s = \n%s\n", __FUNCTION__,param_name, param_value)); \
                printf("param_name (%s) and param_value (%s) \n",param_name, param_value); \
        } \
    }
//PSM access MACRO for DMZ
#define _PSM_WRITE_PARAM_DMZ(_PARAM_NAME) { \
        _ansc_sprintf(param_name, _PARAM_NAME); \
        retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value); \
        if (retPsmSet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d writing %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value));\
        } \
        else \
        { \
            AnscTraceFlow(("%s: retPsmSet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value)); \
           printf("%s: retPsmSet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value); \
        } \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_memset(param_value, 0, sizeof(param_value)); \
    }

#define _PSM_READ_PARAM_DMZ(_PARAM_NAME) { \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_sprintf(param_name, _PARAM_NAME); \
        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d reading %s %s\n", __FUNCTION__, retPsmGet, param_name, param_value));\
        } \
        else { \
            AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s = \n%s\n", __FUNCTION__,param_name, param_value)); \
                printf("param_name (%s) and param_value (%s) \n",param_name, param_value); \
        } \
    }

#endif

#define _PSM_DEL_PARAM(_PARAM_NAME) { \
		 _ansc_sprintf(param_name, _PARAM_NAME, instancenum); \
		retPsmDel = PSM_Del_Record(bus_handle, g_Subsystem, param_name); \
		if (retPsmDel != CCSP_SUCCESS) { \
		    AnscTraceFlow(("%s Error %d deletion %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value));\
		} \
	        else \
        	{ \
            	   AnscTraceFlow(("%s: retPsmSet == CCSP_SUCCESS deletion %s = %s \n", __FUNCTION__,param_name,param_value)); \
	           printf("%s: retPsmSet == CCSP_SUCCESS deletion %s = %s \n", __FUNCTION__,param_name,param_value); \
        	} \
            _ansc_memset(param_name, 0, sizeof(param_name)); \
	}	

COSA_DML_NAT_PTRIGGER g_nat_porttrigger[32]=
        {
        };//RDKB_EMULATOR 

COSA_DML_NAT_PMAPPING   g_nat_portmapping[32] =
    {
        {
            0,
            "",
            TRUE,
            COSA_DML_NAT_STATUS_Enabled,
            "wan0",
            TRUE,
            232332,
            {
                "\x40\x40\x40\x01"
            },
            {
                "\x40\x40\x40\x01"
            },
            80,
            80,
            22222,
            2,    /* UDP */
            {
                "\xc0\xa8\x01\x01"
            },
            "this is for vod",
            "IntenalClientV6_0"
        },
        {
            0,
            "",
            TRUE,
            COSA_DML_NAT_STATUS_Enabled,
            "wan0",
            TRUE,
            21222,
            {
                "\x40\x40\x40\x02"
            },
            {
                "\x40\x40\x40\x02"
            },
            21,
            21,
            22222,
            1,   /* TCP */
            {
                "\xc0\xa8\x01\x02"
            },
            "this is for ftp",
            "IntenalClientV6_1"
        }
    };

COSA_DML_NAT_DMZ    g_Dmz =
    {
        FALSE,
        "10.10.10.10",
        "10.10.10.11",
        "0.0.0.0"
    };
void PSM_Set_DMZ_RecordValues(PCOSA_DML_NAT_DMZ pDmz)
{
        int retPsmSet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char param_value[256] = {0};
        if(pDmz->bEnabled == TRUE)
        AnscCopyString(param_value,"TRUE");
        else
        AnscCopyString(param_value,"FALSE");
        _PSM_WRITE_PARAM_DMZ(PSM_DMZ_ENABLE);
        AnscCopyString(param_value,pDmz->InternalIP);
        _PSM_WRITE_PARAM_DMZ(PSM_DMZ_INTERNALIP);
}
void PSM_Get_DMZ_RecordValues(PCOSA_DML_NAT_DMZ pDmz)
{
         int retPsmGet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char *param_value = NULL;
        _PSM_READ_PARAM_DMZ(PSM_DMZ_ENABLE);
        if(strcmp(param_value,"TRUE") == 0)
        pDmz->bEnabled = TRUE;
        else
        pDmz->bEnabled = FALSE;
        _PSM_READ_PARAM_DMZ(PSM_DMZ_INTERNALIP);
        AnscCopyString(pDmz->InternalIP,param_value);
}

ANSC_STATUS
CosaDmlNatGetLanIP
    (
        ANSC_HANDLE                 hContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

static int _check_PF_range(PCOSA_DML_NAT_PMAPPING pPortMapping){
	PCOSA_DML_NAT_PMAPPING pNatPMapping = NULL;
        char *param_value;
        ULONG numOfEntries = 0;
	if(g_count==0)//RDKB_EMULATOR
        {
           int i = 0;
           PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.NAT.PortMapping.MaxInstance", NULL, &param_value);
           if(!param_value){
              return TRUE;
           }else{
              numOfEntries = atol(param_value);
           }
           pNatPMapping = (PCOSA_DML_NAT_PMAPPING)AnscAllocateMemory( sizeof(g_nat_portmapping[0])*(g_count));
           for(i=1;i<=numOfEntries;i++){
               PSMGetPortMappingRecordValues(pNatPMapping,i);
               if(pPortMapping->InstanceNumber == pNatPMapping->InstanceNumber){
                   continue;
               }
               if(!strcmp(pPortMapping->Description,pNatPMapping->Description) ||
                  (pPortMapping->ExternalPort == pNatPMapping->ExternalPort || (pPortMapping->ExternalPort > pNatPMapping->ExternalPort &&
                  (pPortMapping->ExternalPort <= pNatPMapping->ExternalPortEndRange || pPortMapping->ExternalPortEndRange <= pNatPMapping->ExternalPortEndRange))) ||
                   (pPortMapping->ExternalPort < pNatPMapping->ExternalPort && pPortMapping->ExternalPortEndRange >= pNatPMapping->ExternalPort))
               {
                      return FALSE;
               }
           }
        }else{
            int i = 0;
            numOfEntries = ((sizeof(g_nat_portmapping[0])*g_count)/sizeof(COSA_DML_NAT_PMAPPING));
            for(i = 0;i < numOfEntries;i++){
               if(pPortMapping->InstanceNumber == g_nat_portmapping[i].InstanceNumber){
                   continue;
               }
               if(!strcmp(pPortMapping->Description,g_nat_portmapping[i].Description) ||
                  (pPortMapping->ExternalPort == g_nat_portmapping[i].ExternalPort || (pPortMapping->ExternalPort > g_nat_portmapping[i].ExternalPort &&
                  (pPortMapping->ExternalPort <= g_nat_portmapping[i].ExternalPortEndRange || pPortMapping->ExternalPortEndRange <= g_nat_portmapping[i].ExternalPortEndRange))) ||
                  (pPortMapping->ExternalPort < g_nat_portmapping[i].ExternalPort && pPortMapping->ExternalPortEndRange >= g_nat_portmapping[i].ExternalPort))
               {
                      return FALSE;
               }
           }
        }
        return TRUE;
}

static int _Check_PF_parameter(PCOSA_DML_NAT_PMAPPING pPortMapping)//RDKB_EMULATOR 
{
       if( pPortMapping->PublicIP.Value == 0 &&
                        ((pPortMapping->ExternalPort == 0) ||
                         (pPortMapping->ExternalPortEndRange < pPortMapping->ExternalPort) ||
                         (pPortMapping->Protocol > 3 || pPortMapping->Protocol < 1)) ||
                         (FALSE==_check_PF_range(pPortMapping)))
        {
                CcspTraceWarning(("Wrong Port Mapping parameter external Port %d ~ %d, protocol %d, InternalPort %d,InternalClient %x PublicIP %x\n", \
                                        pPortMapping->ExternalPort, pPortMapping->ExternalPortEndRange, \
                                        pPortMapping->Protocol,pPortMapping->InternalPort, pPortMapping->InternalClient.Value,\
                                        pPortMapping->PublicIP.Value ));
                return FALSE;
        }

        if( pPortMapping->InternalClient.Value == 0 ||
            FALSE == CosaDmlNatChkPortMappingClient(pPortMapping->InternalClient.Value))
        {
                CcspTraceWarning(("Wrong InternalClient value %x\n",pPortMapping->InternalClient.Value ));
                return FALSE;
        }

    return TRUE;
}

static int _check_PT_range(PCOSA_DML_NAT_PTRIGGER pPortTrigger){
        PCOSA_DML_NAT_PTRIGGER pNatPTrigger = NULL;
        char *param_value;
        ULONG numOfEntries = 0;
        if(pt_count==0)//RDKB_EMULATOR
        {
           int i = 0;
           PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Provision.COSALibrary.NAT.PORTTRIGGER.NextInstanceNumber", NULL, &param_value);
           if(!param_value){
              return TRUE;
           }else{
              numOfEntries = atol(param_value) - 1;
           }
           pNatPTrigger = (PCOSA_DML_NAT_PTRIGGER)AnscAllocateMemory(sizeof(g_nat_porttrigger[0])*(pt_count));
           for(i=1;i<=numOfEntries;i++){
               PSMGetPortTriggerRecordValues(pNatPTrigger,i);
                if(pPortTrigger->InstanceNumber == pNatPTrigger->InstanceNumber){
                   continue;
               }
               if(!strcmp(pPortTrigger->Description,pNatPTrigger->Description) ||
                  (pPortTrigger->TriggerPortStart == pNatPTrigger->TriggerPortStart || (pPortTrigger->TriggerPortStart > pNatPTrigger->TriggerPortStart &&
                  (pPortTrigger->TriggerPortStart <= pNatPTrigger->TriggerPortEnd || pPortTrigger->TriggerPortEnd <= pNatPTrigger->TriggerPortEnd))) ||
                  (pPortTrigger->TriggerPortStart < pNatPTrigger->TriggerPortStart && pPortTrigger->TriggerPortEnd >= pNatPTrigger->TriggerPortStart) ||
                  (pPortTrigger->ForwardPortStart == pNatPTrigger->ForwardPortStart || (pPortTrigger->ForwardPortStart > pNatPTrigger->ForwardPortStart &&
                  (pPortTrigger->ForwardPortStart <= pNatPTrigger->ForwardPortEnd || pPortTrigger->ForwardPortEnd <= pNatPTrigger->ForwardPortEnd))) ||
                  (pPortTrigger->ForwardPortStart < pNatPTrigger->ForwardPortStart && pPortTrigger->ForwardPortEnd >= pNatPTrigger->ForwardPortStart))
               {
                      return FALSE;
               }
           }
        }else{
            int i = 0;
            numOfEntries = (sizeof(g_nat_porttrigger[0])*(pt_count+1))/sizeof(COSA_DML_NAT_PTRIGGER);
            for(i = 0;i < numOfEntries;i++){
               if(pPortTrigger->InstanceNumber == g_nat_porttrigger[i].InstanceNumber){
                   continue;
               }
               if(!strcmp(pPortTrigger->Description,g_nat_porttrigger[i].Description) ||
                  (pPortTrigger->TriggerPortStart == g_nat_porttrigger[i].TriggerPortStart || (pPortTrigger->TriggerPortStart > g_nat_porttrigger[i].TriggerPortStart &&
                  (pPortTrigger->TriggerPortStart <= g_nat_porttrigger[i].TriggerPortEnd || pPortTrigger->TriggerPortEnd <= g_nat_porttrigger[i].TriggerPortEnd))) ||
                  (pPortTrigger->TriggerPortStart < g_nat_porttrigger[i].TriggerPortStart && pPortTrigger->TriggerPortEnd >= g_nat_porttrigger[i].TriggerPortStart) ||
                  (pPortTrigger->ForwardPortStart == g_nat_porttrigger[i].ForwardPortStart || (pPortTrigger->ForwardPortStart > g_nat_porttrigger[i].ForwardPortStart &&
                  (pPortTrigger->ForwardPortStart <= g_nat_porttrigger[i].ForwardPortEnd || pPortTrigger->ForwardPortEnd <= g_nat_porttrigger[i].ForwardPortEnd))) ||
                  (pPortTrigger->ForwardPortStart < g_nat_porttrigger[i].ForwardPortStart && pPortTrigger->ForwardPortEnd >= g_nat_porttrigger[i].ForwardPortStart))
               {
                      return FALSE;
               }
           }
        }
        return TRUE;
}

static BOOL _Check_PT_parameter(PCOSA_DML_NAT_PTRIGGER pPortTrigger)
{
    // Check parameter setting
    if( (pPortTrigger->TriggerProtocol > 2 || pPortTrigger->TriggerProtocol < 0) ||
        (pPortTrigger->TriggerPortStart == 0) ||
        (pPortTrigger->TriggerPortEnd == 0) ||
        (pPortTrigger->TriggerPortEnd < pPortTrigger->TriggerPortStart) ||
        (pPortTrigger->ForwardPortEnd == 0) ||
        (pPortTrigger->ForwardPortStart == 0) ||
        (pPortTrigger->ForwardPortEnd < pPortTrigger->ForwardPortStart) ||
        (FALSE==_check_PT_range(pPortTrigger)))
    {
        CcspTraceWarning(("Port Trigger parameter error \n"));
        return FALSE;
    }
    return TRUE;
}
/**********************************************************************

    caller:     self

    prototype:

        BOOL
        CosaDmlNatInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext,
                PFN_COSA_DML_NAT_GEN        pValueGenFn
            );

        Description:
            This is the initialization routine for NAT backend.

        Arguments:
            hDml               Opaque handle from DM adapter. Backend saves this handle for calling pValueGenFn.
             phContext       Opaque handle passed back from backend, needed by CosaDmlNatXyz() routines.
            pValueGenFn    Function pointer to instance number/alias generation callback.

        Return:
            Status of operation.

**********************************************************************/
ANSC_STATUS
CosaDmlNatInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext,
        PFN_COSA_DML_NAT_GEN        pValueGenFn
    )
{
    g_nat_pportmapping_callback = pValueGenFn;

    return ANSC_STATUS_SUCCESS;

}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGet
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_DMZ           pDmlDmz
            )

        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/
ANSC_STATUS
CosaDmlNatGet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    )
{
	char *param_value;//RDKB_EMULATOR
	char *pf_enable = "dmsb.nat.PortMapping.Enable";
	char param_name[100] ={0};
	memset(param_name, 0, sizeof(param_name));
	sprintf(param_name,pf_enable);
	PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value);
	if(param_value!=NULL){
        	if(strcmp(param_value,"0") == 0)
        	g_NatPFEnable = FALSE;
	        else
        	g_NatPFEnable = TRUE;
		COSA_DML_NAT_CUSTOM_SET_ENABLEPORTMAPPING(pDmlNat, g_NatPFEnable);
	}
	else{
		return 0;
	}
	return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSet
            (
                ANSC_HANDLE                 hContext,

            )
        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/
ANSC_STATUS
CosaDmlNatSet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    )
{
    if ( COSA_DML_NAT_CUSTOM_GET_ENABLEPORTMAPPING(pDmlNat) )//RDKB_EMULATOR
    {
	g_NatPFEnable=TRUE;	
	#if 1
	char param_value[50] = {0};
        char param_name[100] ={0};
        char *pf_enable = "dmsb.nat.PortMapping.Enable";
        memset(param_name, 0, sizeof(param_name));
        sprintf(param_name,pf_enable);
        sprintf(param_value,"%d",g_NatPFEnable);
        PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
	#endif
        printf("X_Comcast_com_EnablePortMapping is enabled!\n");
    }
    else
    {
	port_forwarding_disable();//RDKB_EMULATOR
	#if 1
	g_NatPFEnable=FALSE;	
        char param_value[50] = {0};
        char param_name[100] ={0};
        char *pf_enable = "dmsb.nat.PortMapping.Enable";
        memset(param_name, 0, sizeof(param_name));
        sprintf(param_name,pf_enable);
        sprintf(param_value,"%d",g_NatPFEnable);
        PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
        #endif
        printf("X_Comcast_com_EnablePortMapping is disabled!\n");
    }

    if ( COSA_DML_NAT_CUSTOM_GET_ENABLEHSPORTMAPPING(pDmlNat) )
    {
        printf("X_Comcast_com_EnableHSPortMapping is enabled!\n");
    }
    else
    {
        printf("X_Comcast_com_EnableHSPortMapping is disabled!\n");
    }

    if (COSA_DML_NAT_CUSTOM_GET_ENABLENATMAPPING(pDmlNat) )
    {
        printf("X_Comcast_com_EnableNATMapping is enabled!\n");
    }
    else
    {
        printf("X_Comcast_com_EnableNATMapping is disabled!\n");
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGetDmz
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_DMZ           pDmlDmz
            )

        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/

ANSC_STATUS
CosaDmlNatGetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmlDmz
    )
{
    AnscCopyMemory(pDmlDmz, &g_Dmz, sizeof(COSA_DML_NAT_DMZ));
    PSM_Get_DMZ_RecordValues(pDmlDmz);//RDKB_EMULATOR
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSetDmz
            (
                ANSC_HANDLE                 hContext,

            )
        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/

ANSC_STATUS
CosaDmlNatSetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmz
    )
{
    AnscCopyMemory(&g_Dmz, pDmz, sizeof(COSA_DML_NAT_DMZ));
//RDKB_EMULATOR
 	int retPsmSet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char param_value[256] = {0};

        if(pDmz->bEnabled == TRUE)
                AnscCopyString(param_value,"TRUE");
        else
                AnscCopyString(param_value,"FALSE");
        _PSM_WRITE_PARAM_DMZ(PSM_DMZ_ENABLE);
	if(pDmz->bEnabled == FALSE)
        {
                DeleteDMZIptableRules();
                AnscCopyString(pDmz->InternalIP,"0.0.0.0");
                PSM_Set_DMZ_RecordValues(pDmz);
        }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_NAT_PMAPPING
        CosaDmlNatGetPortMappings
            (
                ANSC_HANDLE                 hContext,
                PULONG                          pulCount
            )
        Description:
            This routine is to retrieve the complete list of NAT port mappings, which is a dynamic table.

        Arguments:
            pulCount  is to receive the actual number of entries.

        Return:
            The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
int PSMGetPortMappingRecordValues(PCOSA_DML_NAT_PMAPPING  pMapping,ULONG instancenum)//RDKB_EMULATOR
{
        int retPsmGet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char *param_value = NULL;

        pMapping->InstanceNumber=instancenum;
	
	_PSM_READ_PARAM(PSM_MAPPING_Enable);
	if(strcmp(param_value,"TRUE") == 0)
                pMapping->bEnabled = TRUE;
        else
                pMapping->bEnabled = FALSE;

	_PSM_READ_PARAM(PSM_MAPPING_Status);
	if(strcmp(param_value,"Enabled"))
		pMapping->Status = TRUE;
	else
		pMapping->Status = FALSE;

	_PSM_READ_PARAM(PSM_MAPPING_AllInterfaces);
	if(strcmp(param_value,"TRUE") == 0)
		pMapping->AllInterfaces=TRUE;
	else
		pMapping->AllInterfaces=FALSE;

	_PSM_READ_PARAM(PSM_MAPPING_LeaseDuration)
	pMapping->LeaseDuration=atoi(param_value);
	
	_PSM_READ_PARAM(PSM_MAPPING_ExternalPort);
	pMapping->ExternalPort=atoi(param_value);

	_PSM_READ_PARAM(PSM_MAPPING_ExternalPortEndRange);
	pMapping->ExternalPortEndRange=atoi(param_value);

	_PSM_READ_PARAM(PSM_MAPPING_InternalPort);
	pMapping->InternalPort=atoi(param_value);

	_PSM_READ_PARAM(PSM_MAPPING_Protocol);
	pMapping->Protocol=atoi(param_value);

	_PSM_READ_PARAM(PSM_MAPPING_Description);
	AnscCopyString(	pMapping->Description,param_value);

	_PSM_READ_PARAM(PSM_MAPPING_Alias);
	AnscCopyString( pMapping->Alias,param_value);

	_PSM_READ_PARAM(PSM_MAPPING_InternalClient);
	pMapping->InternalClient.Value=inet_addr(param_value);

    return 0;
}

PCOSA_DML_NAT_PMAPPING
CosaDmlNatGetPortMappings
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
	if(g_count==0)//RDKB_EMULATOR
	{
		PCOSA_DML_NAT_PMAPPING pNatPMapping = NULL;
		char *param_value;
		int i;
		ULONG numOfEntries;
		PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.NAT.PortMapping.MaxInstance", NULL, &param_value);
		if(param_value!=NULL){
			numOfEntries = atol(param_value);
			g_count=numOfEntries;
		}
		else{
			return 0;
		}
		pNatPMapping = (PCOSA_DML_NAT_PMAPPING)AnscAllocateMemory( sizeof(g_nat_portmapping[0])*(g_count));
		if ( pNatPMapping )
		{
			for(i=1;i<=numOfEntries;i++){
				PSMGetPortMappingRecordValues(pNatPMapping,i);
				g_nat_portmapping[i-1]=*pNatPMapping;
			}
			AnscCopyMemory( pNatPMapping,g_nat_portmapping,sizeof(g_nat_portmapping[0])*(g_count));
			*pulCount=g_count;
		}
		else
		{
			*pulCount = 0;
		}

		return pNatPMapping;

	}
	else{
		PCOSA_DML_NAT_PMAPPING pNatPMapping = NULL;
		ULONG                         index = 0;
		for(index =0; index < sizeof(g_nat_portmapping[0])*g_count/sizeof(COSA_DML_NAT_PMAPPING); index++)
		{
			if ( g_nat_portmapping[index].InstanceNumber  ==  0 )
			{
				g_nat_pportmapping_callback( NULL, &g_nat_portmapping[index] );
			}
		}

		pNatPMapping = (PCOSA_DML_NAT_PMAPPING)AnscAllocateMemory( sizeof(g_nat_portmapping[0])*(g_count+1));

		if ( pNatPMapping )
		{
			AnscCopyMemory(pNatPMapping, g_nat_portmapping, sizeof(g_nat_portmapping[0])*g_count);
			*pulCount=g_count;
		}
		else
		{
			*pulCount = 0;
		}

		return pNatPMapping;
	}
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatAddPortMapping
            (
                ANSC_HANDLE                           hContext,
                PCOSA_DML_NAT_PMAPPING      pEntry
            )

    Description:
        The API adds one port mapping entry into NAT.

    Arguments:
        pEntry      Caller does not need to fill in Status or Alias fields. Upon return, callee fills in the generated Alias and associated Status.

    Return:
        he pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
int PSMSetPortMappingRecordValues(PCOSA_DML_NAT_PMAPPING pMapping,ULONG instancenum)//RDKB_EMULATOR
{
        int retPsmSet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char param_value[256] = {0};
        if (pMapping->bEnabled==TRUE){
        _ansc_sprintf(param_value,"%s","TRUE");
        }else{
        _ansc_sprintf(param_value,"%s","FALSE");
        }
        _PSM_WRITE_PARAM(PSM_MAPPING_Enable);

	if(pMapping->Status==COSA_DML_NAT_STATUS_Enabled){
	_ansc_sprintf(param_value,"%s","Enabled");
	}else{
        _ansc_sprintf(param_value,"%s","Disabled");
        }
	_PSM_WRITE_PARAM(PSM_MAPPING_Status);

	if(pMapping->AllInterfaces==TRUE){
	_ansc_sprintf(param_value,"%s","TRUE");
	}else{
        _ansc_sprintf(param_value,"%s","FALSE");
        }
	_PSM_WRITE_PARAM(PSM_MAPPING_AllInterfaces);

	_ansc_sprintf(param_value,"%d",pMapping->LeaseDuration);
	_PSM_WRITE_PARAM(PSM_MAPPING_LeaseDuration);

	_ansc_sprintf(param_value, "%u",pMapping->ExternalPort);
        _PSM_WRITE_PARAM(PSM_MAPPING_ExternalPort);
	
	_ansc_sprintf(param_value, "%u",pMapping->ExternalPortEndRange);
	_PSM_WRITE_PARAM(PSM_MAPPING_ExternalPortEndRange);

	_ansc_sprintf(param_value, "%u",pMapping->InternalPort);
	 _PSM_WRITE_PARAM(PSM_MAPPING_InternalPort);
	
	_ansc_sprintf(param_value, "%u",pMapping->Protocol);
	_PSM_WRITE_PARAM(PSM_MAPPING_Protocol);

	_ansc_sprintf(param_value, "%s",pMapping->Description);
	_PSM_WRITE_PARAM(PSM_MAPPING_Description);
	
	 _ansc_sprintf(param_value, "%s",pMapping->Alias);
	_PSM_WRITE_PARAM(PSM_MAPPING_Alias);

	_ansc_sprintf(param_value, "%u.%u.%u.%u",pMapping->InternalClient.Dot[0],pMapping->InternalClient.Dot[1],pMapping->InternalClient.Dot[2],pMapping->InternalClient.Dot[3]);
	_PSM_WRITE_PARAM(PSM_MAPPING_InternalClient);

    return 0;
}

ANSC_STATUS
CosaDmlNatAddPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry
    )
{
	ULONG                          index = 0;
        char cmd[1024]= {'\0'};
        char *prot;
        index=sizeof(g_nat_portmapping[0])*g_count/sizeof(COSA_DML_NAT_PMAPPING);
	printf("index=%d\n",index);
        PCOSA_DML_NAT_PMAPPING pMapping;
        extern ANSC_HANDLE bus_handle;//RDKB_EMULATOR PSM Access
        extern char g_Subsystem[32];
        char param_value[50] = {0};
        char *Max_instance = "Device.NAT.PortMapping.MaxInstance";
        char param_name[100] ={0};
        pMapping=(PCOSA_DML_NAT_PMAPPING)AnscAllocateMemory( sizeof(COSA_DML_NAT_PMAPPING));

        if (!pEntry)
        {
               return ANSC_STATUS_FAILURE;
        }

        if( !_Check_PF_parameter(pEntry))
        {
               CcspTraceWarning(("Parameter Error in %s \n", __FUNCTION__));
               return ANSC_STATUS_FAILURE;
        }

        if(pEntry != NULL)//RDKB_EMULATOR
        {
                g_count++;
		pMapping->InstanceNumber=pEntry->InstanceNumber;
		pMapping->bEnabled=pEntry->bEnabled;
		pMapping->Status=COSA_DML_NAT_STATUS_Enabled;
		pMapping->AllInterfaces=TRUE;
		pMapping->LeaseDuration = 0;
		pMapping->RemoteHost.Value = 0;
		pMapping->PublicIP.Value = 0;
		pMapping->ExternalPort =  pEntry->ExternalPort;
		pMapping->ExternalPortEndRange = pEntry->ExternalPortEndRange;
		pMapping->InternalPort = pEntry->InternalPort;
          	pMapping->Protocol =  pEntry->Protocol;
		AnscCopyString(pMapping->Alias, pEntry->Description);
                pMapping->InternalClient.Value = pEntry->InternalClient.Value;
                strncpy(pMapping->X_CISCO_COM_InternalClientV6,pEntry->X_CISCO_COM_InternalClientV6,
                                sizeof(pMapping->X_CISCO_COM_InternalClientV6));
                strncpy(pMapping->Description,pEntry->Description, sizeof(pMapping->Description));
		PSMSetPortMappingRecordValues(pMapping,pMapping->InstanceNumber);
		g_nat_portmapping[index]=*pMapping;
		g_nat_portmapping[index].InstanceNumber=pEntry->InstanceNumber;
                //Add PortForwarding Rule For TCP,UDP,TCP/UDP
                        prot = g_nat_portmapping[index].Protocol==1?"tcp":g_nat_portmapping[index].Protocol==2?"udp":"both";
                        port_forwarding_add_rule(g_nat_portmapping[index].InternalClient.Dot,prot,g_nat_portmapping[index].ExternalPort,g_nat_portmapping[index].ExternalPortEndRange);
        }
        memset(param_name, 0, sizeof(param_name));//RDKB_EMULATOR PSM Access
        sprintf(param_name,Max_instance);
        sprintf(param_value,"%d",g_count);
        PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatDelPortMapping
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    Description:
        The API removes the designated port mapping entry from NAT.
    Arguments:
        pAlias      The entry is identified through Alias.
    Return:
        The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNatDelPortMapping
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
    {
	    ULONG                          index = 0;
	    PCOSA_DML_NAT_PMAPPING pNatPMapping;
	    char cmd[1024]= {'\0'};
	    char *prot;
            extern ANSC_HANDLE bus_handle;//RDKB_EMULATOR PSM Access
            extern char g_Subsystem[32];
            char param_value[50] = {0};
            char *Max_instance = "Device.NAT.PortMapping.MaxInstance";
            char param_name[100] ={0};
	    for(index =0; index < sizeof(g_nat_portmapping[0])*g_count/sizeof(COSA_DML_NAT_PMAPPING); index++)
	    {
		    if (g_nat_portmapping[index].InstanceNumber==ulInstanceNumber)//RDKB_EMULATOR
		    {
			    //Delete PortForwarding Rule For TCP,UDP,TCP/UDP
			    prot = g_nat_portmapping[index].Protocol==1?"tcp":g_nat_portmapping[index].Protocol==2?"udp":"both";
			    port_forwarding_delete_rule(g_nat_portmapping[index].InternalClient.Dot,prot,g_nat_portmapping[index].ExternalPort,g_nat_portmapping[index].ExternalPortEndRange);
		    }
	    }
	pNatPMapping = (PCOSA_DML_NAT_PMAPPING)AnscAllocateMemory( sizeof(g_nat_portmapping[0])*(g_count));//PSM_ACCESS

	    for(index=ulInstanceNumber-1;index<g_count-1;index++)
	    {
		    g_nat_portmapping[index]=g_nat_portmapping[index+1];
		    if(g_nat_portmapping[index].InstanceNumber!=0){
			    g_nat_portmapping[index].InstanceNumber--;}
		    /*Update PSM PortMapping Data*/
		    *pNatPMapping = g_nat_portmapping[index];
		    PSMSetPortMappingRecordValues(pNatPMapping,pNatPMapping->InstanceNumber);
	    }
	    g_count--;
            memset(param_name, 0, sizeof(param_name));//RDKB_EMULATOR PSM Access
            sprintf(param_name,Max_instance);
            sprintf(param_value,"%d",g_count);
            PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);

	    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGetPortMappings
            (
                ANSC_HANDLE                 hThisObject
            );

    Description:
        The API re-configures the designated port mapping entry.
    Arguments:
        pAlias      The entry is identified through Alias.
        pEntry      The new configuration is passed through this argument, even Alias field can be changed.

    Return:
    The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNatSetPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry          /* Identified by InstanceNumber */
    )
{
	ULONG                          index = 0;
	char cmd[1024]= {'\0'};
	char str[1024]= {'\0'};
	char *prot,*del;
	PCOSA_DML_NAT_PMAPPING pMapping;
	pMapping=(PCOSA_DML_NAT_PMAPPING)AnscAllocateMemory( sizeof(COSA_DML_NAT_PMAPPING));

        if (!pEntry)
        {
                return ANSC_STATUS_FAILURE;
        }

        /* Check parameter */
        if( !_Check_PF_parameter(pEntry))
        {
                CcspTraceWarning(("Parameter Error in %s \n", __FUNCTION__));
                return ANSC_STATUS_FAILURE;
        }

	for(index =0; index < sizeof(g_nat_portmapping)/sizeof(COSA_DML_NAT_PMAPPING); index++)
	{
		if ( g_nat_portmapping[index].InstanceNumber  ==  pEntry->InstanceNumber )//RDKB_EMULATOR
		{
			del = g_nat_portmapping[index].Protocol==1?"tcp":g_nat_portmapping[index].Protocol==2?"udp":"both";
			port_forwarding_delete_rule(g_nat_portmapping[index].InternalClient.Dot,del,g_nat_portmapping[index].ExternalPort,g_nat_portmapping[index].ExternalPortEndRange);
			if(pEntry != NULL)//RDKB_EMULATOR
			{
				pMapping->InstanceNumber=pEntry->InstanceNumber;
				pMapping->bEnabled=pEntry->bEnabled;
				pMapping->Status=COSA_DML_NAT_STATUS_Enabled;
				pMapping->AllInterfaces=TRUE;
				pMapping->LeaseDuration = 0;
				pMapping->RemoteHost.Value = 0;
				pMapping->PublicIP.Value = 0;
				pMapping->ExternalPort =  pEntry->ExternalPort;
				pMapping->ExternalPortEndRange = pEntry->ExternalPortEndRange;
				pMapping->InternalPort = pEntry->InternalPort;
				pMapping->Protocol =  pEntry->Protocol;
				AnscCopyString(pMapping->Alias, pEntry->Description);
				pMapping->InternalClient.Value = pEntry->InternalClient.Value;
				strncpy(pMapping->X_CISCO_COM_InternalClientV6,pEntry->X_CISCO_COM_InternalClientV6,
						sizeof(pMapping->X_CISCO_COM_InternalClientV6));
				strncpy(pMapping->Description,pEntry->Description, sizeof(pMapping->Description));
				PSMSetPortMappingRecordValues(pMapping,pMapping->InstanceNumber);
				g_nat_portmapping[index]=*pMapping;
				g_nat_portmapping[index].InstanceNumber=pEntry->InstanceNumber;

				//After set function iptable rules are deleted and add new iptable to same ruleNumber
				prot = g_nat_portmapping[index].Protocol==1?"tcp":g_nat_portmapping[index].Protocol==2?"udp":"both";
				port_forwarding_add_rule(g_nat_portmapping[index].InternalClient.Dot,prot,g_nat_portmapping[index].ExternalPort,g_nat_portmapping[index].ExternalPortEndRange);
			}break;
		}
	}
	return ANSC_STATUS_SUCCESS;
}

/* TBC      -- need to be normalized */
BOOL CosaDmlNatChkEnableFlg(PCOSA_DML_NAT_PMAPPING pPortMapping)
{
    return  TRUE;
}

BOOL CosaDmlNatChkPortMappingClient(ULONG client)
{
     ULONG ipaddr;
     ULONG netmask;
     BOOL ret;
     netmask=CosaUtilIoctlXXX("brlan0","netmask",NULL);
     ipaddr=CosaUtilGetIfAddr("brlan0");

    if((client != ipaddr) &&
        !IPv4Addr_IsBroadcast(client, ipaddr, netmask) &&
        !IPv4Addr_IsNetworkAddr(client, ipaddr, netmask) &&
        (IPv4Addr_IsSameNetwork(client, ipaddr, netmask) || IPv4Addr_IsSameNetwork(client, 0xac100c00, 0xffffff00)))
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_NAT_PTRIGGER
        CosaDmlNatGetPortTriggers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            )
        Description:
            This routine is to retrieve the complete list of NAT port triggers, which is a table.

        Arguments:
            pulCount  is to receive the actual number of entries.

        Return:
            The pointer to the array of NAT port triggers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
int PSMGetPortTriggerRecordValues(PCOSA_DML_NAT_PTRIGGER pTrigger,ULONG instancenum)//RDKB_EMULATOR
{
        int retPsmGet = CCSP_SUCCESS;
	char param_name[256] = {0};
        char *param_value = NULL;

	pTrigger->InstanceNumber=instancenum;	

        _PSM_READ_PARAM(PSM_TRIGGERING_Enable);
	if(strcmp(param_value,"TRUE") == 0)
                pTrigger->bEnabled = TRUE;
        else
                pTrigger->bEnabled = FALSE;
	_PSM_READ_PARAM(PSM_TRIGGERING_TriggerProtocol);
        pTrigger->TriggerProtocol=atoi(param_value);

	_PSM_READ_PARAM(PSM_TRIGGERING_ForwardProtocol);
	pTrigger->ForwardProtocol=atoi(param_value);

	_PSM_READ_PARAM(PSM_TRIGGERING_TriggerPortStart);
	pTrigger->TriggerPortStart=atoi(param_value);
	
	 _PSM_READ_PARAM(PSM_TRIGGERING_TriggerPortEnd );
	pTrigger->TriggerPortEnd=atoi(param_value);

	 _PSM_READ_PARAM(PSM_TRIGGERING_ForwardPortStart);
	pTrigger->ForwardPortStart=atoi(param_value);

	_PSM_READ_PARAM(PSM_TRIGGERING_ForwardPortEnd);
	pTrigger->ForwardPortEnd=atoi(param_value);

	 _PSM_READ_PARAM(PSM_TRIGGERING_Description);
	AnscCopyString(pTrigger->Description,param_value);
	AnscCopyString(pTrigger->Alias,pTrigger->Description);
	
        return 0;
}

PCOSA_DML_NAT_PTRIGGER
CosaDmlNatGetPortTriggers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )

{
	if(pt_count==0){//RDKB_EMULATOR
		PCOSA_DML_NAT_PTRIGGER          pNatPTrigger = NULL;
		char *param_value;
		int i;
		ULONG numOfEntries;
		PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Provision.COSALibrary.NAT.PORTTRIGGER.NextInstanceNumber", NULL, &param_value);
                if(param_value==NULL){
                        *pulCount = 0;
                        return NULL;
		}else{
			numOfEntries = atol(param_value);
			numOfEntries=numOfEntries-1;
			pt_count=numOfEntries;
		}
		pNatPTrigger = (PCOSA_DML_NAT_PTRIGGER)AnscAllocateMemory(sizeof(g_nat_porttrigger[0])*(pt_count));//RDKB_EMULATOR
		if (  pNatPTrigger )
		{
			for(i=1;i<=numOfEntries;i++){
				PSMGetPortTriggerRecordValues(pNatPTrigger,i);
				g_nat_porttrigger[i-1]=*pNatPTrigger;
			}
			AnscCopyMemory( pNatPTrigger, g_nat_porttrigger,sizeof(g_nat_porttrigger[0])*(pt_count));
			*pulCount=pt_count;
			printf("pulCount%u\n",*pulCount);
		}
		else
		{
			*pulCount = 0;
		}

		return  pNatPTrigger;
	}
	else{
		PCOSA_DML_NAT_PTRIGGER          pNatPTrigger = NULL;
		pNatPTrigger = (PCOSA_DML_NAT_PTRIGGER)AnscAllocateMemory( sizeof(g_nat_porttrigger[0])*(pt_count+1));//RDKB_EMULATOR
		if (  pNatPTrigger )
		{
			AnscCopyMemory( pNatPTrigger, g_nat_porttrigger, sizeof(g_nat_porttrigger[0])*(pt_count));
			*pulCount=pt_count;
		}
		else
		{
			*pulCount = 0;
		}
		return  pNatPTrigger;
	}
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatAddPortTrigger
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            )

    Description:
        The API adds one port trigger entry into NAT.

    Arguments:
        pEntry      Caller does not need to fill in Status or Alias fields. Upon return, callee fills in the generated Alias and associated Status.

    Return:
        Status of the operation.

**********************************************************************/
int PSMSetPortTriggerRecordValues(PCOSA_DML_NAT_PTRIGGER pTrigger,ULONG instancenum)//RDKB_EMULATOR
{
        int retPsmSet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char param_value[256] = {0};
	if (pTrigger->bEnabled==TRUE){
	_ansc_sprintf(param_value,"%s","TRUE");
	}else{
	_ansc_sprintf(param_value,"%s","FALSE");
	}
	_PSM_WRITE_PARAM(PSM_TRIGGERING_Enable);
	
	_ansc_sprintf(param_value, "%u",pTrigger->TriggerProtocol);
	_PSM_WRITE_PARAM(PSM_TRIGGERING_TriggerProtocol);

	_ansc_sprintf(param_value, "%u",pTrigger->ForwardProtocol);
        _PSM_WRITE_PARAM(PSM_TRIGGERING_ForwardProtocol);

	_ansc_sprintf(param_value,"%u",pTrigger->TriggerPortStart);
	_PSM_WRITE_PARAM( PSM_TRIGGERING_TriggerPortStart);

	_ansc_sprintf(param_value,"%u",pTrigger->TriggerPortEnd);
	_PSM_WRITE_PARAM(PSM_TRIGGERING_TriggerPortEnd );
	
	_ansc_sprintf(param_value,"%u",pTrigger->ForwardPortStart);
	_PSM_WRITE_PARAM(PSM_TRIGGERING_ForwardPortStart);

	_ansc_sprintf(param_value,"%u",pTrigger->ForwardPortEnd);
	_PSM_WRITE_PARAM(PSM_TRIGGERING_ForwardPortEnd);

	_ansc_sprintf(param_value,"%s",pTrigger->Description);
	_PSM_WRITE_PARAM(PSM_TRIGGERING_Description);
	
        return 0;
}
ANSC_STATUS
CosaDmlNatAddPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    )
{
	ULONG                          index = 0;
        char cmd[1024]= {'\0'};
        char *prot;
        index=sizeof(g_nat_porttrigger[0])*pt_count/sizeof(COSA_DML_NAT_PTRIGGER);
	PCOSA_DML_NAT_PTRIGGER pTrigger;
        extern ANSC_HANDLE bus_handle;//RDKB_EMULATOR PSM Access
        extern char g_Subsystem[32];
        char param_value[50] = {0};
        char *PtNext_instance = "Provision.COSALibrary.NAT.PORTTRIGGER.NextInstanceNumber";
        char param_name[100] ={0};
        ULONG ulInstanceNumber = 0;
        PCOSA_DATAMODEL_NAT pNat = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
        ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;
        pTrigger=(PCOSA_DML_NAT_PTRIGGER)AnscAllocateMemory( sizeof(COSA_DML_NAT_PTRIGGER));

        if (!pEntry)
        {
           return ANSC_STATUS_FAILURE;
        }

        if(pEntry != NULL)//RDKB_EMULATOR
        {
                pt_count++;
		pTrigger->InstanceNumber=pEntry->InstanceNumber;
		pTrigger->bEnabled=pEntry->bEnabled;
		pTrigger->TriggerProtocol=pEntry->TriggerProtocol;
		pTrigger->TriggerPortStart=pEntry->TriggerPortStart;
		pTrigger->TriggerPortEnd=pEntry->TriggerPortEnd;
		pTrigger->ForwardProtocol=pEntry->ForwardProtocol;
		pTrigger->ForwardPortStart=pEntry->ForwardPortStart;
		pTrigger->ForwardPortEnd=pEntry->ForwardPortEnd;
		strncpy(pTrigger->Description,pEntry->Description, sizeof(pTrigger->Description));
                PSMSetPortTriggerRecordValues(pTrigger,pTrigger->InstanceNumber);
		g_nat_porttrigger[index]=*pTrigger;
		g_nat_porttrigger[index].InstanceNumber=pEntry->InstanceNumber;
                prot = g_nat_porttrigger[index].TriggerProtocol==0?"tcp":g_nat_porttrigger[index].TriggerProtocol==1?"udp":"both";
                port_triggering_add_rule(g_nat_porttrigger[index].TriggerPortStart,g_nat_porttrigger[index].TriggerPortEnd,prot,g_nat_porttrigger[index].ForwardPortStart,g_nat_porttrigger[index].ForwardPortEnd);
                ulInstanceNumber = pt_count + 1;
        }
        memset(param_name, 0, sizeof(param_name));//PSM Access
        sprintf(param_name,PtNext_instance);
        sprintf(param_value,"%d", ulInstanceNumber);
        PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
        pNat->ulPtNextInstanceNumber = ulInstanceNumber;
        returnStatus = CosaNatRegSetNatInfo(pNat);

        return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatDelPortTrigger
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            );

    Description:
        The API removes the designated port trigger entry from NAT.
    Arguments:
        pAlias      The entry is identified through Alias.
    Return:
        Status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlNatDelPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    )
{
	ULONG                          index = 0;
        ULONG                          count = 0;
        PCOSA_DML_NAT_PTRIGGER         pNatPTrigger;
        ULONG                          ulInstanceNumber=0;
        ulInstanceNumber=pEntry->InstanceNumber;
        char cmd[1024]= {'\0'};
        char *prot;
        extern ANSC_HANDLE bus_handle;//RDKB_EMULATOR PSM Access
        extern char g_Subsystem[32];
        char param_value[50] = {0};
        char *PtNext_instance = "Provision.COSALibrary.NAT.PORTTRIGGER.NextInstanceNumber";
        char param_name[100] ={0};
        PCOSA_DATAMODEL_NAT pNat = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
        ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;
        for(index =0; index < sizeof(g_nat_porttrigger[0])*pt_count/sizeof(COSA_DML_NAT_PTRIGGER); index++)//RDKB_EMULATOR
        {
                if ( g_nat_porttrigger[index].InstanceNumber  ==  pEntry->InstanceNumber )
                {
                        prot = g_nat_porttrigger[index].TriggerProtocol==0?"tcp":g_nat_porttrigger[index].TriggerProtocol==1?"udp":"both";
			//Delete iptable rule
                        port_triggering_delete_rule(g_nat_porttrigger[index].TriggerPortStart,g_nat_porttrigger[index].TriggerPortEnd,prot,g_nat_porttrigger[index].ForwardPortStart,g_nat_porttrigger[index].ForwardPortEnd);
                }
        }
	pNatPTrigger = (PCOSA_DML_NAT_PTRIGGER)AnscAllocateMemory( sizeof(g_nat_porttrigger[0])*(pt_count));//PSM_ACCESS		
        for(index=ulInstanceNumber-1;index<pt_count-1;index++)
        {
                g_nat_porttrigger[index]=g_nat_porttrigger[index+1];
                if(g_nat_porttrigger[index].InstanceNumber!=0){
                        g_nat_porttrigger[index].InstanceNumber--;}
		/*Update PSM PortMapping Data*/
                    *pNatPTrigger = g_nat_porttrigger[index];
		    PSMSetPortTriggerRecordValues(pNatPTrigger,pNatPTrigger->InstanceNumber);
        }
        memset(param_name, 0, sizeof(param_name));//RDKB_EMULATOR PSM Access
        sprintf(param_name,PtNext_instance);
        sprintf(param_value,"%d", pt_count);
        PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
        pNat->ulPtNextInstanceNumber = pt_count;
        pt_count--;

        returnStatus = CosaNatRegSetNatInfo(pNat);

        return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSetPortTrigger
            (
                ANSC_HANDLE                 hThisObject,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            );

    Description:
        The API re-configures the designated port mapping entry.
    Arguments:
        pAlias      The entry is identified through Alias.
        pEntry      The new configuration is passed through this argument, even Alias field can be changed.

    Return:
        Status of the operation

**********************************************************************/

ANSC_STATUS
CosaDmlNatSetPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry          /* Identified by InstanceNumber */
    )
{
	ULONG                          index = 0;
        char cmd[1024]= {'\0'};
        char *prot,*del;
	PCOSA_DML_NAT_PTRIGGER pTrigger;//RDKB_EMULATOR
        pTrigger=(PCOSA_DML_NAT_PTRIGGER)AnscAllocateMemory( sizeof(COSA_DML_NAT_PTRIGGER));

        if (!pEntry)
        {
           return ANSC_STATUS_FAILURE;
        }

        for(index =0; index < sizeof(g_nat_porttrigger[0])*pt_count/sizeof(COSA_DML_NAT_PTRIGGER); index++)//RDKB_EMULATOR
        {
                if ( g_nat_porttrigger[index].InstanceNumber  ==  pEntry->InstanceNumber )
                {
                        del = g_nat_porttrigger[index].TriggerProtocol==0?"tcp":g_nat_porttrigger[index].TriggerProtocol==1?"udp":"both";
                        //Delete iptable table rules 
                        port_triggering_delete_rule(g_nat_porttrigger[index].TriggerPortStart,g_nat_porttrigger[index].TriggerPortEnd,del,g_nat_porttrigger[index].ForwardPortStart,g_nat_porttrigger[index].ForwardPortEnd);
                        if(pEntry != NULL)//RDKB_EMULATOR
                        {
                		pTrigger->InstanceNumber=pEntry->InstanceNumber;
		                pTrigger->bEnabled=pEntry->bEnabled;
                		pTrigger->TriggerProtocol=pEntry->TriggerProtocol;
		                pTrigger->TriggerPortStart=pEntry->TriggerPortStart;
                		pTrigger->TriggerPortEnd=pEntry->TriggerPortEnd;
		                pTrigger->ForwardProtocol=pEntry->ForwardProtocol;
                		pTrigger->ForwardPortStart=pEntry->ForwardPortStart;
		                pTrigger->ForwardPortEnd=pEntry->ForwardPortEnd;
                		strncpy(pTrigger->Description,pEntry->Description, sizeof(pTrigger->Description));
                		PSMSetPortTriggerRecordValues(pTrigger,pTrigger->InstanceNumber);
                		g_nat_porttrigger[index]=*pTrigger;
		                g_nat_porttrigger[index].InstanceNumber=pEntry->InstanceNumber;
                                prot = g_nat_porttrigger[index].TriggerProtocol==0?"tcp":g_nat_porttrigger[index].TriggerProtocol==1?"udp":"both";
                                //Add iptable rule after edit functonality 
                                port_triggering_add_rule(g_nat_porttrigger[index].TriggerPortStart,g_nat_porttrigger[index].TriggerPortEnd,prot,g_nat_porttrigger[index].ForwardPortStart,g_nat_porttrigger[index].ForwardPortEnd);
                        }break;
                }
        }

        return ANSC_STATUS_SUCCESS;

}



static BOOL g_NatPTTriggerEnable = FALSE;

ANSC_STATUS
CosaDmlNatGetPortTriggerEnable(BOOL *pBool)
{
	char *param_value;//RDKB_EMULATOR
	char *pt_enable = "dmsb.nat.X_CISCO_COM_PortTriggers.Enable";
	char param_name[100] ={0};
	memset(param_name, 0, sizeof(param_name));
	sprintf(param_name,pt_enable);
	PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value);
	if(strcmp(param_value,"0") == 0)
	*pBool = FALSE;
	else
	*pBool = TRUE;
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlNatSetPortTriggerEnable(BOOL vBool)
{
	g_NatPTTriggerEnable = vBool;//RDKB_EMULATOR
	if( vBool == FALSE )
	{
		port_triggering_disable();//RDKB_EMULATOR
	}
	char param_value[50] = {0};
        char param_name[100] ={0};
        char *pt_enable = "dmsb.nat.X_CISCO_COM_PortTriggers.Enable";
        memset(param_name, 0, sizeof(param_name));
        sprintf(param_name,pt_enable);
        sprintf(param_value,"%d",g_NatPTTriggerEnable);
        PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);

	return ANSC_STATUS_SUCCESS;
}


int CosaDmlNatChkPortRange(ULONG InstanceNumber, BOOLEAN enabled, int start, int end, int protocol, int is_trigger)
{
    return TRUE;
}

int CosaDmlNatChkTriggerPortRange(ULONG InstanceNumber, BOOLEAN enabled, int start, int end, int protocol, int is_trigger)
{
    return TRUE;
}

ANSC_STATUS
CosaDmlNatGetPortMapping
    (
        ANSC_HANDLE                 hContext,
        ULONG                      InstanceNumber,
        PCOSA_DML_NAT_PMAPPING      pNatPMapping
    )
{
    memset(pNatPMapping, 0, sizeof(*pNatPMapping));
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlNatGetPortTrigger
    (
        ANSC_HANDLE                 hContext,
        ULONG                       InstanceNum,
        PCOSA_DML_NAT_PTRIGGER      pNatPTrigger
    )
{
    memset(pNatPTrigger, 0, sizeof(*pNatPTrigger));
    return ANSC_STATUS_SUCCESS;
}
 

BOOL CosaDmlNatChkPortMappingMaxRuleNum(PCOSA_DML_NAT_PMAPPING pEntry)
{
    return  TRUE;
}

int CosaDmlChkDesp(char *desp)
{
    if(desp == NULL ||
       desp[0] == '\0' || 
       desp[0] == ' ')
        return FALSE;
    return TRUE;
} 
