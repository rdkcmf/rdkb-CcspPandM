/*)
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2017 RDK Management
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
//#include "ssp_global.h"
#include "ansc_platform.h"
#include "ccsp_base_api.h"
#include <libparodus.h>
#include "cJSON.h"
#include <sysevent/sysevent.h>

#define CONTENT_TYPE_JSON  "application/json"

#if defined(_COSA_BCM_MIPS_)
#define DEVICE_MAC                   "Device.DPoE.Mac_address"
#else
#define DEVICE_MAC                   "Device.X_CISCO_COM_CableModem.MACAddress"
#endif

#define CLIENT_PORT_NUM     6670
#define MAX_PARAMETERNAME_LEN    512
#define DEVICE_PROPS_FILE  "/etc/device.properties"
#define ETH_WAN_STATUS_PARAM "Device.Ethernet.X_RDKCENTRAL-COM_WAN.Enabled"
#define RDKB_ETHAGENT_COMPONENT_NAME                  "com.cisco.spvtg.ccsp.ethagent"
#define RDKB_ETHAGENT_DBUS_PATH                       "/com/cisco/spvtg/ccsp/ethagent"

void* connect_parodus();
static void getDeviceMac();
static void macToLower(char macValue[]);
static void get_parodus_url(char **url);
void initparodusTask();
void Send_Notification_Task(char* value);
void* sendNotification(void* buff);
static void waitForEthAgentComponentReady();
static void checkComponentHealthStatus(char * compName, char * dbusPath, char *status, int *retStatus);
static int check_ethernet_wan_status();

static char deviceMAC[32]={'\0'};
libpd_instance_t pam_instance;
extern ANSC_HANDLE bus_handle;

static void get_parodus_url(char **url)
{
    FILE *fp = fopen(DEVICE_PROPS_FILE, "r");

    if( NULL != fp ) {
        char str[255] = {'\0'};
        while( fscanf(fp,"%s", str) != EOF) {
            char *value = NULL;
            if( value = strstr(str, "PARODUS_URL=") ) {
                value = value + strlen("PARODUS_URL=");
                *url = strdup(value);
                CcspTraceDebug(("parodus url is %s\n", *url));
            }
        }
    } else {
        CcspTraceError(("Failed to open device.properties file:%s\n", DEVICE_PROPS_FILE));
    }
    fclose(fp);

    if( NULL == *url ) {
        CcspTraceError(("parodus url is not present in device.properties file\n"));
    }

    CcspTraceDebug(("parodus url formed is %s\n", *url));
}

static void waitForEthAgentComponentReady()
{
    char status[32] = {'\0'};
    int count = 0;
    int ret = -1;
    while(1)
    {
        checkComponentHealthStatus(RDKB_ETHAGENT_COMPONENT_NAME, RDKB_ETHAGENT_DBUS_PATH, status,&ret);
        if(ret == CCSP_SUCCESS && (strcmp(status, "Green") == 0))
        {
            CcspTraceInfo(("%s component health is %s, continue\n", RDKB_ETHAGENT_COMPONENT_NAME, status));
            break;
        }
        else
        {
            count++;
            if(count > 60)
            {
                CcspTraceError(("%s component Health check failed (ret:%d), continue\n",RDKB_ETHAGENT_COMPONENT_NAME, ret));
                break;
            }
            if(count%5 == 0)
            {
                CcspTraceError(("%s component Health, ret:%d, waiting\n", RDKB_ETHAGENT_COMPONENT_NAME, ret));
            }
            sleep(5);
        }
    }
}

static void checkComponentHealthStatus(char * compName, char * dbusPath, char *status, int *retStatus)
{
	int ret = 0, val_size = 0;
	parameterValStruct_t **parameterval = NULL;
	char *parameterNames[1] = {};
	char tmp[MAX_PARAMETERNAME_LEN];
	char str[MAX_PARAMETERNAME_LEN/2];     
	char l_Subsystem[MAX_PARAMETERNAME_LEN/2] = { 0 };

	sprintf(tmp,"%s.%s",compName, "Health");
	parameterNames[0] = tmp;

	strncpy(l_Subsystem, "eRT.",sizeof(l_Subsystem));
	snprintf(str, sizeof(str), "%s%s", l_Subsystem, compName);
	CcspTraceDebug(("str is:%s\n", str));

	ret = CcspBaseIf_getParameterValues(bus_handle, str, dbusPath,  parameterNames, 1, &val_size, &parameterval);
	CcspTraceDebug(("ret = %d val_size = %d\n",ret,val_size));
	if(ret == CCSP_SUCCESS)
	{
		CcspTraceDebug(("parameterval[0]->parameterName : %s parameterval[0]->parameterValue : %s\n",parameterval[0]->parameterName,parameterval[0]->parameterValue));
		strcpy(status, parameterval[0]->parameterValue);
		CcspTraceDebug(("status of component:%s\n", status));
	}
	free_parameterValStruct_t (bus_handle, val_size, parameterval);

	*retStatus = ret;
}

static int check_ethernet_wan_status()
{
    int ret = -1, size =0, val_size =0;
    char compName[MAX_PARAMETERNAME_LEN/2] = { '\0' };
    char dbusPath[MAX_PARAMETERNAME_LEN/2] = { '\0' };
    parameterValStruct_t **parameterval = NULL;
    char *getList[] = {ETH_WAN_STATUS_PARAM};
    componentStruct_t **        ppComponents = NULL;
    char dst_pathname_cr[256] = {0};
    char isEthEnabled[64]={'\0'};
    
    if(0 == syscfg_init())
    {
        if( 0 == syscfg_get( NULL, "eth_wan_enabled", isEthEnabled, sizeof(isEthEnabled)) && (isEthEnabled[0] != '\0' && strncmp(isEthEnabled, "true", strlen("true")) == 0))
        {
            CcspTraceInfo(("Ethernet WAN is enabled\n"));
            ret = CCSP_SUCCESS;
        }
    }
    else
    {
        waitForEthAgentComponentReady();
        sprintf(dst_pathname_cr, "%s%s", "eRT.", CCSP_DBUS_INTERFACE_CR);
        ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle, dst_pathname_cr, ETH_WAN_STATUS_PARAM, "", &ppComponents, &size);
        if ( ret == CCSP_SUCCESS && size >= 1)
        {
            strncpy(compName, ppComponents[0]->componentName, sizeof(compName)-1);
            strncpy(dbusPath, ppComponents[0]->dbusPath, sizeof(compName)-1);
        }
        else
        {
            CcspTraceError(("Failed to get component for %s ret: %d\n",ETH_WAN_STATUS_PARAM,ret));
        }
        free_componentStruct_t(bus_handle, size, ppComponents);

        if(strlen(compName) != 0 && strlen(dbusPath) != 0)
        {
            ret = CcspBaseIf_getParameterValues(bus_handle, compName, dbusPath, getList, 1, &val_size, &parameterval);
            if(ret == CCSP_SUCCESS && val_size > 0)
            {
                if(parameterval[0]->parameterValue != NULL && strncmp(parameterval[0]->parameterValue, "true", strlen("true")) == 0)
                {
                    CcspTraceInfo(("Ethernet WAN is enabled\n"));
                    ret = CCSP_SUCCESS;
                }
                else
                {
                    CcspTraceInfo(("Ethernet WAN is disabled\n"));
                    ret = CCSP_FAILURE;
                }
            }
            else
            {
                CcspTraceError(("Failed to get values for %s ret: %d\n",getList[0],ret));
            }
            free_parameterValStruct_t(bus_handle, val_size, parameterval);
        }
    }
    return ret;
}

static void getDeviceMac()
{
    if(strlen(deviceMAC) == 0)
    {
        int ret = -1, size =0, val_size =0,cnt =0;
        char compName[MAX_PARAMETERNAME_LEN/2] = { '\0' };
        char dbusPath[MAX_PARAMETERNAME_LEN/2] = { '\0' };
        parameterValStruct_t **parameterval = NULL;
        char *getList[] = {DEVICE_MAC};
        componentStruct_t **        ppComponents = NULL;
        char dst_pathname_cr[256] = {0};
        token_t  token;
        int fd = s_sysevent_connect(&token);
        char deviceMACValue[32] = { '\0' };
        char isEthEnabled[64]={'\0'};

        if(CCSP_SUCCESS == check_ethernet_wan_status() && sysevent_get(fd, token, "eth_wan_mac", deviceMACValue, sizeof(deviceMACValue)) == 0 && deviceMACValue[0] != '\0')
        {
            macToLower(deviceMACValue);
            CcspTraceInfo(("deviceMAC is %s\n",deviceMAC));
        }
        else
        {

            sprintf(dst_pathname_cr, "%s%s", "eRT.", CCSP_DBUS_INTERFACE_CR);
            ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle, dst_pathname_cr, DEVICE_MAC, "", &ppComponents, &size);
            if ( ret == CCSP_SUCCESS && size >= 1)
            {
                strncpy(compName, ppComponents[0]->componentName, sizeof(compName)-1);
                strncpy(dbusPath, ppComponents[0]->dbusPath, sizeof(compName)-1);
            }
            else
            {
                CcspTraceError(("Failed to get component for %s ret: %d\n",DEVICE_MAC,ret));
            }
            free_componentStruct_t(bus_handle, size, ppComponents);

            if(strlen(compName) != 0 && strlen(dbusPath) != 0)
            {
                ret = CcspBaseIf_getParameterValues(bus_handle,
                        compName, dbusPath,
                        getList,
                        1, &val_size, &parameterval);

                if(ret == CCSP_SUCCESS)
                {
                    for (cnt = 0; cnt < val_size; cnt++)
                    {
                        CcspTraceDebug(("parameterval[%d]->parameterName : %s\n",cnt,parameterval[cnt]->parameterName));
                        CcspTraceDebug(("parameterval[%d]->parameterValue : %s\n",cnt,parameterval[cnt]->parameterValue));
                        CcspTraceDebug(("parameterval[%d]->type :%d\n",cnt,parameterval[cnt]->type));
                    }
                    macToLower(parameterval[0]->parameterValue);  
                }
                else
                {
                    CcspTraceError(("Failed to get values for %s ret: %d\n",getList[0],ret));
                }
                free_parameterValStruct_t(bus_handle, val_size, parameterval);
            }
        }   
    }
}

static void macToLower(char macValue[])
{
    int i = 0;
    int j;
    char *token[32]={'\0'};
    char tmp[32]={'\0'};
    strncpy(tmp, macValue,sizeof(tmp)-1);
    token[i] = strtok(tmp, ":");
    if(token[i]!=NULL)
    {
        strncat(deviceMAC, token[i],sizeof(deviceMAC)-1);
        deviceMAC[31]='\0';
        i++;
    }
    while ((token[i] = strtok(NULL, ":")) != NULL) 
    {
        strncat(deviceMAC, token[i],sizeof(deviceMAC)-1);
        deviceMAC[31]='\0';
        i++;
    }
    deviceMAC[31]='\0';
    for(j = 0; deviceMAC[j]; j++)
    {
        deviceMAC[j] = tolower(deviceMAC[j]);
    }
}

void* connect_parodus()
{
    int backoffRetryTime = 0;
    int backoff_max_time = 5;
    int max_retry_sleep;
    int c =2;   //Retry Backoff count shall start at c=2 & calculate 2^c - 1.
    int retval=-1;
    char *parodus_url = NULL;
    
    pthread_detach(pthread_self());
    
    max_retry_sleep = (int) pow(2, backoff_max_time) -1;
    CcspTraceInfo(("max_retry_sleep is %d\n", max_retry_sleep ));
    
    get_parodus_url(&parodus_url);
    CcspTraceDebug(("parodus_url is %s\n", parodus_url ));
    
    libpd_cfg_t cfg = { .service_name = "pam",
                        .receive = false, 
                        .keepalive_timeout_secs = 0,
                        .parodus_url = parodus_url,
                        .client_url = NULL
                      };
                      
    CcspTraceInfo(("Configurations => service_name : %s parodus_url : %s client_url : %s\n", cfg.service_name, cfg.parodus_url, cfg.client_url ));

    while(1)
    {
        if(backoffRetryTime < max_retry_sleep)
        {
            backoffRetryTime = (int) pow(2, c) -1;
        }

        CcspTraceDebug(("New backoffRetryTime value calculated as %d seconds\n", backoffRetryTime));
        int ret = libparodus_init (&pam_instance, &cfg);

        if(ret ==0)
        {
            CcspTraceInfo(("Init for parodus Success..!!\n"));
	    printf("Init for PARODUS success!!\n");
            break;
        }
        else
        {
            CcspTraceError(("Init for parodus (url %s) failed: '%s'\n", parodus_url, libparodus_strerror(ret)));
            if( NULL == parodus_url ) {
                get_parodus_url(&parodus_url);
                cfg.parodus_url = parodus_url;
            }
            sleep(backoffRetryTime);
            c++;

	    if(backoffRetryTime == max_retry_sleep)
	    {
		c = 2;
		backoffRetryTime = 0;
		CcspTraceDebug(("backoffRetryTime reached max value, reseting to initial value\n"));
	    }
        }
	retval = libparodus_shutdown(&pam_instance);
        
    }
}

void* sendNotification(void* pValue)
{
    wrp_msg_t *notif_wrp_msg = NULL;
    int retry_count = 0;
    int sendStatus = -1;
    int backoffRetryTime = 0;  
    int c=2;
    char source[MAX_PARAMETERNAME_LEN/2] = {'\0'};
    cJSON *notifyPayload = cJSON_CreateObject();
    char  * stringifiedNotifyPayload;
    unsigned long bootTime;

    char buff[64] = {0};
    char dest[512] = {'\0'};
    
    pthread_detach(pthread_self());

    memset(buff,0,sizeof(buff));
    if(pValue)
    {
	strcpy(buff,pValue);
	free(pValue);		
    }

    getDeviceMac();
    CcspTraceDebug(("buf: %s \n",buff));
    CcspTraceDebug(("deviceMAC: %s\n",deviceMAC));
    snprintf(source, sizeof(source), "mac:%s", deviceMAC);

    bootTime = CosaDmlDiGetBootTime(NULL);

    if(notifyPayload != NULL)
    {
        cJSON_AddStringToObject(notifyPayload,"device_id", source);
        cJSON_AddStringToObject(notifyPayload,"status", "reboot-pending");
        cJSON_AddNumberToObject(notifyPayload,"delay", atoi(buff));
        cJSON_AddNumberToObject(notifyPayload,"boot-time", bootTime);
        stringifiedNotifyPayload = cJSON_PrintUnformatted(notifyPayload);
        CcspTraceInfo(("Notification payload %s\n",stringifiedNotifyPayload));
        cJSON_Delete(notifyPayload);
    }
    
    notif_wrp_msg = (wrp_msg_t *)malloc(sizeof(wrp_msg_t));
    memset(notif_wrp_msg, 0, sizeof(wrp_msg_t));
    if(notif_wrp_msg != NULL)
    {
        notif_wrp_msg ->msg_type = WRP_MSG_TYPE__EVENT;
        notif_wrp_msg ->u.event.source = strdup(source);
        CcspTraceDebug(("source: %s\n",notif_wrp_msg ->u.event.source));

        snprintf(dest,sizeof(dest),"event:device-status/mac:%s/reboot-pending/%d/%s",deviceMAC,bootTime, buff);
        notif_wrp_msg ->u.event.dest = strdup(dest);
        CcspTraceDebug(("destination: %s\n", notif_wrp_msg ->u.event.dest));
        notif_wrp_msg->u.event.content_type = strdup(CONTENT_TYPE_JSON);
        CcspTraceDebug(("content_type is %s\n",notif_wrp_msg->u.event.content_type));
        if(stringifiedNotifyPayload != NULL)
        {
            notif_wrp_msg ->u.event.payload = (void *) stringifiedNotifyPayload;
            notif_wrp_msg ->u.event.payload_size = strlen(stringifiedNotifyPayload);
        }

        while(retry_count<=3)
        {
            backoffRetryTime = (int) pow(2, c) -1;

            sendStatus = libparodus_send(pam_instance, notif_wrp_msg );

            if(sendStatus == 0)
            {
                retry_count = 0;
                CcspTraceInfo(("Notification successfully sent to parodus\n"));
                break;
            }
            else
            {
                CcspTraceError(("Failed to send Notification: '%s', retrying ....\n",libparodus_strerror(sendStatus)));
                CcspTraceDebug(("sendNotification backoffRetryTime %d seconds\n", backoffRetryTime));
                sleep(backoffRetryTime);
                c++;
                retry_count++;
             }
       }
       CcspTraceDebug(("sendStatus is %d\n",sendStatus));
       wrp_free_struct (notif_wrp_msg );
   }
   //free(stringifiedNotifyPayload);
}

void initparodusTask()
{
	int err = 0;
	pthread_t parodusThreadId;
	
	err = pthread_create(&parodusThreadId, NULL, connect_parodus, NULL);
	if (err != 0) 
	{
		CcspTraceError(("Error creating messages thread :[%s]\n", strerror(err)));
	}
	else
	{
		CcspTraceInfo(("connect_parodus thread created Successfully\n"));
	}
}

void Send_Notification_Task(char* value)
{
	int err = 0;
	pthread_t NotificationThreadId;

	char* buff = (char*) malloc(strlen(value)+1);
	memset(buff,0,strlen(value)+1);
	strcpy(buff,value);
	
	err = pthread_create(&NotificationThreadId, NULL, sendNotification, buff);
	if (err != 0) 
	{
		CcspTraceError(("Error creating Notification thread :[%s]\n", strerror(err)));
	}
	else
	{
		CcspTraceInfo(("Notification thread created Successfully\n"));
	}
}
