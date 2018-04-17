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
#include "cosa_x_cisco_com_devicecontrol_apis.h"
#define DEVICE_PROPERTIES    "/etc/device.properties"

#ifdef _COSA_SIM_

// this file is in integration_src.intel_usg_arm directory

#elif (_COSA_BCM_MIPS_ || _COSA_DRG_TPG_)

// These should really be part of the build parameters
#define CONFIG_VENDOR_NAME "ARRIS Group, Inc."
#define CONFIG_TI_GW_DESCRIPTION "DPoE 2.0 Fiber Modem Gateway Device"
#define CONFIG_VENDOR_ID 0xA055DE
#define PARTNERS_INFO_FILE		"/nvram/partners_defaults.json"

#define DMSB_TR181_PSM_WHIX_LogInterval                                 "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.LogInterval"
#define DMSB_TR181_PSM_WHIX_NormalizedRssiList                "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.NormalizedRssiList"
#define DMSB_TR181_PSM_WHIX_CliStatList                                    "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.CliStatList"
#define DMSB_TR181_PSM_WHIX_TxRxRateList                              "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.TxRxRateList"

#include "ccsp_psm_helper.h"            // for PSM_Get_Record_Value2
#include "dmsb_tr181_psm_definitions.h" // for DMSB_TR181_PSM_DeviceInfo_Root/ProductClass

#ifdef _COSA_DRG_TPG_
#include "libplat.h"
#elif _COSA_BCM_MIPS_
//#include "libplat_flash.h"
#endif

 
#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <unistd.h>

#include "platform_hal.h"
#include "autoconf.h"     
 
#define _ERROR_ "NOT SUPPORTED"
#define _START_TIME_12AM_ "0"
#define _END_TIME_3AM_ "10800"
#define _SSH_ERROR_ "NOT SET"

#define DMSB_TR181_PSM_WHIX_LogInterval                                 "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.LogInterval"
#define DMSB_TR181_PSM_WHIX_NormalizedRssiList                "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.NormalizedRssiList"
#define DMSB_TR181_PSM_WHIX_CliStatList                                    "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.CliStatList"
#define DMSB_TR181_PSM_WHIX_TxRxRateList                              "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.TxRxRateList"

extern void* g_pDslhDmlAgent;
extern ANSC_HANDLE bus_handle;


