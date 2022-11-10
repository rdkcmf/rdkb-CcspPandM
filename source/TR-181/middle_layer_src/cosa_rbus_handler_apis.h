/*
 * If not stated otherwise in this file or this component's Licenses.txt file
 * the following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
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

#ifndef  RBUS_HANDLER_APIS_H
#define  RBUS_HANDLER_APIS_H



#include <stdbool.h>
#include <rbus.h>
#include <pthread.h>

#define NUM_OF_RBUS_PARAMS	1
//#define RBUS_COMPONENT_NAME	"RbusPandMSsp"
#define RBUS_COMPONENT_NAME	"CcspPandMSsp"

#define DEVCTRL_NET_MODE_TR181	"Device.X_RDKCENTRAL-COM_DeviceControl.DeviceNetworkingMode"
#define  ARRAY_SZ(x) (sizeof(x) / sizeof((x)[0]))
#if defined  (WAN_FAILOVER_SUPPORTED) || defined(RDKB_EXTENDER_ENABLED)
typedef struct 
_DeviceControl_Net_Mode_
{
    ULONG DevCtrlNetMode;
	
} DeviceControl_Net_Mode;

rbusError_t getUlongHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);
rbusError_t setUlongHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts);

rbusError_t eventDevctrlSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char* eventName, rbusFilter_t filter, int32_t interval, bool* autoPublish);

bool initNetMode();

rbusError_t sendUlongUpdateEvent(char* event_name , uint32_t eventNewData, uint32_t eventOldData);

rbusError_t publishDevCtrlNetMode(uint32_t new_val, uint32_t old_val);

bool PAM_Rbus_SyseventInit();

#endif
#if defined  (WAN_FAILOVER_SUPPORTED) || defined(RDKB_EXTENDER_ENABLED) ||  defined(RBUS_BUILD_FLAG_ENABLE) || defined (_HUB4_PRODUCT_REQ_) || defined (_PLATFORM_RASPBERRYPI_)
rbusError_t devCtrlRbusInit();
#endif
#endif
