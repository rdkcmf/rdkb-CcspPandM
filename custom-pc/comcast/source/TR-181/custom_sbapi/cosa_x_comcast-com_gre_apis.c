/************************************************************************************
  If not stated otherwise in this file or this component's Licenses.txt file the
  following copyright and licenses apply:

  Copyright 2018 RDK Management

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
**************************************************************************/

#ifdef CONFIG_CISCO_HOTSPOT
/**************************************************************************

    module: cosa_x_comcast-com_gre_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        zhicheng_qiu@cable.comcast.com

    -------------------------------------------------------------------

    revision:

        05/18/2015    initial revision.

**************************************************************************/

#include "cosa_apis.h"
#include "cosa_x_comcast-com_gre_apis.h"
#include "ccsp_psm_helper.h"
#include "ansc_platform.h"
#include "plugin_main_apis.h"
#include "syscfg.h" 
#include "hotspotfd.h"
#include "dhcpsnooper.h"

#define GRETEST

#if defined(GRETEST)

#ifdef AnscTraceWarning
#undef AnscTraceWarning
#define AnscTraceWarning(a) printf("%s:%d> ", __FUNCTION__, __LINE__); printf a
#endif

#ifdef AnscTraceError
#undef AnscTraceError
#define AnscTraceError(a) printf("%s:%d> ", __FUNCTION__, __LINE__); printf a
#endif

//#ifdef AnscTraceDebug
#undef AnscTraceDebug
#define AnscTraceDebug(a) printf("%s:%d> ", __FUNCTION__, __LINE__); printf a
//#endif

#endif

#define MAX_GRE_PSM_REC         127

#define GRE_DM_BR_TEMP          "Device.Bridging.Bridge.%d."

#define GRE_OBJ_GRETU           "dmsb.hotspot.tunnel."
#define GRETU_PARAM_ENABLE        	GRE_OBJ_GRETU "%lu.Enable"
#define GRETU_PARAM_PRI_ENDPOINT 	GRE_OBJ_GRETU "%lu.PrimaryRemoteEndpoint"
#define GRETU_PARAM_SEC_ENDPOINT 	GRE_OBJ_GRETU "%lu.SecondaryRemoteEndpoint"
#define GRETU_PARAM_KEYGENPOL     	GRE_OBJ_GRETU "%lu.KeyIDGenPolicy"
#define GRETU_PARAM_KEYID         	GRE_OBJ_GRETU "%lu.KeyID"
#define GRETU_PARAM_USESEQ        	GRE_OBJ_GRETU "%lu.UseSeqNum"
#define GRETU_PARAM_USECSUM       	GRE_OBJ_GRETU "%lu.UseCheckSum"
#define GRETU_PARAM_DSCPPOL 	    GRE_OBJ_GRETU "%lu.DSCPMarkPolicy"
#define GRETU_PARAM_KAPOL 	    	GRE_OBJ_GRETU "%lu.KeepAlivePolicy"
#define GRETU_PARAM_KAITVL 	    	GRE_OBJ_GRETU "%lu.RemoteEndpointHealthCheckPingInterval"		
#define GRETU_PARAM_KATHRE	    	GRE_OBJ_GRETU "%lu.RemoteEndpointHealthCheckPingFailThreshold"		
#define GRETU_PARAM_KACNT 	    	GRE_OBJ_GRETU "%lu.RemoteEndpointHealthCheckPingCount"		
#define GRETU_PARAM_KAFAILITVL 		GRE_OBJ_GRETU "%lu.RemoteEndpointHealthCheckPingIntervalInFailure"		
#define GRETU_PARAM_RECONNPRI    	GRE_OBJ_GRETU "%lu.ReconnectToPrimaryRemoteEndpoint"		
#define GRETU_PARAM_DHCPCIRSSID	    GRE_OBJ_GRETU "%lu.EnableCircuitID"		
#define GRETU_PARAM_DHCPRMID 	    GRE_OBJ_GRETU "%lu.EnableRemoteID"		
//#define GRETU_PARAM_GREIF        	GRE_OBJ_GRETU "%lu.GRENetworkInterface"		//GRENetworkInterface: Device.X_CISCO_COM_GRE.Interface.1.
//TODO: remove the reference to Cisco GRE: Device.X_CISCO_COM_GRE.Interface.1.
#define GRETU_PARAM_GRETU        	GRE_OBJ_GRETU "%lu.GRENetworkTunnel"

#define GRE_OBJ_GRETUIF           "dmsb.hotspot.tunnel.%lu.interface."
#define GRETUIF_PARAM_ENABLE        GRE_OBJ_GRETUIF "%lu.Enable"
#define GRETUIF_PARAM_LOCALIFS      GRE_OBJ_GRETUIF "%lu.LocalInterfaces"		//LocalInterfaces: Device.WiFi.SSID.5.,Device.WiFi.SSID.6.
#define GRETUIF_PARAM_VLANID        GRE_OBJ_GRETUIF "%lu.VLANID"					//VLANID: 102
#define GRETUIF_PARAM_ASSOBRS      	GRE_OBJ_GRETUIF "%lu.AssociatedBridges"		//AssociatedBridges: Device.Bridging.Bridge.3.,Device.Bridging.Bridge.4.
#define GRETUIF_PARAM_ASSOBRSWFP   	GRE_OBJ_GRETUIF "%lu.AssociatedBridgesWiFiPort"	//AssociatedBridgesWiFiPort: Device.Bridging.Bridge.3.Port.2.,Device.Bridging.Bridge.4.Port.2.

static int sysevent_fd;
static token_t sysevent_token;
static hotspotfd_statistics_s *g_hsfdStat = NULL;

extern ANSC_HANDLE bus_handle;
static componentStruct_t **        ppComponents = NULL;
extern char        g_Subsystem[32];
extern void* g_pDslhDmlAgent;

static int
GrePsmGet(const char *param, char *value, int size)
{
    char *val;
    CCSP_MESSAGE_BUS_INFO *businfo;

    if (PSM_Get_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, NULL, &val) != CCSP_SUCCESS)
        return -1;

    snprintf(value, size, "%s", val);

    businfo = g_MessageBusHandle;
    businfo->freefunc(val);
    return 0;
}

/*
 *  Procedure     : xfinitywifi_SetDSCPMarkPolicy
 *  Purpose       : DSCP(Differentiated service code point)will decide the IPPackets Priority.This entry should be in Mangle table.
 *  Parameters    : 
 *    dscp        : Having current dscp value
 *  Return Values : None
 *  DSCP Range should be 0 to 63
 */

void xfinitywifi_SetDSCPMarkPolicy(int dscp)//LNT_EMU
{
        char str[512];
        system("iptables -t mangle -D POSTROUTING 1");
        sprintf(str,"%s %d","iptables -t mangle -I POSTROUTING -o eth0 -p gre -j DSCP --set-dscp",dscp);
        system(str);
}


static int
GrePsmSet(const char *param, const char *value)
{
    if (PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, ccsp_string, (char *)value) != CCSP_SUCCESS)
        return -1;
    return 0;
}

static int
GrePsmGetStr(const char *param, int ins, char *value, int size)
{
    char rec[256], val[1024];

    snprintf(rec, sizeof(rec), param, ins);
    if (GrePsmGet(rec, val, sizeof(val)) != 0)
        return -1;

    if (size <= strlen(val))
        return -1;

    snprintf(value, size, "%s", val);
    return 0;
}

static int
GreTunnelIfPsmGetStr(const char *param, int tuIns, int ins, char *value, int size)
{
    char rec[256], val[1024];

    snprintf(rec, sizeof(rec), param, tuIns, ins);
    if (GrePsmGet(rec, val, sizeof(val)) != 0)
        return -1;

    if (size <= strlen(val))
        return -1;

    snprintf(value, size, "%s", val);
    return 0;
}

