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
#include <syscfg/syscfg.h>
#include <math.h>
#include "cosa_deviceinfo_apis.h"
#include "utapi/utapi.h"
#include "utapi/utapi_util.h"

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

pthread_mutex_t device_mac_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct _notify_params
{
	char * delay;
	char * time;
	char * status;
	char * download_status;
	char * system_ready_time;
	char * priority;
	char * current_fw_ver;
	char * download_fw_ver;
} notify_params_t;

void* connect_parodus(void* arg);
static void getDeviceMac();
static void get_parodus_url(char **url);
void initparodusTask();
void Send_Notification_Task(char* delay, char* startTime, char* download_status, char* status, char *system_ready_time, char * priority,  char *current_fw_ver, char *download_fw_ver);
void* sendNotification(void* buff);
static void waitForEthAgentComponentReady();
static void checkComponentHealthStatus(char * compName, char * dbusPath, char *status, int *retStatus);
static int check_ethernet_wan_status();
char *get_firmware_download_start_time();
void set_firmware_download_start_time(char *start_time);
void free_notify_params_struct(notify_params_t *param);
int readFromFile(char *filename, char **data, int *len);
int writeToFile(char *filename, char *data, int len);
void getCurrentTime(struct timespec *timer);
long timeValDiff(struct timespec *starttime, struct timespec *finishtime);

static char deviceMAC[32]={'\0'};
libpd_instance_t pam_instance;
extern ANSC_HANDLE bus_handle;
static char *FW_download_start_time = "0";

static void get_parodus_url(char **url)
{
    FILE *fp = fopen(DEVICE_PROPS_FILE, "r");

    if( NULL != fp ) {
        char str[255] = {'\0'};
        while( fscanf(fp,"%s", str) != EOF) {
            char *value = NULL;
            if( ( value = strstr(str, "PARODUS_URL=") ) ) {
                value = value + strlen("PARODUS_URL=");
                *url = strdup(value);
                CcspTraceDebug(("parodus url is %s\n", *url));
            }
        }
        /* CID: 59956 Dereference after null check*/
        fclose(fp);
    } else {
        CcspTraceError(("Failed to open device.properties file:%s\n", DEVICE_PROPS_FILE));
    }

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
    int retryCount = 0;
    while(!strlen(deviceMAC))
    {
	pthread_mutex_lock(&device_mac_mutex);
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

	if (strlen(deviceMAC))
	{
	        pthread_mutex_unlock(&device_mac_mutex);
	        break;
	}

	if(CCSP_SUCCESS == check_ethernet_wan_status() && sysevent_get(fd, token, "eth_wan_mac", deviceMACValue, sizeof(deviceMACValue)) == 0 && deviceMACValue[0] != '\0')
	{
                AnscMacToLower(deviceMAC, deviceMACValue, sizeof(deviceMAC));
	        retryCount = 0;
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
	                retryCount++;
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
                            AnscMacToLower(deviceMAC, parameterval[0]->parameterValue, sizeof(deviceMAC));
                            retryCount = 0;
                        }
	                else
	                {
                                CcspTraceError(("Failed to get values for %s ret: %d\n",getList[0],ret));
                                retryCount++;
	                }
	                free_parameterValStruct_t(bus_handle, val_size, parameterval);
	        }
	}

        if(retryCount == 0)
        {
                CcspTraceInfo(("deviceMAC is %s\n",deviceMAC));
          	pthread_mutex_unlock(&device_mac_mutex);
                break;
        }
        else
        {
                if(retryCount > 5 )
		{
			CcspTraceError(("Unable to get CM Mac after %d retry attempts..\n", retryCount));
                  	pthread_mutex_unlock(&device_mac_mutex);
			break;
		}
		else
		{
			CcspTraceError(("Failed to GetValue for MAC. Retrying...retryCount %d\n", retryCount));
                  	pthread_mutex_unlock(&device_mac_mutex);
                        sleep(10);
		}
        }
    }
}

