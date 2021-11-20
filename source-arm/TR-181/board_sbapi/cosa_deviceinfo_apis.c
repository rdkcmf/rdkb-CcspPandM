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

    module: cosa_deviceinfo_api.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlDiInit
        *  CosaDmlDiGetManufacturer
        *  CosaDmlDiGetManufacturerOUI
        *  CosaDmlDiGetModelName
        *  CosaDmlDiGetDescription
        *  CosaDmlDiGetProductClass
        *  CosaDmlDiGetSerialNumber
        *  CosaDmlDiGetHardwareVersion
        *  CosaDmlDiGetSoftwareVersion
        *  CosaDmlDiGetAdditionalHardwareVersion
        *  CosaDmlDiGetAdditionalSoftwareVersion
        *  CosaDmlDiGetProvisioningCode
        *  CosaDmlDiSetProvisioningCode
        *  CosaDmlDiGetFirstUseDate
        *  CosaDmlDiGetUpTime
        *  CosaDmlGetMaxTCPWindowSize
        *  CosaDmlGetTCPImplementation
        *  CosaProcStatusCreate
        *  COSADmlRemoveProcessInfo
        *  COSADmlGetProcessInfo
        *  COSADmlGetCpuUsage
        *  COSADmlGetMemoryStatus
        *  CosaDmlDiGetFirmwareUpgradeStartTime
        *  CosaDmlDiGetFirmwareUpgradeEndTime
        *  CosaDmlDiSetFirmwareUpgradeStartTime
        *  CosaDmlDiSetFirmwareUpgradeEndTime
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
#define _GNU_SOURCE
#include <string.h>
#include <syscfg/syscfg.h>
#include "cosa_deviceinfo_apis.h"
#include "cosa_deviceinfo_apis_custom.h"
#include "dml_tr181_custom_cfg.h" 
#include "cosa_x_cisco_com_devicecontrol_apis.h"
#include "cosa_deviceinfo_internal.h"
#include "cosa_drg_common.h"
#include <syscfg/syscfg.h>
#include "safec_lib_common.h"
#include "ansc_string_util.h"
#define DEVICE_PROPERTIES    "/etc/device.properties"
#define PARTNERS_INFO_FILE              "/nvram/partners_defaults.json"
#define BOOTSTRAP_INFO_FILE		"/nvram/bootstrap.json"
#define RFC_DEFAULTS_FILE       "/etc/rfcDefaults.json"
#define RFC_STORE_FILE       "/opt/secure/RFC/tr181store.json"
#define CUSTOM_DATA_MODEL_ENABLED "custom_data_model_enabled"
#define SYSTEMD "systemd"
#define MAX_TIME_FORMAT     5

#define MAX_PROCESS_NUMBER 300

static int writeToJson(char *data, char *file);

#if defined(_PLATFORM_IPQ_)
#include "ccsp_vendor.h"
#endif

#ifdef _PLATFORM_RASPBERRYPI_
#include "ccsp_vendor.h"
#endif

#ifdef _PLATFORM_TURRIS_
#include "ccsp_vendor.h"
#endif

#ifdef _COSA_SIM_

// this file is in integration_src.intel_usg_arm directory

#elif (_COSA_INTEL_USG_ARM_ || _PLATFORM_IPQ_ || _COSA_BCM_MIPS_)

#include "ccsp_psm_helper.h"            // for PSM_Get_Record_Value2
#include "dmsb_tr181_psm_definitions.h" // for DMSB_TR181_PSM_DeviceInfo_Root/ProductClass

#if   _COSA_INTEL_USG_ARM_
//#include "libplat_flash.h"
#endif
extern  ANSC_HANDLE             bus_handle;
 
#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <time.h>
#include <utapi_util.h>
#include <unistd.h>

#include "platform_hal.h"
#include "autoconf.h"     
#include "secure_wrapper.h"

#define _ERROR_ "NOT SUPPORTED"
#define _START_TIME_12AM_ "0"
#define _END_TIME_3AM_ "10800"
#define _SSH_ERROR_ "NOT SET"



#define DMSB_TR181_PSM_WHIX_LogInterval                                 "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.LogInterval"
#define DMSB_TR181_PSM_WHIX_ChUtilityLogInterval                                 "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.ChUtilityLogInterval"
#define DMSB_TR181_PSM_WHIX_NormalizedRssiList                "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.NormalizedRssiList"
#define DMSB_TR181_PSM_WHIX_CliStatList                                    "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.CliStatList"
#define DMSB_TR181_PSM_WHIX_TxRxRateList                              "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.TxRxRateList"
#define DMSB_TR181_PSM_WIFI_TELEMETRY_SNRList                 "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WIFI_TELEMETRY.SNRList"

#ifdef CISCO_XB3_PLATFORM_CHANGES
#undef CONFIG_VENDOR_NAME
#define CONFIG_VENDOR_NAME "Cisco"
#endif

/* Localhost port range for stunnel client to listen/accept */
#define MIN_PORT_RANGE 3000
#define MAX_PORT_RANGE 3020

extern void* g_pDslhDmlAgent;

static const int OK = 1 ;
static const int NOK = 0 ;
static char reverseSSHArgs[256];
struct stunnelSSHArgs{
        int localport;
        int stunnelport;
        char host[512];
        char hostIp[512];
}stunnelsshargs;
#define sshCommand "/lib/rdk/startTunnel.sh"
#ifdef ENABLE_SHORTS
#define stunnelCommand "/lib/rdk/startStunnel.sh"
#endif
static const char* rsshPidFile = "/var/tmp/rssh.pid";

void strip_line (char *str)
{
    if (NULL==str)
        return;
    int len = strlen(str);
    str[len-1] = 0;    
}

static int
PsmGet(const char *param, char *value, int size)
{
    char *val = NULL;

    if (PSM_Get_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, NULL, &val) != CCSP_SUCCESS)
        return -1;
    
    if(val) {
        snprintf(value, size, "%s", val);
        ((CCSP_MESSAGE_BUS_INFO *)g_MessageBusHandle)->freefunc(val);
    }
    else return -1;

    return 0;
}

/**
 * Form dropbear equivalent options from input arguments accepted by TR-69/181
 */
static char* mapArgsToSSHOption(char *revSSHConfig,bool shortsFlag) {

	char* value = NULL;
	char* option = NULL;
	errno_t rc = -1;

        if (!revSSHConfig)
            return NULL;

        option = (char*) calloc(125, sizeof(char));

	if (option) {
		if ((value = strstr(revSSHConfig, "idletimeout="))) {
			rc = sprintf_s(option, 125, " -I %s -f -N -y -T",
					value + strlen("idletimeout="));
			if(rc < EOK)
			{
				ERR_CHK(rc);
			}
                }else if ((value = strstr(revSSHConfig, "sshport=")) && !(value =
                           strstr(revSSHConfig, "revsshport="))) {
                  value = strstr(revSSHConfig, "sshport=");
                        if(shortsFlag)
                        {
                                rc=sprintf_s(option, 125, " -p %d",stunnelsshargs.localport);
                                if(rc < EOK)
                                    ERR_CHK(rc);
                        }
                        else{
                                rc = sprintf_s(option, 125, " -p %s", value + strlen("sshport="));
                                if(rc < EOK)
                                {
                                      ERR_CHK(rc);
                                }
                        }
		} else if ((value = strstr(revSSHConfig, "revsshport="))) {
			rc = sprintf_s(option, 125, " -R %s:[CM_IP]:22", value + strlen("revsshport="));
			if(rc < EOK)
			{
				ERR_CHK(rc);
			}
		} else {
                        // Sanity check do not include unrecognised options
			free(option);
			option = NULL;
		}
	}

	return option;
}

/*
 * Returns string until the first occurrence of delimiter ';' is found.
 */
static char* findUntilFirstDelimiter(char* input) {

        char tempCopy[512] = { 0 };
	char *tempStr = NULL;
	char* option = NULL;
    unsigned int inputMsgSize = 0;
    char *st = NULL;
    errno_t rc = -1;

        if (!input)
            return NULL;

        option = (char*) calloc(125, sizeof(char));
	inputMsgSize = strlen(input);
    if (sizeof(tempCopy) > inputMsgSize)
    {
        strncpy(tempCopy, input, inputMsgSize);
        tempStr = (char*) strtok_r(tempCopy, ";", &st);
        if (option)
        {
            rc = strcpy_s(option, 125, (tempStr ? tempStr : input));
            ERR_CHK(rc);
        }
    }
	return option;
}

/**
 * Get login username/target for jump server
 */
static char* getHostLogin(char *tempStr,bool shortsFlag) {
	char* value = NULL;
    char* temp = NULL;
	char* hostIp = NULL;
	char* user = NULL;
	char* hostLogin = NULL;
	unsigned int inputMsgSize = 0;
        char tempCopy[512] = { 0 };
	errno_t rc = -1;

        if (!tempStr)
          return NULL;
        inputMsgSize = strlen(tempStr);
        if ( sizeof(tempCopy) > inputMsgSize)
        {
          strncpy(tempCopy, tempStr, inputMsgSize);
        }
        else
        {
          return NULL;
        }
        if ((value = strstr(tempStr, "hostIp=")) && shortsFlag){
                sprintf_s(stunnelsshargs.hostIp, 512, value + strlen("hostIp="));
                CcspTraceInfo(("%s Host: %s \n",__FUNCTION__,stunnelsshargs.hostIp));
                if(stunnelsshargs.hostIp){
                          temp = findUntilFirstDelimiter(stunnelsshargs.hostIp);
                          rc=strcpy_s(stunnelsshargs.hostIp,sizeof(stunnelsshargs.hostIp),temp);
                          free(temp);
                          CcspTraceInfo(("%s HostIp extracted: %s \n",__FUNCTION__,stunnelsshargs.hostIp));
                }
         }
         if ((value = strstr(tempStr, "host="))) {
                 if (shortsFlag) {
                        int host_len=strlen("localhost");
                        hostIp = (char*) calloc(host_len+1,sizeof(char));
                        sprintf_s(hostIp,host_len+1, "%s", "localhost");
                        sprintf_s(stunnelsshargs.host, 512, value + strlen("host="));
                        CcspTraceInfo(("%s Host: %s \n",__FUNCTION__,stunnelsshargs.host));
                        if(stunnelsshargs.host){
                          temp = findUntilFirstDelimiter(stunnelsshargs.host);
                          rc=strcpy_s(stunnelsshargs.host,sizeof(stunnelsshargs.host),temp);
                          free(temp);
                          CcspTraceInfo(("%s Host extracted: %s \n",__FUNCTION__,stunnelsshargs.host));
                        }
                 }
                 else {
                        hostIp = (char*) calloc(256, sizeof(char));
                        sprintf_s(hostIp,256, "%s", value + strlen("host="));
                        CcspTraceInfo(("%s Host: %s \n",__FUNCTION__,hostIp));
                 }
        }
	if ((value = strstr(tempStr, "user="))) {
		user = (char*) calloc(125, sizeof(char));
		snprintf(user, 125, "%s", value + strlen("user="));
	}

	if (user && hostIp) 
    {
        temp = findUntilFirstDelimiter(user);
        if (user)
        {
            free(user);
            user = NULL;
        }
        user = temp;
        CcspTraceInfo(("%s User extracted: %s \n",__FUNCTION__,user));
        temp = findUntilFirstDelimiter(hostIp);
        if (hostIp)
        {
            free(hostIp);
            hostIp = NULL;
        }
        hostIp = temp;
        CcspTraceInfo(("%s Host extracted: %s \n",__FUNCTION__,hostIp));
                hostLogin = (char*) calloc(255, sizeof(char));
                if (hostLogin) {
                        rc = sprintf_s(hostLogin, 255, " %s@%s", user, hostIp);
                        if(rc < EOK)
                        {
                             ERR_CHK(rc);
                        }
                }
                CcspTraceInfo(("%s Host Login value: %s \n",__FUNCTION__,hostLogin));
        }

	if (user)
		free(user);

	if (hostIp)
		free(hostIp);

	return hostLogin;
}

ANSC_STATUS
CosaDmlDiInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    if ( platform_hal_PandMDBInit() != RETURN_OK)
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetManufacturer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t rc = strcpy_s(pValue, *pulSize, CONFIG_VENDOR_NAME);
    if ( rc != EOK) {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    *pulSize = AnscSizeOfString(pValue)+1;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetManufacturerOUI
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t rc = -1;

#if defined(_COSA_BCM_ARM_) || defined(_PLATFORM_TURRIS_)
        rc = sprintf_s(pValue, *pulSize, "%s%c", CONFIG_VENDOR_ID, '\0');
        if(rc < EOK)
        {
            ERR_CHK(rc);
            *pulSize = 0;
            return ANSC_STATUS_FAILURE;
        }
#else
        rc = sprintf_s(pValue, *pulSize, "%06X%c", CONFIG_VENDOR_ID, '\0');
        if(rc < EOK)
        {
            ERR_CHK(rc);
            *pulSize = 0;
            return ANSC_STATUS_FAILURE;
        }
#endif
        *pulSize = AnscSizeOfString(pValue)+1;
        return ANSC_STATUS_SUCCESS;

}
#if !defined(_COSA_BCM_MIPS_) && !defined (_ENABLE_DSL_SUPPORT_)
/*Changes for 6560*/
ANSC_STATUS
CosaDmlDiGetCMTSMac
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
   UNREFERENCED_PARAMETER(hContext);
   if ( platform_hal_getCMTSMac(pValue) != RETURN_OK){
        CcspTraceWarning(("Unable to fetch the CMTS MAC \n"));
        return ANSC_STATUS_FAILURE;
        }
    else {
            *pulSize = AnscSizeOfString(pValue); 
            return ANSC_STATUS_SUCCESS;

         }
}
/*Changes for 6560-end*/
#endif

ANSC_STATUS
CosaDmlDiGetModelName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext); 
#if defined _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_

    if ( platform_hal_GetModelName(pValue) != RETURN_OK)
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue); 
        return ANSC_STATUS_SUCCESS;
    }

#endif
}

ANSC_STATUS
CosaDmlDiGetDescription
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t rc = strcpy_s(pValue, *pulSize, CONFIG_TI_GW_DESCRIPTION);
    if ( rc != EOK) {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    *pulSize = AnscSizeOfString(pValue)+1;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetProductClass
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t                         rc              = -1;
/*    char val[64] = {0};
    char param_name[256] = {0};

    _ansc_sprintf(param_name, "%s%s", DMSB_TR181_PSM_DeviceInfo_Root, DMSB_TR181_PSM_DeviceInfo_ProductClass);        

    if (PsmGet(param_name, val, sizeof(val)) != 0) {
        pValue[0] = '\0';
        *pulSize = 0;
        return ANSC_STATUS_FAILURE;
    }
    else {
        AnscCopyString(pValue, val);
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
    }
*/
#if defined(_CBR_PRODUCT_REQ_)
	{
                rc = strcpy_s(pValue, *pulSize, "CBR");
                if ( rc != EOK) {
                    ERR_CHK(rc);
                    return ANSC_STATUS_FAILURE;
                }
	}
#elif defined(_XB8_PRODUCT_REQ_)
    {
        rc = strcpy_s(pValue, *pulSize, "XB8");
        if ( rc != EOK) {
            ERR_CHK(rc);
            return ANSC_STATUS_FAILURE;
        }
    }
#elif defined(_XB7_PRODUCT_REQ_)
    {
        rc = strcpy_s(pValue, *pulSize, "XB7");
        if ( rc != EOK) {
            ERR_CHK(rc);
            return ANSC_STATUS_FAILURE;
        }
    }
#elif defined(_XB6_PRODUCT_REQ_)
    {
        rc = strcpy_s(pValue, *pulSize, "XB6");
        if ( rc != EOK) {
            ERR_CHK(rc);
            return ANSC_STATUS_FAILURE;
        }
    }
#elif defined( _XF3_PRODUCT_REQ_)
	{
                rc = strcpy_s(pValue, *pulSize, "XF3");
                if ( rc != EOK) {
                    ERR_CHK(rc);
                    return ANSC_STATUS_FAILURE;
                }
	}
#elif defined( _SR300_PRODUCT_REQ_)
        {
                rc = strcpy_s(pValue, *pulSize, "HOMEHUBADA");
                if ( rc != EOK) {
                    ERR_CHK(rc);
                    return ANSC_STATUS_FAILURE;
                }
        }
#elif defined( _HUB4_PRODUCT_REQ_)
        {
                rc = strcpy_s(pValue, *pulSize, "HOMEHUB4");
                if ( rc != EOK) {
                    ERR_CHK(rc);
                    return ANSC_STATUS_FAILURE;
                }
        }
#else
	{
                rc = strcpy_s(pValue, *pulSize, "XB3");
                if ( rc != EOK) {
                    ERR_CHK(rc);
                    return ANSC_STATUS_FAILURE;
                }
	}
#endif

    *pulSize = AnscSizeOfString(pValue) +1;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetSerialNumber
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UCHAR unitsn[128];
    memset(unitsn,0,sizeof(unitsn));

#if   (_COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_)

    if (platform_hal_GetSerialNumber(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;

    /* Remove trailing newline from Serial Number retrieved */
    int len = strlen(pValue);
    if ((len > 0) && (pValue[len-1] == '\n'))
    {
        pValue[len-1] = '\0';
    }
#endif
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetHardwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_    

    if (platform_hal_GetHardwareVersion(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
    }

#endif  
}

ANSC_STATUS
CosaDmlDiGetSoftwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (platform_hal_GetSoftwareVersion(pValue, *pulSize) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
    }     
}

ANSC_STATUS
CosaDmlDiGetAdditionalHardwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    return CosaDmlDiGetHardwareVersion(hContext, pValue, pulSize);
}

ANSC_STATUS
CosaDmlDiGetAdditionalSoftwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    return CosaDmlDiGetSoftwareVersion(hContext,pValue, pulSize);
}

ANSC_STATUS
CosaDmlDiGetProvisioningCode
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if 0
    UtopiaContext ctx;
    int rc = -1;
    char temp[64];
    errno_t rc = -1;

    if (!Utopia_Init(&ctx))
        return ERR_UTCTX_INIT;

    rc = Utopia_Get_Prov_Code(&ctx,temp);

    Utopia_Free(&ctx,0);
    AnscCopyString(pValue,temp);
    *pulSize = AnscSizeOfString(pValue);
#endif

// Provisioning Code sent to ACS is Serial Number of the device
#ifdef _COSA_DRG_TPG_
    plat_GetFlashValue("unitsn", unitsn);
    rc = sprintf_s(pValue, *pulSize, "%c%c%c%c%c%c%c",unitsn[0],unitsn[1],unitsn[2],unitsn[3],unitsn[4],unitsn[5],unitsn[6]);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