static int
GrePsmGetUlong(const char *param, int ins, ULONG *value)
{
    char rec[256], val[1024];

    snprintf(rec, sizeof(rec), param, ins);
    if (GrePsmGet(rec, val, sizeof(val)) != 0)
        return -1;

    sscanf(val, "%lu", value);
    return 0;
}

static int
GrePsmGetInt(const char *param, int ins, int *value)
{
    char rec[256], val[1024];

    snprintf(rec, sizeof(rec), param, ins);
    if (GrePsmGet(rec, val, sizeof(val)) != 0)
        return -1;

    *value = atoi(val);
    return 0;
}

static int
GreTunnelIfPsmGetInt(const char *param, int tuIns, int ins, int *value)
{
    char rec[256], val[1024];

    snprintf(rec, sizeof(rec), param, tuIns, ins);
    if (GrePsmGet(rec, val, sizeof(val)) != 0)
        return -1;

    *value = atoi(val);
    return 0;
}

static int
GrePsmGetBool(const char *param, int ins, BOOL *value)
{
    char rec[256], val[1024];

    snprintf(rec, sizeof(rec), param, ins);
    if (GrePsmGet(rec, val, sizeof(val)) != 0)
        return -1;

    *value = (atoi(val) == 1) ? TRUE : FALSE;
    return 0;
}

static int
GreTunnelIfPsmGetBool(const char *param, int tuIns, int ins, BOOL *value)
{
    char rec[256], val[1024];

    snprintf(rec, sizeof(rec), param, tuIns, ins);
    if (GrePsmGet(rec, val, sizeof(val)) != 0)
        return -1;

    *value = (atoi(val) == 1) ? TRUE : FALSE;
    return 0;
}

static char *GetTunnelIfAssoBridge(ULONG tuIns, ULONG ins)
{
    char *assoBrs = NULL;

    if ((assoBrs = malloc(1024)) == NULL)
        return NULL;

    if (GreTunnelIfPsmGetStr(GRETUIF_PARAM_ASSOBRS, tuIns, ins, assoBrs, 1024) != 0)
    {
        AnscTraceError(("Fail to get AssociatedBridges from Psm.\n"));
        free( assoBrs );
        assoBrs = NULL;
    }

    return assoBrs;
}


static char *GetTunnelIfAssoBridgeWiFiPort(ULONG tuIns, ULONG ins)
{
    char *assoBrsWfp = NULL;

    if ((assoBrsWfp = malloc(1024)) == NULL)
        return NULL;

    if (GreTunnelIfPsmGetStr(GRETUIF_PARAM_ASSOBRSWFP, tuIns, ins, assoBrsWfp, 1024) != 0)
    {
        AnscTraceError(("Fail to get AssociatedBridgesWiFiPort from Psm.\n"));
        free( assoBrsWfp );
        assoBrsWfp = NULL;
    }

    return assoBrsWfp;
}


int GreTunnelIf_hotspot_update_circuit_id(ULONG tuIns, int ins, int queuestart) {
    int retval = 0;
    char localinterfaces[200];
    char paramname[60];
    char circuitid[100];
    char outdata[80];
    char* curInt = NULL;
    int circuitSave = 0;
    int size;
    int inst;
    parameterValStruct_t varStruct;
    varStruct.parameterName = paramname;
    varStruct.parameterValue = outdata;
	
    GreTunnelIfPsmGetStr(GRETUIF_PARAM_LOCALIFS, tuIns, ins, localinterfaces, sizeof(localinterfaces));  //LocalInterfaces: Device.WiFi.SSID.5.,Device.WiFi.SSID.5.
    
    curInt = localinterfaces;
    //while (curInt) {
        circuitSave=0;
        //Trim off the trailing dot if it exists
        size = strlen(curInt);
        if (curInt[size -1] == '.')
            curInt[size - 1]='\0';
        
        inst = atoi(strrchr(curInt,'.')+1);
  
		memset(paramname,0,sizeof(paramname));
		memset(outdata,0,sizeof(outdata));
        
        size = sizeof(outdata);

        if (syscfg_get(NULL, "wan_physical_ifname", paramname, sizeof(paramname)) != 0) {
            AnscTraceWarning(("fail to get wan_physical_ifname\n"));
         //   snprintf(paramname, sizeof(paramname), "erouter0");
            snprintf(paramname, sizeof(paramname), "eth0");//LNT_EMU
        }
        if (get_if_hwaddr(paramname, circuitid, sizeof(circuitid)) != 0) {
            AnscTraceWarning(("fail to get HW Addr for %s\n", paramname));
            snprintf(circuitid, sizeof(circuitid), "00:00:00:00:00:00");
        }

        circuitSave = strlen(circuitid);
        circuitSave += snprintf(circuitid + circuitSave, sizeof(circuitid) - circuitSave, ";");
#if 0
        snprintf(paramname, sizeof(paramname), "%s.%s", curInt, "BSSID");
        retval = COSAGetParamValueByPathName(bus_handle, &varStruct, &size);
        if ( retval != ANSC_STATUS_SUCCESS)
            return -1;
        circuitSave = snprintf(circuitid, sizeof(circuitid), "%s;", varStruct.parameterValue);
#endif
        memset(paramname,0,sizeof(paramname));
		memset(outdata,0,sizeof(outdata));

        size = sizeof(outdata);
        snprintf(paramname, sizeof(paramname),"%s.%s", curInt, "SSID");
        retval = COSAGetParamValueByPathName(bus_handle, &varStruct, &size);
        if ( retval != ANSC_STATUS_SUCCESS)
            return -1;

		if(!(strcmp(varStruct.parameterValue,""))){
			  snprintf(paramname, sizeof(paramname), "eRT.com.cisco.spvtg.ccsp.Device.WiFi.Radio.SSID.%d.SSID",inst);
		  GrePsmGet(paramname,varStruct.parameterValue, size);
		  if(strlen(varStruct.parameterValue)==0) {
				strcpy(varStruct.parameterValue,"xfinitywifi");
		  }
		}
        
        circuitSave += snprintf(circuitid + circuitSave, sizeof(circuitid) - circuitSave, "%s;", varStruct.parameterValue);

	    memset(paramname,0,sizeof(paramname));
	    memset(outdata,0,sizeof(outdata));
        
        size = sizeof(outdata);
        snprintf(paramname, sizeof(paramname), "Device.WiFi.AccessPoint.%d.Security.ModeEnabled", inst);
        retval = COSAGetParamValueByPathName(bus_handle, &varStruct, &size);
        if ( retval != ANSC_STATUS_SUCCESS)
            return -1;
        if(strcmp("None", varStruct.parameterValue)) {
            snprintf(circuitid + circuitSave, sizeof(circuitid) - circuitSave, "s");
        } else {
            snprintf(circuitid + circuitSave, sizeof(circuitid) - circuitSave, "o");
        }
        
        snprintf(paramname, sizeof(paramname), "snooper-queue%d-circuitID", queuestart);
        
        sysevent_set(sysevent_fd, sysevent_token, paramname, circuitid, 0);
        
        snprintf(paramname, sizeof(paramname), "snooper-ssid%d-index", queuestart++);
        snprintf(outdata, sizeof(outdata), "%d", inst);
        
        sysevent_set(sysevent_fd, sysevent_token, paramname, outdata, 0);
        
        //sysevent set snoopereventforcircuitid_queuestart++ circuitid
        
        //curInt = strtok_r(NULL, ",", &save);
    //}
    
    return queuestart;
}

