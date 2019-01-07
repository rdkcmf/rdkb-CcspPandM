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

#ifdef CONFIG_CISCO_HOTSPOT
/**************************************************************************

    module: cosa_x_comcast_com_gre_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        leichen2@cisco.com

    -------------------------------------------------------------------

    revision:

        08/30/2011    initial revision.

**************************************************************************/

#include "cosa_apis.h"
#include "cosa_x_comcast_com_gre_apis.h"
#include "ccsp_psm_helper.h"
#include "ansc_platform.h"
#include "plugin_main_apis.h"

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

#define GRE_OBJ_GREIF           "dmsb.hotspot.gre."

#define GRE_PARAM_ENABLE        GRE_OBJ_GREIF "%d.Enable"
#define GRE_PARAM_LOCALIFS      GRE_OBJ_GREIF "%d.LocalInterfaces"
#define GRE_PARAM_ENDPOINTS     GRE_OBJ_GREIF "%d.Endpoints"
#define GRE_PARAM_KEYGENPOL     GRE_OBJ_GREIF "%d.KeyIDGenPolicy"
#define GRE_PARAM_KEYID         GRE_OBJ_GREIF "%d.KeyID"
#define GRE_PARAM_USESEQ        GRE_OBJ_GREIF "%d.UseSeqNum"
#define GRE_PARAM_USECSUM       GRE_OBJ_GREIF "%d.UseCheckSum"
#define GRE_PARAM_DSCPPOL       GRE_OBJ_GREIF "%d.DSCPMarkPolicy"
#define GRE_PARAM_VLANID        GRE_OBJ_GREIF "%d.VLANID"
#define GRE_PARAM_RECONNPRI     GRE_OBJ_GREIF "%d.ReconnPrimary"
#define GRE_PARAM_KAPOL         GRE_OBJ_GREIF "%d.KeepAlive.Policy"
#define GRE_PARAM_KAITVL        GRE_OBJ_GREIF "%d.KeepAlive.Interval"
#define GRE_PARAM_KATHRE        GRE_OBJ_GREIF "%d.KeepAlive.Threshold"
#define GRE_PARAM_KACNT         GRE_OBJ_GREIF "%d.KeepAlive.Count"
#define GRE_PARAM_KAFAILITVL    GRE_OBJ_GREIF "%d.KeepAlive.FailInterval"
#define GRE_PARAM_DHCPCIRSSID   GRE_OBJ_GREIF "%d.DHCP.CircuitIDSSID"
#define GRE_PARAM_DHCPRMID      GRE_OBJ_GREIF "%d.DHCP.RemoteID"
#define GRE_PARAM_ASSOBRS       GRE_OBJ_GREIF "%d.AssociatedBridges"
#define GRE_PARAM_ASSOBRSWFP    GRE_OBJ_GREIF "%d.AssociatedBridgesWiFiPort"
#define GRE_PARAM_GREIF         GRE_OBJ_GREIF "%d.GRENetworkInterface"
#define LOCALINTERFACES_PRE_SECURE_SSID		"Device.WiFi.SSID.5.,Device.WiFi.SSID.6."

/* when secure ssid is supported in mips products, use below flag instaed of above flag for handling race condition in bbhm readiness 
replace LOCALINTERFACES_PRE_SECURE_SSID with LOCALINTERFACES_POST_SECURE_SSID in function hotspot_update_circuit_ids() */

#define LOCALINTERFACES_POST_SECURE_SSID	"Device.WiFi.SSID.5.,Device.WiFi.SSID.6.,Device.WiFi.SSID.9.,Device.WiFi.SSID.10."

#define HOTSPOT_INTERFACE_PRE_SECURE_SSID_LEN		39

/* when all platforms support Secure SSID move replace HOTSPOT_INTERFACE_PRE_SECURE_SSID_LEN with HOTSPOT_INTERFACE_POST_SECURE_SSID_LEN
in hotspot_update_circuit_ids() */

#define HOTSPOT_INTERFACE_POST_SECURE_SSID_LEN		80
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
GrePsmGetBool(const char *param, int ins, BOOL *value)
{
    char rec[256], val[1024];

    snprintf(rec, sizeof(rec), param, ins);
    if (GrePsmGet(rec, val, sizeof(val)) != 0)
        return -1;

    *value = (atoi(val) == 1) ? TRUE : FALSE;
    return 0;
}