void* connect_parodus(void* arg)
{
    int backoffRetryTime = 0;
    int backoff_max_time = 5;
    int max_retry_sleep;
    int c =2;   //Retry Backoff count shall start at c=2 & calculate 2^c - 1.
    char *parodus_url = NULL;
    UNREFERENCED_PARAMETER(arg);    
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
	libparodus_shutdown(&pam_instance);
        
    }
    return NULL;
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
    char  * stringifiedNotifyPayload = NULL;
    unsigned long bootTime = 0;
    char lastRebootReason[64]={'\0'};
    notify_params_t *msg = NULL;
    char * temp = NULL;
    char dest[512] = {'\0'};
    
    pthread_detach(pthread_self());

    if(pValue)
    {
		msg = (notify_params_t *) pValue;
    }

    getDeviceMac();
    if(strlen(deviceMAC) == 0)
    {
		CcspTraceError(("deviceMAC is NULL, failed to send Notification\n"));
    }
    else
    {
		CcspTraceDebug(("deviceMAC: %s\n",deviceMAC));
		snprintf(source, sizeof(source), "mac:%s", deviceMAC);

		if(notifyPayload != NULL)
		{
			cJSON_AddStringToObject(notifyPayload,"device_id", source);
			if(msg)
			{
				if(msg->status !=NULL)
				{
					cJSON_AddStringToObject(notifyPayload,"status", msg->status);
				}
				if(msg->time !=NULL)
				{
					cJSON_AddStringToObject(notifyPayload,"start-time", msg->time);
				}
				if(msg->download_status !=NULL)
				{
					if(strcmp(msg->download_status, "true") == 0)
					{
						cJSON_AddStringToObject(notifyPayload,"download-status", "success");
					}
					else
					{
						cJSON_AddStringToObject(notifyPayload,"download-status", "failure");
					}
				}

				if ((msg->status !=NULL) && (strcmp(msg->status, "reboot-pending") == 0))
				{
					bootTime = CosaDmlDiGetBootTime(NULL);
					cJSON_AddNumberToObject(notifyPayload,"boot-time", bootTime);
					syscfg_get( NULL, "X_RDKCENTRAL-COM_LastRebootReason", lastRebootReason, sizeof(lastRebootReason));
					CcspTraceDebug(("lastRebootReason is %s\n", lastRebootReason));
                                        /*CID: 57243 Array compared against 0*/
					cJSON_AddStringToObject(notifyPayload,"reboot-reason", lastRebootReason);

					if(msg->delay !=NULL)
					{
						cJSON_AddNumberToObject(notifyPayload,"delay", atoi(msg->delay));
					}
				}

				if ((msg->status !=NULL) && (strcmp(msg->status, "fully-manageable") == 0) && (msg->system_ready_time != NULL))
				{
					bootTime = CosaDmlDiGetBootTime(NULL);
					cJSON_AddNumberToObject(notifyPayload,"boot-time", bootTime);
					cJSON_AddNumberToObject(notifyPayload,"system-ready-time", atoi(msg->system_ready_time));
				}
				if(msg->priority !=NULL)
				{
					cJSON_AddStringToObject(notifyPayload,"priority", msg->priority);
				}
				if(msg->current_fw_ver !=NULL)
				{
					cJSON_AddStringToObject(notifyPayload,"current_fw_ver", msg->current_fw_ver);
				}
				if(msg->download_fw_ver !=NULL)
				{
					cJSON_AddStringToObject(notifyPayload,"download_fw_ver", msg->download_fw_ver);
				}
			}
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

			if(msg)
			{
				if((msg->status != NULL) && (strlen(msg->status) != 0))
				{
					snprintf(dest,sizeof(dest),"event:device-status/mac:%s/%s",deviceMAC, msg->status);

					/* add remaining fields based on status of each events.
					   event:device-status/{device_id}/reboot-pending/{boot-time}/{delay} */
					if (strcmp(msg->status, "reboot-pending") == 0)
					{
						temp = strdup(dest);

						snprintf(dest,sizeof(dest),"%s/%lu/%ss",temp, bootTime, (((msg->delay != NULL) && (strlen(msg->delay) != 0)) ? msg->delay : "unknown"));
						CcspTraceDebug(("dest framed for reboot-pending %s\n", dest));
						free(temp);
					}

					if (strcmp(msg->status, "fully-manageable") == 0)
					{
						temp = strdup(dest);
						snprintf(dest,sizeof(dest),"%s/%s",temp, (((msg->system_ready_time != NULL) && (strlen(msg->system_ready_time) != 0)) ? msg->system_ready_time : "0"));
						CcspTraceDebug(("dest framed for fully-manageable %s\n", dest));
						free(temp);
					}
				}
				else
				{
					CcspTraceError(("Unable to frame dest as status field is empty\n"));
				}
			}
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
			if(msg != NULL)
			{
				free_notify_params_struct(msg);
			}
	   }
	}
    return NULL;
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