#elif (_COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_)

    if (platform_hal_GetSerialNumber(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
#endif
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS; 
}

ANSC_STATUS
CosaDmlDiSetProvisioningCode
    (
        ANSC_HANDLE                 hContext,
        char*                       pProvisioningCode
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext ctx;
    int rc = -1;

    if (!Utopia_Init(&ctx))
        return ERR_UTCTX_INIT;

    rc = Utopia_Set_Prov_Code(&ctx,pProvisioningCode);

    Utopia_Free(&ctx,!rc);

    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlGiGetCustomDataModelEnabled
    (
        ANSC_HANDLE                 hContext,
        BOOL                        *pValue
    )
{
    char buf[2];
    memset(buf, 0, sizeof(buf));
    UNREFERENCED_PARAMETER(hContext);

    if(syscfg_init() == 0)
    {
        syscfg_get( NULL, CUSTOM_DATA_MODEL_ENABLED, buf, sizeof(buf));
    }
    *pValue = (strcmp(buf, "1") == 0);
    return ANSC_STATUS_SUCCESS;
}

static int IsSystemdRunning()
{
    FILE *f_name = fopen("/proc/1/status", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    char initProcess[10] = {0};
    int res = 0;

    if (f_name)
    {
        /* Only need to read the first line to get the process name. Ex. Name:  systemd */
        read = getline(&line, &len, f_name);
        if (read != 0 || read != -1)
        {
            if (line != NULL)
            {
                sscanf(line, "Name:\t%s\n", initProcess);
                if (strncmp(initProcess, SYSTEMD, 7) == 0)
                {
		            res = 1;
                }
            }
        }
        if(line)
            free(line);

        fclose(f_name);
    }
    return res;
}

ULONG
CosaDmlGiSetCustomDataModelEnabled
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if(syscfg_init() == 0)
    {
        syscfg_set(NULL, CUSTOM_DATA_MODEL_ENABLED, bValue ? "1" : "0");
    }

    if (IsSystemdRunning())
    {
        v_secure_system("(sleep 5 ; systemctl restart CcspTr069PaSsp) &");
    }
    else
    {
        v_secure_system("killall CcspTr069PaSsp");
        v_secure_system("cd /usr/ccsp/tr069pa; /usr/bin/CcspTr069PaSsp -subsys eRT. &");
    }

    return ANSC_STATUS_SUCCESS;
}


static void* uploadLogUtilityThread(void* vptr_value)
{
	pthread_detach(pthread_self());
	v_secure_system("/rdklogger/opsLogUpload.sh %s &", (char *) vptr_value);
	return vptr_value;
}

ANSC_STATUS
COSADmlUploadLogsNow
	(
		ANSC_HANDLE                 hContext,
        	BOOL                        bEnable
	)
{
    UNREFERENCED_PARAMETER(hContext);
	pthread_t tid;
	char* operation = NULL;

	if (bEnable)
	{
		operation = "upload";
		pthread_create(&tid, NULL, &uploadLogUtilityThread, (void *)operation);
        return ANSC_STATUS_SUCCESS;
	}
	else 
	{
       	operation = "stop";
		pthread_create(&tid, NULL, &uploadLogUtilityThread, (void *)operation);
    	return ANSC_STATUS_SUCCESS;
    }
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
COSADmlUploadLogsStatus
    (
        ANSC_HANDLE                 Context,
	char* 	pValue,
	ULONG*	pUlSize
    )
{
    UNREFERENCED_PARAMETER(Context);
	FILE *ptr_file;
	char buf[50];
        errno_t                         rc              = -1;

	ptr_file =fopen("/tmp/upload_log_status","r");

	if (ptr_file)
	{
		if (fgets(buf,50, ptr_file)!=NULL)
		{
			strip_line(buf);
                        rc = strcpy_s(pValue, *pUlSize, buf);
                        if ( rc != EOK) {
                            ERR_CHK(rc);
                            fclose(ptr_file);
                            return ANSC_STATUS_FAILURE;
                        }
			*pUlSize = AnscSizeOfString(pValue)+1;
		}
		fclose(ptr_file);
	}
	else
	{
                rc = strcpy_s(pValue, *pUlSize, "Not triggered");
                if ( rc != EOK) {
                    ERR_CHK(rc);
                    return ANSC_STATUS_FAILURE;
                }
		*pUlSize = AnscSizeOfString(pValue)+1;
	}
	
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetFirstUseDate
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext ctx;
    int rc = -1;
    char firstUseDate[64];
    errno_t safec_rc = -1;

    if (!Utopia_Init(&ctx))
        return ERR_UTCTX_INIT;

    rc = Utopia_Get_First_Use_Date(&ctx,firstUseDate);

    Utopia_Free(&ctx,0);

    //    fprintf(stderr, "<RT> rc=%d, First Use Date = '%s'\n", rc, firstUseDate);

    if(rc || firstUseDate[0] == '\0')
    {
        safec_rc = strcpy_s(firstUseDate, sizeof(firstUseDate), "2013-11-22T00:00:00");
        if(rc != EOK)
        {
            ERR_CHK(safec_rc);
            return ANSC_STATUS_FAILURE;
        }
    }

    rc = strcpy_s(pValue, *pulSize, firstUseDate);
    if ( rc != EOK) {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    *pulSize = AnscSizeOfString(pValue)+1;
    
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlDiGetUpTime
    (
        ANSC_HANDLE                 Context
    )
{
    UNREFERENCED_PARAMETER(Context);
    struct sysinfo s_info;

    if(sysinfo(&s_info))
    {
        return 0;
    }
    else
    {
        return s_info.uptime;
    }
}

ULONG
CosaDmlDiGetBootTime
    (
	ANSC_HANDLE                 Context
    )
{
    UNREFERENCED_PARAMETER(Context);
	struct sysinfo s_info;
	struct timeval currentTime;

	if(sysinfo(&s_info))
	{
		return 0;
	}
	int upTime = s_info.uptime;

	gettimeofday(&currentTime, NULL);

	return (currentTime.tv_sec - upTime);
}

ANSC_STATUS
CosaDmlDiGetBootloaderVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (platform_hal_GetBootloaderVersion(pValue, *pulSize) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlDiGetFirmwareName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (!pValue || !pulSize || *pulSize <= 64)
        return ANSC_STATUS_FAILURE;

    memset(pValue, 0, *pulSize);

    if (platform_hal_GetFirmwareName(pValue, *pulSize) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlDiGetFirmwareBuildTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    FILE *fp;
    char line[512];
    char* value_token;
    char* st = NULL;

    if ((fp = fopen("/version.txt", "rb")) == NULL)
        return ANSC_STATUS_FAILURE;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, "BUILD_TIME="))
	{
		value_token = strtok_r(line, "\"", &st);
		if (value_token != NULL) 
		{
			value_token = strtok_r(NULL, "\"", &st); 
			snprintf(pValue, *pulSize, "%s", value_token);
			*pulSize = AnscSizeOfString(pValue);
		
			fclose(fp);
			return ANSC_STATUS_SUCCESS;
		}
    	}
    }

    fclose(fp);
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDiGetBaseMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if ( platform_hal_GetBaseMacAddress(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlDiGetHardware
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if ( platform_hal_GetHardware(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue); 
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlDiGetHardware_MemUsed
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if ( platform_hal_GetHardware_MemUsed(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue); 
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlDiGetHardware_MemFree
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if ( platform_hal_GetHardware_MemFree(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue); 
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlGetTCPImplementation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize    
    )
{
    UNREFERENCED_PARAMETER(hContext);
    char value[25];
    FILE *fp;
    errno_t                         rc              = -1;

    rc = strcpy_s(pValue, *pulSize, _ERROR_);
    if ( rc != EOK) {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    memset(value,0,sizeof(value));

    fp = popen("cat /proc/sys/net/ipv4/tcp_congestion_control", "r");
    if (fp == NULL)
    {
        CcspTraceWarning(("ERROR '%s'\n","ERROR")); 
        return ANSC_STATUS_FAILURE;
    }
   
    while(fgets(value, sizeof(value), fp) != NULL)
    {
        rc = strcpy_s(pValue, *pulSize, value);
        if ( rc != EOK) {
            ERR_CHK(rc);
            pclose(fp);
            return ANSC_STATUS_FAILURE;
        }
    }

    pclose(fp);
    *pulSize = AnscSizeOfString(pValue)+1;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
isValidInput
    (
        char                       *inputparam,
        char                       *wrapped_inputparam,
    	int							lengthof_inputparam,
    	int							sizeof_wrapped_inputparam    	
    )
{
    ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;
    errno_t rc = -1;
    int i=0,count=0;
         /*
	  * Validate input/params 
	  * sizeof_wrapped_inputparam it should always greater that ( lengthof_inputparam  + 2 ) because
	  * we are adding 2 extra charecters here. so we need to have extra bytes 
	  * in copied(wrapped_inputparam) string
	  */      
    if( sizeof_wrapped_inputparam <= ( lengthof_inputparam  + 2 ) )
    {
        return ANSC_STATUS_FAILURE;
    }
    while(inputparam[i]!='\0')
    {
        if(inputparam[i]==':')
            count++;
        i++;
    }
    if((strchr(inputparam,'.')) && (count < 2))
    {
        if(is_ValidIpAddressv4_port((PUCHAR)inputparam))
        {
            returnStatus = ANSC_STATUS_SUCCESS;
        }
        else if(is_ValidHost((PUCHAR)inputparam))
        {
            returnStatus = ANSC_STATUS_SUCCESS;
        }
        else
            returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strchr(inputparam,'['))
    {
        if(is_ValidIpAddressv6_port((PUCHAR)inputparam))
	    returnStatus = ANSC_STATUS_SUCCESS;
        else
	    returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strchr(inputparam,':'))
    {
        if(is_Ipv6_address((PUCHAR)inputparam))      
            returnStatus = ANSC_STATUS_SUCCESS;
        else
            returnStatus = ANSC_STATUS_FAILURE;
    }
    else
        returnStatus = ANSC_STATUS_FAILURE;   

    if(ANSC_STATUS_SUCCESS == returnStatus)
    {
         rc = sprintf_s(wrapped_inputparam, sizeof_wrapped_inputparam, "'%s'",inputparam);
         if(rc < EOK)
         {
             ERR_CHK(rc);
             return ANSC_STATUS_FAILURE;
         }
    }   
  return returnStatus;
}
/* Maitenance window can be customized for bci routers */
#if defined(_COSA_BCM_MIPS_) || defined(_PLATFORM_RASPBERRYPI_)
ANSC_STATUS
CosaDmlDiGetFirmwareUpgradeStartTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    char value[25] = {0};
    FILE *fp;
    errno_t                         rc              = -1;

    rc = strcpy_s(pValue, *pulSize, _START_TIME_12AM_);
    if ( rc != EOK) {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }

    fp = popen("cat /nvram/.FirmwareUpgradeStartTime", "r");
    if (fp == NULL)
    {
        CcspTraceError(("ERROR '%s'\n","ERROR"));
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        while(fgets(value, sizeof(value), fp) != NULL)
        {
            rc = strcpy_s(pValue, *pulSize, value);
            if ( rc != EOK) {
                ERR_CHK(rc);
                pclose(fp);
                return ANSC_STATUS_FAILURE;
            }
        }
    }

    if ( !value[0] )  // if file does not exist
    {
        CosaDmlDiSetFirmwareUpgradeStartTime( pValue );
    }
    pclose(fp);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

#else
ANSC_STATUS
CosaDmlDiGetFirmwareUpgradeStartTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    char value[25];
    FILE *fp;
    errno_t                         rc              = -1;
    rc = strcpy_s(pValue, *pulSize, _ERROR_);
    if ( rc != EOK) {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    memset(value,0,sizeof(value));
    fp = popen("cat /nvram/.FirmwareUpgradeStartTime", "r");
    if (fp == NULL)
    {
        CcspTraceError(("ERROR '%s'\n","ERROR"));
        return ANSC_STATUS_FAILURE;
    }
    while(fgets(value, sizeof(value), fp) != NULL)
    {
        rc = strcpy_s(pValue, *pulSize, value);
        if ( rc != EOK) {
            ERR_CHK(rc);
            pclose(fp);
            return ANSC_STATUS_FAILURE;
        }
    }
    pclose(fp);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}
#endif

#if defined(_COSA_BCM_MIPS_) || defined(_PLATFORM_RASPBERRYPI_)
ANSC_STATUS
CosaDmlDiGetFirmwareUpgradeEndTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    char value[25] = {0};
    FILE *fp;
    errno_t                         rc              = -1;

    rc = strcpy_s(pValue, *pulSize, _END_TIME_3AM_);
    if ( rc != EOK) {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }

    fp = popen("cat /nvram/.FirmwareUpgradeEndTime", "r");
    if (fp == NULL)
    {
        CcspTraceError(("ERROR '%s'\n","ERROR"));
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        while(fgets(value, sizeof(value), fp) != NULL)
        {
            rc = strcpy_s(pValue, *pulSize, value);
            if ( rc != EOK) {
                ERR_CHK(rc);
                pclose(fp);
                return ANSC_STATUS_FAILURE;
            }
        }
    }

    if ( !value[0] )  // if file does not exist
    {
        CosaDmlDiSetFirmwareUpgradeEndTime( pValue );
    }
    pclose(fp);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}
#else
ANSC_STATUS
CosaDmlDiGetFirmwareUpgradeEndTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    char value[25];
    FILE *fp;
    errno_t                         rc              = -1;

    rc = strcpy_s(pValue, *pulSize, _ERROR_);
    if ( rc != EOK) {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    memset(value,0,sizeof(value));
    fp = popen("cat /nvram/.FirmwareUpgradeEndTime", "r");
    if (fp == NULL)
    {
        CcspTraceError(("ERROR '%s'\n","ERROR"));
        return ANSC_STATUS_FAILURE;
    }
    while(fgets(value, sizeof(value), fp) != NULL)
    {
        rc = strcpy_s(pValue, *pulSize, value);
        if ( rc != EOK) {
            ERR_CHK(rc);
            pclose(fp);
            return ANSC_STATUS_FAILURE;
        }
    }
    pclose(fp);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}
#endif
ANSC_STATUS
CosaDmlDiSetFirmwareUpgradeStartTime (char* pString)
{
    /* collect value */
    FILE *fptr;
    fptr = fopen("/nvram/.FirmwareUpgradeStartTime", "w");
    if(fptr == NULL)
    {
        CcspTraceError(("Cant open file /nvram/.FirmwareUpgradeStartTime"));
        return ANSC_STATUS_FAILURE;
    }
    fprintf(fptr,"%s", pString);
    fclose(fptr);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiSetFirmwareUpgradeEndTime (char* pString)
{
    FILE *fptr;
    fptr = fopen("/nvram/.FirmwareUpgradeEndTime", "w");
    if(fptr == NULL)
    {
        CcspTraceError(("Cant open file /nvram/.FirmwareUpgradeEndTime"));
        return ANSC_STATUS_FAILURE;
    }
    fprintf(fptr,"%s", pString);
    fclose(fptr);
    return ANSC_STATUS_SUCCESS;
}



ANSC_HANDLE CosaProcStatusCreate()
{
    PCOSA_DATAMODEL_PROCSTATUS p_info = NULL;

    p_info = AnscAllocateMemory(sizeof(COSA_DATAMODEL_PROCSTATUS));
    AnscZeroMemory(p_info, sizeof(COSA_DATAMODEL_PROCSTATUS));
    p_info->pProcTable = NULL;

    return p_info;
}


void COSADmlRemoveProcessInfo(PCOSA_DATAMODEL_PROCSTATUS pObj)
{
    if (pObj)
    {
        if (pObj->pProcTable) AnscFreeMemory(pObj->pProcTable);
        AnscFreeMemory(pObj);
    }
}

static int read_proc_stat(char * line, char * p_cmd, char * p_state, int * p_size, int* p_priority, int * p_start_time)
{
    int utime, stime, cutime, cstime;
    char * tmp = NULL, *tmp1 = NULL;
    int HZ;

    if (!p_cmd || !p_size || !p_priority || !p_start_time) return -1;

    tmp1 = strchr(line, '(');
    if (!tmp1) return -1;
    else {
        tmp = strchr(line, ')');
        if (!tmp) return -1;
        else {
            strncpy(p_cmd, tmp1+1, tmp-tmp1-1);
            
            if(strlen(p_cmd) == 0)
            {
                fprintf(stderr,"\n %s %d p_cmd length is 0 return ",__func__,__LINE__);
                CcspTraceWarning(("\n %s %d p_cmd length is 0 return",__func__,__LINE__));
                return -1;
            }

            tmp += 2;
            if (sscanf(tmp, "%c %*d %*d %*d %*d %*d %*u %*u \
%*u %*u %*u %d %d %d %d %d %*d %*d 0 %*u %d", 
                       p_state,
                       &utime,
                       &stime,
                       &cutime,
                       &cstime,
                       p_priority,
                       p_size
                    ) != 7) return -1;

            HZ = sysconf(_SC_CLK_TCK);
            /*unit in millseconds*/
            *p_start_time = 1000/HZ*(utime+stime);
            /*unit in kBytes*/
            *p_size = *p_size/1024; 
        
        }

    }
    return 0;
}

void COSADmlGetProcessInfo(PCOSA_DATAMODEL_PROCSTATUS p_info)
{
    PCOSA_PROCESS_ENTRY         p_proc = NULL;
    ULONG                       ProcessNumber       = MAX_PROCESS_NUMBER;
    struct dirent               *result = NULL;
    DIR                         *dir;
    FILE                        *fp;
    char*                       name;
    ULONG                       i;
    ULONG                       pid;
    char                        status[32];
    char                        buf[400];
    char                        state[64];
    errno_t                     rc = -1;
    
    p_info->pProcTable = AnscAllocateMemory(sizeof(COSA_PROCESS_ENTRY) * ProcessNumber);  
                
    if( !p_info->pProcTable )
    {
        return ;
    }
    AnscZeroMemory(p_info->pProcTable, sizeof(COSA_PROCESS_ENTRY) * ProcessNumber);        
        
    dir = opendir("/proc");
        
    if ( !dir )
    {
        CcspTraceWarning(("Failed to open /proc!\n"));
        return ;
    }
        
	result = NULL;
    for(i = 0; i < ProcessNumber; )
    {
        result = readdir(dir);
        if( result == NULL )
        {
            closedir(dir);
            dir = NULL;
            break;
        }

        name = result->d_name;
            
        if ( *name >= '0' && *name <= '9' )
        {
            /*CcspTraceWarning(("Begin to parse process %lu!", i));*/
            p_proc = p_info->pProcTable+i;
            pid = atoi(name);
            p_proc->Pid = pid;
            rc = sprintf_s(status, sizeof(status), "/proc/%lu/stat", pid);
            if(rc < EOK)
            {
                ERR_CHK(rc);
                continue;
            }
                
            if ( !(fp = fopen(status, "r")) )
            {   
                CcspTraceWarning(("Failed to open %s!\n", status));
                continue;
            }
    
            name = fgets(buf, sizeof(buf), fp);
            fclose(fp);  
                
            if ( !name )
            {
                CcspTraceWarning(("Failed to get process %lu information!\n", pid));
                continue;
            }                  

            memset(state, 0, sizeof(state));

            if (read_proc_stat(name, p_proc->Command, state, (int*)&p_proc->Size, (int*)&p_proc->Priority, (int*)&p_proc->CPUTime ))
            {
                CcspTraceWarning(("Failed to parse process %lu information!\n", pid));
                continue;
            }
            i++;
	    
            /*CcspTraceWarning((" Cmd:%s, size, priority, cputime %d:%d:%d \n", p_proc->Command, p_proc->Size, p_proc->Priority, p_proc->CPUTime));*/
            name = strchr(p_proc->Command, ')');
                
            if ( name )
            {
                *name = '\0';
            }
            switch (*state)
            {
            case 'R':
                p_proc->State = COSA_DML_PROC_STATUS_Running;
                break;
            
            case 'S':
                p_proc->State = COSA_DML_PROC_STATUS_Sleeping;
                break;
             
            case 'D':
                p_proc->State = COSA_DML_PROC_STATUS_Uninterruptible;
                break;
            
            case 'T':
                p_proc->State = COSA_DML_PROC_STATUS_Stopped;
                break;
            
            case 'Z':
            case 'X':
                p_proc->State = COSA_DML_PROC_STATUS_Zombie;
                break;
         
            default:
                p_proc->State = COSA_DML_PROC_STATUS_Idle;
            }

        }
    }

#if defined (INTEL_PUMA7)
    //Intel Proposed RDKB Generic Bug Fix from XB6 SDK
    if ( dir != NULL )
    {
        closedir(dir);
        dir = NULL;
    }
#endif
	
    p_info->ProcessNumberOfEntries = i;

    fprintf(stderr,"\n %s %d  ProcessNumberOfEntries:%lu",__func__,__LINE__,p_info->ProcessNumberOfEntries);
    CcspTraceWarning(("\n %s %d  ProcessNumberOfEntries:%lu\n",__func__,__LINE__,p_info->ProcessNumberOfEntries));

    if ( dir != NULL )
    {
       closedir(dir);
       dir = NULL;
    }

    return; 
}

void test_get_proc_info()
{
     PCOSA_DATAMODEL_PROCSTATUS p_info = (PCOSA_DATAMODEL_PROCSTATUS)CosaProcStatusCreate();

     if (p_info) {
         COSADmlGetProcessInfo(p_info);
         /*CID: 57768 Resource leak*/
         COSADmlRemoveProcessInfo(p_info);
     }

}

typedef  struct
_CPUTIME_INFO
{
    ULONG                           UserTime;
    ULONG                           NiceTime;
    ULONG                           SystemTime;
    ULONG                           IdleTime;
    ULONG                           IowaitTime;
    ULONG                           IrqTime;
    ULONG                           SoftirqTime;
    ULONG                           GuestTime;
}
COSA_CPUTIME_INFO, *PCOSA_CPUTIME_INFO;

ULONG COSADmlGetCpuUsage()
{
    FILE                        *fp;
    int                         num;
    COSA_CPUTIME_INFO           time[2];
    ULONG                       UsedTime = 0;
    ULONG                       IdleTime = 0;
    double                      CPUUsage;
    int                         CPUNum;

    AnscZeroMemory(time, sizeof(time));

    CPUNum = sysconf(_SC_NPROCESSORS_ONLN);
    CcspTraceWarning(("There are %d cpus!\n", CPUNum));
        
    if ( !(fp = fopen("/proc/stat", "r")) )
    {   
        CcspTraceWarning(("Failed to open /proc/stat!\n"));
        return 0;
    }
    num = fscanf(fp, "cpu %lu %lu %lu %lu %lu %lu %lu %lu\n", &time[0].UserTime, &time[0].NiceTime,
               &time[0].SystemTime, &time[0].IdleTime, &time[0].IowaitTime, &time[0].IrqTime,
               &time[0].SoftirqTime, &time[0].GuestTime);
                     
    if ( num != 8 )
    {   
        CcspTraceWarning(("Failed to parse current cpu time info!\n"));
        fclose(fp);
        return 0;
    }
    sleep(1);
    rewind(fp);
    fflush(fp);
    num = fscanf(fp, "cpu %lu %lu %lu %lu %lu %lu %lu %lu\n", &time[1].UserTime, &time[1].NiceTime,
               &time[1].SystemTime, &time[1].IdleTime, &time[1].IowaitTime, &time[1].IrqTime,
               &time[1].SoftirqTime, &time[1].GuestTime);
           
    fclose(fp);
        
    if ( num != 8 )
    {   
        CcspTraceWarning(("Failed to parse cpu time info!\n"));
        return 0;
    }
    UsedTime = time[1].UserTime + time[1].NiceTime + time[1].SystemTime + time[1].IowaitTime
       + time[1].IrqTime + time[1].SoftirqTime + time[1].GuestTime - time[0].UserTime 
       - time[0].NiceTime - time[0].SystemTime - time[0].IowaitTime
       - time[0].IrqTime - time[0].SoftirqTime - time[0].GuestTime;
    IdleTime = time[1].IdleTime - time[0].IdleTime;
        
    CcspTraceWarning(("UsedTime = %lu\n", UsedTime));
    CcspTraceWarning(("IdleTime = %lu\n", IdleTime));
        
    CPUUsage = (UsedTime *100 / (UsedTime + IdleTime)) / CPUNum ;

    if( !CPUUsage )
    {
        CcspTraceWarning(("CPU usage shouldn't be zero!!!\n"));
        CPUUsage = 1;
    }

    return  CPUUsage;
}

int COSADmlSetMemoryStatus(char * ParamName, ULONG val)
{
    if(AnscEqualString(ParamName, "X_RDKCENTRAL-COM_FreeMemThreshold", TRUE))
     {
            char buf[10];
	    memset(buf,0,sizeof(buf));
	    snprintf(buf,sizeof(buf),"%lu",val);            		    
	    if ((syscfg_set(NULL, "MinMemoryThreshold_Value", buf) != 0)) 
	    {
	        CcspTraceWarning(("syscfg_set failed\n"));
	        return -1;
	    }
	    else 
	    {
	        if (syscfg_commit() != 0) 
	        {
		    CcspTraceWarning(("syscfg_commit failed\n"));
		    return -1;
	        }
			
	       return 0;
	     } 
     }
    return 0; 
}
ULONG COSADmlGetMemoryStatus(char * ParamName)
{
     struct sysinfo si;
     ULONG tmp;
     if (sysinfo(&si))
     {
          /*Error*/
          return 0;
     }
     if( AnscEqualString(ParamName, "Total", TRUE))
     {
#if defined  _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
#if 0
         /* we want to get the real Physical memory size */
        FILE *fp;
        char *line = NULL;
        size_t size;
        char *str;
        int mem = 0;

        fp = fopen("/proc/bootparams","r");
        if(fp == NULL){
            return 0;
        }
        while(-1 != getline(&line, &size, fp)){
            if(0 != (str = strstr(line, "RAM Size"))){
               if(0 != (str = strstr(str, "0x"))){
                   mem = strtol(str, NULL, 16);
                   free(line);
                   return mem/1024;
               }
            }
            free(line);
            line = NULL;
        }
        return 0;
#endif

    if ( platform_hal_GetTotalMemorySize(&tmp) != RETURN_OK )
        return 0;
    else
        return tmp;
        /* return  512*1024; */

#else
        return si.totalram*si.mem_unit/(1024);
#endif
     }
     else if(AnscEqualString(ParamName, "Free", TRUE))
     {
#ifdef _COSA_INTEL_USG_ARM_
	if ( platform_hal_GetFreeMemorySize(&tmp) != RETURN_OK )
        return 0;
    	else
        return tmp;
#else
#if defined(_XF3_PRODUCT_REQ_)
         return si.freeram*si.mem_unit/(1024*1024);
    #else
         return si.freeram*si.mem_unit/(1024);
#endif
#endif
     }


     else if(AnscEqualString(ParamName, "Used", TRUE))
     {
#if (defined _COSA_INTEL_USG_ARM_) || (defined  _COSA_BCM_MIPS_)
	if ( platform_hal_GetUsedMemorySize(&tmp) != RETURN_OK )
        return 0;
    	else 
        return tmp;
#endif
     }
     else if(AnscEqualString(ParamName, "X_RDKCENTRAL-COM_FreeMemThreshold", TRUE))
     {
	char buf[10];
	memset(buf,0, sizeof(buf));
        /* CID:56435 Array compared against 0*/
        if(!syscfg_get( NULL, "MinMemoryThreshold_Value", buf, sizeof(buf)))
        {
            return atoi(buf);
        }
	return 0;
     }
     else 
     {
          return 0;
     }
}


ULONG COSADmlGetMaxWindowSize()
{
    FILE *fp;
    ULONG winSize = 0;
    char value[10];

    memset(value,0,sizeof(value));
    fp = popen("cat /proc/sys/net/core/wmem_max", "r");
    if (fp == NULL)
    {
       CcspTraceWarning(("ERROR '%s'\n","ERROR")); 
       return 0;
    }
   
    while(fgets(value, 10, fp) != NULL)
    {
       winSize = AnscString2Int(value);
       CcspTraceWarning(("********MaxWindowSize is '%lu\n", winSize));
    }
    pclose(fp);

    return winSize;   
}

ANSC_STATUS
CosaDmlDiGetAdvancedServices
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t                         rc              = -1;

    rc = strcpy_s(pValue, *pulSize, "");
    if ( rc != EOK) {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    *pulSize = 0;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetProcessorSpeed
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    #define TOKEN_STR       "BogoMIPS"
    #define MAX_LINE_SIZE   30
    char line[MAX_LINE_SIZE];
    FILE *fp;
    errno_t rc = -1;

    memset(line, 0, sizeof(line));

#ifdef _COSA_BCM_ARM_

    fp = popen("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "r");
    if (fp == NULL)
    {
        CcspTraceWarning(("Read cpuinfo ERROR '%s'\n","ERROR"));
        return ANSC_STATUS_FAILURE;
    }

    /* Processor speed shown in file is in KHz. Convert it into MHz to comply with GUI */
    while(fgets(line, MAX_LINE_SIZE, fp) != NULL )
    {
        int procSpeed;
        procSpeed = atoi (line);
        procSpeed = procSpeed / 1000;
        rc = sprintf_s (line, sizeof(line), "%d", procSpeed);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        rc = strcpy_s(pValue, *pulSize, line);
        ERR_CHK(rc);
    }

    pclose(fp);
    fp = NULL;
#else
    char *pcur;
#ifdef _COSA_INTEL_XB3_ARM_

    fp = popen("cat /proc/avalanche/base_psp_version | grep -i \"Cpu Frequency\" | cut -d \":\" -f2 | awk '{$1=$1};1' | cut -d \" \" -f1", "r");
    if (fp == NULL)
    {
        CcspTraceWarning(("Read CPU Info ERROR '%s'\n","ERROR"));
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        fgets(line, MAX_LINE_SIZE, fp);
        pclose( fp );
        fp = NULL;
        // Remove line \n character from string
        if (( pcur = strchr( line, '\n' )) != NULL )
        {
            *pcur = '\0';
        }
        rc = strcpy_s(pValue, *pulSize, line);
        ERR_CHK(rc);
    }

#endif

#ifdef SA_CUSTOM
    char out_val[100]={0};
    char out2[100]={0};
    FILE *fp1=NULL;
    char *urlPtr = NULL;

    fp1 = fopen("/etc/device.properties", "r");
    if (fp1 == NULL)
    {
        CcspTraceError(("Error opening properties file! \n"));
        return FALSE;
    }

    while (fgets(out_val,100, fp1) != NULL)
    {
        // Look for ATOM_ARPING_IP
        if (strstr(out_val, "ATOM_ARPING_IP") != NULL)
        {
            out_val[strcspn(out_val, "\r\n")] = 0; // Strip off any carriage returns

            // grab URL from string
            urlPtr = strstr(out_val, "=");
            urlPtr++;
            break;
        }
    }
    fclose(fp1);
    rc = sprintf_s(out2, sizeof(out2), "\"cat /proc/cpuinfo\"");
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    fp = v_secure_popen("r", "rpcclient %s %s",urlPtr,out2);
#else
    fp = v_secure_popen("r", "cat /proc/cpuinfo");
#endif
    if (fp == NULL)
    {
        CcspTraceWarning(("Read cpuinfo ERROR '%s'\n","ERROR")); 
        return ANSC_STATUS_FAILURE;
    }
   
    while(fgets(line, MAX_LINE_SIZE, fp) != NULL )
    {
       if(strcasestr(line, TOKEN_STR))
       {
        pcur = strstr(line, ":");
        pcur++;
        while(*pcur == ' ') pcur++;           
        rc = strcpy_s(pValue, *pulSize, pcur);
        ERR_CHK(rc);
       }     
    }
#endif
    
    if(fp != NULL) {
        v_secure_pclose(fp);
	fp = NULL;
    }
    *pulSize = AnscSizeOfString(pValue);
    if(pValue[*pulSize-1] == '\n') pValue[--(*pulSize)] = '\0';
    return ANSC_STATUS_SUCCESS; 
}

ANSC_STATUS
CosaDmlDiGetFactoryResetCount
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
	platform_hal_GetFactoryResetCount(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDiClearResetCount
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bValue
   )
{
    UNREFERENCED_PARAMETER(hContext);
	platform_hal_ClearResetCount(bValue);
    return ANSC_STATUS_SUCCESS;
}
   
ANSC_STATUS
CosaDmlDiGetAndProcessDhcpServDetectionFlag
  (
	  ANSC_HANDLE				  hContext,
	  BOOLEAN*					  pValue
  )
{
    UNREFERENCED_PARAMETER(hContext);
	char buf[ 8 ] = { 0 };

	if( 0 == syscfg_get( NULL, "DhcpServDetectEnable", buf, sizeof( buf ) ) )
	{
		if( 0 == strcmp( buf, "true" ) )
		{
			*pValue = 1;
		}

		if( 0 == strcmp( buf, "false" ) )
		{
			*pValue = 0;
		}
		
		/* 
		* To schedule/deschedule server test execution based on DhcpServDetectEnable flag 
		*/
		v_secure_system( "/usr/ccsp/tad/schd_dhcp_server_detection_test.sh" );
	}
	else
	{
        CcspTraceWarning(("syscfg_get failed\n")); 

		return ANSC_STATUS_FAILURE;
	}

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiSetAndProcessDhcpServDetectionFlag
   (
	   ANSC_HANDLE				   hContext,
	   BOOLEAN*					   pValue,
	   BOOLEAN*					   pDhcpServDetectEnable
   )
{
    UNREFERENCED_PARAMETER(hContext);
	if ( syscfg_set( NULL, 
					  "DhcpServDetectEnable", 
					  ((*pValue == 1 ) ? "true" : "false") )!= 0 ) 
	{
		CcspTraceWarning(("syscfg_set failed\n")); 
		return ANSC_STATUS_FAILURE;
	}
	else 
	{
		if ( syscfg_commit( ) != 0 ) 
		{
			CcspTraceWarning(("syscfg_commit failed\n")); 
			return ANSC_STATUS_FAILURE;
		}

		*pDhcpServDetectEnable = *pValue;

		/* 
		* To schedule/deschedule server test execution based on DhcpServDetectEnable flag 
		*/
		v_secure_system( "/usr/ccsp/tad/schd_dhcp_server_detection_test.sh" );
	}  

    return ANSC_STATUS_SUCCESS;
}

#endif
int getRebootCounter()
{       
        char buf[8];
                /* CID: 74840 Array compared against 0*/
		if(!syscfg_get( NULL, "X_RDKCENTRAL-COM_LastRebootCounter", buf, sizeof(buf)))
                {
                    return atoi(buf);
                }
                else
                {
                     AnscTraceWarning(("syscfg_get failed\n"));
                     return -1;
                }
	   		
}

int setRebootCounter()
{
           
            int val = 0;
            char buf[8];
		    snprintf(buf,sizeof(buf),"%d",val);     
        
                if ((syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootCounter", buf) != 0)) 
	            {
			        AnscTraceWarning(("syscfg_set failed\n"));
			        return -1;
			    }
		   	    else 
		        {
		         if (syscfg_commit() != 0) 
		        {
				    AnscTraceWarning(("syscfg_commit failed\n"));
				     return -1;
				
			    }
			
			    return 0;
		    }  
}

int setUnknownRebootReason()
{
   
            int val = 0;
            char buf[8];
		    snprintf(buf,sizeof(buf),"%d",val);     
            OnboardLog("Device reboot due to reason unknown\n");
                if ((syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootReason", "unknown") != 0)) 
	            {
			        AnscTraceWarning(("syscfg_set failed\n"));
			        return -1;
			    }
		   	    else 
		        {
		         if (syscfg_commit() != 0) 
		        {
				    AnscTraceWarning(("syscfg_commit failed\n"));
				     return -1;
				
			    }
			
			    return 0;
		    }        
}

void setLastRebootReason(char* reason)
{

	int val = 1;
	char buf[8];
	snprintf(buf,sizeof(buf),"%d",val);

	OnboardLog("Device reboot due to reason %s\n", reason);
	if (syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootReason", reason) != 0)
	{
		AnscTraceWarning(("syscfg_set failed for Reason\n"));
	}
	else
	{
		if (syscfg_commit() != 0)
		{
			AnscTraceWarning(("syscfg_commit failed for Reason\n"));
		}

	}

	if (syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootCounter", buf) != 0)
	{
		AnscTraceWarning(("syscfg_set failed for Counter\n"));
	}
	else
	{
		if (syscfg_commit() != 0)
		{
			AnscTraceWarning(("syscfg_commit failed for Counter\n"));
		}
	}
}
// Check SHORTS RFC
bool isShortsEnabled(){

        char buf[8]={0};
        if (syscfg_get( NULL, "ShortsEnabled", buf, sizeof(buf)) == 0)
        {
                if (strncmp(buf, "true", sizeof(buf)) == 0){
                        CcspTraceInfo(("%s SHORTS Enabled: %s \n",__FUNCTION__,"True"));
                        return TRUE;
                }
                else {
                        CcspTraceInfo(("%s SHORTS Enabled: %s \n",__FUNCTION__,"False"));
                        return FALSE;
                }
        }
        else
        {
                CcspTraceError(("%s syscfg_get failed  for SHORTS Enable\n",__FUNCTION__));
                return FALSE;
        }
}
// Find localhost port available in specified range for stunnel-client to listen
int findLocalPortAvailable()
{
        struct sockaddr_in address;
        int sockfd = -1, status;
        int port = MIN_PORT_RANGE;

        while (port <= MAX_PORT_RANGE) {
                address.sin_family = AF_INET;
                address.sin_addr.s_addr = inet_addr("127.0.0.1");
                address.sin_port = htons(port);

                sockfd = socket(AF_INET, SOCK_STREAM, 0);

                status = connect(sockfd, (struct sockaddr *)&address, sizeof(address));

                if (status<0){
                         CcspTraceInfo(("[%s] Port %d is available.\n", __FUNCTION__, port));
                         close(sockfd);
                         return port;
                }
                CcspTraceInfo(("[%s] Port %d is in use.\n", __FUNCTION__, port));
                close(sockfd);
                port++;
        }
        return -1;
}
bool isStunnelPortEnabled(char* pString){
        char tempCopy[512]={ "\0"};
        char* value=NULL;
        char *st = NULL;
        char* tempStr = NULL;
        errno_t rc = -1;
        rc = strcpy_s(tempCopy,512, pString);
        ERR_CHK(rc);
        tempStr = (char*) strtok_r(tempCopy, ";", &st);
        while(NULL != tempStr) {
                if((value = strstr(tempStr, "stunnelport="))){
                        stunnelsshargs.stunnelport=atoi(value + strlen("stunnelport="));
                        return TRUE;
                }
                tempStr = (char*) strtok_r(NULL, ";", &st);
        }
        return FALSE;
}
int setXOpsReverseSshArgs(char* pString) {

    char tempCopy[512] = { "\0" };
    char* tempStr = NULL;
    char* option = NULL;
    char* hostLogin = NULL;
    int inputMsgSize = 0;
    int hostloglen = 0;
    char *st = NULL;
    errno_t rc = -1;

    bool shortsFlag= false;
#ifdef ENABLE_SHORTS
    shortsFlag=(isShortsEnabled() && isStunnelPortEnabled(pString));
#endif
    memset(reverseSSHArgs,0,sizeof(reverseSSHArgs));
    if (!pString)
    {
        return 1;
    }
    inputMsgSize = strlen(pString);
    hostLogin = getHostLogin(pString,shortsFlag);
    if (!hostLogin) {
            AnscTraceWarning(("Warning !!! Target host for establishing reverse SSH tunnel is missing !!!\n"));
            rc = strcpy_s(reverseSSHArgs, sizeof(reverseSSHArgs), "");
            ERR_CHK(rc);
            return 1;
    }
    if(shortsFlag)
    {
            stunnelsshargs.localport=findLocalPortAvailable();
            if(stunnelsshargs.localport == -1)
            {
                    CcspTraceInfo(("[%s] Reserved ports are not availale... \n",__FUNCTION__));
                    return NOK;
            }
            CcspTraceInfo(("[%s] Stunnel Args = %d %s %s %d \n",__FUNCTION__,stunnelsshargs.localport,stunnelsshargs.hostIp,stunnelsshargs.host,stunnelsshargs.stunnelport));
    }
    if (sizeof(tempCopy) > (unsigned int)inputMsgSize)
    {
            rc=strcpy_s(tempCopy, 512, pString);
    }
    else
    {
       if (hostLogin)
         free(hostLogin);
            return 1;
    }
    tempStr = (char*) strtok_r(tempCopy, ";", &st);
    if (NULL != tempStr) {
      option = mapArgsToSSHOption(tempStr,shortsFlag);
           if (option)
           {
                unsigned int len = strlen(option);
                if (sizeof(reverseSSHArgs) > len)
                {
                    strncpy(reverseSSHArgs, option,len);
                }
            }
     } else {
            AnscTraceWarning(("No Match Found !!!!\n"));
            printf("No Match Found !!!!\n");
        }
     if (option) {
            free(option);
     }

     while ((tempStr = strtok_r(NULL, ";", &st)) != NULL) {
       option = mapArgsToSSHOption(tempStr,shortsFlag);
            if ( NULL != option) {
                int len = strlen(option);
                if (sizeof(reverseSSHArgs) > (strlen(reverseSSHArgs) + len))
                {
                    strncat(reverseSSHArgs, option,len);
                }
                free(option);
            }
     }
     hostloglen = strlen(hostLogin);
     if (sizeof(reverseSSHArgs) > (strlen(reverseSSHArgs) + hostloglen)){
       strncat(reverseSSHArgs, hostLogin,hostloglen);
     }
     if (hostLogin)
       free(hostLogin);
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS getXOpsReverseSshArgs
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t                         rc              = -1;

    rc = strcpy_s(pValue, *pulSize, reverseSSHArgs);
    if ( rc != EOK) {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    *pulSize = AnscSizeOfString(pValue) +1;
    return ANSC_STATUS_SUCCESS;
}

int setXOpsReverseSshTrigger(char *input) {

    char *trigger = NULL;
    if (!input) {
        printf("Input args are empty \n");
        AnscTraceWarning(("Input args are empty !!!!\n"));
        return NOK;
    }

    trigger = strstr(input, "start");
    if (trigger) {
    #ifdef ENABLE_SHORTS
        char *trigger_shorts = NULL;
        trigger_shorts = strstr(input,"start shorts");
        if (isShortsEnabled() && trigger_shorts)
        {
                CcspTraceInfo(("[%s] Starting Stunnel \n",__FUNCTION__));
                CcspTraceInfo(("[%s] Stunnel Commmand = %s %d %s %s %d %s \n",__FUNCTION__,stunnelCommand,stunnelsshargs.localport,stunnelsshargs.host,stunnelsshargs.hostIp,stunnelsshargs.stunnelport,reverseSSHArgs));
                v_secure_system("/bin/sh %s %d %s %s %d %s &",stunnelCommand,stunnelsshargs.localport,stunnelsshargs.host,stunnelsshargs.hostIp,stunnelsshargs.stunnelport,reverseSSHArgs);
        }
        else {
    #endif
                CcspTraceInfo(("[%s] ReverseSSH arguments = %s  \n",__FUNCTION__,reverseSSHArgs));
                v_secure_system(sshCommand " start %s", reverseSSHArgs);
    #ifdef ENABLE_SHORTS
        }
    #endif
    } else {
        v_secure_system(sshCommand " stop ");
    }
    return OK;
}

int isRevSshActive(void) {
    int status = NOK;
    int pid = 0;
    int ret = 0 ;
    FILE* pidFilePtr = NULL;
    pidFilePtr = fopen(rsshPidFile, "r");
    if ( NULL != pidFilePtr) {
        if ( (ret = fscanf(pidFilePtr, "%d", &pid)) > 0 ) {
            if ( -1 != getpgid(pid)) {
                status = OK;
            } else {
                status = NOK;
            }
        }
        fclose(pidFilePtr);
    }
    return status;
}

#define PARTNER_ID_LEN 64 

ANSC_STATUS
CosaDmlDiGetSyndicationPartnerId
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ANSC_STATUS retVal = ANSC_STATUS_FAILURE;
    char fileContent[256] = {0};
    FILE *deviceFilePtr = NULL;
    char *pPartnerId = NULL;
    const char partnerStr[] = "PARTNER_ID";
    errno_t rc = -1;

    if (!pValue || !pulSize || *pulSize >= PARTNER_ID_LEN)
        return ANSC_STATUS_FAILURE;

    rc = STRCPY_S_NOCLOBBER(pValue, *pulSize, "comcast"); // Set the default to comcast in case the partner id is not set in props file
    if(rc != EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    *pulSize = AnscSizeOfString(pValue);
    retVal = ANSC_STATUS_SUCCESS;

    deviceFilePtr = fopen( DEVICE_PROPERTIES, "r" );
    if (deviceFilePtr)
    {
        while (fgets(fileContent, sizeof(fileContent), deviceFilePtr) != NULL)
        {
            if ((pPartnerId = strstr(fileContent, partnerStr)) != NULL)
            { 
                rc = strcpy_s(pValue, *pulSize, pPartnerId+sizeof(partnerStr));
                ERR_CHK(rc);
                *pulSize = AnscSizeOfString(pValue);
                break;
            }
        }
        fclose(deviceFilePtr);
    }
    return retVal;
}

#define DMSB_TR181_PSM_Syndication_Tr069CertLocation "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_Syndication.TR69CertLocation"

ANSC_STATUS
CosaDmlDiGetSyndicationTR69CertLocation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t                         rc              = -1;
	char val[ 256 ] = {0};
	
	if ( PsmGet( DMSB_TR181_PSM_Syndication_Tr069CertLocation, val, sizeof( val ) ) != 0 ) 
	{
		pValue[ 0 ] = '\0';
		CcspTraceError(("%s - Failed Get for '%s' \n", __FUNCTION__, DMSB_TR181_PSM_Syndication_Tr069CertLocation));
		return ANSC_STATUS_FAILURE;
	}
	else 
	{
                rc = strcpy_s(pValue, 512, val);      // here pValue size is 512 getting from calling function
                if ( rc != EOK) {
                    ERR_CHK(rc);
                    return ANSC_STATUS_FAILURE;
                }
	}

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiSetSyndicationTR69CertLocation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
	int  retPsmSet = CCSP_SUCCESS;
	
	retPsmSet = PSM_Set_Record_Value2( g_MessageBusHandle, 
									   g_GetSubsystemPrefix(g_pDslhDmlAgent), 
									   DMSB_TR181_PSM_Syndication_Tr069CertLocation, 
									   ccsp_string, 
									   pValue );
	if ( retPsmSet != CCSP_SUCCESS ) 
	{
		CcspTraceError(("%s - Failed Set for '%s' \n", __FUNCTION__, DMSB_TR181_PSM_Syndication_Tr069CertLocation));
		return ANSC_STATUS_FAILURE;
	}

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS getFactoryPartnerId
	(
		char*                       pValue,
        PULONG                      pulSize
	)
{
#if defined(_XB6_PRODUCT_REQ_) || defined(_HUB4_PRODUCT_REQ_)
	if(ANSC_STATUS_SUCCESS == platform_hal_getFactoryPartnerId(pValue))
	{
		*pulSize = AnscSizeOfString(pValue);
		CcspTraceInfo(("%s - %s\n",__FUNCTION__,pValue));
		return ANSC_STATUS_SUCCESS; 
	}
	else
	{
		//TCCBR-4426 - getFactoryPartnerId is only implemented for XB6/HUB4 Products as of now.
		CcspTraceError(("%s - Failed Get factoryPartnerId \n", __FUNCTION__));
	}
#endif
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pulSize);
	return ANSC_STATUS_FAILURE;
}

ANSC_STATUS fillCurrentPartnerId
	(
		char*                       pValue,
        PULONG                      pulSize
    )
{
	char buf[PARTNER_ID_LEN];
	memset(buf, 0, sizeof(buf));
    if(ANSC_STATUS_SUCCESS == syscfg_get( NULL, "PartnerID", buf, sizeof(buf)))
    {
         /* CID:66248 Array compared against 0*/
         /*CID: 56187 Logically dead code*/
 	 strncpy(pValue ,buf,strlen(buf));
         *pulSize = AnscSizeOfString(pValue);
         return ANSC_STATUS_SUCCESS;
    }
	else
		return ANSC_STATUS_FAILURE;

}
ANSC_STATUS setPartnerId
	(
		char*                       pValue
    )
{

	if ((syscfg_set(NULL, "PartnerID", pValue) != 0)) 
	{
        AnscTraceWarning(("setPartnerId : syscfg_set failed\n"));
		return ANSC_STATUS_FAILURE;
	}
	else 
	{
	        if (syscfg_commit() != 0) 
			{
				AnscTraceWarning(("setPartnerId : syscfg_commit failed\n"));
				return ANSC_STATUS_FAILURE;
			}
		return ANSC_STATUS_SUCCESS;
	}
}

#if defined(_COSA_BCM_ARM_) && !defined(_CBR_PRODUCT_REQ_)
ANSC_STATUS setCMVoiceImg(char* pValue)
{

        if ((syscfg_set(NULL, "CMVoiceImg", pValue) != 0))
        {
        	AnscTraceWarning(("setCMVoiceImg : syscfg_set failed\n"));
                return ANSC_STATUS_FAILURE;
        }
        else
        {
                if (syscfg_commit() != 0)
                {
                	AnscTraceWarning(("setCMVoiceImg : syscfg_commit failed\n"));
                        return ANSC_STATUS_FAILURE;
                }
                return ANSC_STATUS_SUCCESS;
        }
}
#endif

ANSC_STATUS activatePartnerId()
{
	pthread_t tid;

	if ( access( PARTNERID_FILE , F_OK ) == 0 )	 
	{
		pthread_create(&tid, NULL, &CosaDmlDiPartnerIDChangeHandling, NULL);
		return ANSC_STATUS_SUCCESS;
	}
	AnscTraceWarning(("%s: Partner ID set %s File not exist, so cannot activate partnerID  \n", __FUNCTION__,PARTNERID_FILE));
	return ANSC_STATUS_FAILURE;	

}

ANSC_STATUS setTempPartnerId
	(
		char*                       pValue
    )
{
	FILE		*fp	 =  NULL;

	fp = fopen( PARTNERID_FILE, "w" );

	if ( fp != NULL ) 
	{
		fwrite( pValue, strlen( pValue ), 1, fp );
		fclose( fp );
		AnscTraceWarning(("%s: Partner ID %s is Written into %s File\n", __FUNCTION__, pValue, PARTNERID_FILE ));
		return ANSC_STATUS_SUCCESS;
	}
	return ANSC_STATUS_FAILURE;
	
}

void CosaDmlPresenceEnable(BOOL enable)
{
    parameterValStruct_t notif_val[1];
    char                 param_name[256] = "Device.Hosts.X_RDK_PresenceDetectEnable";
    char                 component[256]  = "eRT.com.cisco.spvtg.ccsp.lmlite";
    char                 bus[256]        = "/com/cisco/spvtg/ccsp/lmlite";
    char*                faultParam      = NULL;
    int                  ret             = 0; 

    notif_val[0].parameterName  = param_name;
    if (enable)
    {
        notif_val[0].parameterValue = "true";
    }
    else
    {
        notif_val[0].parameterValue = "false";
    }
    notif_val[0].type           = ccsp_boolean;

    ret = CcspBaseIf_setParameterValues( 
            bus_handle,
            component,
            bus,
            0,
            0,
            notif_val,
            1,
            TRUE,
            &faultParam
            );
    if(ret != CCSP_SUCCESS)
    {
        if ( faultParam )
        {
	        AnscTraceWarning(("%s Failed to SetValue for param '%s'\n",__FUNCTION__,faultParam ) );
            CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
            bus_info->freefunc(faultParam);
        }

    }
    else
    {
		AnscTraceWarning(("%s: Presence enable sent to lmlite\n", __FUNCTION__ ));

    }
}

void *CosaDmlDiPartnerIDChangeHandling( __attribute__((unused)) void* buff )
{

    CCSP_MESSAGE_BUS_INFO *bus_info 		  = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
	parameterValStruct_t param_val[ 1 ] 	  = {{ "Device.X_CISCO_COM_DeviceControl.FactoryReset", "Router,Wifi,VoIP,Dect,MoCA", ccsp_string }};
	char 				*pComponentName = "eRT.com.cisco.spvtg.ccsp.pam";
	char 				*pComponentPath = "/com/cisco/spvtg/ccsp/pam";
	char				*faultParam 		  = NULL;
    int   				 ret             	  = 0;

	pthread_detach(pthread_self());	

	// Create /nvram/.apply_partner_defaults file to apply partners default
	v_secure_system( "touch /nvram/.apply_partner_defaults" );
	v_secure_system( "syscfg set PartnerID_FR 1; syscfg commit" );

	/* Need to do factory reset the device here */
    ret = CcspBaseIf_setParameterValues
			(
				bus_handle, 
				pComponentName, 
				pComponentPath,
				0, 
				0x0,   /* session id and write id */
				(void*)&param_val, 
                1, 
				TRUE,   /* Commit  */
				&faultParam
			);	

	if ( ( ret != CCSP_SUCCESS ) && \
		 ( faultParam )
		)
	{
	    AnscTraceWarning(("%s Failed to SetValue for param '%s'\n",__FUNCTION__,faultParam ) );
	    bus_info->freefunc( faultParam );
	} 
	else
	{
		AnscTraceWarning(("%s: Device will reboot in some time\n", __FUNCTION__ ));
	}
       return NULL;
}

ANSC_STATUS
CosaDeriveSyndicationPartnerID(char *Partner_ID)
{
	char PartnerID[PARTNER_ID_LEN];
	ULONG size = PARTNER_ID_LEN - 1;
	memset(PartnerID, 0, sizeof(PartnerID));
	CcspTraceInfo(("%s \n",__FUNCTION__));
// get if PartnerID is already available
	if(ANSC_STATUS_SUCCESS == fillCurrentPartnerId(PartnerID, &size))
	{
		// TODO: check PartnerID from Boot config file
	}
	else
	{
		//Get FactoryPartnerID
		CcspTraceInfo(("%s Get FactoryPartnerID\n",__FUNCTION__));
		if(ANSC_STATUS_SUCCESS == getFactoryPartnerId(PartnerID,&size))
			{
				if(ANSC_STATUS_FAILURE == setPartnerId(PartnerID))
					CcspTraceError(("%s - Failed Set for PartnerID \n", __FUNCTION__ ));
			}
		else
			{
			 // Check for PartnerID available in RDKB-build, if not then return default
			CcspTraceInfo(("%s Check for PartnerID available in RDKB-build, if not then return defaul\n",__FUNCTION__));
			 if(ANSC_STATUS_FAILURE == CosaDmlDiGetSyndicationPartnerId(NULL,PartnerID, &size))
			 	{
			 		CcspTraceError(("%s - Failed to get PartnerID available in build \n", __FUNCTION__ ));
			 	}
			 else
			 	{
			 		if(ANSC_STATUS_FAILURE == setPartnerId(PartnerID))
						CcspTraceError(("%s - Failed Set for PartnerID \n", __FUNCTION__ ));
			 	}
			}
	}
	strncpy(Partner_ID,PartnerID, PARTNER_ID_LEN);
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetSyndicationLocalUIBrandingTable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pulSize);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetSyndicationWifiUIBrandingTable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pulSize);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiWiFiTelemetryInit
  (
	PCOSA_DATAMODEL_RDKB_WIFI_TELEMETRY PWiFi_Telemetry
  )
 {
    char val[256] = {0};
    errno_t                         rc              = -1;

    if (!PWiFi_Telemetry)
    {
        CcspTraceWarning(("%s-%d : NULL param\n" , __FUNCTION__, __LINE__ ));
        return ANSC_STATUS_FAILURE;
    }

    memset(PWiFi_Telemetry, 0, sizeof(COSA_DATAMODEL_RDKB_WIFI_TELEMETRY));

    if (PsmGet(DMSB_TR181_PSM_WHIX_ChUtilityLogInterval, val, sizeof(val)) != 0)
    {
            PWiFi_Telemetry->ChUtilityLogInterval = 900;
    }
    else
    {
        if (val[0] != '\0' )
        {
            PWiFi_Telemetry->ChUtilityLogInterval = atoi(val);
        }
        else
        {
            PWiFi_Telemetry->ChUtilityLogInterval = 900;
        }
    }

    if (PsmGet(DMSB_TR181_PSM_WHIX_LogInterval, val, sizeof(val)) != 0)
    {
            PWiFi_Telemetry->LogInterval = 3600;
    }
    else
    {
        if (val[0] != '\0' )
        {
            PWiFi_Telemetry->LogInterval = atoi(val);
        }
        else
        {
            PWiFi_Telemetry->LogInterval = 3600;
        }
    }

    if (PsmGet(DMSB_TR181_PSM_WHIX_NormalizedRssiList, val, sizeof(val)) != 0)
    {
        rc = strcpy_s(PWiFi_Telemetry->NormalizedRssiList, sizeof(PWiFi_Telemetry->NormalizedRssiList), "1,2");
        ERR_CHK(rc);
    }
    else
    {
        rc = strcpy_s(PWiFi_Telemetry->NormalizedRssiList, sizeof(PWiFi_Telemetry->NormalizedRssiList), ((val[0] != '\0') ?  val : "1,2"));
        ERR_CHK(rc);
    }

    if (PsmGet(DMSB_TR181_PSM_WHIX_CliStatList, val, sizeof(val)) != 0)
    {
        rc = strcpy_s(PWiFi_Telemetry->CliStatList, sizeof(PWiFi_Telemetry->CliStatList), "1,2");
        ERR_CHK(rc);
    }
    else
    {
        rc = strcpy_s(PWiFi_Telemetry->CliStatList, sizeof(PWiFi_Telemetry->CliStatList), ((val[0] != '\0') ?  val : "1,2"));
        ERR_CHK(rc);
    }

    if (PsmGet(DMSB_TR181_PSM_WHIX_TxRxRateList, val, sizeof(val)) != 0)
    {
        rc = strcpy_s(PWiFi_Telemetry->TxRxRateList, sizeof(PWiFi_Telemetry->TxRxRateList), "1,2");
        ERR_CHK(rc);
    }
    else
    {
        rc = strcpy_s(PWiFi_Telemetry->TxRxRateList, sizeof(PWiFi_Telemetry->TxRxRateList), ((val[0] != '\0') ?  val : "1,2"));
        ERR_CHK(rc);
    }

    if (PsmGet(DMSB_TR181_PSM_WIFI_TELEMETRY_SNRList, val, sizeof(val)) != 0)
    {
        rc = strcpy_s(PWiFi_Telemetry->SNRList, sizeof(PWiFi_Telemetry->SNRList), "1,2");
        ERR_CHK(rc);
    }
    else
    {
        rc = strcpy_s(PWiFi_Telemetry->SNRList, sizeof(PWiFi_Telemetry->SNRList), ((val[0] != '\0') ?  val : "1,2"));
        ERR_CHK(rc);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiUniqueTelemetryIdInit
  (
	PCOSA_DATAMODEL_RDKB_UNIQUE_TELEMETRY_ID PUniqueTelemetryId
  )
{
    char buf[256] = {0};
    errno_t                         rc              = -1;

    PUniqueTelemetryId->Enable = FALSE;
    memset(PUniqueTelemetryId->TagString, 0, sizeof(PUniqueTelemetryId->TagString));
    PUniqueTelemetryId->TimingInterval = 0;

    if (syscfg_get(NULL, "unique_telemetry_enable", buf, sizeof(buf)) == 0)
    {
            /*CID: 64386 Array compared against 0*/
            PUniqueTelemetryId->Enable = (strcmp(buf,"true") ? FALSE : TRUE);
    }

    memset(buf, 0, sizeof(buf));

    if (syscfg_get(NULL, "unique_telemetry_tag", buf,  sizeof(buf) ) == 0)
    {
            /*CID: 64386 Array compared against 0*/
            rc = STRCPY_S_NOCLOBBER(PUniqueTelemetryId->TagString, sizeof(PUniqueTelemetryId->TagString), buf);
            ERR_CHK(rc);
    }

    memset(buf, 0, sizeof(buf));

    if (syscfg_get( NULL, "unique_telemetry_interval", buf, sizeof(buf)) == 0)
    {
            /*CID: 64386 Array compared against 0*/
            PUniqueTelemetryId->TimingInterval =  atoi(buf);
    }
    
    return ANSC_STATUS_SUCCESS;
}

// Convert time interval(in miniutes) to days, hours and minutes.
void ConvertTime(int time, char day[], char hour[], char mins[]) {
        int d = 0, h = 0, m = 0;
        errno_t rc = -1;

        d = (time / (60*24));
        h = ((time % (60*24)) / 60);
        m = ((time % (60*24)) % 60);

        if(d > 0) {
                rc = sprintf_s(day, MAX_TIME_FORMAT, "*/%d", d);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                rc = sprintf_s(hour, MAX_TIME_FORMAT, "%d", h);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                rc = sprintf_s(mins, MAX_TIME_FORMAT, "%d", m);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
        }
        else if(h > 0) {
                rc = sprintf_s(day, MAX_TIME_FORMAT, "*");
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                rc = sprintf_s(hour, MAX_TIME_FORMAT, "*/%d", h);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                rc = sprintf_s(mins, MAX_TIME_FORMAT, "%d", m);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }

        }
        else {
                rc = sprintf_s(day, MAX_TIME_FORMAT, "*");
                if(rc < EOK)
                {
                     ERR_CHK(rc);
                }
                rc = sprintf_s(hour, MAX_TIME_FORMAT, "*");
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                rc = sprintf_s(mins, MAX_TIME_FORMAT, "*/%d", m);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
        }

}

//Handle UniqueTelemetry Cron Job
void UniqueTelemetryCronJob(BOOL enable, INT timeInterval, char* tagString) {
        char day[MAX_TIME_FORMAT] = {0}, hour[MAX_TIME_FORMAT]={0}, mins[MAX_TIME_FORMAT] = {0};

        if(enable) {       //Add unique_telemetry_id Cron job to job list
            if( timeInterval != 0 && strlen(tagString) > 0) {
                ConvertTime(timeInterval, day, hour, mins);      // Convert time interval
                v_secure_system("( crontab -l | grep -v '/usr/ccsp/pam/unique_telemetry_id.sh' ; echo '%s %s %s * * /usr/ccsp/pam/unique_telemetry_id.sh' ) | crontab -", mins, hour, day);
            }
        }
        else {          //Remove unique_telemetry_id Cron job from job list
            v_secure_system("crontab -l | grep -v '/usr/ccsp/pam/unique_telemetry_id.sh'  | crontab -");
        }
}

ANSC_STATUS
CosaDmlDiUiBrandingInit
  (
	ANSC_HANDLE                 hContext,
	PCOSA_DATAMODEL_RDKB_UIBRANDING	PUiBrand
  )
 {
        
	char *data = NULL;
	char buf[64] = {0};
	cJSON *json = NULL;
	FILE *fileRead = NULL;
	char PartnerID[PARTNER_ID_LEN] = {0};
	ULONG size = PARTNER_ID_LEN - 1;
	int len;
	errno_t rc = -1;
	if (!PUiBrand)
	{
		CcspTraceWarning(("%s-%d : NULL param\n" , __FUNCTION__, __LINE__ ));
		return ANSC_STATUS_FAILURE;	
	}

	memset(PUiBrand, 0, sizeof(COSA_DATAMODEL_RDKB_UIBRANDING));	
	if (access(BOOTSTRAP_INFO_FILE, F_OK) != 0)	
	{
		/*	char cmd[512] = {0};
        snprintf(cmd, sizeof(cmd), "cp %s %s", "/etc/partners_defaults.json", PARTNERS_INFO_FILE);
		CcspTraceWarning(("%s\n",cmd));
		system(cmd);*/
                return ANSC_STATUS_FAILURE;
	}

	 fileRead = fopen( BOOTSTRAP_INFO_FILE, "r" );
	 if( fileRead == NULL ) 
	 {
		 CcspTraceWarning(("%s-%d : Error in opening JSON file\n" , __FUNCTION__, __LINE__ ));
		 return ANSC_STATUS_FAILURE;
	 }
	 
	 fseek( fileRead, 0, SEEK_END );
	 len = ftell( fileRead );
         if (len < 0) {
            fclose( fileRead );
            CcspTraceWarning(("%s-%d : File handle error\n" , __FUNCTION__, __LINE__ ));
            return ANSC_STATUS_FAILURE;
         }
	 fseek( fileRead, 0, SEEK_SET );
	 data = ( char* )malloc( sizeof(char) * (len + 1) );
	 if (data != NULL) 
	 {
		memset( data, 0, ( sizeof(char) * (len + 1) ));
	 	fread( data, 1, len, fileRead );
	 } 
	 else 
	 {
		 CcspTraceWarning(("%s-%d : Memory allocation failed \n", __FUNCTION__, __LINE__));
		 fclose( fileRead );
		 return ANSC_STATUS_FAILURE;
	 }
	 
	 fclose( fileRead );
         /* CID: 135572 String not null terminated*/
	 data[len] = '\0'; 
	 if ( data == NULL )
	 {
		CcspTraceWarning(("%s-%d : fileRead failed \n", __FUNCTION__, __LINE__));
		return ANSC_STATUS_FAILURE;
	 }
	 else if ( strlen(data) != 0)
	 {
		 json = cJSON_Parse( data );
		 if( !json ) 
		 {
			 CcspTraceWarning((  "%s : json file parser error : [%d]\n", __FUNCTION__,__LINE__));
			 free(data);
			 return ANSC_STATUS_FAILURE;
		 } 
		 else 
		 {
			 if(ANSC_STATUS_SUCCESS == fillCurrentPartnerId(PartnerID, &size))
			 {
			 	if ( PartnerID[0] != '\0' )
			 	{
					CcspTraceWarning(("%s : Partner = %s \n", __FUNCTION__, PartnerID));
					FillPartnerIDValues(json, PartnerID, PUiBrand, hContext);
			 	}
				else
				{
					CcspTraceWarning(( "Reading Deafult PartnerID Values \n" ));
					rc = strcpy_s(PartnerID, sizeof(PartnerID), "comcast");
					if(rc != EOK)
					{
						ERR_CHK(rc);
						cJSON_Delete(json);
						free(data);
						return ANSC_STATUS_FAILURE;
					}
					FillPartnerIDValues(json, PartnerID, PUiBrand, hContext);
				}
			}
	 		else{
				CcspTraceWarning(("Failed to get Partner ID\n"));
	 		}
			cJSON_Delete(json);
		}
		free(data);
		data = NULL;
                /* CID: 63746 Array compared against 0*/
		if(!syscfg_get(NULL,"AllowEthernetWAN",buf, sizeof(buf)))
		{
			if (strcmp(buf, "true") == 0)
			{
				PUiBrand->AllowEthernetWAN.ActiveValue = TRUE;
			}
			else
			{
				PUiBrand->AllowEthernetWAN.ActiveValue = FALSE;
			}
		}
	 }
	 else
	 {
		CcspTraceWarning(("BOOTSTRAP_INFO_FILE %s is empty\n", BOOTSTRAP_INFO_FILE));
                /*CID: 71167 Resource leak*/
                if (data)
                    free(data);
		return ANSC_STATUS_FAILURE;
	 }
	 return ANSC_STATUS_SUCCESS;
 }

void FillParamBool(cJSON *partnerObj, char *key, COSA_BOOTSTRAP_BOOL *paramData)
{
    cJSON *paramObj = cJSON_GetObjectItem( partnerObj, key);
    errno_t                         rc              = -1;
    if ( paramObj != NULL )
    {
        cJSON *paramObjVal = cJSON_GetObjectItem(paramObj, "ActiveValue");
        char *valuestr = NULL;
        if (paramObjVal)
            valuestr = paramObjVal->valuestring;
        if (valuestr != NULL)
        {
            paramData->ActiveValue = FALSE;
            if ( strcmp(valuestr,"true") == 0)
            {
                paramData->ActiveValue = TRUE;
                valuestr = NULL;
            }
        }
        else
        {
            CcspTraceWarning(("%s - %s ActiveValue is NULL\n", __FUNCTION__, key ));
        }

        paramObjVal = cJSON_GetObjectItem(paramObj, "UpdateSource");
        if (paramObjVal)
            valuestr = paramObjVal->valuestring;
        if (valuestr != NULL)
        {
            rc = STRCPY_S_NOCLOBBER(paramData->UpdateSource, sizeof(paramData->UpdateSource), valuestr);
            ERR_CHK(rc);
            valuestr = NULL;
        }
        else
        {
            CcspTraceWarning(("%s - %s UpdateSource is NULL\n", __FUNCTION__, key ));
        }
    }
    else
    {
        CcspTraceWarning(("%s - %s Object is NULL\n", __FUNCTION__, key ));
    }
}


void FillParamString(cJSON *partnerObj, char *key, COSA_BOOTSTRAP_STR *paramData)
{
    cJSON *paramObj = cJSON_GetObjectItem( partnerObj, key);
    errno_t                         rc              = -1;
    if ( paramObj != NULL )
    {
        cJSON *paramObjVal = cJSON_GetObjectItem(paramObj, "ActiveValue");
        char *valuestr = NULL;
        if (paramObjVal)
            valuestr = paramObjVal->valuestring;
        if (valuestr != NULL)
        {
            rc = STRCPY_S_NOCLOBBER(paramData->ActiveValue, sizeof(paramData->ActiveValue), valuestr);
            ERR_CHK(rc);
            valuestr = NULL;
        }
        else
        {
            CcspTraceWarning(("%s - %s ActiveValue is NULL\n", __FUNCTION__, key ));
        }

        paramObjVal = cJSON_GetObjectItem(paramObj, "UpdateSource");
        if (paramObjVal)
            valuestr = paramObjVal->valuestring;
        if (valuestr != NULL)
        {
            rc = STRCPY_S_NOCLOBBER(paramData->UpdateSource, sizeof(paramData->UpdateSource), valuestr);
            ERR_CHK(rc);
            valuestr = NULL;
        }
        else
        {
            CcspTraceWarning(("%s - %s UpdateSource is NULL\n", __FUNCTION__, key ));
        }
    }
    else
    {
        CcspTraceWarning(("%s - %s Object is NULL\n", __FUNCTION__, key ));
    }
}

void FillPartnerIDValues(cJSON *json , char *partnerID , PCOSA_DATAMODEL_RDKB_UIBRANDING PUiBrand, ANSC_HANDLE hContext)
{
		cJSON *partnerObj = NULL;
		char buf[64] = {0};
        errno_t rc   = -1;
		
		PCOSA_DATAMODEL_DEVICEINFO pDeviceInfo = (PCOSA_DATAMODEL_DEVICEINFO)hContext;

		partnerObj = cJSON_GetObjectItem( json, partnerID );
		if( partnerObj != NULL) 
		{
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerLink", &PUiBrand->Footer.PartnerLink);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideLink", &PUiBrand->Footer.UserGuideLink);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralLink", &PUiBrand->Footer.CustomerCentralLink);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerText", &PUiBrand->Footer.PartnerText);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideText", &PUiBrand->Footer.UserGuideText);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralText", &PUiBrand->Footer.CustomerCentralText);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOmenu", &PUiBrand->Connection.MSOmenu);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOinfo", &PUiBrand->Connection.MSOinfo);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusTitle", &PUiBrand->Connection.StatusTitle);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusInfo", &PUiBrand->Connection.StatusInfo);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.NetworkDiagnosticTools.ConnectivityTestURL", &PUiBrand->NDiagTool.ConnectivityTestURL);

                                FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.PartnerHelpLink", &PUiBrand->WifiPersonal.PartnerHelpLink);
                                FillParamBool(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.SMSsupport", &PUiBrand->WifiPersonal.SMSsupport);
                                FillParamBool(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.MyAccountAppSupport", &PUiBrand->WifiPersonal.MyAccountAppSupport);
                                FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.MSOLogo", &PUiBrand->LocalUI.MSOLogo);

                                cJSON *paramObj = NULL;
                                cJSON *paramObjVal = NULL;

                                paramObjVal =cJSON_GetObjectItem(cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.Support"), "ActiveValue");
                                if ( paramObjVal != NULL )
                                {
					if ( strcmp(paramObjVal->valuestring, "true") != 0)
					{
						//to prevent traffic being redirected to lan ip
						if(syscfg_set(NULL, "CaptivePortal_Enable", "false") == 0)
                                                {
						   if ( syscfg_commit() == 0)
						   {
							   CcspTraceWarning(("%s - CaptivePortal_Enable set to false\n", __FUNCTION__));
						   }
						   else
						   {
							   CcspTraceWarning(("syscfg_commit failed for CaptivePortal_Enable\n"));
						   }
					        }
						else
						{
						    CcspTraceWarning(("syscfg_set failed for CaptivePortal_Enable\n"))
						}
					}
					FillParamBool(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.Support", &PUiBrand->WifiPersonal.Support);
				}
				else
				{
					CcspTraceWarning(("%s - support Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.DefaultLoginUsername") != NULL )
                                {
					//Check whether this is comcast partner or not
					if( 0 == strcmp( "comcast", partnerID ) )
					{
						syscfg_get(NULL, "user_name_3", PUiBrand->LocalUI.DefaultLoginUsername.ActiveValue, sizeof(PUiBrand->LocalUI.DefaultLoginUsername.ActiveValue));
					}
					else
					{
						FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.DefaultLoginUsername", &PUiBrand->LocalUI.DefaultLoginUsername);
					}
				}
				else
				{
					CcspTraceWarning(("%s - DefaultLoginUsername Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.DefaultLoginPassword") != NULL )
                                {
					//Check whether this is comcast partner or not
					if( 0 == strcmp( "comcast", partnerID ) )
					{
						syscfg_get(NULL, "user_password_3", PUiBrand->LocalUI.DefaultLoginPassword.ActiveValue, sizeof(PUiBrand->LocalUI.DefaultLoginPassword.ActiveValue));
					}
					else if (strstr( partnerID, "sky-" ) != NULL)
					{
						// For Sky, we need to pull the default login from the /tmp/serial.txt file.
						FILE *fp = NULL;
						char DefaultPassword[25] = {0};

						fp = popen("grep 'WIFIPASSWORD' /tmp/serial.txt | cut -d '=' -f 2 | tr -d [:space:]", "r");
						if (fp == NULL)
						{
							CcspTraceWarning(("%s - ERROR Grabbing the default password\n",__FUNCTION__));
						} else {
							fgets(DefaultPassword, sizeof(DefaultPassword), fp);
							pclose(fp);
						}

						if (DefaultPassword[0] != '\0')
						{
                                                        rc = STRCPY_S_NOCLOBBER(PUiBrand->LocalUI.DefaultLoginPassword.ActiveValue, sizeof(PUiBrand->LocalUI.DefaultLoginPassword.ActiveValue), DefaultPassword);
                                                        ERR_CHK(rc);
						}
						else
						{
							CcspTraceWarning(("%s - DefaultLoginPassword Value is NULL\n", __FUNCTION__ ));
						}
					}
					else
					{
						FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.DefaultLoginPassword", &PUiBrand->LocalUI.DefaultLoginPassword);
					}
				}
				else
				{
					CcspTraceWarning(("%s - DefaultLoginPassword Object is NULL\n", __FUNCTION__ ));
				}

				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.MSOLogoTitle", &PUiBrand->LocalUI.MSOLogoTitle);
				FillParamBool(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.HomeNetworkControl", &PUiBrand->LocalUI.HomeNetworkControl);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.MSOLogo", &PUiBrand->WifiPersonal.MSOLogo);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.Title", &PUiBrand->WifiPersonal.Title);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.WelcomeMessage", &PUiBrand->WifiPersonal.WelcomeMessage);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.WelcomeMessage_fre", &PUiBrand->WifiPersonal.WelcomeMessage_fre);

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.DefaultLocalIPv4SubnetRange") != NULL )
                                {
                                        //Check whether this is comcast partner or not
                                        if( 0 == strcmp( "comcast", partnerID ) )
                                        {
                                                syscfg_get(NULL, "lan_netmask", PUiBrand->DefaultLocalIPv4SubnetRange.ActiveValue, sizeof(PUiBrand->DefaultLocalIPv4SubnetRange.ActiveValue));
                                        }
                                        else
                                        {
						FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.DefaultLocalIPv4SubnetRange", &PUiBrand->DefaultLocalIPv4SubnetRange);
                                        }
                                }

				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.DefaultLanguage", &PUiBrand->DefaultLanguage);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.DefaultAdminIP", &PUiBrand->DefaultAdminIP);

				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.PauseScreenFileLocation", &PUiBrand->PauseScreenFileLocation);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.HelpTip.NetworkName", &PUiBrand->HelpTip.NetworkName);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.brandname", &PUiBrand->CloudUI.brandname);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.productname", &PUiBrand->CloudUI.productname);
				FillParamString(partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.link", &PUiBrand->CloudUI.link);

                                paramObj = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.AllowEthernetWAN");
                                if ( paramObj != NULL )
                                {
					char *AllowEthernetWAN = NULL;
                                        paramObjVal = cJSON_GetObjectItem(paramObj, "ActiveValue");
                                        if (paramObjVal)
                                                AllowEthernetWAN = paramObjVal->valuestring;

					if (AllowEthernetWAN != NULL)
					{
						syscfg_get( NULL, "AllowEthernetWAN", buf, sizeof(buf));
						if (buf[0] == '\0')
						{
							if (syscfg_set(NULL, "AllowEthernetWAN", AllowEthernetWAN) == 0)
							{
							   if (syscfg_commit() == 0)
							   {
								CcspTraceWarning(("%s - AllowEthernetWAN is %s\n", __FUNCTION__,AllowEthernetWAN));
							   }
							   else
							   {
								CcspTraceWarning(("syscfg_commit failed for AllowEthernetWAN\n"));
							   }
							}
                                                        else
                                                        {
                                                            CcspTraceWarning(("syscfg_set failed for AllowEthernetWAN\n"));
                                                        }
							AllowEthernetWAN = NULL;
						}
						else
						{
							CcspTraceWarning(("%s - AllowEthernetWAN is NULL\n", __FUNCTION__ ));
						}
					}
					char *valuestr = NULL;
					paramObjVal = cJSON_GetObjectItem(paramObj, "UpdateSource");
				        if (paramObjVal)
				            valuestr = paramObjVal->valuestring;
				        if (valuestr != NULL)
				        {
                                            rc = STRCPY_S_NOCLOBBER(PUiBrand->AllowEthernetWAN.UpdateSource, sizeof(PUiBrand->AllowEthernetWAN.UpdateSource), valuestr);
                                            ERR_CHK(rc);
				            valuestr = NULL;
				        }
				        else
				        {
				            CcspTraceWarning(("%s - AllowEthernetWAN.UpdateSource is NULL\n", __FUNCTION__ ));
				        }
				}
				else
				{
					CcspTraceWarning(("%s - AllowEthernetWAN Object is NULL\n", __FUNCTION__ ));
				}

                                paramObj = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SyndicationFlowControl.Enable");
                                if ( paramObj != NULL )
                                {
                                        char *valuestr = NULL;
                                        paramObjVal = cJSON_GetObjectItem(paramObj, "UpdateSource");
                                        if (paramObjVal)
                                            valuestr = paramObjVal->valuestring;
                                        if (valuestr != NULL)
                                        {
                                            rc = STRCPY_S_NOCLOBBER(pDeviceInfo->SyndicatonFlowControl.Enable.UpdateSource, sizeof(pDeviceInfo->SyndicatonFlowControl.Enable.UpdateSource), valuestr);
                                            ERR_CHK(rc);
                                            valuestr = NULL;
                                        }
                                        else
                                        {
                                            CcspTraceWarning(("%s - SyndicationFlowControl.Enable.UpdateSource is NULL\n", __FUNCTION__ ));
                                        }
                                }
                                else
                                {
                                        CcspTraceWarning(("%s - SyndicationFlowControl.Enable Object is NULL\n", __FUNCTION__ ));
                                }

                                paramObj = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SyndicationFlowControl.InitialForwardedMark");
                                if ( paramObj != NULL )
                                {
					char *InitialForwardedMark = NULL;
                                        paramObjVal = cJSON_GetObjectItem(paramObj, "ActiveValue");
                                        if (paramObjVal)
                                                InitialForwardedMark = paramObjVal->valuestring;

					if (InitialForwardedMark != NULL)
					{
						syscfg_get( NULL, "DSCP_InitialForwardedMark", buf, sizeof(buf));
						if (buf[0] == '\0')
						{
							if(syscfg_set(NULL, "DSCP_InitialForwardedMark", InitialForwardedMark) == 0)
							{
							    if(syscfg_commit() == 0)
							    {
								    CcspTraceWarning(("%s - InitialForwardedMark is %s\n", __FUNCTION__,InitialForwardedMark));
							    }
							    else
							    {
								    CcspTraceWarning(("syscfg_commit failed for InitialForwardedMark\n"));		 
							    }
							}
							else
							{
							    CcspTraceWarning(("syscfg_set failed for InitialForwardedMark\n"));
							}
							InitialForwardedMark = NULL;
						}
						else
						{
							CcspTraceWarning(("%s - InitialForwardedMark is NULL\n", __FUNCTION__ ));
						}
					}

                                        char *valuestr = NULL;
                                        paramObjVal = cJSON_GetObjectItem(paramObj, "UpdateSource");
                                        if (paramObjVal)
                                            valuestr = paramObjVal->valuestring;
                                        if (valuestr != NULL)
                                        {
                                            rc = STRCPY_S_NOCLOBBER(pDeviceInfo->SyndicatonFlowControl.InitialForwardedMark.UpdateSource, sizeof(pDeviceInfo->SyndicatonFlowControl.InitialForwardedMark.UpdateSource), valuestr);
                                            ERR_CHK(rc);
                                            valuestr = NULL;
                                        }
                                        else
                                        {
                                            CcspTraceWarning(("%s - InitialForwardedMark.UpdateSource is NULL\n", __FUNCTION__ ));
                                        }
				}
				else
				{
					CcspTraceWarning(("%s - InitialForwardedMark Object is NULL\n", __FUNCTION__ ));
				}

                                paramObj = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SyndicationFlowControl.InitialOutputMark");
                                if ( paramObj != NULL )
                                {
					char *InitialOutputMark = NULL;
                                        paramObjVal = cJSON_GetObjectItem(paramObj, "ActiveValue");
                                        if (paramObjVal)
                                                InitialOutputMark = paramObjVal->valuestring;

					if (InitialOutputMark != NULL)
					{
						syscfg_get( NULL, "DSCP_InitialOutputMark", buf, sizeof(buf));
						if (buf[0] == '\0')
						{
							syscfg_set(NULL, "DSCP_InitialOutputMark", InitialOutputMark);
							syscfg_commit();
							CcspTraceWarning(("%s - InitialOutputMark is %s\n", __FUNCTION__,InitialOutputMark));
							InitialOutputMark = NULL;
						}
						else
						{
							CcspTraceWarning(("%s - InitialOutputMark is NULL\n", __FUNCTION__ ));
						}
					}

                                        char *valuestr = NULL;
                                        paramObjVal = cJSON_GetObjectItem(paramObj, "UpdateSource");
                                        if (paramObjVal)
                                            valuestr = paramObjVal->valuestring;
                                        if (valuestr != NULL)
                                        {
                                            rc = STRCPY_S_NOCLOBBER(pDeviceInfo->SyndicatonFlowControl.InitialOutputMark.UpdateSource, sizeof(pDeviceInfo->SyndicatonFlowControl.InitialOutputMark.UpdateSource), valuestr);
                                            ERR_CHK(rc);
                                            valuestr = NULL;
                                        }
                                        else
                                        {
                                            CcspTraceWarning(("%s - InitialOutputMark.UpdateSource is NULL\n", __FUNCTION__ ));
                                        }
				}
				else
				{
					CcspTraceWarning(("%s - InitialOutputMark Object is NULL\n", __FUNCTION__ ));
				}

                                paramObj = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.TR69CertLocation");
                                if ( paramObj != NULL )
                                {
                                        char *valuestr = NULL;
                                        paramObjVal = cJSON_GetObjectItem(paramObj, "UpdateSource");
                                        if (paramObjVal)
                                            valuestr = paramObjVal->valuestring;
                                        if (valuestr != NULL)
                                        {
                                            rc = STRCPY_S_NOCLOBBER(pDeviceInfo->TR69CertLocation.UpdateSource, sizeof(pDeviceInfo->TR69CertLocation.UpdateSource), valuestr);
                                            ERR_CHK(rc);
                                            valuestr = NULL;
                                        }
                                        else
                                        {
                                            CcspTraceWarning(("%s - TR69CertLocation.UpdateSource is NULL\n", __FUNCTION__ ));
                                        }
                                }
                                else
                                {
                                        CcspTraceWarning(("%s - TR69CertLocation Object is NULL\n", __FUNCTION__ ));
                                }

				//if WANsideSSH_Enable param  is not available in syscfg
				//then read it from partners_defaults.json
				pDeviceInfo->bWANsideSSHEnable.ActiveValue =  FALSE;
				if (syscfg_get( NULL, "WANsideSSH_Enable", buf, sizeof(buf)) == 0)
				{
					pDeviceInfo->bWANsideSSHEnable.ActiveValue = strcmp(buf, "true") == 0 ? TRUE : FALSE;
				}
				else
                                {
					paramObj = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.WANsideSSH.Enable");
					if ( paramObj != NULL )
					{
                                        	char *WANsideSSH_Enable = NULL;
                                                paramObjVal = cJSON_GetObjectItem(paramObj, "ActiveValue");
                                                if (paramObjVal)
                                     		    WANsideSSH_Enable = paramObjVal->valuestring;

                                  	        if (WANsideSSH_Enable != NULL)
                                                {
							pDeviceInfo->bWANsideSSHEnable.ActiveValue = strcmp(WANsideSSH_Enable, "true") == 0 ? TRUE : FALSE;
							WANsideSSH_Enable = NULL;
                                        	}
                                        	else
                                        	{
                                                	CcspTraceWarning(("%s - WANsideSSH_Enable Value is NULL\n", __FUNCTION__ ));
                                        	}

                                                char *valuestr = NULL;
                                                paramObjVal = cJSON_GetObjectItem(paramObj, "UpdateSource");
                                                if (paramObjVal)
                                                    valuestr = paramObjVal->valuestring;
                                                if (valuestr != NULL)
                                                {
                                                    rc = STRCPY_S_NOCLOBBER(pDeviceInfo->bWANsideSSHEnable.UpdateSource, sizeof(pDeviceInfo->bWANsideSSHEnable.UpdateSource), valuestr);
                                                    ERR_CHK(rc);
                                                    valuestr = NULL;
                                                }
                                                else
                                                {
                                                    CcspTraceWarning(("%s - bWANsideSSHEnable.UpdateSource is NULL\n", __FUNCTION__ ));
                                                }
                                	}
                                	else
                                	{
                                        	CcspTraceWarning(("%s - WANsideSSH_Enable Object is NULL\n", __FUNCTION__ ));
                                	}
				}

				if (pDeviceInfo->bWANsideSSHEnable.ActiveValue ==  TRUE)
				{
					CcspTraceWarning(("%s - Enabling SSH on WAN side\n", __FUNCTION__ ));
					v_secure_system("sh /lib/rdk/wan_ssh.sh enable &");
				}
				else
				{
					CcspTraceWarning(("%s -Disabling SSH on WAN side\n", __FUNCTION__ ));
					v_secure_system("sh /lib/rdk/wan_ssh.sh disable &");
				}

#if defined(_COSA_BCM_ARM_) && !defined(_CBR_PRODUCT_REQ_) && !defined(_PLATFORM_RASPBERRYPI_) && !defined(_ENABLE_DSL_SUPPORT_)
                                paramObjVal = cJSON_GetObjectItem(cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.CMVoiceImageSelect"), "ActiveValue");
                                if ( paramObjVal != NULL )
                                {
					char *CMVoiceImg = NULL;
					char platform_info[64] = {0};
					CMVoiceImg = paramObjVal->valuestring;
					if (CMVoiceImg != NULL)
					{
						CcspTraceInfo(("%s CM variant returned from partner_defaults.json: %s\n", __FUNCTION__, CMVoiceImg));
						if(ANSC_STATUS_FAILURE == setCMVoiceImg(CMVoiceImg))
						{
							CcspTraceError(("%s - Failed Set for CMVoiceImg\n", __FUNCTION__ ));
						}
						if (platform_hal_getFactoryCmVariant(platform_info) != RETURN_OK)
						{
							CcspTraceError(("%s Unable to fetch CM Variant from platform\n", __FUNCTION__));
						}
						CcspTraceInfo(("%s CM variant returned by platform: %s\n", __FUNCTION__, platform_info));
						if (strcmp(CMVoiceImg, platform_info) != 0)
						{
							CcspTraceWarning(("%s - CM Image doesn't match.Setting %s\n", __FUNCTION__, CMVoiceImg));
							if (platform_hal_setFactoryCmVariant(CMVoiceImg) == RETURN_OK)
							{
								CcspTraceInfo(("%s CM variant set to %s. Intiating reboot..\n", __FUNCTION__, CMVoiceImg));
								v_secure_system("sh /lib/rdk/reboot_CMchange.sh");
							}
							else
							{
								CcspTraceError(("%s Unable to set CM Variant %s\n", __FUNCTION__, CMVoiceImg));
							}
						}
					}
					else
					{
						CcspTraceWarning(("%s - CMVoiceImageSelect Value is NULL\n", __FUNCTION__ ));
					}
				}
				else
				{
					CcspTraceWarning(("%s - CMVoiceImageSelect Object is NULL\n", __FUNCTION__ ));
				}
#endif
			}
			else
			{
				CcspTraceWarning(("%s - PARTNER ID OBJECT Value is NULL\n", __FUNCTION__ ));
			}

}

ANSC_STATUS UpdateJsonParamLegacy
	(
		char*                       pKey,
		char*			PartnerId,
		char*			pValue
    )
{
	cJSON *partnerObj = NULL;
	cJSON *json = NULL;
	FILE *fileRead = NULL;
	char * cJsonOut = NULL;
	char* data = NULL;
	 int len ;
	 int configUpdateStatus = -1;
	 fileRead = fopen( PARTNERS_INFO_FILE, "r" );
	 if( fileRead == NULL ) 
	 {
		 CcspTraceWarning(("%s-%d : Error in opening JSON file\n" , __FUNCTION__, __LINE__ ));
		 return ANSC_STATUS_FAILURE;
	 }
	 
	 fseek( fileRead, 0, SEEK_END );
	 len = ftell( fileRead );
         /* CID: 55623 Argument cannot be negative*/
         if(len < 0) {
               CcspTraceWarning(("%s-%d : Error in file handle\n" , __FUNCTION__, __LINE__ ));
               fclose( fileRead );
               return ANSC_STATUS_FAILURE;
         }
	 fseek( fileRead, 0, SEEK_SET );
	 data = ( char* )malloc( sizeof(char) * (len + 1) );
	 if (data != NULL) 
	 {
		memset( data, 0, ( sizeof(char) * (len + 1) ));
	 	fread( data, 1, len, fileRead );
	 } 
	 else 
	 {
		 CcspTraceWarning(("%s-%d : Memory allocation failed \n", __FUNCTION__, __LINE__));
		 fclose( fileRead );
		 return ANSC_STATUS_FAILURE;
	 }
	 
	 fclose( fileRead );
         /* CID: 135238 String not null terminated*/
         data[len]='\0';
	 if ( data == NULL )
	 {
		CcspTraceWarning(("%s-%d : fileRead failed \n", __FUNCTION__, __LINE__));
		return ANSC_STATUS_FAILURE;
	 }
	 else if ( strlen(data) != 0)
	 {
		 json = cJSON_Parse( data );
		 if( !json ) 
		 {
			 CcspTraceWarning((  "%s : json file parser error : [%d]\n", __FUNCTION__,__LINE__));
			 free(data);
			 return ANSC_STATUS_FAILURE;
		 } 
		 else
		 {
			 partnerObj = cJSON_GetObjectItem( json, PartnerId );
			 if ( NULL != partnerObj)
			 {
				 if (NULL != cJSON_GetObjectItem( partnerObj, pKey) )
				 {
					 cJSON_ReplaceItemInObject(partnerObj, pKey, cJSON_CreateString(pValue));
					 cJsonOut = cJSON_Print(json);
					 CcspTraceWarning(( "Updated json content is %s\n", cJsonOut));
					 configUpdateStatus = writeToJson(cJsonOut, PARTNERS_INFO_FILE);
                                         free(cJsonOut);
					 if ( !configUpdateStatus)
					 {
						 CcspTraceWarning(( "Updated Value for %s partner\n",PartnerId));
						 CcspTraceWarning(( "Param:%s - Value:%s\n",pKey,pValue));
					 }
					 else
				 	{
						 CcspTraceWarning(( "Failed to update value for %s partner\n",PartnerId));
						 CcspTraceWarning(( "Param:%s\n",pKey));
			 			 cJSON_Delete(json);
						 return ANSC_STATUS_FAILURE;						
				 	}
				 }
				else
			 	{
			 		CcspTraceWarning(("%s - OBJECT  Value is NULL %s\n", pKey,__FUNCTION__ ));
			 		cJSON_Delete(json);
			 		return ANSC_STATUS_FAILURE;
			 	}
			 
			 }
			 else
			 {
			 	CcspTraceWarning(("%s - PARTNER ID OBJECT Value is NULL\n", __FUNCTION__ ));
			 	cJSON_Delete(json);
			 	return ANSC_STATUS_FAILURE;
			 }
			cJSON_Delete(json);
		 }
	  }
	  else
	  {
		CcspTraceWarning(("PARTNERS_INFO_FILE %s is empty\n", PARTNERS_INFO_FILE));
                /* CID: 65542 Resource leak*/
                if (data)
                   free(data);
		return ANSC_STATUS_FAILURE;
	  }
	 return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS UpdateJsonParam
	(
		char*                       pKey,
		char*			PartnerId,
		char*			pValue,
		char*			pSource,
		char*			pCurrentTime
    )
{
	cJSON *partnerObj = NULL;
	cJSON *json = NULL;
	FILE *fileRead = NULL;
	char * cJsonOut = NULL;
	char* data = NULL;
	 int len ;
	 int configUpdateStatus = -1;
	 fileRead = fopen( BOOTSTRAP_INFO_FILE, "r" );
	 if( fileRead == NULL ) 
	 {
		 CcspTraceWarning(("%s-%d : Error in opening JSON file\n" , __FUNCTION__, __LINE__ ));
		 return ANSC_STATUS_FAILURE;
	 }
	 
	 fseek( fileRead, 0, SEEK_END );
	 len = ftell( fileRead );
         /* CID: 56120 Argument cannot be negative*/
         if (len < 0) {
             CcspTraceWarning(("%s-%d : Error in File handle\n" , __FUNCTION__, __LINE__ ));
             fclose( fileRead );
             return ANSC_STATUS_FAILURE;
         }
	 fseek( fileRead, 0, SEEK_SET );
	 data = ( char* )malloc( sizeof(char) * (len + 1) );
	 if (data != NULL) 
	 {
		memset( data, 0, ( sizeof(char) * (len + 1) ));
	 	fread( data, 1, len, fileRead );
	 } 
	 else 
	 {
		 CcspTraceWarning(("%s-%d : Memory allocation failed \n", __FUNCTION__, __LINE__));
		 fclose( fileRead );
		 return ANSC_STATUS_FAILURE;
	 }
	 
	 fclose( fileRead );
         /* CID:135285 String not null terminated*/
         data[len] = '\0';
	 if ( data == NULL )
	 {
		CcspTraceWarning(("%s-%d : fileRead failed \n", __FUNCTION__, __LINE__));
		return ANSC_STATUS_FAILURE;
	 }
	 else if ( strlen(data) != 0)
	 {
		 json = cJSON_Parse( data );
		 if( !json ) 
		 {
			 CcspTraceWarning((  "%s : json file parser error : [%d]\n", __FUNCTION__,__LINE__));
			 free(data);
			 return ANSC_STATUS_FAILURE;
		 } 
		 else
		 {
			 partnerObj = cJSON_GetObjectItem( json, PartnerId );
			 if ( NULL != partnerObj)
			 {
                                 cJSON *paramObj = cJSON_GetObjectItem( partnerObj, pKey);
                                 if (NULL != paramObj )
                                 {
                                         cJSON_ReplaceItemInObject(paramObj, "ActiveValue", cJSON_CreateString(pValue));
                                         cJSON_ReplaceItemInObject(paramObj, "UpdateTime", cJSON_CreateString(pCurrentTime));
                                         cJSON_ReplaceItemInObject(paramObj, "UpdateSource", cJSON_CreateString(pSource));

					 cJsonOut = cJSON_Print(json);
					 CcspTraceWarning(( "Updated json content is %s\n", cJsonOut));
					 configUpdateStatus = writeToJson(cJsonOut, BOOTSTRAP_INFO_FILE);
                                         free(cJsonOut);
					 if ( !configUpdateStatus)
					 {
                                         	 CcspTraceWarning(( "Bootstrap config update: %s, %s, %s, %s \n", pKey, pValue, PartnerId, pSource));
					 }
					 else
				 	{
						 CcspTraceWarning(( "Failed to update value for %s partner\n",PartnerId));
						 CcspTraceWarning(( "Param:%s\n",pKey));
			 			 cJSON_Delete(json);
						 return ANSC_STATUS_FAILURE;						
				 	}
				 }
				else
			 	{
			 		CcspTraceWarning(("%s - OBJECT  Value is NULL %s\n", pKey,__FUNCTION__ ));
			 		cJSON_Delete(json);
			 		return ANSC_STATUS_FAILURE;
			 	}
			 
			 }
			 else
			 {
			 	CcspTraceWarning(("%s - PARTNER ID OBJECT Value is NULL\n", __FUNCTION__ ));
			 	cJSON_Delete(json);
			 	return ANSC_STATUS_FAILURE;
			 }
			cJSON_Delete(json);
		 }
	  }
	  else
	  {
		CcspTraceWarning(("BOOTSTRAP_INFO_FILE %s is empty\n", BOOTSTRAP_INFO_FILE));
		return ANSC_STATUS_FAILURE;
	  }

	 //Also update in the legacy file /nvram/partners_defaults.json for firmware roll over purposes.
         UpdateJsonParamLegacy(pKey, PartnerId, pValue);

	 return ANSC_STATUS_SUCCESS;
}

static int writeToJson(char *data, char *file)
{
    if (file == NULL || data == NULL)
    {
        CcspTraceWarning(("%s : %d Invalid input parameter", __FUNCTION__,__LINE__));
        return -1;
    }
    FILE *fp;
    fp = fopen(file, "w");
    if (fp == NULL ) 
    {
        CcspTraceWarning(("%s : %d Failed to open file %s\n", __FUNCTION__,__LINE__,file));
        return -1;
    }

    fwrite(data, strlen(data), 1, fp);
    fclose(fp);
    return 0;
}

#ifndef FEATURE_FWUPGRADE_MANAGER
void CosaDmlDiGet_DeferFWDownloadReboot(ULONG* puLong)
{
	char buf[8] = { 0 };

	if( 0 == syscfg_get( NULL, "DeferFWDownloadReboot", buf, sizeof( buf ) ) )
	{
		*puLong = atoi(buf);
	}
	else
	{
        CcspTraceWarning(("syscfg_get failed\n")); 
	}

}


void CosaDmlDiSet_DeferFWDownloadReboot(ULONG* DeferFWDownloadReboot , ULONG uValue)
{
	char buf[8] = { 0 };
	errno_t rc = -1;
	
	rc = sprintf_s(buf, sizeof(buf), "%lu", uValue);
	if(rc < EOK)
	{
		ERR_CHK(rc);
	}
	if ( syscfg_set( NULL,"DeferFWDownloadReboot",buf)!= 0 ) 
	{
		CcspTraceWarning(("syscfg_set failed\n"));
	}
	else 
	{
		if ( syscfg_commit( ) != 0 ) 
		{
			CcspTraceWarning(("syscfg_commit failed\n")); 
		}
		else
		{
			*DeferFWDownloadReboot = 	uValue;
		}
	}
}
#endif

void* DisableRemoteManagement_thread(void* arg)
{
    UNREFERENCED_PARAMETER(arg);
    int                         ret = -1;
    int                         size = 0;
    componentStruct_t **        ppComponents = NULL;
    char*   faultParam = NULL;
    extern char        g_Subsystem[32];
    char   dst_pathname_cr[64]  =  {0};
    errno_t safec_rc = -1;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    pthread_detach(pthread_self());
	
    safec_rc = sprintf_s(dst_pathname_cr, sizeof(dst_pathname_cr), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(safec_rc < EOK)
    {
        ERR_CHK(safec_rc);
    }

    ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
                dst_pathname_cr,
                "Device.UserInterface.X_CISCO_COM_RemoteAccess.",
                g_Subsystem,        /* prefix */
                &ppComponents,
                &size);

    if ( ret == CCSP_SUCCESS && size == 1)
    {
	parameterValStruct_t val[ ] = { { "Device.UserInterface.X_CISCO_COM_RemoteAccess.HttpEnable", "false", ccsp_boolean},{ "Device.UserInterface.X_CISCO_COM_RemoteAccess.HttpsEnable", "false", ccsp_boolean} };
	ret = CcspBaseIf_setParameterValues
                    (
                     bus_handle,
                     ppComponents[0]->componentName,
                     ppComponents[0]->dbusPath,
                     0, 0x0,  
                     val,
                     2,
                     TRUE,   
                     &faultParam
                    );
	if (ret != CCSP_SUCCESS && faultParam)
        {   
          	AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                bus_info->freefunc(faultParam);
        }

        free_componentStruct_t(bus_handle, size, ppComponents);
        CcspTraceInfo(("Successfully set for Disable Remote Management \n"));
    }
    return NULL;
}

void* RebootDevice_thread(void* buff)
{
	char pValue[128],source_str[64];
	char* source = NULL;
	int router, wifi, voip, dect, moca, all;
    int delay_time = 0;
	errno_t rc = -1;

	pthread_detach(pthread_self());
	
	memset(pValue,0,sizeof(pValue));
	if(buff)
	{
		rc = strcpy_s(pValue, sizeof(pValue), buff);
		ERR_CHK(rc);
		free(buff);		
	}

    router = wifi = voip = dect = moca = all = 0;
    if (strcasestr(pValue, "Router")) {
        router = 1;
    }
    if (strcasestr(pValue, "Wifi")) {
        wifi = 1;
    }
    if (strcasestr(pValue, "VoIP")) {
        voip = 1;
    }
    if (strcasestr(pValue, "Dect")) {
        dect = 1;
    }
    if (strcasestr(pValue, "MoCA")) {
        moca = 1;
    }
    if (strcasestr(pValue, "Device")) {
        all = 1;
    }
	
    if (strcasestr(pValue, "delay=")) {
        delay_time = atoi((const char*)strcasestr(pValue, "delay=") + strlen("delay="));
    }
	
	if(strcasestr(pValue, "source=")){
		source = strcasestr(pValue, "source=") + strlen("source=");
		int i=0;
		while(source[i] != ' ' && source[i] != '\0'){
			source_str[i] = source[i];
			i++;
		}
		source_str[i] = '\0';
	}
	else{
		rc = strcpy_s(source_str, sizeof(source_str), "webpa-reboot");
		ERR_CHK(rc);
	}

	CcspTraceInfo(("reboot source - %s\n",source_str));
	
	if(!router && !wifi && !voip && !dect && !moca && !all){
		all = 1;
	}
	
    if (router && wifi && voip && dect && moca) {
        all = 1;
    }
	
	if(delay_time)
	{
		CcspTraceInfo(("Sleeping for %d seconds before reboot\n",delay_time));
		sleep (delay_time);
	}
		
    if (all) {

		char buf[7] = {0};
		int rebootcount = 0;
    	syscfg_get( NULL, "reboot_count", buf, sizeof(buf));
		rebootcount = atoi(buf);
		rebootcount++;
		memset(buf,0,sizeof(buf));
		rc = sprintf_s(buf,sizeof(buf),"%d",rebootcount);
		if(rc < EOK) ERR_CHK(rc);
		syscfg_set(NULL, "reboot_count", buf);

		FILE *fp = NULL;
		char buffer[50] = {0};
		memset(buffer,0,sizeof(buffer));
        fp = v_secure_popen("r", "date");
		if( fp != NULL) {         
		    while(fgets(buffer, sizeof(buffer), fp)!=NULL){
			    buffer[strlen(buffer) - 1] = '\0';
				syscfg_set(NULL, "latest_reboot_time", buffer);
			}
			v_secure_pclose(fp);
		}

		char tmp[7] = {0};
		syscfg_get(NULL, "restore_reboot", tmp, sizeof(tmp));

		if(strcmp(tmp,"true") != 0)
		{
			if (syscfg_commit() != 0)
			{
				CcspTraceWarning(("syscfg_commit failed\n"));
			}
		}
		else
		{
			CcspTraceWarning(("RebootDevice:Device is going to reboot to restore configuration \n"));
		}
		setLastRebootReason(source_str);
		CcspTraceWarning(("REBOOT_COUNT : %d Time : %s  \n",rebootcount,buffer));
		CcspTraceWarning(("RebootDevice:Device is going to reboot after taking log backups \n"));
		CosaDmlDcSaveWiFiHealthStatusintoNVRAM( );
		v_secure_system("/rdklogger/backupLogs.sh");
		return NULL;
    }

    if (router) {
        fprintf(stderr, "Router is going to reboot\n");
		CcspTraceWarning(("RebootDevice:Router is going to reboot\n"));

		pthread_t tid2;
		pthread_create(&tid2, NULL, &CosaDmlDcRestartRouter, NULL); 
    }

    if (wifi) {
		fprintf(stderr, "WiFi is going to reboot\n");
		CcspTraceWarning(("RebootDevice:WiFi is going to reboot\n"));
		
		pthread_t tid;
		pthread_create(&tid, NULL, &CosaDmlDcRebootWifi, NULL);
    }
    
    if (voip) {
        fprintf(stderr, "VoIP is going to reboot\n");
        // TODO: 
    }
    if (dect) {
        fprintf(stderr, "Dect is going to reboot\n");
        // TODO: 
    }
    if (moca) {
        fprintf(stderr, "MoCA is going to reboot\n");
        // TODO: 
    }
    return NULL;
	
}

void CosaDmlDiSet_DisableRemoteManagement()
{
	pthread_t tid;
    	pthread_create(&tid, NULL, &DisableRemoteManagement_thread, NULL);
        CcspTraceInfo(("Successfully pthread created for Disable Remote Management \n"));
}

void CosaDmlDiSet_RebootDevice(char* pValue)
{
	pthread_t tid;
	errno_t rc = -1;

	char* buff = (char*) malloc(strlen(pValue)+1);
	rc = strcpy_s(buff, strlen(pValue)+1, pValue);
	ERR_CHK(rc);
	pthread_create(&tid, NULL, &RebootDevice_thread, (void*) buff); 
    
}

#ifndef FEATURE_FWUPGRADE_MANAGER
static void*
FirmwareDownloadAndFactoryReset(void* arg)
{
    UNREFERENCED_PARAMETER(arg);
    FILE *fp;
    char URL[256]={0};
    char Imagename[256]={0};
    char line[512];
    char *token;
    char *val;
    errno_t rc = -1;
    if((fp = fopen("/tmp/FactoryReset.txt", "r")) == NULL)
    {
        CcspTraceInfo(( "/tmp/FactoryReset.txt doesnot exist go for snmp reboot .\n"));
        if( RETURN_ERR == cm_hal_FWupdateAndFactoryReset( NULL, NULL ))
        {
            commonSyseventSet("fw_update_inprogress", "false");
            CcspTraceError(("FirmwareDownloadAndFactoryReset :cm_hal_FWupdateAndFactoryReset failed\n"));
        }
    }
    else
    {
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            token=strtok(line,"=");
            if(token != NULL)
            {
                val = strtok(NULL, "=");
                if( NULL != val )
                {
                    int new_line = strlen(val) -1;
                    if (val[new_line] == '\n')
                    val[new_line] = '\0';
                    if(0 == strcmp(token,"Url"))
                    {
                        rc = strcpy_s(URL, sizeof(URL), val);
                        if(rc != EOK)
                        {
                            ERR_CHK(rc);
                            commonSyseventSet("fw_update_inprogress", "false");
                            CcspTraceError(("FirmwareDownloadAndFactoryReset: failed to copy url"));
                            fclose(fp);
                            return NULL;
                        }
                    }
                    else if(0 == strcmp(token,"Image"))
                    {
                        rc = strcpy_s(Imagename, sizeof(Imagename), val);
                        if(rc != EOK)
                        {
                            ERR_CHK(rc);
                            commonSyseventSet("fw_update_inprogress", "false");
                            CcspTraceError(("FirmwareDownloadAndFactoryReset: failed to copy image name"));
                            fclose(fp);
                            return NULL;
                        }
                    }
                }
            }
        }
        fclose(fp);
        CcspTraceWarning(("%s: ImageName %s, url %s\n", __FUNCTION__, Imagename, URL));
        if( RETURN_ERR == cm_hal_FWupdateAndFactoryReset( URL, Imagename ))
        {
            CcspTraceError(("FirmwareDownloadAndFactoryReset :cm_hal_FWupdateAndFactoryReset failed\n"));
            commonSyseventSet("fw_update_inprogress", "false");
            v_secure_system("rm -rf /tmp/FactoryReset.txt");
        }
    }
    return NULL;
}

ANSC_STATUS
CosaDmlDiSetFirmwareDownloadAndFactoryReset()
{
    pthread_t tid;
    char evtValue[64] = {0};
    
    commonSyseventGet("fw_update_inprogress", evtValue, sizeof(evtValue));
     if (0 == strncmp(evtValue, "true", strlen("true")))
    {
        CcspTraceWarning(("FirmwareDownloadAndFactoryReset already in progress\n"))
        return ANSC_STATUS_FAILURE;
    }
    commonSyseventSet("fw_update_inprogress", "true");
    CcspTraceWarning(("Calling FirmwareDownloadAndFactoryReset thread\n"));
    pthread_create(&tid, NULL, &FirmwareDownloadAndFactoryReset, NULL);

    return ANSC_STATUS_SUCCESS;
}
#endif //FEATURE_RDKB_WAN_MANAGER

BOOL
CosaDmlDi_ValidateRebootDeviceParam( char *pValue )
{
	BOOL IsProceedFurther	= FALSE,
		  IsActionValid 	= FALSE,
		  IsSourceValid 	= FALSE,
		  IsDelayValid		= FALSE;
        char *st = NULL;
	CcspTraceWarning(("%s %d - String :%s", __FUNCTION__, __LINE__, ( pValue != NULL ) ?  pValue : "NULL" ));

        /*CID: 61097 Dereference before null check*/
        if (!pValue)
            return FALSE;
        
	if (strcasestr(pValue, "delay=")) {
		IsDelayValid = TRUE;
	}

	if(strcasestr(pValue, "source=")) {
		IsSourceValid = TRUE;
	}

	if (strcasestr(pValue, "Router")) {
		IsActionValid = TRUE;
	}

	if (strcasestr(pValue, "Wifi")) {
		IsActionValid = TRUE;
	}

	if (strcasestr(pValue, "VoIP")) {
		IsActionValid = TRUE;
	}

	if (strcasestr(pValue, "Dect")) {
		IsActionValid = TRUE;
	}

	if (strcasestr(pValue, "MoCA")) {
		IsActionValid = TRUE;
	}

	if (strcasestr(pValue, "Device")) {
		IsActionValid = TRUE;
	}
	if ( ( NULL != pValue )  && ( strlen( pValue )	== 0 ) )
	{
		IsProceedFurther = TRUE;
	}
	else if( IsActionValid && ( IsSourceValid || IsDelayValid ) )
	{
		IsProceedFurther = TRUE;
	}
	else if (  IsActionValid )
	{
		IsProceedFurther = TRUE;
	}
	else if( IsSourceValid || IsDelayValid )
	{
		if(  ( !IsSourceValid ) && IsDelayValid ) 
		{
			char   tmpCharBuffer [ 256 ] = { 0 };
			errno_t rc = -1;
			char *subStringForDelay  = NULL,
			     *subStringForDummy  = NULL;

			rc = strcpy_s( tmpCharBuffer, sizeof(tmpCharBuffer), pValue );
			if(rc != EOK)
			{
				ERR_CHK(rc);
				return FALSE;
			}
			subStringForDelay       = strtok_r( tmpCharBuffer, " ", &st );
			subStringForDummy   = strtok_r( NULL, " ", &st );

                        /* CID: 74460 Dereference before null check*/
                        if(!subStringForDelay)
                           return FALSE;
			if ( strcasestr(subStringForDelay, "delay="))
			{
				if ( subStringForDummy != NULL )
				{
					IsProceedFurther = FALSE;
				}
				else
				{
					IsProceedFurther = TRUE;
				}

			}
			else if ( subStringForDelay != NULL )
			{
				IsProceedFurther = FALSE;
			}
		}
		else if(  IsSourceValid  && ( !IsDelayValid ) ) 
		{
			char   tmpCharBuffer [ 256 ] = { 0 };
			errno_t rc = -1;
			char *subStringForSource = NULL,
			     *subStringForDummy  = NULL;

			rc = strcpy_s( tmpCharBuffer, sizeof(tmpCharBuffer), pValue );
			if(rc != EOK)
			{
				ERR_CHK(rc);
				return FALSE;
			}
			subStringForSource   = strtok_r( tmpCharBuffer, " ", &st );
			subStringForDummy   = strtok_r( NULL, " ", &st );

                        /*CID: 55040 Dereference before null check*/
                        if(!subStringForSource)
                            return FALSE;
			if ( strcasestr(subStringForSource, "source="))
			{
				if ( subStringForDummy != NULL )
				{
					IsProceedFurther = FALSE;
				}
				else
				{
					IsProceedFurther = TRUE;
				}

			}
			else if( subStringForSource != NULL )
			{
				IsProceedFurther = FALSE;
			}
		}
		 else if(  IsSourceValid && IsDelayValid ) 
		{
			char   tmpCharBuffer [ 256 ] = { 0 };
			errno_t rc = -1;
			char *subStringForDelay 	 = NULL,
				*subStringForSource 	 = NULL,
				*subStringForDummy  = NULL;
			rc = strcpy_s( tmpCharBuffer, sizeof(tmpCharBuffer), pValue );
			if(rc != EOK)
			{
				ERR_CHK(rc);
				return FALSE;
			}
			subStringForDelay   = strtok_r( tmpCharBuffer, " ", &st );
			if ( (strcasestr(subStringForDelay, "delay="))  || (strcasestr(subStringForDelay, "source=")) )
			{
				subStringForSource = strtok_r( NULL, " ", &st );

                                /* CID: 55040 Dereference before null check*/
                                if(!subStringForSource)
                                    return FALSE;

				if ( (strcasestr(subStringForSource, "delay="))  || (strcasestr(subStringForSource, "source=")) )
				{
					subStringForDummy   = strtok_r( NULL, " ", &st );
					if( subStringForDummy != NULL )
					{
						IsProceedFurther = FALSE;
					}
					else
					{
						IsProceedFurther = TRUE;
					}
				}
				else if(subStringForSource != NULL )
				{
					IsProceedFurther = FALSE;
				}

			}
			else if( subStringForDelay != NULL )
			{
				IsProceedFurther = FALSE;
			}
		}
	}

	 return IsProceedFurther;
}

ANSC_STATUS
CosaDmlDiSyndicationFlowControlInit
  (
    PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL pSyndicatonFlowControl
  )
{
    char buf[64] = {0};
    errno_t                         rc              = -1;
    /* CID: 73135 Array compared against 0*/
    if(!syscfg_get(NULL,"SyndicationFlowControlEnable",buf, sizeof(buf)))
    {
        if (strcmp(buf, "true") == 0)
        {
            pSyndicatonFlowControl->Enable.ActiveValue = TRUE;
        }
        else
        {
            pSyndicatonFlowControl->Enable.ActiveValue = FALSE;
        }
    }
    memset(pSyndicatonFlowControl->InitialForwardedMark.ActiveValue, 0, sizeof(pSyndicatonFlowControl->InitialForwardedMark.ActiveValue));
    memset(pSyndicatonFlowControl->InitialOutputMark.ActiveValue, 0, sizeof(pSyndicatonFlowControl->InitialOutputMark.ActiveValue));
    if (syscfg_get( NULL, "DSCP_InitialForwardedMark", buf, sizeof(buf)) == 0)
    {
        if (buf[0] != '\0')
        {
            rc = STRCPY_S_NOCLOBBER(pSyndicatonFlowControl->InitialForwardedMark.ActiveValue, sizeof(pSyndicatonFlowControl->InitialForwardedMark.ActiveValue), buf);
            ERR_CHK(rc);
        }
    }
    memset(buf, 0, sizeof(buf));
    if (syscfg_get( NULL, "DSCP_InitialOutputMark", buf, sizeof(buf)) == 0)
    {
        if (buf[0] != '\0')
        {
            rc = STRCPY_S_NOCLOBBER(pSyndicatonFlowControl->InitialOutputMark.ActiveValue, sizeof(pSyndicatonFlowControl->InitialOutputMark.ActiveValue), buf);
            ERR_CHK(rc);
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiSet_SyndicationFlowControl_Enable
  (
    char bValue
  )
{
    if(syscfg_set(NULL, "SyndicationFlowControlEnable", ((bValue == TRUE ) ? "true" : "false"))==0)
    {
        syscfg_commit();
        v_secure_system("sysevent set firewall-restart");
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }
}

ANSC_STATUS
CosaDmlDiSet_SyndicationFlowControl_InitialForwardedMark
  (
    char *pString
  )
{
    if(syscfg_set(NULL, "DSCP_InitialForwardedMark", pString)==0)
    {
        syscfg_commit();
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }
}

ANSC_STATUS
CosaDmlDiSet_SyndicationFlowControl_InitialOutputMark
  (
    char *pString
  )
{
    if(syscfg_set(NULL, "DSCP_InitialOutputMark", pString)==0)
    {
        syscfg_commit();
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
       return ANSC_STATUS_FAILURE;
    }
}


// Init RFC json objects
ANSC_STATUS
RfcJsonInit
  (
    cJSON **pJsonObj, char *jsonFileName
  )
{
    FILE *fileRead = NULL;
    char* data = NULL;
    int len ;

    if (access(jsonFileName, F_OK) != 0)
    {
        return ANSC_STATUS_FAILURE;
    }

    fileRead = fopen( jsonFileName, "r" );
    if( fileRead == NULL )
    {
       CcspTraceWarning(("%s-%d : Error in opening JSON file\n" , __FUNCTION__, __LINE__ ));
       return ANSC_STATUS_FAILURE;
    }

    fseek( fileRead, 0, SEEK_END );
    len = ftell( fileRead );
    /* CID: 128847 Argument cannot be negative*/
    if (len < 0) {
       CcspTraceWarning(("%s-%d : Error in file handle\n" , __FUNCTION__, __LINE__ ));
       fclose( fileRead );
       return ANSC_STATUS_FAILURE;
    }

    fseek( fileRead, 0, SEEK_SET );
    data = ( char* )malloc( sizeof(char) * (len + 1) );
    if (data != NULL)
    {
        memset( data, 0, ( sizeof(char) * (len + 1) ));
        fread( data, 1, len, fileRead );
    }
    else
    {
        CcspTraceWarning(("%s-%d : Memory allocation failed \n", __FUNCTION__, __LINE__));
        fclose( fileRead );
        return ANSC_STATUS_FAILURE;
    }

    fclose( fileRead );
    /*CID: 140452 String not null terminated*/
    data[len] = '\0';
    if ( data == NULL )
    {
         CcspTraceWarning(("%s-%d : fileRead failed \n", __FUNCTION__, __LINE__));
         return ANSC_STATUS_FAILURE;
    }
    else if ( strlen(data) != 0)
    {
        *pJsonObj = cJSON_Parse( data );
        if( !*pJsonObj )
        {
            CcspTraceWarning((  "%s : json file parser error : [%d]\n", __FUNCTION__,__LINE__));
            free(data);
            return ANSC_STATUS_FAILURE;
        }
        else
        {
            CcspTraceWarning(("%s-%d : json file parser  success \n", __FUNCTION__, __LINE__));
        }
    }
    free(data);
    data = NULL;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiRfcDefaultsInit
  (
    cJSON **pRfcDefaults
  )
{
   return RfcJsonInit(pRfcDefaults, RFC_DEFAULTS_FILE);
}

ANSC_STATUS
CosaDmlDiRfcStoreInit
  (
    cJSON **pRfcStore
  )
{
   return RfcJsonInit(pRfcStore, RFC_STORE_FILE);
}

static cJSON *rfcNewJson;
ANSC_STATUS
StartRfcProcessing()
{
   CcspTraceWarning((  "%s \n", __FUNCTION__ ));
   rfcNewJson = cJSON_CreateObject();
   return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
ProcessRfcSet(cJSON **pRfcStore, BOOL clearDB, char *paramFullName, char *value, char *pSource, char *pCurrentTime)
{
   CcspTraceWarning((  "%s : paramFullName=%s, value=%s, clearDB=%d\n", __FUNCTION__, paramFullName, value, clearDB));

   cJSON *paramObj = cJSON_CreateObject();
   cJSON_AddItemToObject(paramObj, "Value", cJSON_CreateString(value));
   cJSON_AddItemToObject(paramObj, "UpdateTime", cJSON_CreateString(pCurrentTime));
   cJSON_AddItemToObject(paramObj, "UpdateSource", cJSON_CreateString(pSource));
   if (clearDB == false)
   {
      if ( *pRfcStore == NULL)
      {
         CcspTraceWarning((  "%s : pRfcStore is NULL : [%d]\n", __FUNCTION__,__LINE__));
         *pRfcStore = cJSON_CreateObject();
      }
      cJSON *obj = cJSON_GetObjectItem(*pRfcStore, paramFullName);
      if (obj)
         cJSON_ReplaceItemInObject(*pRfcStore, paramFullName, paramObj);
      else
         cJSON_AddItemToObject(*pRfcStore, paramFullName, paramObj);

      char *cJsonOut = cJSON_Print(*pRfcStore);
      //CcspTraceWarning((  "%s : cJsonOut=%s\n", __FUNCTION__,cJsonOut));
      writeToJson(cJsonOut, RFC_STORE_FILE);
      free(cJsonOut);
   }
   else
   {
      cJSON *obj = cJSON_GetObjectItem(rfcNewJson, paramFullName);
      if (obj)
         cJSON_ReplaceItemInObject(rfcNewJson, paramFullName, paramObj);
      else
         cJSON_AddItemToObject(rfcNewJson, paramFullName, paramObj);
   }
   return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
EndRfcProcessing(cJSON **pRfcStore)
{
   CcspTraceWarning((  "%s \n", __FUNCTION__ ));

   cJSON *current_element = NULL;
   cJSON_ArrayForEach(current_element, *pRfcStore)
   {
      cJSON *updateSourceObj = cJSON_GetObjectItem(current_element, "UpdateSource");
      if ( updateSourceObj && strcmp(updateSourceObj->valuestring, "webpa") == 0)
      {
         char *current_key = current_element->string;
         CcspTraceWarning((  "%s: Found a previously webpa set param: %s \n", __FUNCTION__, current_key ));
         // If previously set webpa param is not present in the new RFC config, add it to new RFC config.
         if ( NULL == cJSON_GetObjectItem(rfcNewJson, current_key))
         {
            CcspTraceWarning((  "%s: Add the previously set webpa param to current json \n", __FUNCTION__ ));
            cJSON_AddItemToObject(rfcNewJson, current_key, cJSON_Duplicate(current_element, 1));
         }
      }
   }

   char *cJsonOut = cJSON_Print(rfcNewJson);
   writeToJson(cJsonOut, RFC_STORE_FILE);
   free(cJsonOut);
   cJSON_Delete(*pRfcStore);
   *pRfcStore = rfcNewJson;
   rfcNewJson = NULL;
   return ANSC_STATUS_SUCCESS;
}

#define MAX_NTP_SERVER 5

ANSC_STATUS
ApplyNTPPartnerDefaults()
{
    FILE *fileRead = NULL;
    char* data = NULL;
    int len ;
    cJSON *json = NULL;
    cJSON *partnerObj = NULL;
    char *objVal = NULL;
    char PartnerID[PARTNER_ID_LEN] = {0};
    fileRead = fopen( BOOTSTRAP_INFO_FILE , "r" );
    if( fileRead == NULL )
    {
        CcspTraceWarning(("%s-%d : Error in opening JSON file\n" , __FUNCTION__, __LINE__ ));
        return ANSC_STATUS_FAILURE;
    }
    fseek( fileRead, 0, SEEK_END );
    len = ftell( fileRead );
    /* CID: 58127 Argument cannot be negative*/
    if (len < 0) {
        CcspTraceWarning(("%s-%d : Error in file handle\n" , __FUNCTION__, __LINE__ ));
        fclose( fileRead );
        return ANSC_STATUS_FAILURE;
    }
    fseek( fileRead, 0, SEEK_SET );
    data = ( char* )malloc( sizeof(char) * (len + 1) );
    if (data != NULL)
    {
        memset( data, 0, ( sizeof(char) * (len + 1) ));
        fread( data, 1, len, fileRead );
    }
    else
    {
        CcspTraceWarning(("%s-%d : Memory allocation failed \n", __FUNCTION__, __LINE__));
        fclose( fileRead );
        return ANSC_STATUS_FAILURE;
    }

    fclose( fileRead );
    /* CID: 135336 String not null terminated*/
    data[len] ='\0';
    if ( data == NULL )
    {
          CcspTraceWarning(("%s-%d : fileRead failed \n", __FUNCTION__, __LINE__));
          return ANSC_STATUS_FAILURE;
    }
    else if ( strlen(data) != 0)
    {
         json = cJSON_Parse( data );
         if( !json )
         {
             CcspTraceWarning((  "%s : json file parser error : [%d]\n", __FUNCTION__,__LINE__));
             free(data);
             return ANSC_STATUS_FAILURE;
         }
         else
         {
             if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
             {
                  partnerObj = cJSON_GetObjectItem( json, PartnerID );
                  if ( NULL != partnerObj)
                  {
                       cJSON *objItem = NULL;
                       int i;
                       char *key[]={"Device.Time.NTPServer1","Device.Time.NTPServer2","Device.Time.NTPServer3","Device.Time.NTPServer4","Device.Time.NTPServer5"};
                       char *name[]={"ntp_server1","ntp_server2","ntp_server3","ntp_server4","ntp_server5"};

                       for (i=0;i<MAX_NTP_SERVER;i++)
                       {
                            objItem = cJSON_GetObjectItem(cJSON_GetObjectItem( partnerObj, key[i]), "ActiveValue");
                            if ( objItem != NULL )
                            {
                                 objVal = objItem->valuestring;
                                 objItem = NULL;
                                 if ( objVal != NULL )
                                 {
                                      if ( syscfg_set(NULL,name[i],objVal) != 0)
                                      {
                                           CcspTraceWarning(("syscfg_set failed for %s\n",name[i]));
                                      }
                                      else
                                      {
                                          if ( syscfg_commit( ) != 0 )
                                          {
                                               CcspTraceWarning(("syscfg_commit failed for %s\n",name[i]));
                                          }
                                      }
                                      objVal = NULL;
                                 }
                                 else
                                 {
                                      CcspTraceWarning(("%s - obj Value is NULL\n", __FUNCTION__ ));
                                 }
                            }
                            else
                            {
                                 CcspTraceWarning(("%s - %s Object is NULL\n", __FUNCTION__, key[i] ));
                            }
                      }
                      cJSON_Delete(json);
                      return ANSC_STATUS_SUCCESS;
                 }
             }
             cJSON_Delete(json);
         }
    }
    /*58557 Resource leak*/
    if(data)
      free(data);
    return ANSC_STATUS_FAILURE;
}
ANSC_STATUS
CosaDmlSetnewNTPEnable(BOOL bValue)
{
     if( bValue == TRUE)
     {
         AnscTraceWarning(("Enabling newNTP from RFC \n"));
         if( 0 != syscfg_set(NULL, "new_ntp_enabled", "true"))
	 {
             AnscTraceWarning(("syscfg_set failed for new_ntp_enabled\n"));
             return ANSC_STATUS_FAILURE;
         }
         else if ( syscfg_commit( ) != 0 )
         {
             CcspTraceWarning(("syscfg_commit failed\n"));
             return ANSC_STATUS_FAILURE;
         } 
         if( ANSC_STATUS_SUCCESS != ApplyNTPPartnerDefaults() )
             return ANSC_STATUS_FAILURE;
     }
     else
     {
         AnscTraceWarning(("Disabling newNTP from RFC \n"));
         if( 0 != syscfg_set(NULL, "new_ntp_enabled", "false"))
         {
             AnscTraceWarning(("syscfg_set failed for new_ntp_enabled\n"));
             return ANSC_STATUS_FAILURE;
         }
         else if ( syscfg_commit( ) != 0 )
         {
             CcspTraceWarning(("syscfg_commit failed\n"));
             return ANSC_STATUS_FAILURE;
         }

     }

     commonSyseventSet("ntpd-restart", "");
   
     return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS 
CosaDmlScheduleAutoReboot(int ConfiguredUpTime, BOOL bValue)
{
    int RebootDay = 0;
    if( bValue )
    {
        if((1 > ConfiguredUpTime || ConfiguredUpTime > 30))
        {
            ConfiguredUpTime = 10;
        }    
        int DeviceUptime = CosaDmlDiGetUpTime(NULL)/(3600*24);
        CcspTraceInfo(("%s Device uptime in days %d \n",__FUNCTION__, DeviceUptime ));
        if( DeviceUptime < ConfiguredUpTime )
        {
            RebootDay = ConfiguredUpTime - DeviceUptime;
            CcspTraceInfo(("%s Scheduling reboot after %d days \n",__FUNCTION__, RebootDay ));
        }  
    }
    CcspTraceInfo(("%s Scheduling cron sh /etc/ScheduleAutoReboot.sh %d %d& \n",__FUNCTION__, RebootDay,bValue ));
    v_secure_system("/etc/ScheduleAutoReboot.sh %d %d&",RebootDay,bValue);
    return ANSC_STATUS_SUCCESS;    
}

BOOL CosaDmlGetInternetStatus()
{
    char PingValue[64] = {0};
    char WanValue[64] = {0}; 
    commonSyseventGet("wan-status", WanValue, sizeof(WanValue));

    BOOL bWanStatus = TRUE;
    BOOL bPing = TRUE;
    if (WanValue[0] != '\0' )
    {
        if( 0 != strcmp( "started", WanValue ))
        {
            bWanStatus = FALSE;
            CcspTraceWarning(("CosaDmlGetInternetStatus wan status:%d\n", bWanStatus));
        }
    }

    char partnerId[20];
    if(!syscfg_get(NULL, "PartnerID", partnerId, sizeof(partnerId)))
    {
        if (strcmp( "comcast", partnerId ) == 0 )
        {
            commonSyseventGet("ping-status", PingValue, sizeof(PingValue));
            if (PingValue[0] != '\0' )
            {
                if( 0 == strcmp( "missed", PingValue ))
                {
                    bPing = FALSE;
                    CcspTraceWarning(("CosaDmlGetInternetStatus Ping status:%d\n", bPing));
                }
            }
            return (( bWanStatus && bPing ) ? TRUE : FALSE );
        }
        else
        {
            return bWanStatus;
        }
    }
  return TRUE;
}

#if defined(_COSA_FOR_BCI_)
#define XDNS_RESOLV_CONF "/etc/resolv.conf"
#define XDNS_DNSMASQ_SERVERS_BAK "/nvram/dnsmasq_servers.bak"
#define XDNS_DNSMASQ_SERVERS_CONF "/nvram/dnsmasq_servers.conf"


int setMultiProfileXdnsConfig(BOOL bValue)
{

        char confEntry[256] = {0};

  
        FILE *fp1 = NULL, *fp2 = NULL, *fp3 = NULL;

        fp1 = fopen(XDNS_RESOLV_CONF, "r");
        if(fp1 == NULL)
        {
                fprintf(stderr,"### XDNS : setMultiProfileXdnsConfig() - fopen(XDNS_RESOLV_CONF, 'r') Error !!\n");
                return 0;
        }

        fp2 = fopen(XDNS_DNSMASQ_SERVERS_CONF ,"r");
        if(fp2 == NULL)
        {
                fprintf(stderr,"### XDNS : setMultiProfileXdnsConfig() - fopen(XDNS_DNSMASQ_SERVERS_CONF, 'r') Error !!\n");
                fclose(fp1);
                fp1 = NULL;
                return 0;
        }

        unlink(XDNS_DNSMASQ_SERVERS_BAK);

        fp3 = fopen(XDNS_DNSMASQ_SERVERS_BAK ,"a");
        if(fp3 == NULL)
        {
                fprintf(stderr,"### XDNS : setMultiProfileXdnsConfig() - fopen(XDNS_DNSMASQ_SERVERS_BAK, 'a') Error !!\n");
                fclose(fp2);
                fp2 = NULL;
                fclose(fp1);
                fp1 = NULL;
                return 0;
        }


        //Get all entries (other than XDNS_Multi_Profile) from resolv.conf file//
        while(fgets(confEntry, sizeof(confEntry), fp1) != NULL)
        {
                if ( strstr(confEntry, "XDNS_Multi_Profile"))
                {
                        continue;
                }

                fprintf(fp3, "%s", confEntry);
        }

        if(bValue)
        {
                fprintf(fp3, "XDNS_Multi_Profile Enabled\n");
                CcspTraceWarning(("%s XDNS_Multi_Profile Feature Enabled\n", __FUNCTION__));
        }
        else
        {
                fprintf(fp3, "XDNS_Multi_Profile Disabled\n");
                CcspTraceWarning(("%s XDNS_Multi_Profile Feature Disabled\n", __FUNCTION__));
        }

        fclose(fp3); 
  	fp3 = NULL;
        fclose(fp2); 
  	fp2 = NULL;
        fclose(fp1); 
  	fp1 = NULL;

        fp1 = fopen(XDNS_RESOLV_CONF, "w");
        if(fp1 == NULL)
        {
                fprintf(stderr,"### XDNS : setMultiProfileXdnsConfig() - fopen(XDNS_RESOLV_CONF, 'w') Error !!\n");
                return 0;
        }

        fp2 = fopen(XDNS_DNSMASQ_SERVERS_CONF,"w");
        if(fp2 == NULL)
        {
                fprintf(stderr,"### XDNS : setMultiProfileXdnsConfig() - fopen(XDNS_DNSMASQ_SERVERS_CONF, 'w') Error !!\n");
                if(fp1)
                {
                    fclose(fp1);
                    fp1 = NULL;
                }

                return 0;
        }

        fp3 = fopen(XDNS_DNSMASQ_SERVERS_BAK ,"r");
        if(fp3 == NULL)
        {
                fprintf(stderr,"### XDNS : setMultiProfileXdnsConfig() - fopen(XDNS_DNSMASQ_SERVERS_BAK, 'r') Error !!\n");
                fclose(fp2); 
          	fp2 = NULL;
                if(fp1) 
                {
                    fclose(fp1);
                    fp1 = NULL;
                }
                return 0;
        }

        while(fgets(confEntry, sizeof(confEntry), fp3) != NULL)
        {
                //copy back entries to resolv.conf if default entry is found. else keep the old resolv.
                if(fp1)
                {
                        fprintf(fp1, "%s", confEntry);
                }

                //copy only dnsoverride entries and Multi_profile into nvram
                if (strstr(confEntry, "dnsoverride") || strstr(confEntry, "XDNS_Multi_Profile"))
                {

                        fprintf(fp2, "%s", confEntry);
                }
        }

        if(fp3)
        {
            fclose(fp3);
            fp3 = NULL;
        }
        if(fp2)
        {
            fclose(fp2);
            fp2 = NULL;
        }
        if(fp1)
        {
            fclose(fp1);
            fp1 = NULL;
        }

        return 1; //success


}

#endif

#if defined (FEATURE_SUPPORT_RADIUSGREYLIST)
BOOL
CosaDmlSetRadiusGreyListEnable
    (
        BOOL        bValue
    )
{
    parameterValStruct_t pVal[1];
    char                 paramName[256] = "Device.WiFi.X_RDKCENTRAL-COM_EnableRadiusGreyList";
    char                 compName[256]  = "eRT.com.cisco.spvtg.ccsp.wifi";
    char                 dbusPath[256]  = "/com/cisco/spvtg/ccsp/wifi";
    char*                faultParam     = NULL;
    int                  ret            = 0;
    errno_t              rc             = -1;
    CCSP_MESSAGE_BUS_INFO *bus_info               = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    char str[2] = {0};

    rc = sprintf_s(str, sizeof(str), "%d", bValue);
    if(rc < EOK) ERR_CHK(rc);
    if (PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                              "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.RadiusGreyList.Enable",
                              ccsp_string, str) == CCSP_SUCCESS)
    {
       CcspTraceError(("%s - %d - PSM value is updated successfully\n", __FUNCTION__, __LINE__));
       pVal[0].parameterName  = paramName;
       pVal[0].parameterValue = bValue ? "true" : "false";
       pVal[0].type           = ccsp_boolean;

       ret = CcspBaseIf_setParameterValues(
                 bus_handle,
                 compName,
                 dbusPath,
                 0,
                 0,
                 pVal,
                 1,
                 TRUE,
                 &faultParam
             );

       if (ret != CCSP_SUCCESS)
       {
           CcspTraceError(("%s - %d - Failed to notify WiFi component - Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
           bus_info->freefunc(faultParam);

           //Restore the value in PSM
           snprintf(str, sizeof(str), "%s", bValue ? "0" : "1");
           PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                              "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.RadiusGreyList.Enable",
                              ccsp_string, str);
           return FALSE;
       }

       CcspTraceError(("%s - %d - WiFi Component is notified\n", __FUNCTION__, __LINE__));
       commonSyseventSet("firewall-restart", "");
       CcspTraceInfo(("%s - %d - Triggered Firewall Restart\n", __FUNCTION__, __LINE__));
       return TRUE;
    }
    return FALSE;
}
#endif

#if defined(FEATURE_HOSTAP_AUTHENTICATOR)
BOOL
CosaDmlSetNativeHostapdState
    (
        BOOL        bValue
    )
{
    char str[2] = {0};
    parameterValStruct_t pVal[1];
    char                 paramName[256] = "Device.WiFi.X_RDKCENTRAL-COM_EnableHostapdAuthenticator";
    char                 compName[256]  = "eRT.com.cisco.spvtg.ccsp.wifi";
    char                 dbusPath[256]  = "/com/cisco/spvtg/ccsp/wifi";
    char*                faultParam     = NULL;
    int                  ret            = 0;
    errno_t              rc             = -1;
    CCSP_MESSAGE_BUS_INFO *bus_info               = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    rc = sprintf_s(str, sizeof(str), "%d", bValue);
    if(rc < EOK) ERR_CHK(rc);
    if (PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                              "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Control.DisableNativeHostapd",
                              ccsp_string, str) == CCSP_SUCCESS)
    {
        CcspTraceInfo(("%s - %d - PSM value is updated successfully\n", __FUNCTION__, __LINE__));
        pVal[0].parameterName  = paramName;
        pVal[0].parameterValue = bValue ? "true" : "false";
        pVal[0].type           = ccsp_boolean;

        ret = CcspBaseIf_setParameterValues(
                 bus_handle,
                 compName,
                 dbusPath,
                 0,
                 0,
                 pVal,
                 1,
                 TRUE,
                 &faultParam
             );

        if (ret != CCSP_SUCCESS)
        {
            CcspTraceError(("%s - %d - Failed to notify WiFi component - Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
            bus_info->freefunc(faultParam);

            //Restore the value in PSM
            snprintf(str, sizeof(str), "%s", bValue ? "0" : "1");
            PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                               "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Control.DisableNativeHostapd",
                               ccsp_string, str);
            return FALSE;
        }

        CcspTraceInfo(("%s - %d - WiFi Component is notified\n", __FUNCTION__, __LINE__));
        return TRUE;
    }

    CcspTraceError(("%s - %d - WiFi Component notification Failed\n", __FUNCTION__, __LINE__));
    return FALSE;
}
#endif //FEATURE_HOSTAP_AUTHENTICATOR

BOOL CosaDmlSetDFS(BOOL bValue)
{
    char str[2] = {0};
    parameterValStruct_t pVal[1];
    char                 paramName[256] = "Device.WiFi.DFS";
    char                 compName[256]  = "eRT.com.cisco.spvtg.ccsp.wifi";
    char                 dbusPath[256]  = "/com/cisco/spvtg/ccsp/wifi";
    char*                faultParam     = NULL;
    int                  ret            = 0;
    errno_t              rc             = -1;
    CCSP_MESSAGE_BUS_INFO *bus_info               = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    rc = sprintf_s(str, sizeof(str), "%d", bValue);
    if(rc < EOK) ERR_CHK(rc);
    if (PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                              "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.DFS.Enable",
                              ccsp_string, str) == CCSP_SUCCESS)
    {
        CcspTraceInfo(("%s - %d - PSM value is updated successfully\n", __FUNCTION__, __LINE__));
        pVal[0].parameterName  = paramName;
        pVal[0].parameterValue = bValue ? "true" : "false";
        pVal[0].type           = ccsp_boolean;

        ret = CcspBaseIf_setParameterValues(
                 bus_handle,
                 compName,
                 dbusPath,
                 0,
                 0,
                 pVal,
                 1,
                 TRUE,
                 &faultParam
             );

        if (ret != CCSP_SUCCESS)
        {
            CcspTraceError(("%s - %d - Failed to notify WiFi component - Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
            bus_info->freefunc(faultParam);

            //Restore the value in PSM
            snprintf(str, sizeof(str), "%s", bValue ? "0" : "1");
            PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                               "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.DFS.Enable",
                               ccsp_string, str);
            return FALSE;
        }

        CcspTraceInfo(("%s - %d - WiFi Component is notified\n", __FUNCTION__, __LINE__));
        return TRUE;
    }

    CcspTraceError(("%s - %d - WiFi Component notification Failed\n", __FUNCTION__, __LINE__));
    return FALSE;
}

BOOL CosaDmlSetDFSatBootUp(BOOL bValue)
{
    char str[2] = {0};
    parameterValStruct_t pVal[1];
    char                 paramName[256] = "Device.WiFi.DFSatBootUp";
    char                 compName[256]  = "eRT.com.cisco.spvtg.ccsp.wifi";
    char                 dbusPath[256]  = "/com/cisco/spvtg/ccsp/wifi";
    char*                faultParam     = NULL;
    int                  ret            = 0;
    errno_t              rc             = -1;
    CCSP_MESSAGE_BUS_INFO *bus_info               = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    rc = sprintf_s(str, sizeof(str), "%d", bValue);
    if(rc < EOK) ERR_CHK(rc);
    if (PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                              "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.DFSatBootUp.Enable",
                              ccsp_string, str) == CCSP_SUCCESS)
    {
        CcspTraceInfo(("%s - %d - PSM value is updated successfully\n", __FUNCTION__, __LINE__));
        pVal[0].parameterName  = paramName;
        pVal[0].parameterValue = bValue ? "true" : "false";
        pVal[0].type           = ccsp_boolean;

        ret = CcspBaseIf_setParameterValues(
                 bus_handle,
                 compName,
                 dbusPath,
                 0,
                 0,
                 pVal,
                 1,
                 TRUE,
                 &faultParam
             );

        if (ret != CCSP_SUCCESS)
        {
            CcspTraceError(("%s - %d - Failed to notify WiFi component - Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
            bus_info->freefunc(faultParam);

            //Restore the value in PSM
            snprintf(str, sizeof(str), "%s", bValue ? "0" : "1");
            PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                               "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.DFSatBootUp.Enable",
                               ccsp_string, str);
            return FALSE;
        }

        CcspTraceInfo(("%s - %d - WiFi Component is notified\n", __FUNCTION__, __LINE__));
        return TRUE;
    }

    CcspTraceError(("%s - %d - WiFi Component notification Failed\n", __FUNCTION__, __LINE__));
    return FALSE;
}
