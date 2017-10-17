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

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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

#include "cosa_deviceinfo_apis.h"
#include "cosa_deviceinfo_apis_custom.h"
#include "dml_tr181_custom_cfg.h" 

#define DEVICE_PROPERTIES    "/etc/device.properties"

#ifdef _PLATFORM_RASPBERRYPI_
#include "ccsp_vendor.h"
#endif

#ifdef _COSA_SIM_

// this file is in integration_src.intel_usg_arm directory

#elif (_COSA_INTEL_USG_ARM_ || _COSA_DRG_TPG_)

#include "ccsp_psm_helper.h"            // for PSM_Get_Record_Value2
#include "dmsb_tr181_psm_definitions.h" // for DMSB_TR181_PSM_DeviceInfo_Root/ProductClass

#ifdef _COSA_DRG_TPG_
#include "libplat.h"
#elif _COSA_INTEL_USG_ARM_
//#include "libplat_flash.h"
#endif

 
#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <time.h>
#include <utapi_util.h>
#include <unistd.h>

#include "platform_hal.h"
#include "autoconf.h"     
 
#define _ERROR_ "NOT SUPPORTED"
#define _SSH_ERROR_ "NOT SET"

extern void* g_pDslhDmlAgent;

static const int OK = 1 ;
static const int NOK = 0 ;
static char reverseSSHArgs[255] = { "\0" };
const char* sshCommand = "/lib/rdk/startTunnel.sh";
const char* rsshPidFile = "/var/tmp/rssh.pid";

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
static char* mapArgsToSSHOption(char *revSSHConfig) {

	char* value = NULL;
	char* option = NULL;
	option = (char*) calloc(125, sizeof(char));

	if (option) {
		if ((value = strstr(revSSHConfig, "idletimeout="))) {
			sprintf(option, " -I %s -f -N -y -T",
					value + strlen("idletimeout="));
		} else if ((value = strstr(revSSHConfig, "sshport=")) && !(value =
				strstr(revSSHConfig, "revsshport="))) {
			value = strstr(revSSHConfig, "sshport=");
			sprintf(option, " -p %s", value + strlen("sshport="));
		} else if ((value = strstr(revSSHConfig, "revsshport="))) {
			sprintf(option, " -R %s:[CM_IP]:22", value + strlen("revsshport="));
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

	char tempCopy[255] = { "\0" };
	char *tempStr;
	char* option = NULL;
	option = (char*) calloc(125, sizeof(char));

	int inputMsgSize = strlen(input);
	strncpy(tempCopy, input, inputMsgSize);
	tempStr = (char*) strtok(tempCopy, ";");
	if (tempStr) {
		sprintf(option, "%s", tempStr);
	} else {
		sprintf(option, "%s", input);
	}
	return option;
}

/**
 * Get login username/target for jump server
 */
static char* getHostLogin(char *tempStr) {
	char* value = NULL;
	char* hostIp = NULL;
	char* user = NULL;
	char* hostLogin = NULL;

	int inputMsgSize = strlen(tempStr);
	char tempCopy[255] = { "\0" };
	strncpy(tempCopy, tempStr, inputMsgSize);

	if ((value = strstr(tempStr, "host="))) {
		hostIp = (char*) calloc(125, sizeof(char));
		sprintf(hostIp, "%s", value + strlen("host="));
	}

	if ((value = strstr(tempStr, "user="))) {
		user = (char*) calloc(125, sizeof(char));
		sprintf(user, "%s", value + strlen("user="));
	}

	if (user && hostIp) {
		user = findUntilFirstDelimiter(user);
		hostIp = findUntilFirstDelimiter(hostIp);

		hostLogin = (char*) calloc(255, sizeof(char));
		if (hostLogin) {
			sprintf(hostLogin, " %s@%s", user, hostIp);
		}
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
    AnscCopyString(pValue, CONFIG_VENDOR_NAME);
    *pulSize = AnscSizeOfString(pValue);
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

 /*
    UCHAR strMaceMG[128];
    memset(strMaceMG,0,128);
#ifdef _COSA_DRG_TPG_
    plat_GetFlashValue("macmgwan", strMaceMG);
#elif _COSA_INTEL_USG_ARM_
    //    SaPermanentDb_GetFactoryId(pValue);
    ProdDb_GetCmMacAddress(strMaceMG);
#if 0
    UtopiaContext ctx;
    int rc = -1;
    if (!Utopia_Init(&ctx)) return ERR_UTCTX_INIT;
    rc = Utopia_Get_Mac_MgWan(&ctx,strMaceMG); // As MAC address is stored in syscfg in CNS 
    Utopia_Free(&ctx,0); 
#endif
#endif
    sprintf(pValue, "%02X%02X%02X",strMaceMG[0],strMaceMG[1],strMaceMG[2]);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
*/
/*
    char val[64] = {0};
    char param_name[256] = {0};

    _ansc_sprintf(param_name, "%s%s", DMSB_TR181_PSM_DeviceInfo_Root, DMSB_TR181_PSM_DeviceInfo_ManufacturerOUI);        

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
#if defined(_COSA_BCM_ARM_)
        sprintf(pValue, "%s%c", CONFIG_VENDOR_ID, '\0');
#else
        sprintf(pValue, "%06X%c", CONFIG_VENDOR_ID, '\0');
#endif
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;

}
/*Changes for 6560*/
ANSC_STATUS
CosaDmlDiGetCMTSMac
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
  #ifdef _COSA_INTEL_XB3_ARM_
   if ( platform_hal_getCMTSMac(pValue) != RETURN_OK)
        return ANSC_STATUS_FAILURE;
    else {
            *pulSize = AnscSizeOfString(pValue); 
            return ANSC_STATUS_SUCCESS;

         }
   #else
     return ANSC_STATUS_FAILURE;
   #endif
}
/*Changes for 6560-end*/

ANSC_STATUS
CosaDmlDiGetModelName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{    
#ifdef _COSA_INTEL_USG_ARM_

    if ( platform_hal_GetModelName(pValue) != RETURN_OK)
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue); 
        return ANSC_STATUS_SUCCESS;
    }

#elif _COSA_DRG_TPG_

    UCHAR model[128];
    char temp[2];

    memset(model,0,128);
    plat_GetFlashValue("model", model);
    
    sprintf(temp, "%x%x",model[0],model[1]);
    
    if((0 == strcmp(temp,"f4c"))||(0 == strcmp(temp,"3916")))
    {
        AnscCopyString(pValue, "DRG 3916");
    }
    else
    {
        AnscCopyString(pValue, "UnKNOWN");
    }
    return ANSC_STATUS_SUCCESS;

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
    AnscCopyString(pValue, CONFIG_TI_GW_DESCRIPTION);
    *pulSize = AnscSizeOfString(pValue);
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
		AnscCopyString(pValue, "CBR");
	}
#elif defined(_XB6_PRODUCT_REQ_)
	{
		 AnscCopyString(pValue, "XB6");
	}
#else
	{
		AnscCopyString(pValue, "XB3");
	}
#endif

    *pulSize = AnscSizeOfString(pValue);
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
    UCHAR unitsn[128];
    memset(unitsn,0,128);

#ifdef _COSA_DRG_TPG_
    plat_GetFlashValue("unitsn", unitsn);
    sprintf(pValue, "%c%c%c%c%c%c%c",unitsn[0],unitsn[1],unitsn[2],unitsn[3],unitsn[4],unitsn[5],unitsn[6]);
#elif _COSA_INTEL_USG_ARM_

    if (platform_hal_GetSerialNumber(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
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

#ifdef _COSA_INTEL_USG_ARM_    

    if (platform_hal_GetHardwareVersion(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
    }

#elif _COSA_DRG_TPG_
//Replace this with syscfg if we are pulling this from Cable modem later on 
    UCHAR hwVersion[128];
    memset(hwVersion,0,128);    
    plat_GetFlashValue("hwid", hwVersion);
    sprintf(pValue, "%X%X",hwVersion[0],hwVersion[1]);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
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
#if 0
    UtopiaContext ctx;
    int rc = -1;
    char temp[64];

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
    sprintf(pValue, "%c%c%c%c%c%c%c",unitsn[0],unitsn[1],unitsn[2],unitsn[3],unitsn[4],unitsn[5],unitsn[6]);
#elif _COSA_INTEL_USG_ARM_

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
    UtopiaContext ctx;
    int rc = -1;

    if (!Utopia_Init(&ctx))
        return ERR_UTCTX_INIT;

    rc = Utopia_Set_Prov_Code(&ctx,pProvisioningCode);

    Utopia_Free(&ctx,!rc);

    return ANSC_STATUS_SUCCESS;
}



void uploadLogUtilityThread(void* vptr_value)
{
	pthread_detach(pthread_self());
	char uploadOnRequest[150];
	char *str = (char *) vptr_value;
	snprintf(uploadOnRequest,sizeof(uploadOnRequest),"sh /rdklogger/opsLogUpload.sh %s &",str);
	system(uploadOnRequest);
	return;

}
ANSC_STATUS
COSADmlUploadLogsNow
	(
		ANSC_HANDLE                 hContext,
        	BOOL                        bEnable
	)
{
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
	FILE *ptr_file;
	char buf[50];

	ptr_file =fopen("/tmp/upload_log_status","r");

	if (ptr_file)
	{
		if (fgets(buf,50, ptr_file)!=NULL)
		{
			strip_line(buf);
			AnscCopyString(pValue, buf);
			*pUlSize = AnscSizeOfString(pValue);
		}
		fclose(ptr_file);
	}
	else
	{
		AnscCopyString(pValue, "Not triggered");
		*pUlSize = AnscSizeOfString(pValue);
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
    UtopiaContext ctx;
    int rc = -1;
    char firstUseDate[64];

    if (!Utopia_Init(&ctx))
        return ERR_UTCTX_INIT;

    rc = Utopia_Get_First_Use_Date(&ctx,firstUseDate);

    Utopia_Free(&ctx,0);

    //    fprintf(stderr, "<RT> rc=%d, First Use Date = '%s'\n", rc, firstUseDate);

    if(rc || firstUseDate[0] == '\0') _ansc_sprintf(firstUseDate, "2013-11-22T00:00:00");

    AnscCopyString(pValue,firstUseDate);
    *pulSize = AnscSizeOfString(pValue);
    
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlDiGetUpTime
    (
        ANSC_HANDLE                 Context
    )
{
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
	static ULONG value;
	struct sysinfo s_info;
	struct timeval currentTime;
	
	if(!value)
	{
		if(sysinfo(&s_info))
		{
			return 0;
		}
		int upTime = s_info.uptime; 

		gettimeofday(&currentTime, NULL);

		value = currentTime.tv_sec - upTime;
    }	
        
	if(value != 0)
	{
		return value;
	}
	else
		return 0;
		
}

ANSC_STATUS
CosaDmlDiGetBootloaderVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
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
    FILE *fp;
    char line[512];

    if ((fp = fopen("/etc/versions", "rb")) == NULL)
        return ANSC_STATUS_FAILURE;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strncmp(line, "FSSTAMP", strlen("FSSTAMP")) != 0)
            continue;

        snprintf(pValue, pulSize, "%s", line);
        *pulSize = AnscSizeOfString(pValue);
        if(pValue[*pulSize-1] == '\n') pValue[--(*pulSize)] = '\0';
        fclose(fp);
        return ANSC_STATUS_SUCCESS;
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
    char value[25];
    FILE *fp;

    AnscCopyString(pValue, _ERROR_);
    memset(value,0,10);

    fp = popen("cat /proc/sys/net/ipv4/tcp_congestion_control", "r");
    if (fp == NULL)
    {
        CcspTraceWarning(("ERROR '%s'\n","ERROR")); 
        return ANSC_STATUS_FAILURE;
    }
   
    while(fgets(value, 25, fp) != NULL)
    {
        AnscCopyString(pValue ,value);
    }

    pclose(fp);
    *pulSize = AnscSizeOfString(pValue);
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

	/*
	  * Validate input/params 
	  * sizeof_wrapped_inputparam it should always greater that ( lengthof_inputparam  + 2 ) because
	  * we are adding 2 extra charecters here. so we need to have extra bytes 
	  * in copied(wrapped_inputparam) string
	  */ 
	if( sizeof_wrapped_inputparam <= ( lengthof_inputparam  + 2 ) )
	{
        returnStatus = ANSC_STATUS_FAILURE;
	}
	else if(strstr(inputparam,";")) // check for possible command injection	
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam,"&"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam,"|"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam,"'"))
        returnStatus = ANSC_STATUS_FAILURE;

    if(ANSC_STATUS_SUCCESS == returnStatus)
    {
        sprintf(wrapped_inputparam,"'%s'",inputparam);
    }
	
	return returnStatus;

}

/* Maitenance window can be customized for bci routers */

ANSC_STATUS
CosaDmlDiGetFirmwareUpgradeStartTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    char value[25];
    FILE *fp;

    AnscCopyString(pValue, _ERROR_);
    memset(value,0,10);

    fp = popen("cat /nvram/.FirmwareUpgradeStartTime", "r");
    if (fp == NULL)
    {
        CcspTraceError(("ERROR '%s'\n","ERROR"));
        return ANSC_STATUS_FAILURE;
    }

    while(fgets(value, 25, fp) != NULL)
    {
        AnscCopyString(pValue ,value);
    }

    pclose(fp);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetFirmwareUpgradeEndTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    char value[25];
    FILE *fp;

    AnscCopyString(pValue, _ERROR_);
    memset(value,0,10);

    fp = popen("cat /nvram/.FirmwareUpgradeEndTime", "r");
    if (fp == NULL)
    {
        CcspTraceError(("ERROR '%s'\n","ERROR"));
        return ANSC_STATUS_FAILURE;
    }

    while(fgets(value, 25, fp) != NULL)
    {
        AnscCopyString(pValue ,value);
    }

    pclose(fp);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

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
            
            tmp += 2;

            if (sscanf(tmp, "%c %*d %*d %*d %*d %*d %*u %*lu \
%*lu %*lu %*lu %lu %lu %ld %ld %ld %*ld %*d 0 %*llu %lu", 
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

    static ULONG                ProcessTimeStamp;
    ULONG                       ProcessNumber       = 0;
    struct dirent               *entry;
    DIR                         *dir;
    FILE                        *fp;
    char*                       name;
    int                         num;
    int                         i;
    ULONG                       pid;
    char                        status[32];
    char                        buf[400];
    ULONG                       utime;
    ULONG                       stime;
    char                        state[64];

    dir = opendir("/proc");
        
    if ( !dir )
    {
        CcspTraceWarning(("Failed to open /proc!\n"));
        return ;
    }

    for(;;)
    {
        if ( (entry = readdir(dir)) == NULL )
        {
            closedir(dir);
            dir = NULL;
            break;
        }

        name = entry->d_name;
            
        if ( *name >= '0' && *name <= '9' )
        {
            ProcessNumber++;
        }
    }
    /*CcspTraceWarning(("ProcessNumber = %d!\n", ProcessNumber));*/
        
    p_info->pProcTable = AnscAllocateMemory(sizeof(COSA_PROCESS_ENTRY) * ProcessNumber);  
                
    if( !p_info->pProcTable )
    {
        return ;
    }
    AnscZeroMemory(p_info->pProcTable, sizeof(COSA_PROCESS_ENTRY) * ProcessNumber);        
    p_info->ProcessNumberOfEntries = ProcessNumber;
        
    dir = opendir("/proc");
        
    if ( !dir )
    {
        CcspTraceWarning(("Failed to open /proc!\n"));
        return ;
    }
        
    for(i = 0; i < ProcessNumber; )
    {
        
        if ( (entry = readdir(dir)) == NULL )
        {
            closedir(dir);
            dir = NULL;
            break;
        }

        name = entry->d_name;
            
        if ( *name >= '0' && *name <= '9' )
        {
            /*CcspTraceWarning(("Begin to parse process %lu!", i));*/
            p_proc = p_info->pProcTable+i;
            i++;
            pid = atoi(name);
            p_proc->Pid = pid;
            sprintf(status, "/proc/%lu/stat", pid);
                
            if ( !(fp = fopen(status, "r")) )
            {   
                CcspTraceWarning(("Failed to open %s!\n", status));
                continue;
            }
    
            name = fgets(buf, sizeof(buf), fp);
            fclose(fp);  
                
            if ( !name )
            {
                CcspTraceWarning(("Failed to get process %d information!\n", pid));
                continue;
            }                  

            memset(state, 0, sizeof(state));

            if (read_proc_stat(name, p_proc->Command, &state, &p_proc->Size, &p_proc->Priority, &p_proc->CPUTime ))
            {
                CcspTraceWarning(("Failed to parse process %d information!\n", pid));
                continue;
            }
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
        
    if ( i != p_info->ProcessNumberOfEntries )
    {
        p_info->ProcessNumberOfEntries = i;
    }

    return; 
}

void test_get_proc_info()
{
     PCOSA_DATAMODEL_PROCSTATUS p_info = (PCOSA_DATAMODEL_PROCSTATUS)CosaProcStatusCreate();

     if (p_info)  COSADmlGetProcessInfo(p_info);
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
    struct dirent               *entry;
    DIR                         *dir;
    FILE                        *fp;
    char*                       name;
    int                         num;
    COSA_CPUTIME_INFO           time[2];
    ULONG                       UsedTime = 0;
    ULONG                       IdleTime = 0;
    double                      CPUUsage;
    int                         CPUNum;
    int                         i;
    ULONG                       pid;
    char                        status[32];
    char                        buf[400];
    ULONG                       utime;
    ULONG                       stime;

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
        
    CcspTraceWarning(("UsedTime = %ul\n", UsedTime));
    CcspTraceWarning(("IdleTime = %ul\n", IdleTime));
        
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
	    memset(buf,sizeof(buf),0);
	    snprintf(buf,sizeof(buf),"%d",val);            		    
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
    CcspTraceWarning(("AnscEqualString failed\n"));
    return -1;
}
ULONG COSADmlGetMemoryStatus(char * ParamName)
{
     struct sysinfo si;
     int tmp;
     if (sysinfo(&si))
     {
          /*Error*/
          return 0;
     }
     if( AnscEqualString(ParamName, "Total", TRUE))
     {
#ifdef  _COSA_INTEL_USG_ARM_
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
         return si.freeram*si.mem_unit/(1024);
#endif
     }


     else if(AnscEqualString(ParamName, "Used", TRUE))
     {
#ifdef _COSA_INTEL_USG_ARM_
	if ( platform_hal_GetUsedMemorySize(&tmp) != RETURN_OK )
        return 0;
    	else 
        return tmp;
#endif
     }
     else if(AnscEqualString(ParamName, "X_RDKCENTRAL-COM_FreeMemThreshold", TRUE))
     {
	char buf[10];
	memset(buf,sizeof(buf),0);
        syscfg_get( NULL, "MinMemoryThreshold_Value", buf, sizeof(buf));
        if( buf != NULL )
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

    memset(value,0,10);
    fp = popen("cat /proc/sys/net/core/wmem_max", "r");
    if (fp == NULL)
    {
       CcspTraceWarning(("ERROR '%s'\n","ERROR")); 
       return 0;
    }
   
    while(fgets(value, 10, fp) != NULL)
    {
       winSize = AnscString2Int(value);
       CcspTraceWarning(("********MaxWindowSize is '%d\n", winSize)); 
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
    AnscCopyString(pValue, "");
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
    #define TOKEN_STR       "BogoMIPS"
    #define MAX_LINE_SIZE   30
    char line[MAX_LINE_SIZE];
    char *pcur;
    FILE *fp;
    int status;

    memset(line, 0, MAX_LINE_SIZE);

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
        sprintf (line, "%d", procSpeed);
        AnscCopyString(pValue, line);
    }

#else

#ifdef SA_CUSTOM
    char buf[100]={0};
    char out_val[100]={0};
    char out1[100]={0};
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
    sprintf(out2,"\"cat /proc/cpuinfo\"");
    sprintf(out1,"rpcclient %s %s",urlPtr,out2);
    fp = popen(out1, "r");
#else
    fp = popen("cat /proc/cpuinfo", "r");
#endif
    if (fp == NULL)
    {
        CcspTraceWarning(("Read cpuinfo ERROR '%s'\n","ERROR")); 
        return ANSC_STATUS_FAILURE;
    }
   
    while(fgets(line, MAX_LINE_SIZE, fp) != NULL )
    {
       if(strcasestr(line, TOKEN_STR) != NULL)
       {
        pcur = strstr(line, ":");
        pcur++;
        while(*pcur == ' ') pcur++;           
        AnscCopyString(pValue, pcur);   
       }     
    }

#endif

    status = pclose(fp);
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

	platform_hal_GetFactoryResetCount(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDiClearResetCount
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bValue
   )
{

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
		system( "sh /usr/ccsp/tad/schd_dhcp_server_detection_test.sh" );
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
		system( "sh /usr/ccsp/tad/schd_dhcp_server_detection_test.sh" );
	}  

    return ANSC_STATUS_SUCCESS;
}

#endif
int getRebootCounter()
{       
        char buf[8];
		syscfg_get( NULL, "X_RDKCENTRAL-COM_LastRebootCounter", buf, sizeof(buf));
    	    if( buf != NULL )
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


int setXOpsReverseSshArgs(char* pString) {

    char tempCopy[255] = { "\0" };
    char* tempStr;
    char* option;
    char* hostLogin = NULL;

    int inputMsgSize = strlen(pString);

    hostLogin = getHostLogin(pString);
    if (!hostLogin) {
        AnscTraceWarning(("Warning !!! Target host for establishing reverse SSH tunnel is missing !!!\n"));
        strcpy(reverseSSHArgs,"");
        return 1;
    }

    strncpy(tempCopy, pString, inputMsgSize);
    tempStr = (char*) strtok(tempCopy, ";");
    if (NULL != tempStr) {
        option = mapArgsToSSHOption(tempStr);
        strcpy(reverseSSHArgs, option);
    } else {
        AnscTraceWarning(("No Match Found !!!!\n"));
        printf("No Match Found !!!!\n");
    }

    if (option) {
        free(option);
    }

    while ((tempStr = strtok(NULL, ";")) != NULL) {
        option = mapArgsToSSHOption(tempStr);
        if ( NULL != option) {
            strcat(reverseSSHArgs, option);
            free(option);
        }
    }
    strcat(reverseSSHArgs, hostLogin);
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
    AnscCopyString(pValue, reverseSSHArgs);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

int setXOpsReverseSshTrigger(char *input) {

    char *trigger = NULL;
    char command[255] = { '\0' };
    if (!input) {
        printf("Input args are empty \n");
        AnscTraceWarning(("Input args are empty !!!!\n"));
        return NOK;
    }
    
    trigger = strstr(input, "start");
    if (trigger) {
            strcpy(command, sshCommand);
            strcat(command, " start");
            strcat(command, reverseSSHArgs);
    } else {
        strcpy(command, sshCommand);
        strcat(command, " stop ");
    }
    system(command);
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


ANSC_STATUS
CosaDmlDiGetSyndicationPartnerId
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    ANSC_STATUS retVal = ANSC_STATUS_FAILURE;
    char fileContent[256] = {0};
    FILE *deviceFilePtr = NULL;
    char *pPartnerId = NULL;
    const char partnerStr[] = "PARTNER_ID";

    if (!pValue || !pulSize || *pulSize <= 64)
        return ANSC_STATUS_FAILURE;

    strcpy(pValue, "comcast"); // Set the default to comcast in case the partner id is not set in props file
    *pulSize = AnscSizeOfString(pValue);
    retVal = ANSC_STATUS_SUCCESS;

    deviceFilePtr = fopen( DEVICE_PROPERTIES, "r" );
    if (deviceFilePtr)
    {
        while (fgets(fileContent, sizeof(fileContent), deviceFilePtr) != NULL)
        {
            if ((pPartnerId = strstr(fileContent, partnerStr)) != NULL)
            { 
                AnscCopyString(pValue ,pPartnerId+sizeof(partnerStr));
                *pulSize = AnscSizeOfString(pValue);
                break;
            }
        }
        fclose(deviceFilePtr);
    }
    return retVal;
}

ANSC_STATUS
CosaDmlDiGetSyndicationTR69CertLocation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
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
    return ANSC_STATUS_SUCCESS;
}

static void
FirmwareDownloadAndFactoryReset()
{
    if( RETURN_ERR == cm_hal_FWupdateAndFactoryReset())
    {
       CcspTraceWarning(("FirmwareDownloadAndFactoryReset Thread:FWupdateAndFactoryReset already in progress\n"));
    }

}

ANSC_STATUS
CosaDmlDiSetFirmwareDownloadAndFactoryReset()
{
    pthread_t tid;
    token_t  se_token;
    int se_fd = s_sysevent_connect(&se_token);
    char evtValue[64] = {0};

    sysevent_get(se_fd, se_token, "fw_update_inprogress", evtValue, sizeof(evtValue));

    if (0 == strncmp(evtValue, "true", strlen("true")))
    {
        CcspTraceWarning(("FirmwareDownloadAndFactoryReset already in progress\n"))
        return ANSC_STATUS_FAILURE;
    }
    sysevent_set(se_fd, se_token,"fw_update_inprogress", "true",0);

    CcspTraceWarning(("Calling FirmwareDownloadAndFactoryReset thread\n"));
    pthread_create(&tid, NULL, &FirmwareDownloadAndFactoryReset, NULL);

    return ANSC_STATUS_SUCCESS;
}