void Send_Notification_Task(char* delay, char* startTime, char* download_status, char* status, char* system_ready_time, char * priority,  char *current_fw_ver, char *download_fw_ver)
{
	int err = 0;
	pthread_t NotificationThreadId;

	notify_params_t *args = NULL;

	args = (notify_params_t *)malloc(sizeof(notify_params_t));
	if(args != NULL)
	{
		memset(args, 0, sizeof(notify_params_t));

		if(delay != NULL)
		{
			args->delay = strdup(delay);
		}
		if(startTime != NULL)
		{
			args->time = strdup(startTime);
		}
		if(download_status != NULL)
		{
			args->download_status = strdup(download_status);
		}
		if(status != NULL)
		{
			args->status = strdup(status);
		}
		if(system_ready_time != NULL)
		{
		    args->system_ready_time = strdup(system_ready_time);
		}
		if(priority != NULL)
		{
			args->priority = strdup(priority);
		}
		if(current_fw_ver != NULL)
		{
			args->current_fw_ver = strdup(current_fw_ver);
		}
		if(download_fw_ver!= NULL)
		{
		    args->download_fw_ver = strdup(download_fw_ver);
		}
		err = pthread_create(&NotificationThreadId, NULL, sendNotification, (void *) args);
		if (err != 0)
		{
			CcspTraceError(("Error creating Notification thread :[%s]\n", strerror(err)));
		}
		else
		{
			CcspTraceInfo(("Notification thread created Successfully\n"));
		}
	}
}

void set_firmware_download_start_time(char *start_time)
{
    FW_download_start_time = start_time;
}

char *get_firmware_download_start_time()
{
    return FW_download_start_time;
}

void free_notify_params_struct(notify_params_t *param)
{
    if(param != NULL)
    {
        if(param->delay != NULL)
        {
            free(param->delay);
            param->delay = NULL;
        }
        if(param->time != NULL)
        {
            free(param->time);
            param->time = NULL;
        }
        if(param->status != NULL)
        {
            free(param->status);
            param->status = NULL;
        }
        if(param->download_status != NULL)
        {
            free(param->download_status);
            param->download_status = NULL;
        }
        if(param->system_ready_time != NULL)
        {
            free(param->system_ready_time);
            param->system_ready_time = NULL;
        }
	if(param->priority != NULL)
        {
            free(param->priority);
            param->priority = NULL;
        }
        if(param->current_fw_ver != NULL)
        {
            free(param->current_fw_ver);
            param->current_fw_ver = NULL;
        }
        if(param->download_fw_ver != NULL)
        {
            free(param->download_fw_ver);
            param->download_fw_ver = NULL;
        }
        free(param);
        param = NULL;
    }
}

int readFromFile(char *filename, char **data, int *len)
{
	FILE *fp;
	int ch_count = 0;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("Failed to open file %s\n", filename);
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	ch_count = ftell(fp);
        /* CID: 119907 Argument cannot be negative*/
        if (ch_count < 0) {
            printf("Negative return file\n");
            fclose(fp);
            return 0;
        }
	fseek(fp, 0, SEEK_SET);
	*data = (char *) malloc(sizeof(char) * (ch_count + 1));
	
	fread(*data, 1, ch_count,fp);
        //fgets(*data,400,fp);
        printf("........data is %s len%zu\n", *data, strlen(*data));
	*len = ch_count;
	(*data)[ch_count] ='\0';
        printf("character count is %d\n",ch_count);
        printf("data is %s len %zu\n", *data, strlen(*data));
	fclose(fp);
	return 1;
}

int writeToFile(char *filename, char *data, int len)
{
	FILE *fp;
	fp = fopen(filename , "w+");
	if (fp == NULL)
	{
		CcspTraceInfo(("Failed to open file %s\n", filename ));
		return 0;
	}
	if(data !=NULL)
	{
		fwrite(data, len, 1, fp);
		fclose(fp);
		return 1;
	}
	else
	{
		CcspTraceInfo(("WriteToFile failed, Data is NULL\n"));
                /*CID: 143642 Resource leak*/
                fclose(fp);
		return 0;
	}
}

void getCurrentTime(struct timespec *timer)
{
	clock_gettime(CLOCK_REALTIME, timer);
}

long timeValDiff(struct timespec *starttime, struct timespec *finishtime)
{
	long msec;
	msec=(finishtime->tv_sec-starttime->tv_sec)*1000;
	msec+=(finishtime->tv_nsec-starttime->tv_nsec)/1000000;
	return msec;
}