int GreTunnel_hotspot_update_circuit_ids(ULONG tuIns, int queuestart) {
	int ins=0;
	int ques=queuestart;
	//TODO: get InterfaceNumberOfEntries
	int InterfaceNumberOfEntries=CosaDml_GreTunnelIfGetNumberOfEntries(tuIns);
	for (ins =1; ins<=InterfaceNumberOfEntries; ins++) {
		ques= GreTunnelIf_hotspot_update_circuit_id(tuIns, ins, ques);
	}
	return ques;
}

#define INITIAL_CIRCUIT_ID_SLEEP 5
#define POLL_CIRCUIT_ID_SLEEP 3
#define INITIAL_SNOOPER_QUEUE 1


static void* GreTunnel_circuit_id_init_thread(void* arg) {
    int ret = -1;
    sleep(INITIAL_CIRCUIT_ID_SLEEP);

    ret = GreTunnel_hotspot_update_circuit_ids(1, INITIAL_SNOOPER_QUEUE);
    
    
    while (ret < 0) {
        sleep(POLL_CIRCUIT_ID_SLEEP);
        ret = GreTunnel_hotspot_update_circuit_ids(1, INITIAL_SNOOPER_QUEUE);
    }
    
    return NULL;
}

ANSC_STATUS 
CosaDml_GreTunnelInit(void)
{
    int shmid;
    void *params = NULL;
 
    sysevent_fd = sysevent_open(
            "127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, 
            SE_VERSION, kHotspotfd_events, 
            &sysevent_token
    );

    if (sysevent_fd < 0)
    {
        AnscTraceError(("sysevent_open failed\n"));
    } else {
        AnscTraceWarning(("sysevent_open success\n"));
    }

    if ((shmid = shmget(kKeepAlive_Statistics, kKeepAlive_SharedMemSize, IPC_CREAT | 0666)) < 0) {
        AnscTraceError(("shmget: %s\n", strerror(errno)));
        return ANSC_STATUS_FAILURE;
    }

    if ((g_hsfdStat = shmat(shmid, NULL, 0)) == (hotspotfd_statistics_s *)-1) {
        AnscTraceError(("shmat: %s\n", strerror(errno)));
        g_hsfdStat = NULL;
        return ANSC_STATUS_FAILURE;
    }
    
    AnscCreateTask(GreTunnel_circuit_id_init_thread, USER_DEFAULT_TASK_STACK_SIZE, USER_DEFAULT_TASK_PRIORITY, params, "CircuitIDInitThread");

    AnscTraceDebug(("Init Hotspot GRE Done\n"));

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS 
CosaDml_GreTunnelFinalize(void)
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDml_GreTunnelIfFinalize(void)
{
    return ANSC_STATUS_SUCCESS;
}


ULONG 
CosaDml_GreTunnelGetNumberOfEntries(void)
{
    /* only support one GRE Tunnel for Comcast Hotspot */
    return 1;
}

ULONG 
CosaDml_GreTunnelIfGetNumberOfEntries(ULONG tuIns)
{
    /* support 2 GRE interface for Comcast Hotspot */
	if(tuIns==1)
		return 2;
	return 0;
}

#define kHotspotfd_tunnelEP                 "hotspotfd-tunnelEP" 


ANSC_STATUS
CosaDml_GreTunnelGetConnectedRemoteEndpoint(ULONG tuIdx, COSA_DML_GRE_TUNNEL *greTu)
{
	char cmd[126] = {0};
	char line_buf[126] = {0};
	FILE *fp = NULL;

	if(!greTu)
			return ANSC_STATUS_FAILURE;

	snprintf(cmd, sizeof(cmd), "sysevent get %s",kHotspotfd_tunnelEP);       
	
    if (((fp = popen(cmd, "r")) != NULL) && (fgets(line_buf, sizeof(line_buf), fp)))
    {
        sprintf(greTu->ConnectedRemoteEndpoint,"%s",line_buf);
    }
	if(fp)
		pclose(fp);

	return ANSC_STATUS_SUCCESS;     
}


ANSC_STATUS
CosaDml_GreTunnelGetEntryByIndex(ULONG ins, COSA_DML_GRE_TUNNEL *greTu)
{
    if (!greTu)
        return ANSC_STATUS_FAILURE;

    memset(greTu, 0, sizeof(COSA_DML_GRE_TUNNEL));

    greTu->InstanceNumber = ins;
	greTu->Enable = TRUE;
    //if (GrePsmGetBool(GRETU_PARAM_ENABLE, ins, &greTu->Enable) != 0)
    //    return ANSC_STATUS_FAILURE;
	//if (GrePsmGetStr(GRETU_PARAM_PRI_ENDPOINT, ins, greTu->PrimaryRemoteEndpoint, sizeof(greTu->PrimaryRemoteEndpoint)) != 0)
    //    return ANSC_STATUS_FAILURE;
	//if (GrePsmGetStr(GRETU_PARAM_SEC_ENDPOINT, ins, greTu->SecondaryRemoteEndpoint, sizeof(greTu->SecondaryRemoteEndpoint)) != 0)
    //    return ANSC_STATUS_FAILURE;
	if (CosaDml_GreTunnelGetEnable(ins, &greTu->Enable) == ANSC_STATUS_FAILURE)
		return ANSC_STATUS_FAILURE;
	if (CosaDml_GreTunnelGetPrimaryEndpoint(ins, greTu->PrimaryRemoteEndpoint, sizeof(greTu->PrimaryRemoteEndpoint)) == ANSC_STATUS_FAILURE)
		return ANSC_STATUS_FAILURE;
	if (CosaDml_GreTunnelGetSecondaryEndpoint(ins, greTu->SecondaryRemoteEndpoint, sizeof(greTu->SecondaryRemoteEndpoint)) == ANSC_STATUS_FAILURE)
		return ANSC_STATUS_FAILURE;	
    if (GrePsmGetInt(GRETU_PARAM_KEYGENPOL, ins, (int *)&greTu->KeyIdentifierGenerationPolicy) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetStr(GRETU_PARAM_KEYID, ins, greTu->KeyIdentifier, sizeof(greTu->KeyIdentifier)) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetBool(GRETU_PARAM_USESEQ, ins, &greTu->UseSequenceNumber) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetBool(GRETU_PARAM_USECSUM, ins, &greTu->UseChecksum) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetInt(GRETU_PARAM_DSCPPOL, ins, &greTu->DSCPMarkPolicy) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetInt(GRETU_PARAM_KAPOL, ins, (int *)&greTu->KeepAlivePolicy) != 0)
        return ANSC_STATUS_FAILURE;
	if (GrePsmGetUlong(GRETU_PARAM_KAITVL, ins, &greTu->RemoteEndpointHealthCheckPingInterval) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetUlong(GRETU_PARAM_KATHRE, ins, &greTu->RemoteEndpointHealthCheckPingFailThreshold) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetUlong(GRETU_PARAM_KACNT, ins, &greTu->RemoteEndpointHealthCheckPingCount) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetUlong(GRETU_PARAM_KAFAILITVL, ins, &greTu->RemoteEndpointHealthCheckPingIntervalInFailure) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetUlong(GRETU_PARAM_RECONNPRI, ins, &greTu->ReconnectToPrimaryRemoteEndpoint) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetBool(GRETU_PARAM_DHCPCIRSSID, ins, &greTu->EnableCircuitID) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetBool(GRETU_PARAM_DHCPRMID, ins, &greTu->EnableRemoteID) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetStr(GRETU_PARAM_GRETU, ins, greTu->GRENetworkTunnel, sizeof(greTu->GRENetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelIfGetEntryByIndex(ULONG tuIns, ULONG ins, COSA_DML_GRE_TUNNEL_IF *greTuIf)
{
    if (!greTuIf)
        return ANSC_STATUS_FAILURE;

    memset(greTuIf, 0, sizeof(COSA_DML_GRE_TUNNEL_IF));

    greTuIf->InstanceNumber = ins;

    if (GreTunnelIfPsmGetBool(GRETUIF_PARAM_ENABLE, tuIns, ins, &greTuIf->Enable) != 0)
        return ANSC_STATUS_FAILURE;
    if (GreTunnelIfPsmGetStr(GRETUIF_PARAM_LOCALIFS, tuIns, ins, greTuIf->LocalInterfaces, sizeof(greTuIf->LocalInterfaces)) != 0)
        return ANSC_STATUS_FAILURE;
    if (GreTunnelIfPsmGetInt(GRETUIF_PARAM_VLANID, tuIns, ins, &greTuIf->VLANID) != 0)
        return ANSC_STATUS_FAILURE;
    if (GreTunnelIfPsmGetStr(GRETUIF_PARAM_ASSOBRS, tuIns, ins, greTuIf->AssociatedBridges, sizeof(greTuIf->AssociatedBridges)) != 0)
        return ANSC_STATUS_FAILURE;
    if (GreTunnelIfPsmGetStr(GRETUIF_PARAM_ASSOBRSWFP, tuIns, ins, greTuIf->AssociatedBridgesWiFiPort, sizeof(greTuIf->AssociatedBridgesWiFiPort)) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetIns(ULONG idx, ULONG ins)
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelIfSetIns(ULONG tuIdx, ULONG idx, ULONG ins)
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetEnable(ULONG tuIns, BOOL *enable)
{
    if (!enable)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetBool(GRETU_PARAM_ENABLE, tuIns, enable) != 0)
        return ANSC_STATUS_FAILURE;

	//zqiu: try to read enable from the old config
	BOOL bEnable=FALSE; 
	if( tuIns==1 && *enable==FALSE) {	
		if( GrePsmGetBool("dmsb.hotspot.gre.%d.Enable", 1, &bEnable) == 0 &&  bEnable==TRUE) {			
			CosaDml_GreTunnelSetEnable(1, TRUE);
			//CosaDml_GreTunnelIfSetEnable(1, 1, TRUE);
			//CosaDml_GreTunnelIfSetEnable(1, 2, TRUE);
			//remove the old record
			GrePsmSet("dmsb.hotspot.gre.1.Enable", "0");			
		}
    }	
		
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelIfGetEnable(ULONG tuIns, ULONG ins, BOOL *enable)
{
    if (!enable)
        return ANSC_STATUS_FAILURE;

    if (GreTunnelIfPsmGetBool(GRETUIF_PARAM_ENABLE, tuIns, ins, enable) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDml_GreTunnelSetEnable(ULONG tuIns, BOOL enable)
{
	char psmRec[MAX_GRE_PSM_REC + 1];
    char greNetworkTunnel[256];
    char tmpPath[256];
	if (tuIns != 1)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sEnable", greNetworkTunnel);	//Device.X_CISCO_COM_GRE.Interface.1.Enable
    if (g_SetParamValueBool(tmpPath, enable) != ANSC_STATUS_SUCCESS) {
		fprintf(stderr, "Set %s   fail\n", tmpPath);		
        //return ANSC_STATUS_FAILURE;
	}	
	//if (g_SetParamValueBool("Device.X_COMCAST-COM_GRE.Tunnel.1.Enable", enable) != ANSC_STATUS_SUCCESS) {
	//	fprintf(stderr, "-- %s %d  fail\n", __func__, __LINE__);	
    //}
    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_ENABLE, tuIns);
    if (GrePsmSet(psmRec, enable ? "1" : "0") != 0)
        return ANSC_STATUS_FAILURE;
	BOOL bEnable=FALSE;
	if( tuIns==1 && enable==FALSE) {
		//zqiu: try to erase the old endpoint
		if( GrePsmGetBool("dmsb.hotspot.gre.%d.Enable", 1, &bEnable) == 0 &&  bEnable!=FALSE) {
			GrePsmSet("dmsb.hotspot.gre.1.Enable", "0");	
		}
    }	
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelIfSetEnable(ULONG tuIns, ULONG ins, BOOL enable)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char greNetworkTunnel[256];
    char tmpPath[256];

	if (tuIns != 1)
        return ANSC_STATUS_FAILURE;
		
    if (ins != 1)
        return ANSC_STATUS_FAILURE;

    if (GreTunnelIfPsmGetStr(GRETU_PARAM_GRETU, tuIns, ins, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
	//greNetworkTunnel:	Device.X_CISCO_COM_GRE.Tunnel.1.
    snprintf(tmpPath, sizeof(tmpPath), "%sEnable", greNetworkTunnel);
    if (g_SetParamValueBool(tmpPath, enable) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETUIF_PARAM_ENABLE, tuIns, ins);
    if (GrePsmSet(psmRec, enable ? "1" : "0") != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;

}
			
ANSC_STATUS
CosaDml_GreTunnelGetStatus(ULONG tuIns, COSA_DML_GRE_STATUS *st)
{
    char status[64];
    ULONG size = sizeof(status);
    char greNetworkTunnel[256];
    char tmpPath[256];

    //REFPLTB-287 :: Default value to be down
    *st = COSA_DML_GRE_STATUS_DOWN;

    if (!st)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sStatus", greNetworkTunnel);
    if (g_GetParamValueString(g_pDslhDmlAgent, tmpPath, status, &size) != 0)
        return ANSC_STATUS_FAILURE;
    if (strcmp(status, "Up") == 0)
        *st = COSA_DML_GRE_STATUS_UP;
    else if (strcmp(status, "Down") == 0 || strcmp(status, "LowerLayerDown") == 0)
        *st = COSA_DML_GRE_STATUS_DOWN;
    else if (strcmp(status, "Error") == 0)
        *st = COSA_DML_GRE_STATUS_ERROR;
    else
        return ANSC_STATUS_FAILURE;
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelIfGetStatus(ULONG tuIns, ULONG ins, COSA_DML_GRE_STATUS *st)
{
	char status[64];
    ULONG size = sizeof(status);
    char greNetworkTunnel[256];
    char tmpPath[256];
    if (!st)
        return ANSC_STATUS_FAILURE;

	//TODO: need IF status instead of Tunnel status
    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sStatus", greNetworkTunnel);

    if (g_GetParamValueString(g_pDslhDmlAgent, tmpPath, status, &size) != 0)
        return ANSC_STATUS_FAILURE;

    if (strcmp(status, "Up") == 0)
        *st = COSA_DML_GRE_STATUS_UP;
    else if (strcmp(status, "Down") == 0 || strcmp(status, "LowerLayerDown") == 0)
        *st = COSA_DML_GRE_STATUS_DOWN;
    else if (strcmp(status, "Error") == 0)
        *st = COSA_DML_GRE_STATUS_ERROR;
    else
        return ANSC_STATUS_FAILURE;
	return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDml_GreTunnelGetLastchange(ULONG tuIns, ULONG *time)
{
	char greNetworkTunnel[256];
    char tmpPath[256];
    if (!time)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sLastChange", greNetworkTunnel);

    *time = g_GetParamValueUlong(g_pDslhDmlAgent, tmpPath);
    return ANSC_STATUS_SUCCESS;


}


ANSC_STATUS
CosaDml_GreTunnelIfGetLastchange(ULONG tuIns, ULONG ins, ULONG *time)
{
	char greNetworkTunnel[256];
    char tmpPath[256];

    if (!time)
        return ANSC_STATUS_FAILURE;

	//TODO: need to get IF change time instead of Tunnel change time
    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sLastChange", greNetworkTunnel);

    *time = g_GetParamValueUlong(g_pDslhDmlAgent, tmpPath);

    return ANSC_STATUS_SUCCESS;


}

ANSC_STATUS
CosaDml_GreTunnelIfGetLocalInterfaces(ULONG tuIns, ULONG ins, char *ifs, ULONG size)
{
	char *brlist, *br, *delim, *start, *sp;
    ULONG ptInsList[16], ptInsCnt = 16;
    char dm[1024], dmval[1024 + 1];
    ULONG dmsize;
    int i;

    if (!ifs)
        return ANSC_STATUS_FAILURE;

    memset(ifs, 0, size);

    if ((brlist = GetTunnelIfAssoBridge(tuIns, ins)) == NULL)
        return ANSC_STATUS_FAILURE;

    /* for each bridge */
    for (start = brlist, delim = ",";
            (br = strtok_r(start, delim, &sp)) != NULL;
            start = NULL) {

        AnscTraceDebug(("Bridge: %s\n", br));

        /* for each port */
        snprintf(dm, sizeof(dm), "%sPort.", br);
        if (g_GetInstanceNumbers(dm, ptInsList, &ptInsCnt) != ANSC_STATUS_SUCCESS) {
            AnscTraceError(("Fail to get port tuIns numbers\n"));
            continue;
        }

        AnscTraceDebug(("  Port Num: %lu\n", ptInsCnt));

        for (i = 0; i < ptInsCnt; i++) {
            /* skip management port */
            snprintf(dm, sizeof(dm), "%sPort.%d.ManagementPort", br, ptInsList[i]);
            if (g_GetParamValueBool(g_pDslhDmlAgent, dm)) {
                AnscTraceDebug(("  Skip Port[%d], it's a management port\n", ptInsList[i]));
                continue;
            }

            /* skip upstream port (GRE IF) */
            dmsize = sizeof(dmval) - 1;
            snprintf(dm, sizeof(dm), "%sPort.%d.LowerLayers", br, ptInsList[i]);
            if (g_GetParamValueString(g_pDslhDmlAgent, dm, dmval, &dmsize) != 0 
                    || strstr(dmval, "Device.WiFi.SSID") == NULL) {
                AnscTraceDebug(("  Skip Port[%d]: %s\n", ptInsList[i], dmval));
                continue;
            }

            /* XXX: MultiLAN DM do not use "." for object path */
            if (strlen(dmval) && dmval[strlen(dmval) - 1] != '.' && strlen(dmval) < sizeof(dmval) - 1) {
                //AnscTraceDebug(("  Adding '.' to local if path\n"));
                strcat(dmval, ".");
            }

            /* add it to list */
            AnscTraceDebug(("  Add Port[%d] `%s/%s` to Local IF\n", ptInsList[i], dm, dmval));
            //if (strlen(ifs)) {
            if (!start || strlen(ifs)) { /* Local If and Bridge are 1:1 mapping, we need "," */
                snprintf(ifs +  strlen(ifs), size - strlen(ifs), ",%s", dmval);
            } else {
                snprintf(ifs +  strlen(ifs), size - strlen(ifs), "%s", dmval);
            }
        }
    }

    free(brlist);

    AnscTraceDebug(("  Local IFs: %s\n", ifs));

    //if (GrePsmGetStr(GRE_PARAM_LOCALIFS, ins, ifs, size) != 0)
    //    return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelIfSetLocalInterfaces(ULONG tuIns, ULONG ins, const char *ifs)
{
	char psmRec[MAX_GRE_PSM_REC + 1], dm[1024];
    char *cp, *if1, *if2, *br1, *br2, *brwfp1, *brwfp2;
    char *ifsBuf, *brsBuf, *brswfpBuf;
    int brIns;
	char brInsStr[4];

    if (!ifs)
        return ANSC_STATUS_FAILURE;

    if1 = if2 = br1 = br2 = brwfp1 = brwfp2 = NULL;
    ifsBuf = brsBuf = brswfpBuf = NULL;

    /*
     * when we got more then one local interfaces, we have to "models":
     * 1. each VLAN(Bridge) per SSID, 
     *    how can we determine which VLAN the IF belongs to ?
     *    a) let's use 1:1 mapping for IF and VLAN.
     *       first IF for first VLAN (Bridge)
     *    b) assume that port1 is for management port, port2 for upstream, port3 for local IF.
     *       that' ok since in this mode, each bridge has only one local IF.
     * 2. single VLAN for all SSIDs
     *    XXX: we do not support this model now
     *         to support this mode , we have to add/del port.
     */
    if ((ifsBuf = strdup(ifs)) == NULL)
        return ANSC_STATUS_FAILURE;
    if ((brsBuf = GetTunnelIfAssoBridge(tuIns, ins)) == NULL) {
        free(ifsBuf);
        return ANSC_STATUS_FAILURE;
    }
    if ((brswfpBuf = GetTunnelIfAssoBridgeWiFiPort(tuIns, ins)) == NULL) {
        free(ifsBuf);
        free(brsBuf);
        return ANSC_STATUS_FAILURE;
    }

    if1 = ifsBuf;
    if ((cp = strchr(ifsBuf, ',')) != NULL) {
        *cp++ = '\0';
        if2 = cp;
        if ((cp = strchr(cp, ',')) != NULL)
            *cp++ = '\0';
    }

    br1 = brsBuf;
    if ((cp = strchr(brsBuf, ',')) != NULL) {
        *cp++ = '\0';
        br2 = cp;
        if ((cp = strchr(cp, ',')) != NULL)
            *cp++ = '\0';
    }

    brwfp1 = brswfpBuf;
    if ((cp = strchr(brswfpBuf, ',')) != NULL) {
        *cp++ = '\0';
        brwfp2 = cp;
        if ((cp = strchr(cp, ',')) != NULL)
            *cp++ = '\0';
    }

    AnscTraceDebug(("if1 %s, br1 %s brwfp1 %s\n", if1 ? if1 : "n/a", br1 ? br1 : "n/a", brwfp1 ? brwfp1 : "n/a"));
    AnscTraceDebug(("if2 %s, br2 %s brwfp2 %s\n", if2 ? if2 : "n/a", br2 ? br2 : "n/a", brwfp2 ? brwfp2 : "n/a"));

    /* XXX: MultiLAN DM do not use "." for object path */
    if (if1 && strlen(if1) && if1[strlen(if1) - 1] == '.')
        if1[strlen(if1) - 1] = '\0';
    if (if2 && strlen(if2) && if2[strlen(if2) - 1] == '.')
        if2[strlen(if2) - 1] = '\0';

    if (br1 && strlen(br1)) {
        if (!if1 || !strlen(if1))
            if1 = "";
        snprintf(dm, sizeof(dm), "%sLowerLayers", brwfp1);
        AnscTraceDebug(("set %s to %s\n", dm, if1));
        if (g_SetParamValueString(dm, if1) != ANSC_STATUS_SUCCESS)
            AnscTraceError(("Fail to set %s to %s\n", dm, if1));

        /* inform the scripts about the changing */
        sscanf(br1, GRE_DM_BR_TEMP, &brIns);
        snprintf(brInsStr, sizeof(brInsStr), "%d", brIns);
        if (sysevent_set(sysevent_fd, sysevent_token, "hotspot-update_bridges", brInsStr, 0) != 0)
            AnscTraceError(("Fail to set sysevent: %s to %s\n", "hotspot_bridge-update", brInsStr));
    }

    if (br2 && strlen(br2)) {
        if (!if2 || !strlen(if2))
            if2 = "";
        snprintf(dm, sizeof(dm), "%sLowerLayers", brwfp2);
        AnscTraceDebug(("set %s to %s\n", dm, if2));
        if (g_SetParamValueString(dm, if2) != ANSC_STATUS_SUCCESS)
            AnscTraceError(("Fail to set %s to %s\n", dm, if2));

        /* inform the scripts about the changing */
        sscanf(br2, GRE_DM_BR_TEMP, &brIns);
        snprintf(brInsStr, sizeof(brInsStr), "%d", brIns);
        if (sysevent_set(sysevent_fd, sysevent_token, "hotspot-update_bridges", brInsStr, 0) != 0)
            AnscTraceError(("Fail to set sysevent: %s to %s\n", "hotspot_bridge-update", brInsStr));
    }

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETUIF_PARAM_LOCALIFS, tuIns, ins);
    if (GrePsmSet(psmRec, ifs) != 0) {
        free(ifsBuf);
        free(brsBuf);
        free(brswfpBuf);
        return ANSC_STATUS_FAILURE;
    }

    free(ifsBuf);
    free(brsBuf);
    free(brswfpBuf);
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDml_GreTunnelGetPrimaryEndpoint(ULONG tuIns, char *eps, ULONG size)
{
	if (!eps)
        return ANSC_STATUS_FAILURE;

    memset(eps, 0, size);
	if (GrePsmGetStr(GRETU_PARAM_PRI_ENDPOINT, tuIns, eps, size) != 0)
        return ANSC_STATUS_FAILURE;
		
	//zqiu: try to read endpoint from the old config
	char endpoints[64]="", endpoints_sv[64]="0.0.0.0", *pt=NULL;
	if( tuIns==1 && strcmp("0.0.0.0", eps)==0) {
		if( GrePsmGetStr("dmsb.hotspot.gre.%d.Endpoints", 1, endpoints, 64) == 0 &&  strcmp("0.0.0.0,0.0.0.0", endpoints)!=0) {
			pt=strchr(endpoints, ',');
			if(pt && strncmp("0.0.0.0", endpoints, pt-endpoints) !=0) {
				strncpy(eps, endpoints, pt-endpoints);
				eps[pt-endpoints]=0;
				CosaDml_GreTunnelSetPrimaryEndpoints(1, eps);
				//remove the old record
				strncat(endpoints_sv, pt, 50); //0.0.0.0,xx.xx.xx.xx
				GrePsmSet("dmsb.hotspot.gre.1.Endpoints", endpoints_sv);				
			}
		}
    }

	return ANSC_STATUS_SUCCESS;
}
//By default SecondaryRemoteEndPoint is 68.85.6.243
ANSC_STATUS
CosaDml_GreTunnelGetSecondaryEndpoint(ULONG tuIns, char *eps, ULONG size)
{
	if (!eps)
        return ANSC_STATUS_FAILURE;

    memset(eps, 0, size);
	if (GrePsmGetStr(GRETU_PARAM_SEC_ENDPOINT, tuIns, eps, size) != 0)
        return ANSC_STATUS_FAILURE;
		
	//zqiu: try to read endpoint from the old config
	char endpoints[64]="", *pt=NULL;
	if( tuIns==1 && strcmp("0.0.0.0", eps)==0) {
		if( GrePsmGetStr("dmsb.hotspot.gre.%d.Endpoints", 1, endpoints, 64) == 0 &&  strcmp("0.0.0.0,0.0.0.0", endpoints)!=0) {
			pt=strchr(endpoints, ',');
			if(pt && strcmp("0.0.0.0", pt+1) !=0) {
				strncpy(eps, pt+1, size);
				CosaDml_GreTunnelSetSecondaryEndpoints(1, eps);
				//remove the old record
				snprintf(pt+1, 10, "0.0.0.0"); //xx.xx.xx.xx,0.0.0.0
				GrePsmSet("dmsb.hotspot.gre.1.Endpoints", endpoints);	
			}
		}
    }	
	
	return ANSC_STATUS_SUCCESS;	
}
//By default PrimaryRemoteEndPoint is 68.85.15.238
ANSC_STATUS
CosaDml_GreTunnelSetPrimaryEndpoints(ULONG tuIns, const char *pri)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    
    if (pri && strlen(pri)) {
        if (sysevent_set(sysevent_fd, sysevent_token, kHotspotfd_primary, pri, 0) != 0) {
            AnscTraceError(("Fail to set sysevent: %s to %s\n", kHotspotfd_primary, pri));
            return ANSC_STATUS_FAILURE;
        }
    }

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_PRI_ENDPOINT, tuIns);
    if (GrePsmSet(psmRec, pri) != 0)
        return ANSC_STATUS_FAILURE;
	
	char endpoints[64]="", endpoints_sv[64]="0.0.0.0", *pt=NULL;
	if( tuIns==1 && strcmp("0.0.0.0", pri)==0) {
		//zqiu: try to erase the old endpoint
		if( GrePsmGetStr("dmsb.hotspot.gre.%d.Endpoints", 1, endpoints, 64) == 0 &&  strcmp("0.0.0.0,0.0.0.0", endpoints)!=0) {
			pt=strchr(endpoints, ',');
			if(pt && strncmp("0.0.0.0", endpoints, pt-endpoints) !=0) {				
				strncat(endpoints_sv, pt, 50); //0.0.0.0,xx.xx.xx.xx
				GrePsmSet("dmsb.hotspot.gre.1.Endpoints", endpoints_sv);				
			}
		}
    }	
	
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetSecondaryEndpoints(ULONG tuIns, const char *sec)
{
	char psmRec[MAX_GRE_PSM_REC + 1];

	if (sec && strlen(sec)) {
        if (sysevent_set(sysevent_fd, sysevent_token, khotspotfd_secondary, sec, 0) != 0) {
            AnscTraceError(("Fail to set sysevent: %s to %s\n", khotspotfd_secondary, sec));
            return ANSC_STATUS_FAILURE;
        }
    }
	
	/* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_SEC_ENDPOINT, tuIns);
    if (GrePsmSet(psmRec, sec) != 0)
        return ANSC_STATUS_FAILURE;
		
	
	char endpoints[64]="", *pt=NULL;
	if( tuIns==1 && strcmp("0.0.0.0", sec)==0) {
		//zqiu: try to erase sec endpoint in old config
		if( GrePsmGetStr("dmsb.hotspot.gre.%d.Endpoints", 1, endpoints, 64) == 0 &&  strcmp("0.0.0.0,0.0.0.0", endpoints)!=0) {
			pt=strchr(endpoints, ',');
			if(pt && strcmp("0.0.0.0", pt+1) !=0) {				
				snprintf(pt+1, 10, "0.0.0.0"); //xx.xx.xx.xx,0.0.0.0
				GrePsmSet("dmsb.hotspot.gre.1.Endpoints", endpoints);	
			}
		}
    }		

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetConnEndpoint(ULONG tuIns, char *ep, ULONG size)
{
	char greNetworkTunnel[256];
    char tmpPath[256];

    if (!ep)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sRemoteEndpoint", greNetworkTunnel);

    if (g_GetParamValueString(g_pDslhDmlAgent, tmpPath, ep, &size) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDml_GreTunnelGetKeyGenPolicy(ULONG tuIns, COSA_DML_KEY_ID_GEN_POLICY *policy)
{
    if (!policy)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetInt(GRETU_PARAM_KEYGENPOL, tuIns, (int *)policy) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetKeyGenPolicy(ULONG tuIns, COSA_DML_KEY_ID_GEN_POLICY policy)
{
    char psmRec[MAX_GRE_PSM_REC + 1], psmVal[64];
    char *mode;
    char greNetworkTunnel[256];
    char tmpPath[256];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    switch (policy) {
    case COSA_DML_KEY_ID_GEN_POLICY_DISABLED:
        mode = "Disabled";
        break;
    case COSA_DML_KEY_ID_GEN_POLICY_PROVISIONED:
        mode = "Manual";
        break;
    case COSA_DML_KEY_ID_GEN_POLICY_AUTO:
        mode = "Auto";
        break;
    default:
        return ANSC_STATUS_FAILURE;
    }

    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sKeyMode", greNetworkTunnel);

    if (g_SetParamValueString(tmpPath, mode) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_KEYGENPOL, tuIns);
    snprintf(psmVal, sizeof(psmVal), "%d", policy);

    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetKeyId(ULONG tuIns, char *keyId, ULONG size)
{
    if (!keyId)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRETU_PARAM_KEYID, tuIns, keyId, size) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetKeyId(ULONG tuIns, const char *keyId)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char greNetworkTunnel[256];
    char tmpPath[256];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sKey", greNetworkTunnel);

    if (g_SetParamValueString(tmpPath, (char *)keyId) != 0)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_KEYID, tuIns);
    if (GrePsmSet(psmRec, keyId) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetUseSeqNum(ULONG tuIns, BOOL *enable)
{
    if (!enable)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetBool(GRETU_PARAM_USESEQ, tuIns, enable) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetUseSeqNum(ULONG tuIns, BOOL enable)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char greNetworkTunnel[256];
    char tmpPath[256];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sSequenceNumberEnabled", greNetworkTunnel);

    if (g_SetParamValueBool(tmpPath, enable) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_USESEQ, tuIns);
    if (GrePsmSet(psmRec, enable ? "1" : "0") != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetUseChecksum(ULONG tuIns, BOOL *enable)
{
    if (!enable)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetBool(GRETU_PARAM_USECSUM, tuIns, enable) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetUseChecksum(ULONG tuIns, BOOL enable)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char greNetworkTunnel[256];
    char tmpPath[256];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sChecksumEnabled", greNetworkTunnel);

    if (g_SetParamValueBool(tmpPath, enable) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_USECSUM, tuIns);
    if (GrePsmSet(psmRec, enable ? "1" : "0") != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetDSCPMarkPolicy(ULONG tuIns, INT *dscp)
{
    if (!dscp)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetInt(GRETU_PARAM_DSCPPOL, tuIns, dscp) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDml_GreTunnelSetDSCPMarkPolicy(ULONG tuIns, INT dscp)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];
    char greNetworkTunnel[256];
    char tmpPath[256];
    char tmp2Path[256];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

     xfinitywifi_SetDSCPMarkPolicy(dscp);//LNT_EMU
    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_DSCPPOL, tuIns);
    snprintf(psmVal, sizeof(psmVal), "%d", dscp);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sTOSMode", greNetworkTunnel);
    snprintf(tmp2Path, sizeof(tmp2Path), "%sTOS", greNetworkTunnel);

    if (dscp >= 0) {
        if (g_SetParamValueString(tmpPath, "Static") != ANSC_STATUS_SUCCESS
                || g_SetParamValueUlong(tmp2Path, dscp << 2) != ANSC_STATUS_SUCCESS) {
            AnscTraceError(("Fail to set Cisco GRE DM\n"));
            return ANSC_STATUS_FAILURE;
        }
    } else if (dscp == -1) {
        if (g_SetParamValueString(tmpPath, "Inherited") != ANSC_STATUS_SUCCESS) {
            AnscTraceError(("Fail to set Cisco GRE DM\n"));
            return ANSC_STATUS_FAILURE;
        }
    } else if (dscp == -2) {
        if (g_SetParamValueString(tmpPath, "AutoMapped") != ANSC_STATUS_SUCCESS) {
            AnscTraceError(("Fail to set Cisco GRE DM\n"));
            return ANSC_STATUS_FAILURE;
        }
    } else {
        AnscTraceError(("Bad DSCP value\n"));
        return ANSC_STATUS_FAILURE;
    }


    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelIfGetVlanId(ULONG tuIns, ULONG ins, INT *vlanId)
{
    if (!vlanId)
        return ANSC_STATUS_FAILURE;

    if (GreTunnelIfPsmGetInt(GRETUIF_PARAM_VLANID, tuIns, ins, vlanId) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDml_GreTunnelIfSetVlanId(ULONG tuIns, ULONG ins, INT vlanId)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETUIF_PARAM_VLANID, tuIns, ins);
    snprintf(psmVal, sizeof(psmVal), "%d", vlanId);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDml_GreTunnelGetKeepAlivePolicy(ULONG tuIns, COSA_DML_KEEPALIVE_POLICY *policy)
{
     if (!policy)
        return ANSC_STATUS_FAILURE;

    /* do not read khotspotfd_keep_alive_policy, 
     * it's for set and may empty when bootup */
    if (GrePsmGetInt(GRETU_PARAM_KAPOL, tuIns, (int *)policy) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDml_GreTunnelSetKeepAlivePolicy(ULONG tuIns, COSA_DML_KEEPALIVE_POLICY policy)
{
    char str_value[khotspotfd_policy_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1], psmVal[16];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    status = snprintf(str_value, khotspotfd_policy_len, "%d", policy);

    if(status > 0) {

        if(sysevent_set(sysevent_fd, sysevent_token, 
            khotspotfd_keep_alive_policy, str_value, 0)) {

            AnscTraceError(("sysevent set %s failed\n", khotspotfd_keep_alive_policy)); 
        } else {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_keep_alive_policy, str_value)); 
        }
    }

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_KAPOL, tuIns);
    snprintf(psmVal, sizeof(psmVal), "%d", policy);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetKeepAliveInterval(ULONG tuIns, ULONG *val)
{
    if (!val)
        return ANSC_STATUS_FAILURE;

    if (!g_hsfdStat) {
        AnscTraceError(("hotspotfd shm not set\n"));
        return ANSC_STATUS_FAILURE;
    }
	//TODO: tuIns & why not from psm
    *val = g_hsfdStat->keepAliveInterval;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetKeepAliveInterval(ULONG tuIns, ULONG val)
{
    char str_value[khotspotfd_keep_alive_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    status = snprintf(str_value, khotspotfd_keep_alive_len, "%d", val);

    if(status > 0) {

        if(sysevent_set(sysevent_fd, sysevent_token, 
            khotspotfd_keep_alive, str_value, 0)) {

            AnscTraceError(("sysevent set %s failed\n", str_value)); 
        } else {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_keep_alive, str_value)); 
        }
    }
	//TODO: verify the logic
	g_hsfdStat->keepAliveInterval=val;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_KAITVL, tuIns);
    snprintf(psmVal, sizeof(psmVal), "%lu", val);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetKeepAliveThreshold(ULONG tuIns, ULONG *val)
{
    if (!val)
        return ANSC_STATUS_FAILURE;

    if (!g_hsfdStat) {
        AnscTraceError(("hotspotfd shm not set\n"));
        return ANSC_STATUS_FAILURE;
    }
	//TODO: tuIns & why not from PSM
    *val = g_hsfdStat->keepAliveThreshold;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetKeepAliveThreshold(ULONG tuIns, ULONG val)
{
    char str_value[khotspotfd_keep_alive_threshold_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    status = snprintf(str_value, khotspotfd_keep_alive_threshold_len, "%d", val);

    if(status > 0) {

        if(sysevent_set(sysevent_fd, sysevent_token, 
            khotspotfd_keep_alive_threshold, str_value, 0)) {

            AnscTraceError(("sysevent set %s failed\n", str_value)); 
        } else {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_keep_alive_threshold, str_value)); 
        }
    }

	//TODO: check logic
	g_hsfdStat->keepAliveThreshold=val;
	
    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_KATHRE, tuIns);
    snprintf(psmVal, sizeof(psmVal), "%lu", val);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetKeepAliveCount(ULONG tuIns, ULONG *val)
{
    if (!val)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetUlong(GRETU_PARAM_KACNT, tuIns, val) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetKeepAliveCount(ULONG tuIns, ULONG val)
{
    char str_value[khotspotfd_keep_alive_count_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    status = snprintf(str_value, sizeof(str_value), "%d", val); 

    if(status > 0) {

        if(sysevent_set(sysevent_fd, sysevent_token, 
            khotspotfd_keep_alive_count, str_value, 0)) {

            AnscTraceError(("sysevent set %s failed\n", str_value)); 
        } else {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_keep_alive_count, str_value)); 
        }
    }

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_KACNT, tuIns);
    snprintf(psmVal, sizeof(psmVal), "%lu", val);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetKeepAliveFailInterval(ULONG tuIns, ULONG *val)
{
    if (!val)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetUlong(GRETU_PARAM_KAFAILITVL, tuIns, val) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetKeepAliveFailInterval(ULONG tuIns, ULONG val)
{
    char str_value[khotspotfd_keep_alive_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    status = snprintf(str_value, khotspotfd_keep_alive_len, "%d", val);

    if(status > 0) {

        if(sysevent_set(sysevent_fd, sysevent_token, 
            khotspotfd_dead_interval, str_value, 0)) {

            AnscTraceError(("sysevent set %s failed\n", str_value)); 
        } else {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_dead_interval, str_value)); 
        }
    }

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_KAFAILITVL, tuIns);
    snprintf(psmVal, sizeof(psmVal), "%lu", val);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetReconnPrimary(ULONG tuIns, ULONG *time)
{
    if (!time)
        return ANSC_STATUS_FAILURE;

    if (!g_hsfdStat) {
        AnscTraceError(("hotspotfd shm not set\n"));
        return ANSC_STATUS_FAILURE;
    }
	//TODO: tuIns & PSM
    *time = g_hsfdStat->secondaryMaxTime;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetReconnPrimary(ULONG tuIns, ULONG time)
{
    char str_value[khotspotfd_max_secondary_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    status = snprintf(str_value, khotspotfd_max_secondary_len, "%d", time);

    if(status > 0) {

        if(sysevent_set(sysevent_fd, sysevent_token, 
            khotspotfd_max_secondary, str_value, 0)) {

            AnscTraceError(("sysevent set %s failed\n", str_value)); 
        } else {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_max_secondary, str_value)); 
        }
    }
	
	//TODO: check logic
	g_hsfdStat->secondaryMaxTime=time;
	
    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_RECONNPRI, tuIns);
    snprintf(psmVal, sizeof(psmVal), "%lu", time);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetDhcpCircuitSsid(ULONG tuIns, BOOL *enable)
{
    if (!enable)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetBool(GRETU_PARAM_DHCPCIRSSID, tuIns, enable) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetDhcpCircuitSsid(ULONG tuIns, BOOL enable)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char val[64];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    snprintf(val, sizeof(val), "%d", enable ? 1 : 0);
    if(sysevent_set(sysevent_fd, sysevent_token, 
                kSnooper_circuit_enable, val, 0) != 0) {
        AnscTraceError(("Fail to set sysevent %s\n", kSnooper_circuit_enable));
        return ANSC_STATUS_FAILURE;
    } else {
        AnscTraceDebug(("Set sysevent %s OK\n", kSnooper_circuit_enable));
    }

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_DHCPCIRSSID, tuIns);
    if (GrePsmSet(psmRec, enable ? "1" : "0") != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDml_GreTunnelGetDhcpRemoteId(ULONG tuIns, BOOL *enable)
{
    if (!enable)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetBool(GRETU_PARAM_DHCPRMID, tuIns, enable) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelSetDhcpRemoteId(ULONG tuIns, BOOL enable)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char val[64];

    if (tuIns != 1)
        return ANSC_STATUS_FAILURE;

    snprintf(val, sizeof(val), "%d", enable ? 1 : 0);
    if(sysevent_set(sysevent_fd, sysevent_token, 
                kSnooper_remote_enable, val, 0) != 0) {
        AnscTraceError(("Fail to set sysevent %s\n", kSnooper_remote_enable));
        return ANSC_STATUS_FAILURE;
    } else {
        AnscTraceDebug(("Set sysevent %s OK\n", kSnooper_remote_enable));
    }

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRETU_PARAM_DHCPRMID, tuIns);
    if (GrePsmSet(psmRec, enable ? "1" : "0") != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelIfGetAssociatedBridges(ULONG tuIns, ULONG ins, char *brs, ULONG size)
{
    if (!brs)
        return ANSC_STATUS_FAILURE;

    if (GreTunnelIfPsmGetStr(GRETUIF_PARAM_ASSOBRS, tuIns, ins,  brs, size) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelIfSetAssociatedBridges(ULONG tuIns, ULONG ins, const char *brs)
{
    char psmRec[MAX_GRE_PSM_REC + 1];

    if (!brs)
        return ANSC_STATUS_FAILURE;

    snprintf(psmRec, sizeof(psmRec), GRETUIF_PARAM_ASSOBRS, tuIns, ins);
    if (GrePsmSet(psmRec, brs) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDml_GreTunnelIfGetAssociatedBridgesWiFiPort(ULONG tuIns, ULONG ins, char *brswfp, ULONG size)
{
    if (!brswfp)
        return ANSC_STATUS_FAILURE;

    if (GreTunnelIfPsmGetStr(GRETUIF_PARAM_ASSOBRSWFP, tuIns, ins, brswfp, size) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelIfSetAssociatedBridgesWiFiPort(ULONG tuIns, ULONG ins, const char *brswfp)
{
    char psmRec[MAX_GRE_PSM_REC + 1];

    if (!brswfp)
        return ANSC_STATUS_FAILURE;

    snprintf(psmRec, sizeof(psmRec), GRETUIF_PARAM_ASSOBRSWFP, ins, tuIns);
    if (GrePsmSet(psmRec, brswfp) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreTunnelGetGRETunnel(ULONG tuIns, char *greif, ULONG size)
{
    char greNetworkTunnel[256];

    if (!greif)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRETU_PARAM_GRETU, tuIns, greNetworkTunnel, sizeof(greNetworkTunnel)) != 0)
        return ANSC_STATUS_FAILURE;

    snprintf(greif, size, "%s", greNetworkTunnel);
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDml_GreTunnelSetGREInterface(ULONG ins, const char *greif)
{
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDml_GreTunnelIfSetGREInterface(ULONG tuIns, ULONG ins, const char *greif)
{
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDml_GreTunnelGetStats( ULONG ins, COSA_DML_GRE_TUNNEL_STATS *stats)
{
    if (ins != 1|| !stats)
        return ANSC_STATUS_FAILURE;

    if (!g_hsfdStat) {
        AnscTraceError(("hotspotfd shm not set\n"));
        return ANSC_STATUS_FAILURE;
    }
	
    stats->KeepAliveSent = g_hsfdStat->keepAlivesSent;
    stats->KeepAliveReceived = g_hsfdStat->keepAlivesReceived;
    stats->DiscardChecksumReceived = 0; // XXX: Backend not supportted
    stats->DiscardSequenceNumberReceived = 0; // XXX: Backend not supportted

    return ANSC_STATUS_SUCCESS;
}

#endif