/* need free */
static char *GetAssoBridge(ULONG ins)
{
    char *assoBrs = NULL;

    if ((assoBrs = malloc(1024)) == NULL)
        return NULL;

    if (GrePsmGetStr(GRE_PARAM_ASSOBRS, ins, assoBrs, 1024) != 0)
    {
        AnscTraceError(("Fail to get AssociatedBridges from Psm.\n"));
        free( assoBrs );
        assoBrs = NULL;
    }

    return assoBrs;
}

static char *GetAssoBridgeWiFiPort(ULONG ins)
{
    char *assoBrsWfp = NULL;

    if ((assoBrsWfp = malloc(1024)) == NULL)
        return NULL;

    if (GrePsmGetStr(GRE_PARAM_ASSOBRSWFP, ins, assoBrsWfp, 1024) != 0)
    {
        AnscTraceError(("Fail to get AssociatedBridgesWiFiPort from Psm.\n"));
        free( assoBrsWfp );
        assoBrsWfp = NULL;
    }

    return assoBrsWfp;
}

int hotspot_update_circuit_ids(int greinst, int queuestart) {
    int retval = 0;
    char localinterfaces[200];
    char paramname[60];
    char circuitid[100];
    char outdata[80];
    char* save = NULL;
    char* curInt = NULL;
    char* testcurInt = NULL;
    int nameSave = 0;
    int circuitSave = 0;
    int ssidInst = 0;
    int size;
    int inst;
    int retry_count =0;
    parameterValStruct_t varStruct;
    varStruct.parameterName = paramname;
    varStruct.parameterValue = outdata;
//     /*if (ppComponents == NULL) {
//         initWifiComp();
//     }*/
    
    //snprintf(paramname, sizeof(paramname), 
    CcspTraceInfo(("entered%s\n","hotspot_update_circuit_ids"));
    retval=GrePsmGetStr(GRE_PARAM_LOCALIFS, greinst, localinterfaces, sizeof(localinterfaces));
    while(retval == -1 || strnlen(localinterfaces,sizeof(localinterfaces)) < HOTSPOT_INTERFACE_PRE_SECURE_SSID_LEN)
    {
        	CcspTraceError(("could not fetch proper hotspot interface name from psm\n"));
    		sleep(1);
        	retry_count++;
    		retval=GrePsmGetStr(GRE_PARAM_LOCALIFS, greinst, localinterfaces, sizeof(localinterfaces));
                if(retry_count >=5) break;
    }
    if(retval == -1 || strnlen(localinterfaces,sizeof(localinterfaces)) < HOTSPOT_INTERFACE_PRE_SECURE_SSID_LEN)
    {	
        CcspTraceError(("could not fetch hotspot interface name from psm after multiple attempts, setting it\n"));
	strncpy(localinterfaces,LOCALINTERFACES_PRE_SECURE_SSID,sizeof(localinterfaces));
    }
    CcspTraceInfo(("localinterfaces %s\n", localinterfaces));
    
    curInt = strtok_r(localinterfaces, ",", &save);

    CcspTraceInfo(("curInt %s\n", curInt));
    
    while (curInt) {
        circuitSave=0;
        //Trim off the trailing dot if it exists
        size = strlen(curInt);
        if (curInt[size -1] == '.')
            curInt[size - 1]='\0';
        testcurInt=strrchr(curInt,'.');
        CcspTraceInfo(("testcurInt is %s\n",testcurInt));       
        inst = atoi(strrchr(curInt,'.')+1);
  
		memset(paramname,0,sizeof(paramname));
		memset(outdata,0,sizeof(outdata));
        
        size = sizeof(outdata);
        
        if (syscfg_get(NULL, "wan_physical_ifname", paramname, sizeof(paramname)) != 0) {
            AnscTraceError(("fail to get wan_physical_ifname\n"));
            snprintf(paramname, sizeof(paramname), "erouter0");
        }
        if (get_if_hwaddr(paramname, circuitid, sizeof(circuitid)) != 0) {
            AnscTraceError(("fail to get HW Addr for %s\n", paramname));
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
        
        curInt = strtok_r(NULL, ",", &save);
    }
    
    return queuestart;
    ////get_wifi_param(dm, buf);
    //get local interfaces
    //foreach
        //read bssid
        //read ssid
        //read security type
        //sysevent set circuit id
    //ef
}
#define INITIAL_CIRCUIT_ID_SLEEP 5
#define POLL_CIRCUIT_ID_SLEEP 3
#define INITIAL_SNOOPER_QUEUE 1
static void* circuit_id_init_thread(void* arg) {
    int ret = -1;
    sleep(INITIAL_CIRCUIT_ID_SLEEP);

    ret = hotspot_update_circuit_ids(1, INITIAL_SNOOPER_QUEUE);
    
    
    while (ret < 0) {
        sleep(POLL_CIRCUIT_ID_SLEEP);
        ret = hotspot_update_circuit_ids(1, INITIAL_SNOOPER_QUEUE);
    }
    
    return NULL;
}

ANSC_STATUS 
CosaDml_GreInit(void)
{
    int shmid;
    void *params = NULL;

	CcspTraceInfo(( "%s %d - Entry\n", __FUNCTION__, __LINE__ ));

    sysevent_fd = sysevent_open(
            "127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, 
            SE_VERSION, kHotspotfd_events, 
            &sysevent_token
    );

    if (sysevent_fd < 0)
    {
        CcspTraceError(("sysevent_open failed\n"));
    } else {
        CcspTraceInfo(("sysevent_open success\n"));
    }

	CcspTraceInfo(( "%s %d - Acquiring shared memory\n", __FUNCTION__, __LINE__ ));

    if ((shmid = shmget(kKeepAlive_Statistics, kKeepAlive_SharedMemSize, IPC_CREAT | 0666)) < 0) {
        CcspTraceError(("shmget: %s\n", strerror(errno)));
        return ANSC_STATUS_FAILURE;
    }

	CcspTraceInfo(( "%s %d - Attaching shared memory shmid:%d\n", __FUNCTION__, __LINE__, shmid ));

    if ((g_hsfdStat = shmat(shmid, NULL, 0)) == (hotspotfd_statistics_s *)-1) {
        CcspTraceError(("shmat: %s\n", strerror(errno)));
        g_hsfdStat = NULL;
        return ANSC_STATUS_FAILURE;
    }
    
	CcspTraceInfo(( "%s %d - Creating circuit_id_init_thread thread\n", __FUNCTION__, __LINE__ ));

    AnscCreateTask(circuit_id_init_thread, USER_DEFAULT_TASK_STACK_SIZE, USER_DEFAULT_TASK_PRIORITY, params, "CircuitIDInitThread");

	CcspTraceInfo(( "%s %d - Init Hotspot GRE Done\n", __FUNCTION__, __LINE__ ));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDml_GreFinalize(void)
{
    return ANSC_STATUS_SUCCESS;
}

ULONG 
CosaDml_GreIfGetNumberOfEntries(void)
{
    /* only support one GRE for Comcast Hotspot */
    return 1;
}

#define kHotspotfd_tunnelEP                 "hotspotfd-tunnelEP" 

ANSC_STATUS
CosaDml_GreIfGetConnectedRemoteEndpoint(ULONG idx, COSA_DML_GRE_IF *greIf)
{

	char cmd[126] = {0};
	char line_buf[126] = {0};
	FILE *fp = NULL;

	if(!greIf)
		return ANSC_STATUS_FAILURE;

	snprintf(cmd, sizeof(cmd), "sysevent get %s",kHotspotfd_tunnelEP);       
    if (((fp = popen(cmd,"r")) != NULL) && (fgets(line_buf, sizeof(line_buf), fp)))
    {
		sprintf(greIf->ConnectedRemoteEndpoint,"%s",line_buf);
    }
	if(fp)
		pclose(fp);

	return ANSC_STATUS_SUCCESS;	
}

ANSC_STATUS
CosaDml_GreIfGetEntryByIndex(ULONG idx, COSA_DML_GRE_IF *greIf)
{
    int ins = 1;

	CcspTraceInfo(( "%s %d - Entry\n", __FUNCTION__, __LINE__ ));

    if (idx != 0 || !greIf)
        return ANSC_STATUS_FAILURE;

    memset(greIf, 0, sizeof(COSA_DML_GRE_IF));

    greIf->InstanceNumber = ins;

    if (GrePsmGetBool(GRE_PARAM_ENABLE, ins, &greIf->Enable) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetStr(GRE_PARAM_LOCALIFS, ins, greIf->LocalInterfaces, sizeof(greIf->LocalInterfaces)) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetStr(GRE_PARAM_ENDPOINTS, ins, greIf->RemoteEndpoints, sizeof(greIf->RemoteEndpoints)) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetInt(GRE_PARAM_KEYGENPOL, ins, (int *)&greIf->KeyIdentifierGenerationPolicy) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetStr(GRE_PARAM_KEYID, ins, greIf->KeyIdentifier, sizeof(greIf->KeyIdentifier)) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetBool(GRE_PARAM_USESEQ, ins, &greIf->UseSequenceNumber) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetBool(GRE_PARAM_USECSUM, ins, &greIf->UseChecksum) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetInt(GRE_PARAM_DSCPPOL, ins, &greIf->DSCPMarkPolicy) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetInt(GRE_PARAM_VLANID, ins, &greIf->VLANID) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetInt(GRE_PARAM_KAPOL, ins, (int *)&greIf->KeepAlivePolicy) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetUlong(GRE_PARAM_KAITVL, ins, &greIf->KeepAliveInterval) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetUlong(GRE_PARAM_KATHRE, ins, &greIf->KeepAliveThreshold) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetUlong(GRE_PARAM_KACNT, ins, &greIf->KeepAliveCount) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetUlong(GRE_PARAM_KAFAILITVL, ins, &greIf->KeepAliveFailInterval) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetUlong(GRE_PARAM_RECONNPRI, ins, &greIf->ReconnectPrimary) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetBool(GRE_PARAM_DHCPCIRSSID, ins, &greIf->DHCPCircuitIDSSID) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetBool(GRE_PARAM_DHCPRMID, ins, &greIf->DHCPRemoteID) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetStr(GRE_PARAM_ASSOBRS, ins, greIf->AssociatedBridges, sizeof(greIf->AssociatedBridges)) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetStr(GRE_PARAM_ASSOBRSWFP, ins, greIf->AssociatedBridgesWiFiPort, sizeof(greIf->AssociatedBridgesWiFiPort)) != 0)
        return ANSC_STATUS_FAILURE;
    if (GrePsmGetStr(GRE_PARAM_GREIF, ins, greIf->GRENetworkInterface, sizeof(greIf->GRENetworkInterface)) != 0)
        return ANSC_STATUS_FAILURE;

	CcspTraceInfo(( "%s %d - Exit\n", __FUNCTION__, __LINE__ ));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetIns(ULONG idx, ULONG ins)
{
    if (idx != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetEnable(ULONG ins, BOOL *enable)
{
    if (ins != 1 || !enable)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetBool(GRE_PARAM_ENABLE, ins, enable) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetEnable(ULONG ins, BOOL enable)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char greNetworkInterface[256];
    char tmpPath[256];

    if (ins != 1)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_GREIF, ins, greNetworkInterface, sizeof(greNetworkInterface)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sEnable", greNetworkInterface);

    if (g_SetParamValueBool(tmpPath, enable) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_ENABLE, ins);
    if (GrePsmSet(psmRec, enable ? "1" : "0") != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetStatus(ULONG ins, COSA_DML_GRE_STATUS *st)
{
    char status[64];
    ULONG size = sizeof(status);
    char greNetworkInterface[256];
    char tmpPath[256];

    if (ins != 1 || !st)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_GREIF, ins, greNetworkInterface, sizeof(greNetworkInterface)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sStatus", greNetworkInterface);

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
CosaDml_GreIfGetLastchange(ULONG ins, ULONG *time)
{
    char greNetworkInterface[256];
    char tmpPath[256];

    if (ins != 1 || !time)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_GREIF, ins, greNetworkInterface, sizeof(greNetworkInterface)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sLastChange", greNetworkInterface);

    *time = g_GetParamValueUlong(g_pDslhDmlAgent, tmpPath);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetLocalInterfaces(ULONG ins, char *ifs, ULONG size)
{
    char *brlist, *br, *delim, *start, *sp;
    ULONG ptInsList[16], ptInsCnt = 16;
    char dm[1024], dmval[1024 + 1];
    ULONG dmsize;
    int i;
    char *tif;

    if (ins != 1 || !ifs)
        return ANSC_STATUS_FAILURE;

    memset(ifs, 0, size);

    if ((brlist = GetAssoBridge(ins)) == NULL)
        return ANSC_STATUS_FAILURE;

    /* for each bridge */
    for (start = brlist, delim = ",";
            (br = strtok_r(start, delim, &sp)) != NULL;
            start = NULL) {

        AnscTraceDebug(("Bridge: %s\n", br));

        /* for each port */
        snprintf(dm, sizeof(dm), "%sPort.", br);
        if (g_GetInstanceNumbers(dm, ptInsList, &ptInsCnt) != ANSC_STATUS_SUCCESS) {
            AnscTraceError(("Fail to get port ins numbers\n"));
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
CosaDml_GreIfSetLocalInterfaces(ULONG ins, const char *ifs)
{
    char psmRec[MAX_GRE_PSM_REC + 1], dm[1024];
    char *cp, *if1, *if2, *br1, *br2, *brwfp1, *brwfp2;
    char *ifsBuf, *brsBuf, *brswfpBuf;
    int brIns, brInsStr[3];

    if (ins != 1 || !ifs)
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
    if ((brsBuf = GetAssoBridge(ins)) == NULL) {
        free(ifsBuf);
        return ANSC_STATUS_FAILURE;
    }
    if ((brswfpBuf = GetAssoBridgeWiFiPort(ins)) == NULL) {
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
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_LOCALIFS, ins);
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
CosaDml_GreIfGetEndpoints(ULONG ins, char *eps, ULONG size)
{
    if (ins != 1 || !eps)
        return ANSC_STATUS_FAILURE;

    memset(eps, 0, size);
    if (GrePsmGetStr(GRE_PARAM_ENDPOINTS, ins, eps, size) != 0)
            return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetEndpoints(ULONG ins, const char *eps)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char buf[1025];
    char *pri, *sec;

    if (ins != 1 || !eps)
        return ANSC_STATUS_FAILURE;

    snprintf(buf, sizeof(buf), "%s", eps);
    pri = buf;
    if ((sec = strchr(buf, ',')) != NULL)
        *sec++ = '\0';

    if (pri && strlen(pri)) {
        if (sysevent_set(sysevent_fd, sysevent_token, kHotspotfd_primary, pri, 0) != 0) {
            AnscTraceError(("Fail to set sysevent: %s to %s\n", kHotspotfd_primary, pri));
            return ANSC_STATUS_FAILURE;
        }
    }

    if (sec && strlen(sec)) {
        if (sysevent_set(sysevent_fd, sysevent_token, khotspotfd_secondary, sec, 0) != 0) {
            AnscTraceError(("Fail to set sysevent: %s to %s\n", khotspotfd_secondary, sec));
            return ANSC_STATUS_FAILURE;
        }
    }

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_ENDPOINTS, ins);
    if (GrePsmSet(psmRec, eps) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetConnEndpoint(ULONG ins, char *ep, ULONG size)
{
    char greNetworkInterface[256];
    char tmpPath[256];

    if (ins != 1 || !ep)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_GREIF, ins, greNetworkInterface, sizeof(greNetworkInterface)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sRemoteEndpoint", greNetworkInterface);

    if (g_GetParamValueString(g_pDslhDmlAgent, tmpPath, ep, &size) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetKeyGenPolicy(ULONG ins, COSA_DML_KEY_ID_GEN_POLICY *policy)
{
    if (ins != 1 || !policy)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetInt(GRE_PARAM_KEYGENPOL, ins, (int *)policy) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetKeyGenPolicy(ULONG ins, COSA_DML_KEY_ID_GEN_POLICY policy)
{
    char psmRec[MAX_GRE_PSM_REC + 1], psmVal[64];
    char *mode;
    char greNetworkInterface[256];
    char tmpPath[256];

    if (ins != 1)
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

    if (GrePsmGetStr(GRE_PARAM_GREIF, ins, greNetworkInterface, sizeof(greNetworkInterface)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sKeyMode", greNetworkInterface);

    if (g_SetParamValueString(tmpPath, mode) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_KEYGENPOL, ins);
    snprintf(psmVal, sizeof(psmVal), "%d", policy);

    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetKeyId(ULONG ins, char *keyId, ULONG size)
{
    if (ins != 1 || !keyId)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_KEYID, ins, keyId, size) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetKeyId(ULONG ins, const char *keyId)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char greNetworkInterface[256];
    char tmpPath[256];

    if (ins != 1)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_GREIF, ins, greNetworkInterface, sizeof(greNetworkInterface)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sKey", greNetworkInterface);

    if (g_SetParamValueString(tmpPath, (char *)keyId) != 0)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_KEYID, ins);
    if (GrePsmSet(psmRec, keyId) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetUseSeqNum(ULONG ins, BOOL *enable)
{
    if (ins != 1 || !enable)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetBool(GRE_PARAM_USESEQ, ins, enable) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetUseSeqNum(ULONG ins, BOOL enable)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char greNetworkInterface[256];
    char tmpPath[256];

    if (ins != 1)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_GREIF, ins, greNetworkInterface, sizeof(greNetworkInterface)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sSequenceNumberEnabled", greNetworkInterface);

    if (g_SetParamValueBool(tmpPath, enable) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_USESEQ, ins);
    if (GrePsmSet(psmRec, enable ? "1" : "0") != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetUseChecksum(ULONG ins, BOOL *enable)
{
    if (ins != 1 || !enable)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetBool(GRE_PARAM_USECSUM, ins, enable) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetUseChecksum(ULONG ins, BOOL enable)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char greNetworkInterface[256];
    char tmpPath[256];

    if (ins != 1)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_GREIF, ins, greNetworkInterface, sizeof(greNetworkInterface)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sChecksumEnabled", greNetworkInterface);

    if (g_SetParamValueBool(tmpPath, enable) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_USECSUM, ins);
    if (GrePsmSet(psmRec, enable ? "1" : "0") != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetDSCPMarkPolicy(ULONG ins, INT *dscp)
{
    if (ins != 1 || !dscp)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetInt(GRE_PARAM_DSCPPOL, ins, dscp) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetDSCPMarkPolicy(ULONG ins, INT dscp)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];
    char greNetworkInterface[256];
    char tmpPath[256];
    char tmp2Path[256];

    if (ins != 1)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_GREIF, ins, greNetworkInterface, sizeof(greNetworkInterface)) != 0)
        return ANSC_STATUS_FAILURE;
    snprintf(tmpPath, sizeof(tmpPath), "%sTOSMode", greNetworkInterface);
    snprintf(tmp2Path, sizeof(tmp2Path), "%sTOS", greNetworkInterface);

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

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_DSCPPOL, ins);
    snprintf(psmVal, sizeof(psmVal), "%d", dscp);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetVlanId(ULONG ins, INT *vlanId)
{
    if (ins != 1 || !vlanId)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetInt(GRE_PARAM_VLANID, ins, vlanId) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetVlanId(ULONG ins, INT vlanId)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (ins != 1)
        return ANSC_STATUS_FAILURE;

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_VLANID, ins);
    snprintf(psmVal, sizeof(psmVal), "%d", vlanId);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetKeepAlivePolicy(ULONG ins, COSA_DML_KEEPALIVE_POLICY *policy)
{
    char val[64];

    if (ins != 1 || !policy)
        return ANSC_STATUS_FAILURE;

    /* do not read khotspotfd_keep_alive_policy, 
     * it's for set and may empty when bootup */
    if (GrePsmGetInt(GRE_PARAM_KAPOL, ins, (int *)policy) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetKeepAlivePolicy(ULONG ins, COSA_DML_KEEPALIVE_POLICY policy)
{
    char str_value[khotspotfd_policy_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1], psmVal[16];

    if (ins != 1)
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
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_KAPOL, ins);
    snprintf(psmVal, sizeof(psmVal), "%d", policy);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetKeepAliveInterval(ULONG ins, ULONG *val)
{
    if (ins != 1 || !val)
        return ANSC_STATUS_FAILURE;

    if (!g_hsfdStat) {
        AnscTraceError(("hotspotfd shm not set\n"));
        return ANSC_STATUS_FAILURE;
    }

    *val = g_hsfdStat->keepAliveInterval;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetKeepAliveInterval(ULONG ins, ULONG val)
{
    char str_value[khotspotfd_keep_alive_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (ins != 1)
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

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_KAITVL, ins);
    snprintf(psmVal, sizeof(psmVal), "%lu", val);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetKeepAliveThreshold(ULONG ins, ULONG *val)
{
    if (ins != 1 || !val)
        return ANSC_STATUS_FAILURE;

    if (!g_hsfdStat) {
        AnscTraceError(("hotspotfd shm not set\n"));
        return ANSC_STATUS_FAILURE;
    }

    *val = g_hsfdStat->keepAliveThreshold;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetKeepAliveThreshold(ULONG ins, ULONG val)
{
    char str_value[khotspotfd_keep_alive_threshold_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (ins != 1)
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

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_KATHRE, ins);
    snprintf(psmVal, sizeof(psmVal), "%lu", val);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetKeepAliveCount(ULONG ins, ULONG *val)
{
    if (ins != 1 || !val)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetUlong(GRE_PARAM_KACNT, ins, val) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetKeepAliveCount(ULONG ins, ULONG val)
{
    char str_value[khotspotfd_keep_alive_count_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (ins != 1)
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
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_KACNT, ins);
    snprintf(psmVal, sizeof(psmVal), "%lu", val);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetKeepAliveFailInterval(ULONG ins, ULONG *val)
{
    if (ins != 1 || !val)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetUlong(GRE_PARAM_KAFAILITVL, ins, val) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetKeepAliveFailInterval(ULONG ins, ULONG val)
{
    char str_value[khotspotfd_keep_alive_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (ins != 1)
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
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_KAFAILITVL, ins);
    snprintf(psmVal, sizeof(psmVal), "%lu", val);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetReconnPrimary(ULONG ins, ULONG *time)
{
    if (ins != 1 || !time)
        return ANSC_STATUS_FAILURE;

    if (!g_hsfdStat) {
        AnscTraceError(("hotspotfd shm not set\n"));
        return ANSC_STATUS_FAILURE;
    }

    *time = g_hsfdStat->secondaryMaxTime;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetReconnPrimary(ULONG ins, ULONG time)
{
    char str_value[khotspotfd_max_secondary_len+1];
    int status;
    char psmRec[MAX_GRE_PSM_REC + 1];
    char psmVal[16];

    if (ins != 1)
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

    /* save to PSM */
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_RECONNPRI, ins);
    snprintf(psmVal, sizeof(psmVal), "%lu", time);
    if (GrePsmSet(psmRec, psmVal) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetDhcpCircuitSsid(ULONG ins, BOOL *enable)
{
    if (ins != 1 || !enable)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetBool(GRE_PARAM_DHCPCIRSSID, ins, enable) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetDhcpCircuitSsid(ULONG ins, BOOL enable)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char val[64];

    if (ins != 1)
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
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_DHCPCIRSSID, ins);
    if (GrePsmSet(psmRec, enable ? "1" : "0") != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetDhcpRemoteId(ULONG ins, BOOL *enable)
{
    if (ins != 1 || !enable)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetBool(GRE_PARAM_DHCPRMID, ins, enable) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetDhcpRemoteId(ULONG ins, BOOL enable)
{
    char psmRec[MAX_GRE_PSM_REC + 1];
    char val[64];

    if (ins != 1)
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
    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_DHCPRMID, ins);
    if (GrePsmSet(psmRec, enable ? "1" : "0") != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetAssociatedBridges(ULONG ins, char *brs, ULONG size)
{
    if (ins != 1 || !brs)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_ASSOBRS, ins, brs, size) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetAssociatedBridges(ULONG ins, const char *brs)
{
    char psmRec[MAX_GRE_PSM_REC + 1];

    if (ins != 1 || !brs)
        return ANSC_STATUS_FAILURE;

    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_ASSOBRS, ins);
    if (GrePsmSet(psmRec, brs) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetAssociatedBridgesWiFiPort(ULONG ins, char *brswfp, ULONG size)
{
    if (ins != 1 || !brswfp)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_ASSOBRSWFP, ins, brswfp, size) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetAssociatedBridgesWiFiPort(ULONG ins, const char *brswfp)
{
    char psmRec[MAX_GRE_PSM_REC + 1];

    if (ins != 1 || !brswfp)
        return ANSC_STATUS_FAILURE;

    snprintf(psmRec, sizeof(psmRec), GRE_PARAM_ASSOBRSWFP, ins);
    if (GrePsmSet(psmRec, brswfp) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfGetGREInterface(ULONG ins, char *greif, ULONG size)
{
    char greNetworkInterface[256];

    if (ins != 1 || !greif)
        return ANSC_STATUS_FAILURE;

    if (GrePsmGetStr(GRE_PARAM_GREIF, ins, greNetworkInterface, sizeof(greNetworkInterface)) != 0)
        return ANSC_STATUS_FAILURE;

    snprintf(greif, size, "%s", greNetworkInterface);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_GreIfSetGREInterface(ULONG ins, const char *greif)
{
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDml_GreIfGetStats(ULONG ins, COSA_DML_GRE_IF_STATS *stats)
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