ANSC_STATUS
CosaDmlDiGetEnableMoCAforXi5Flag
  (
	  ANSC_HANDLE				  hContext,
	  BOOLEAN*					  pValue
  )
{
	char buf[ 8 ] = { 0 };

	if( 0 == syscfg_get( NULL, "X_RDKCENTRAL-COM_EnableMoCAforXi5", buf, sizeof( buf ) ) )
	{
		if( 0 == strcmp( buf, "true" ) )
		{
			*pValue = 1;
		}

		if( 0 == strcmp( buf, "false" ) )
		{
			*pValue = 0;
		}
	}
	else
	{
        CcspTraceWarning(("syscfg_get failed\n")); 

		return ANSC_STATUS_FAILURE;
	}

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiSetEnableMoCAforXi5Flag
   (
	   ANSC_HANDLE				   hContext,
	   BOOLEAN*					   pValue,
	   BOOLEAN*					   pEnableMoCAforXi5Flag
   )
{
	if ( syscfg_set( NULL, 
					  "X_RDKCENTRAL-COM_EnableMoCAforXi5", 
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

		*pEnableMoCAforXi5Flag = *pValue;

		/* 
		 * We have to enable MoCA based on already xi5 device connected case 
		 * during enable this feature 
		 */
		if( TRUE == *pEnableMoCAforXi5Flag )
		{
			CosaDmlDiCheckAndEnableMoCA( );
		}
	}  

    return ANSC_STATUS_SUCCESS;
}

/* CosaDmlDiCheckAndEnableMoCA() */
void CosaDmlDiCheckAndEnableMoCA( void )
{
	FILE *fp 							 	= NULL;
	BOOL  bMoCAforXi5DeviceConnFileAvail	= FALSE;	
	
	if( ( fp = fopen( "/tmp/MoCAforXi5DeviceConnected", "r" ) ) != NULL )
	{
		fclose( fp );
		bMoCAforXi5DeviceConnFileAvail = TRUE;
	}

	if( bMoCAforXi5DeviceConnFileAvail )
	{
		parameterValStruct_t	value		 = { "Device.MoCA.Interface.1.Enable", "true", ccsp_boolean};
		char					*paramNames[]= { "Device.MoCA.Interface.1.Enable" };
		parameterValStruct_t	**valStrMoCAEnable;
		char  compo[ 256 ]			  = "eRT.com.cisco.spvtg.ccsp.moca";
		char  bus[ 256 ]			  = "/com/cisco/spvtg/ccsp/moca";
		char* faultParam			  = NULL;
		int   ret					  = 0, 
			  nval					  = 0;
		BOOL  bNeedtoEnablMoCA		  = FALSE;
		
		ret = CcspBaseIf_getParameterValues ( bus_handle,
											  compo,
											  bus,
											  paramNames,
											  1,
											  &nval,
											  &valStrMoCAEnable
											 );
		if( ret != CCSP_Message_Bus_OK )
		{
			CcspTraceError(("%s MoCA-Get %s Failed ret %d\n", __FUNCTION__, ret));
			return;
		}
		
		if( strcmp( "false", valStrMoCAEnable[0]->parameterValue ) == 0 )
		{
			bNeedtoEnablMoCA = TRUE;
		}
		
		free_parameterValStruct_t ( bus_handle, nval, valStrMoCAEnable );
		
		/* If MoCA disabled then we have to enable when this case */
		if( bNeedtoEnablMoCA )
		{
			ret = CcspBaseIf_setParameterValues(  bus_handle,
												  compo,
												  bus,
												  0,
												  0,
												  &value,
												  1,
												  TRUE,
												  &faultParam );
		
			CcspTraceWarning(("RDK_LOG_WARN, xi5 detected enabling moca \n"));
		
			if( ret != CCSP_Message_Bus_OK )
			{
				CcspTraceWarning(("RDK_LOG_WARN, MoCA-Set %s : Failed ret %d\n",__FUNCTION__,ret));
			}	
		}
	}
}

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
#elif _COSA_BCM_MIPS_
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
        sprintf(pValue, "%06X%c", CONFIG_VENDOR_ID, '\0');
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDiGetModelName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{    
#ifdef _COSA_BCM_MIPS_

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
#if defined(_XF3_PRODUCT_REQ_)
    AnscCopyString(pValue, "XF3");
#else
    AnscCopyString(pValue, "XB3");
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
#elif _COSA_BCM_MIPS_

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

#ifdef _COSA_BCM_MIPS_

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
#elif _COSA_BCM_MIPS_

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
#if defined(_XF3_PRODUCT_REQ_)
    if ( platform_hal_GetHardware_MemUsed(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue); 
        return ANSC_STATUS_SUCCESS;
    }
#else
        return ANSC_STATUS_SUCCESS;
#endif
}

ANSC_STATUS
CosaDmlDiGetHardware_MemFree
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
#if defined(_XF3_PRODUCT_REQ_)
    if ( platform_hal_GetHardware_MemFree(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else {
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
    }
#else
        return ANSC_STATUS_SUCCESS;
#endif
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
    char value[25] = {0};
    FILE *fp;

    AnscCopyString(pValue, _START_TIME_12AM_);

    fp = popen("cat /nvram/.FirmwareUpgradeStartTime", "r");
    if (fp == NULL)
    {
        CcspTraceError(("ERROR '%s'\n","ERROR"));
    }
    else
    {
        while(fgets(value, 25, fp) != NULL)
        {
            AnscCopyString(pValue ,value);
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

ANSC_STATUS
CosaDmlDiGetFirmwareUpgradeEndTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    char value[25] = {0};
    FILE *fp;

    AnscCopyString(pValue, _END_TIME_3AM_);

    fp = popen("cat /nvram/.FirmwareUpgradeEndTime", "r");
    if (fp == NULL)
    {
        CcspTraceError(("ERROR '%s'\n","ERROR"));
    }
    else
    {
        while(fgets(value, 25, fp) != NULL)
        {
            AnscCopyString(pValue ,value);
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
#ifdef  _COSA_BCM_MIPS_
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
#ifdef _COSA_INTEL_USG_ARM_
	if ( platform_hal_GetUsedMemorySize(&tmp) != RETURN_OK )
        return 0;
    	else 
        return tmp;
#else
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
    fp = popen("cat /proc/cpuinfo", "r");
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

#if defined(_XF3_PRODUCT_REQ_)
    platform_hal_GetFactoryResetCount(pValue);
#endif
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDiClearResetCount
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bValue
   )
{

//	platform_hal_ClearResetCount(bValue);
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

void setLastRebootReason(char* reason)
{

        int val = 1;
        char buf[8];
        snprintf(buf,sizeof(buf),"%d",val);

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


int setXOpsReverseSshArgs(char* pString) {

    char tempCopy[255] = { "\0" };
    char* tempStr;
    char* option;
    char* hostLogin = NULL;

    int inputMsgSize = strlen(pString);

    hostLogin = getHostLogin(pString);
    if (!hostLogin) {
        AnscTraceWarning(("syscfg_get failed\n"));
        printf("Warning !!! Target host for establishing reverse SSH tunnel is missing !!!\n" );
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

#define PARTNER_ID_LEN 64 

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

    if (!pValue || !pulSize || *pulSize >= PARTNER_ID_LEN)
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

#define DMSB_TR181_PSM_Syndication_Tr069CertLocation "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_Syndication.TR69CertLocation"
#define DMSB_TR181_PSM_Syndication_Enable			 "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_Syndication.enable"

ANSC_STATUS
CosaDmlDiGetSyndicationTR69CertLocation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
	char val[ 256 ] = {0};
	
	if ( PsmGet( DMSB_TR181_PSM_Syndication_Tr069CertLocation, val, sizeof( val ) ) != 0 ) 
	{
		pValue[ 0 ] = '\0';
		CcspTraceError(("%s - Failed Get for '%s' \n", __FUNCTION__, DMSB_TR181_PSM_Syndication_Tr069CertLocation));
		return ANSC_STATUS_FAILURE;
	}
	else 
	{
		AnscCopyString( pValue, val );
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

ANSC_STATUS
CosaDmlDiGetSyndicationEnable
    (
        ANSC_HANDLE                  hContext,
		BOOL						*pbEnable
    )
{
	char val[ 16 ] = {0};
	
	if ( PsmGet( DMSB_TR181_PSM_Syndication_Enable, val, sizeof( val ) ) != 0 ) 
	{
		*pbEnable = 0;
		CcspTraceError(("%s - Failed Get for '%s' \n", __FUNCTION__, DMSB_TR181_PSM_Syndication_Enable));
		return ANSC_STATUS_FAILURE;
	}
	else 
	{
		*pbEnable = ( strcmp ( val, "TRUE" ) ? 0 : 1 );
	}

    return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS getFactoryPartnerId
	(
		char*                       pValue,
        PULONG                      pulSize
	)
{
	CcspTraceError(("%s - Failed Get factoryPartnerId \n", __FUNCTION__));
	return ANSC_STATUS_FAILURE;
}

ANSC_STATUS getPartnerId
	(
		char*                       pValue,
        PULONG                      pulSize
    )
{
	char buf[PARTNER_ID_LEN];
	memset(buf, 0, sizeof(buf));
    if(ANSC_STATUS_SUCCESS == syscfg_get( NULL, "PartnerID", buf, sizeof(buf)))
    {
   		if( buf != NULL )
   		{
 			strncpy(pValue ,buf,strlen(buf));
            *pulSize = AnscSizeOfString(pValue);
			return ANSC_STATUS_SUCCESS;
   		}
		else
			return ANSC_STATUS_FAILURE;
    }
	else
		return ANSC_STATUS_FAILURE;

}
ANSC_STATUS setPartnerId
	(
		char*                       pValue
    )
{
	pthread_t tid;
	char* buff = (char*) malloc(strlen(pValue)+1);
	memset(buff,0,strlen(pValue)+1);
	strcpy(buff,pValue);
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

		//Change Handling
		pthread_create(&tid, NULL, &CosaDmlDiPartnerIDChangeHandling, (void*) buff);
	
		return ANSC_STATUS_SUCCESS;
	}
}

#define PARTNERID_FILE  "/nvram/.partner_ID"
void CosaDmlDiPartnerIDChangeHandling( void* buff )
{
	CCSP_MESSAGE_BUS_INFO *bus_info 		  = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
	parameterValStruct_t param_val[ 1 ] 	  = { "Device.X_CISCO_COM_DeviceControl.FactoryReset", "Router,Wifi,VoIP,Dect,MoCA", ccsp_string };
	char				 pComponentName[ 64 ] = "eRT.com.cisco.spvtg.ccsp.pam";
	char				 pComponentPath[ 64 ] = "/com/cisco/spvtg/ccsp/pam";
	char 			PartnerID[64]={0};
	char				*faultParam 		  = NULL;
	FILE				*fp 				  = NULL;
	int 				 ret				  = 0;

	
	pthread_detach(pthread_self());	
	memset(PartnerID,0,sizeof(PartnerID));
	if(buff)
	{
		strcpy(PartnerID,(char*)buff);
		free(buff);		
	}
	
	// Create /nvram/.apply_partner_defaults file to apply partners default
	system( "touch /nvram/.apply_partner_defaults" );
	system( "syscfg set PartnerID_FR 1; syscfg commit" );
	//Creating and Writing partner ID into /nvram/.partner_ID file
	fp = fopen( PARTNERID_FILE, "w" );
	
	if ( fp != NULL ) 
	{
		fwrite( PartnerID, strlen( PartnerID ), 1, fp );
		fclose( fp );
		AnscTraceWarning(("%s: Partner ID %s is Written into %s File\n", __FUNCTION__, PartnerID, PARTNERID_FILE ));
	}

	/* Need to do factory reset the device here */
	ret = CcspBaseIf_setParameterValues
			(
				bus_handle, 
				pComponentName, 
				pComponentPath,
				0, 
				0x0,   /* session id and write id */
				&param_val, 
				1, 
				TRUE,	/* Commit  */
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
}

ANSC_STATUS
CosaDeriveSyndicationPartnerID(char *Partner_ID)
{
	char PartnerID[PARTNER_ID_LEN];
	ULONG size = PARTNER_ID_LEN - 1;
	memset(PartnerID, 0, sizeof(PartnerID));
	CcspTraceInfo(("%s \n",__FUNCTION__));
// get if PartnerID is already available
	if(ANSC_STATUS_SUCCESS == getPartnerId(PartnerID, &size))
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
			 if(ANSC_STATUS_FAILURE == CosaDmlDiGetSyndicationPartnerId(NULL,&PartnerID, &size))
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
	strncpy(Partner_ID,PartnerID,sizeof(PartnerID));
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiSetSyndicationEnable
    (
        ANSC_HANDLE                  hContext,
		BOOL						 bEnable
    )
{
	int  retPsmSet = CCSP_SUCCESS;
	
	retPsmSet = PSM_Set_Record_Value2( g_MessageBusHandle, 
									   g_GetSubsystemPrefix(g_pDslhDmlAgent), 
									   DMSB_TR181_PSM_Syndication_Enable, 
									   ccsp_string,
									   ( bEnable ) ? "TRUE"  : "FALSE" );
	if ( retPsmSet != CCSP_SUCCESS ) 
	{
		CcspTraceError(("%s - Failed Set for '%s' \n", __FUNCTION__, DMSB_TR181_PSM_Syndication_Enable));
		return ANSC_STATUS_FAILURE;
	}

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

ANSC_STATUS
CosaDmlDiUiBrandingInit
  (
	PCOSA_DATAMODEL_RDKB_UIBRANDING	PUiBrand
  )
 {
        
	char *data = NULL;
	cJSON *cfgValObj = NULL;
	cJSON *json = NULL;
	FILE *fileRead = NULL;
	char PartnerID[PARTNER_ID_LEN] = {0};
	char cmd[512] = {0};
	ULONG size = PARTNER_ID_LEN - 1;
	int len;

	if (!PUiBrand)
	{
		CcspTraceWarning(("%s-%d : NULL param\n" , __FUNCTION__, __LINE__ ));
		return ANSC_STATUS_FAILURE;	
	}

	memset(PUiBrand, 0, sizeof(COSA_DATAMODEL_RDKB_UIBRANDING));
	if (access(PARTNERS_INFO_FILE, F_OK) != 0)	
	{
		snprintf(cmd, sizeof(cmd), "cp %s %s", "/etc/partners_defaults.json", PARTNERS_INFO_FILE);
		CcspTraceWarning(("%s\n",cmd));
		system(cmd);
	}


	 fileRead = fopen( PARTNERS_INFO_FILE, "r" );
	 if( fileRead == NULL ) 
	 {
		 CcspTraceWarning(("%s-%d : Error in opening JSON file\n" , __FUNCTION__, __LINE__ ));
		 return ANSC_STATUS_FAILURE;
	 }
	 
	 fseek( fileRead, 0, SEEK_END );
	 len = ftell( fileRead );
	 fseek( fileRead, 0, SEEK_SET );
	 data = ( char* )malloc( len + 1 );
	 if (data != NULL) 
	 {
	 	fread( data, 1, len, fileRead );
	 } 
	 else 
	 {
		 CcspTraceWarning(("%s-%d : Memory allocation failed \n", __FUNCTION__, __LINE__));
		 fclose( fileRead );
		 return ANSC_STATUS_FAILURE;
	 }
	 
	 fclose( fileRead );
	 
	 if( data != NULL ) 
	 {
		 json = cJSON_Parse( data );
		 if( !json ) 
		 {
			 CcspTraceWarning((  "%s-%s : json file parser error : [%d]\n", cJSON_GetErrorPtr() ,__FUNCTION__,__LINE__));
			 free(data);
			 return ANSC_STATUS_FAILURE;
		 } 
		 else 
		 {
			 if(ANSC_STATUS_SUCCESS == getPartnerId(PartnerID, &size))
			 {
			 	if ( PartnerID[0] != '\0' )
			 	{
					FillPartnerIDValues(json, PartnerID, PUiBrand);
			 	}
				else
				{
					CcspTraceWarning(( "Reading Deafult PartnerID Values \n" ));
					strcpy(PartnerID, "comcast");
					FillPartnerIDValues(json, PartnerID, PUiBrand);
				}
			}
	 		else{
				CcspTraceWarning(("Failed to get Partner ID\n"));
	 		}
			cJSON_Delete(json);
		}
	  free(data);
	  data = NULL;			 
	 }
	 return ANSC_STATUS_SUCCESS;
 }


ANSC_STATUS
CosaDmlDiWiFiTelemetryInit
  (
	PCOSA_DATAMODEL_RDKB_WIFI_TELEMETRY PWiFi_Telemetry
  )
 {
    char val[256] = {0};

    if (!PWiFi_Telemetry)
    {
        CcspTraceWarning(("%s-%d : NULL param\n" , __FUNCTION__, __LINE__ ));
        return ANSC_STATUS_FAILURE;
    }

    memset(PWiFi_Telemetry, 0, sizeof(COSA_DATAMODEL_RDKB_WIFI_TELEMETRY));

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
            AnscCopyString(PWiFi_Telemetry->NormalizedRssiList, "1,2");
    }
    else
    {
        if (val[0] != '\0' )
        {
            AnscCopyString(PWiFi_Telemetry->NormalizedRssiList, val);
        }
        else
        {
            AnscCopyString(PWiFi_Telemetry->NormalizedRssiList, "1,2");
        }
    }

    if (PsmGet(DMSB_TR181_PSM_WHIX_CliStatList, val, sizeof(val)) != 0)
    {
            AnscCopyString(PWiFi_Telemetry->CliStatList,"");
    }
    else
    {
        if (val[0] != '\0' )
        {
            AnscCopyString(PWiFi_Telemetry->CliStatList, val);
        }
        else
        {
            AnscCopyString(PWiFi_Telemetry->CliStatList,"");
        }
    }

    if (PsmGet(DMSB_TR181_PSM_WHIX_TxRxRateList, val, sizeof(val)) != 0)
    {
            AnscCopyString(PWiFi_Telemetry->TxRxRateList, "1,2");
    }
    else
    {
        if (val[0] != '\0' )
        {
            AnscCopyString(PWiFi_Telemetry->TxRxRateList, val);
        }
        else
        {
            AnscCopyString(PWiFi_Telemetry->TxRxRateList,"1,2");
        }
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

    PUniqueTelemetryId->Enable = FALSE;
    memset(PUniqueTelemetryId->TagString, 0, sizeof(PUniqueTelemetryId->TagString));
    PUniqueTelemetryId->TimingInterval = 0;

    if (syscfg_get(NULL, "unique_telemetry_enable", buf, sizeof(buf)) == 0)
    {
        if (buf != NULL)
        {
            PUniqueTelemetryId->Enable = (strcmp(buf,"true") ? FALSE : TRUE);
        }
    }

    memset(buf, 0, sizeof(buf));

    if (syscfg_get(NULL, "unique_telemetry_tag", buf,  sizeof(buf) ) == 0)
    {
        if (buf != NULL)
        {
            AnscCopyString(PUniqueTelemetryId->TagString, buf);
        }
    }

    memset(buf, 0, sizeof(buf));

    if (syscfg_get( NULL, "unique_telemetry_interval", buf, sizeof(buf)) == 0)
    {
        if (buf != NULL)
        {
            PUniqueTelemetryId->TimingInterval =  atoi(buf);
        }
    }
 
    return ANSC_STATUS_SUCCESS;
}

// Convert time interval(in miniutes) to days, hours and minutes.
void ConvertTime(int time, char day[], char hour[], char mins[]) {
        int d = 0, h = 0, m = 0;

        d = (time / (60*24));
        h = ((time % (60*24)) / 60);
        m = ((time % (60*24)) % 60);

        if(d > 0) {
                sprintf(day, "*/%d", d);
                sprintf(hour, "%d", h);
                sprintf(mins, "%d", m);
        }
        else if(h > 0) {
                sprintf(day, "*");
                sprintf(hour, "*/%d", h);
                sprintf(mins, "%d", m);

        }
        else {
                sprintf(day, "*");
                sprintf(hour, "*");
                sprintf(mins, "*/%d", m);
        }

}

//Handle UniqueTelemetry Cron Job
void UniqueTelemetryCronJob(enable, timeInterval, tagString) {
        char command[256] = {0};
        char day[5] = {0}, hour[5]={0}, mins[5] = {0};

        if(enable) {       //Add unique_telemetry_id Cron job to job list
            if( timeInterval != 0 && strlen(tagString) > 0) {
                ConvertTime(timeInterval, day, hour, mins);      // Convert time interval

                system("crontab -l | grep -v '/usr/ccsp/pam/unique_telemetry_id.sh'  | crontab -");     //Remove unique_telemetry_id Cron job if already exists
                sprintf(command, "(crontab -l ; echo \"%s %s %s * * /usr/ccsp/pam/unique_telemetry_id.sh\") | crontab -", mins, hour, day);
                system(command);
            }
        }
        else {          //Remove unique_telemetry_id Cron job from job list
            system("crontab -l | grep -v '/usr/ccsp/pam/unique_telemetry_id.sh'  | crontab -");
        }
}

void FillPartnerIDValues(cJSON *json , char *partnerID , PCOSA_DATAMODEL_RDKB_UIBRANDING	PUiBrand)
{
		cJSON *partnerObj = NULL;
		char *partnerLink = NULL;
		char *userGuideLink = NULL;
		char *customerCentralLink = NULL;
		char *msoMenu = NULL;
		char *msoInfo = NULL;
		char *statusTitle = NULL;
		char *statusInfo = NULL;
		char *connectivityTestURL = NULL;
		char *support = NULL;
		char *partnerHelpLink = NULL;
		char *smsSupport = NULL;
		char *myAccountAppSupport = NULL;
		char *DefaultLocalIPv4SubnetRange = NULL;
		char *DefaultAdminIP = NULL; 
		char *WifiMSOLogo = NULL;
		char *DefaultLoginPassword = NULL;
		char *DefaultLoginUsername = NULL;
		char *UIMSOLogo = NULL;
		char *PauseScreenFileLocation = NULL;		
		char *partnerText = NULL;
		char *userGuideText = NULL;
		char *customerCentralText = NULL;
		char *wifiTitle = NULL;
		char *wifiWelcomeMessage = NULL;
		
		partnerObj = cJSON_GetObjectItem( json, partnerID );
		if( partnerObj != NULL) 
		{
				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerLink") != NULL )
				{
					partnerLink = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerLink")->valuestring; 

					if (partnerLink != NULL) 
					{
						AnscCopyString(PUiBrand->Footer.PartnerLink, partnerLink);
						partnerLink = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - partnerLink Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - partnerLink Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideLink") != NULL)
				{
					userGuideLink = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideLink")->valuestring;

					if (userGuideLink != NULL) 
					{
						AnscCopyString(PUiBrand->Footer.UserGuideLink, userGuideLink);
						userGuideLink = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - userGuideLink Value is NULL\n", __FUNCTION__ ));
					}
				}

				else
				{
					CcspTraceWarning(("%s - userGuideLink Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralLink") != NULL)
				{
					customerCentralLink = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralLink")->valuestring;

					if (customerCentralLink != NULL) 
					{
						AnscCopyString(PUiBrand->Footer.CustomerCentralLink, customerCentralLink);
						customerCentralLink = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - customerCentralLink Value is NULL\n", __FUNCTION__ ));
					}

				}

				else
				{
					CcspTraceWarning(("%s - customerCentralLink Object is NULL\n", __FUNCTION__ ));
				}
				
				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerText") != NULL )
				{
					partnerText = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerText")->valuestring; 

					if (partnerText != NULL) 
					{
						AnscCopyString(PUiBrand->Footer.PartnerText, partnerText);
						partnerText = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - partnerText Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - partnerText Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideText") != NULL )
				{
					userGuideText = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideText")->valuestring; 

					if (userGuideText != NULL) 
					{
						AnscCopyString(PUiBrand->Footer.UserGuideText, userGuideText);
						userGuideText = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - userGuideText Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - userGuideText Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralText") != NULL )
				{
					customerCentralText = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralText")->valuestring; 

					if (customerCentralText != NULL) 
					{
						AnscCopyString(PUiBrand->Footer.CustomerCentralText, customerCentralText);
						customerCentralText = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - customerCentralText Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - customerCentralText Object is NULL\n", __FUNCTION__ ));
				}
				
				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOmenu") != NULL )
				{
					msoMenu = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOmenu")->valuestring;

					if (msoMenu != NULL) 
					{
						AnscCopyString(PUiBrand->Connection.MSOmenu, msoMenu);
						msoMenu = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - msoMenu Value is NULL\n", __FUNCTION__ ));
					}
				}

				else
				{
					CcspTraceWarning(("%s - msoMenu Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOinfo") != NULL)
				{
					msoInfo = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOinfo")->valuestring;

					if (msoInfo != NULL) 
					{
						AnscCopyString(PUiBrand->Connection.MSOinfo, msoInfo);
						msoInfo = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - msoInfo Value is NULL\n", __FUNCTION__ ));
					}
				}

				else
				{
					CcspTraceWarning(("%s - msoInfo Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusTitle") != NULL)
				{
					statusTitle = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusTitle")->valuestring;

					if (statusTitle != NULL) 
					{
						AnscCopyString(PUiBrand->Connection.StatusTitle, statusTitle);
						statusTitle = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - statusTitle Value is NULL\n", __FUNCTION__ ));
					}
				}

				else
				{
					CcspTraceWarning(("%s - statusTitle Object is NULL\n", __FUNCTION__ ));
				}
				
				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusInfo") != NULL)
				{
					statusInfo = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusInfo")->valuestring;

					if (statusInfo != NULL) 
					{
						AnscCopyString(PUiBrand->Connection.StatusInfo, statusInfo);
						statusInfo = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - statusInfo Value is NULL\n", __FUNCTION__ ));
					}
				}

				else
				{
					CcspTraceWarning(("%s - statusInfo Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.NetworkDiagnosticTools.ConnectivityTestURL") != NULL)
				{
					connectivityTestURL = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.NetworkDiagnosticTools.ConnectivityTestURL")->valuestring;

					if (connectivityTestURL != NULL) 
					{
						AnscCopyString(PUiBrand->NDiagTool.ConnectivityTestURL, connectivityTestURL);
						connectivityTestURL = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - connectivityTestURL Value is NULL\n", __FUNCTION__ ));
					}
				}

				else
				{
					CcspTraceWarning(("%s - connectivityTestURL Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.Support") != NULL)
				{
					support = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.Support")->valuestring;

					if (support != NULL) 
					{
						PUiBrand->WifiPersonal.Support = FALSE;
						if ( strcmp(support,"true") == 0)
						{
							PUiBrand->WifiPersonal.Support = TRUE;
							support = NULL;
						}
					}	
					else
					{
						CcspTraceWarning(("%s - support Value is NULL\n", __FUNCTION__ ));
					}
				}

				else
				{
					CcspTraceWarning(("%s - support Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.PartnerHelpLink") != NULL)
				{
					partnerHelpLink = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.PartnerHelpLink")->valuestring;

					if (partnerHelpLink != NULL) 
					{
						AnscCopyString(PUiBrand->WifiPersonal.PartnerHelpLink, partnerHelpLink);
						partnerHelpLink = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - partnerHelpLink Value is NULL\n", __FUNCTION__ ));
					}
				}

				else
				{
					CcspTraceWarning(("%s - partnerHelpLink Object is NULL\n", __FUNCTION__ ));
				}
				
				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.SMSsupport") != NULL)
				{
					smsSupport = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.SMSsupport")->valuestring;

					if (smsSupport != NULL) 
					{
						PUiBrand->WifiPersonal.SMSsupport = FALSE;
						if ( strcmp(smsSupport,"true") == 0)
						{
							PUiBrand->WifiPersonal.SMSsupport = TRUE;
							smsSupport = NULL;
						}
					}	
					else
					{
						CcspTraceWarning(("%s - smsSupport Value is NULL\n", __FUNCTION__ ));
					}
				}

				else
				{
					CcspTraceWarning(("%s - smsSupport Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.MyAccountAppSupport") != NULL)
				{
					myAccountAppSupport = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.MyAccountAppSupport")->valuestring;	

					if (myAccountAppSupport != NULL) 
					{
						PUiBrand->WifiPersonal.MyAccountAppSupport = FALSE;
						if ( strcmp(myAccountAppSupport,"true") == 0)
						{
							PUiBrand->WifiPersonal.MyAccountAppSupport = TRUE;
							myAccountAppSupport = NULL;
						}
					}	
					else
					{
						CcspTraceWarning(("%s - MyAccountAppSupport Value is NULL\n", __FUNCTION__ ));
					}
				}

				else
				{
					CcspTraceWarning(("%s - MyAccountAppSupport Object is NULL\n", __FUNCTION__ ));
				}
				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.MSOLogo") != NULL )
				{
					UIMSOLogo = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.MSOLogo")->valuestring; 

					if (UIMSOLogo != NULL) 
					{
						AnscCopyString(PUiBrand->LocalUI.MSOLogo, UIMSOLogo);
						UIMSOLogo = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - UIMSOLogo Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - UIMSOLogo Object is NULL\n", __FUNCTION__ ));
				}
				
				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.DefaultLoginUsername") != NULL )
				{
					DefaultLoginUsername = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.DefaultLoginUsername")->valuestring; 

					if (DefaultLoginUsername != NULL) 
					{
						AnscCopyString(PUiBrand->LocalUI.DefaultLoginUsername, DefaultLoginUsername);
						DefaultLoginUsername = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - DefaultLoginUsername Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - DefaultLoginUsername Object is NULL\n", __FUNCTION__ ));
				}
				
				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.DefaultLoginPassword") != NULL )
				{
					DefaultLoginPassword = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.DefaultLoginPassword")->valuestring; 

					if (DefaultLoginPassword != NULL) 
					{
						AnscCopyString(PUiBrand->LocalUI.DefaultLoginPassword, DefaultLoginPassword);
						DefaultLoginPassword = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - DefaultLoginPassword Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - DefaultLoginPassword Object is NULL\n", __FUNCTION__ ));
				}
				
				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.MSOLogo") != NULL )
				{
					WifiMSOLogo = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.MSOLogo")->valuestring; 

					if (WifiMSOLogo != NULL) 
					{
						AnscCopyString(PUiBrand->WifiPersonal.MSOLogo, WifiMSOLogo);
						WifiMSOLogo = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - WifiMSOLogo Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - WifiMSOLogo Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.Title") != NULL )
				{
					wifiTitle = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.Title")->valuestring; 

					if (wifiTitle != NULL) 
					{
						AnscCopyString(PUiBrand->WifiPersonal.Title, wifiTitle);
						wifiTitle = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - wifiTitle Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - wifiTitle Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.WelcomeMessage") != NULL )
				{
					wifiWelcomeMessage = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.WelcomeMessage")->valuestring; 

					if (wifiWelcomeMessage != NULL) 
					{
						AnscCopyString(PUiBrand->WifiPersonal.WelcomeMessage, wifiWelcomeMessage);
						wifiWelcomeMessage = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - wifiWelcomeMessage Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - wifiWelcomeMessage Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.DefaultAdminIP") != NULL )
				{
					DefaultAdminIP = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.DefaultAdminIP")->valuestring; 

					if (DefaultAdminIP != NULL) 
					{
						AnscCopyString(PUiBrand->DefaultAdminIP, DefaultAdminIP);
						DefaultAdminIP = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - DefaultAdminIP Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - DefaultAdminIP Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.DefaultLocalIPv4SubnetRange") != NULL )
				{
					DefaultLocalIPv4SubnetRange = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.DefaultLocalIPv4SubnetRange")->valuestring; 

					if (DefaultLocalIPv4SubnetRange != NULL) 
					{
						AnscCopyString(PUiBrand->DefaultLocalIPv4SubnetRange, DefaultLocalIPv4SubnetRange);
						DefaultLocalIPv4SubnetRange = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - DefaultLocalIPv4SubnetRange Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - DefaultLocalIPv4SubnetRange Object is NULL\n", __FUNCTION__ ));
				}

				if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.PauseScreenFileLocation") != NULL )
				{
					PauseScreenFileLocation = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.PauseScreenFileLocation")->valuestring; 

					if (PauseScreenFileLocation != NULL) 
					{
						AnscCopyString(PUiBrand->PauseScreenFileLocation, PauseScreenFileLocation);
						PauseScreenFileLocation = NULL;
					}	
					else
					{
						CcspTraceWarning(("%s - PauseScreenFileLocation Value is NULL\n", __FUNCTION__ ));
					}
					
				}

				else
				{
					CcspTraceWarning(("%s - PauseScreenFileLocation Object is NULL\n", __FUNCTION__ ));
				}
				
			}
			
			else
			{
				CcspTraceWarning(("%s - PARTNER ID OBJECT Value is NULL\n", __FUNCTION__ ));
			}
		

}

ANSC_STATUS UpdateJsonParam
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
	 fseek( fileRead, 0, SEEK_SET );
	 data = ( char* )malloc( len + 1 );
	 if (data != NULL) 
	 {
	 	fread( data, 1, len, fileRead );
	 } 
	 else 
	 {
		 CcspTraceWarning(("%s-%d : Memory allocation failed \n", __FUNCTION__, __LINE__));
		 fclose( fileRead );
		 return ANSC_STATUS_FAILURE;
	 }
	 
	 fclose( fileRead );
	  if( data != NULL ) 
	 {
		 json = cJSON_Parse( data );
		 if( !json ) 
		 {
			 CcspTraceWarning((  "%s-%d : json file parser error : [%s]\n", cJSON_GetErrorPtr() ,__FUNCTION__,__LINE__));
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
					configUpdateStatus = writeToJson(cJsonOut);
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
	 return ANSC_STATUS_SUCCESS;
}

static int writeToJson(char *data)
{
    FILE *fp;
    fp = fopen(PARTNERS_INFO_FILE, "w");
    if (fp == NULL) 
    {
        CcspTraceWarning(("%s : %d Failed to open file %s\n", __FUNCTION__,__LINE__,PARTNERS_INFO_FILE));
        return -1;
    }
    
    fwrite(data, strlen(data), 1, fp);
    fclose(fp);
    return 0;
}
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

        sprintf(buf,"%d",uValue);
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
                        *DeferFWDownloadReboot =        uValue;
                }
        }
}

void* RebootDevice_thread(void* buff)
{
        char pValue[128],source_str[64];
        char* source = NULL;
        int router, wifi, voip, dect, moca, all;
    int delay_time = 0;

        pthread_detach(pthread_self());

        memset(pValue,0,sizeof(pValue));
        if(buff)
        {
                strcpy(pValue,buff);
                free(buff);
        }

    router = wifi = voip = dect = moca = all = 0;
    if (strcasestr(pValue, "Router") != NULL) {
        router = 1;
    }
    if (strcasestr(pValue, "Wifi") != NULL) {
        wifi = 1;
    }
    if (strcasestr(pValue, "VoIP") != NULL) {
        voip = 1;
    }
    if (strcasestr(pValue, "Dect") != NULL) {
        dect = 1;
    }
    if (strcasestr(pValue, "MoCA") != NULL) {
        moca = 1;
    }
    if (strcasestr(pValue, "Device") != NULL) {
        all = 1;
    }

    if (strcasestr(pValue, "delay=") != NULL) {
        delay_time = atoi(strcasestr(pValue, "delay=") + strlen("delay="));
    }

        if(strcasestr(pValue, "source=") != NULL){
                source = strcasestr(pValue, "source=") + strlen("source=");
                int i=0;
                while(source[i] != ' ' && source[i] != '\0'){
                        source_str[i] = source[i];
                        i++;
                }
                source_str[i] = '\0';
        }
        else{
                strcpy(source_str,"webpa-reboot");
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
                snprintf(buf,sizeof(buf),"%d",rebootcount);
                syscfg_set(NULL, "reboot_count", buf);

                FILE *fp = NULL;
                memset(buf,0,sizeof(buf));
                sprintf(buf, "date");
                char buffer[50] = {0};
                memset(buffer,0,sizeof(buffer));
        fp = popen(buf, "r");
                if( fp != NULL) {
                    while(fgets(buffer, sizeof(buffer), fp)!=NULL){
                            buffer[strlen(buffer) - 1] = '\0';
                                syscfg_set(NULL, "latest_reboot_time", buffer);
                        }
                        pclose(fp);
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
               system("/fss/gw/rdklogger/backupLogs.sh");
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

}

void CosaDmlDiSet_RebootDevice(char* pValue)
{
        pthread_t tid;

        char* buff = (char*) malloc(strlen(pValue)+1);
        memset(buff,0,strlen(pValue)+1);
        strcpy(buff,pValue);
        pthread_create(&tid, NULL, &RebootDevice_thread, (void*) buff);

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

BOOL
CosaDmlDi_ValidateRebootDeviceParam( char *pValue )
{
	BOOL IsProceedFurther	= FALSE,
		  IsActionValid 	= FALSE,
		  IsSourceValid 	= FALSE,
		  IsDelayValid		= FALSE;
	CcspTraceWarning(("%s %d - String :%s", __FUNCTION__, __LINE__, ( pValue != NULL ) ?  pValue : "NULL" ));
	if (strcasestr(pValue, "delay=") != NULL) {
		IsDelayValid = TRUE;
	}

	if(strcasestr(pValue, "source=") != NULL) {
		IsSourceValid = TRUE;
	}

	if (strcasestr(pValue, "Router") != NULL) {
		IsActionValid = TRUE;
	}

	if (strcasestr(pValue, "Wifi") != NULL) {
		IsActionValid = TRUE;
	}

	if (strcasestr(pValue, "VoIP") != NULL) {
		IsActionValid = TRUE;
	}

	if (strcasestr(pValue, "Dect") != NULL) {
		IsActionValid = TRUE;
	}

	if (strcasestr(pValue, "MoCA") != NULL) {
		IsActionValid = TRUE;
	}

	if (strcasestr(pValue, "Device") != NULL) {
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
			char *subStringForDelay  = NULL,
			     *subStringForDummy  = NULL;

			strcpy( tmpCharBuffer,	pValue );
			subStringForDelay       = strtok( tmpCharBuffer, " " );
			subStringForDummy   = strtok( NULL, " " );
			if ( strcasestr(subStringForDelay, "delay=") != NULL )
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
			char *subStringForSource = NULL,
			     *subStringForDummy  = NULL;

			strcpy( tmpCharBuffer,	pValue );
			subStringForSource   = strtok( tmpCharBuffer, " " );
			subStringForDummy   = strtok( NULL, " " );
			if ( strcasestr(subStringForSource, "source=") != NULL )
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
			char *subStringForDelay 	 = NULL,
				*subStringForSource 	 = NULL,
				*subStringForDummy  = NULL;
			strcpy( tmpCharBuffer,	pValue );
			subStringForDelay   = strtok( tmpCharBuffer, " " );
			if ( (strcasestr(subStringForDelay, "delay=") != NULL )  || (strcasestr(subStringForDelay, "source=") != NULL ) )
			{
				subStringForSource = strtok( NULL, " " );
				if ( (strcasestr(subStringForSource, "delay=") != NULL )  || (strcasestr(subStringForSource, "source=") != NULL ) )
				{
					subStringForDummy   = strtok( NULL, " " );
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
