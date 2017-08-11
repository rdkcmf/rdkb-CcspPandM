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

#include "cosa_x_comcast_com_parentalcontrol_apis.h"
//RDKB-EMULATOR
#include "dmsb_tr181_psm_definitions.h"
#include<stdbool.h>
//PSM-ACCESS
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
#define _PSM_WRITE_PARAM(_PARAM_NAME) { \
	_ansc_sprintf(param_name, _PARAM_NAME,instancenum); \
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
#define _PSM_WRITE_PARAMs(_PARAM_NAME) { \
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
#define _PSM_READ_PARAM(_PARAM_NAME) { \
	_ansc_memset(param_name, 0, sizeof(param_name)); \
	_ansc_sprintf(param_name, _PARAM_NAME,instancenum); \
	retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value); \
	if (retPsmGet != CCSP_SUCCESS) { \
		AnscTraceFlow(("%s Error %d reading %s %s\n", __FUNCTION__, retPsmGet, param_name, param_value));\
	} \
	else { \
		AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s = \n%s\n", __FUNCTION__,param_name, param_value)); \
		printf("param_name (%s) and param_value (%s) \n",param_name, param_value); \
	} \
}
#define _PSM_READ_PARAMs(_PARAM_NAME) { \
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

#define _PSM_DEL_PARAM(_PARAM_NAME) { \
	_ansc_sprintf(param_name,_PARAM_NAME,ins); \
	AnscTraceFlow(("%s ins = %ld param_name=%s\n", __FUNCTION__,\
				ins,param_name));\
	PSM_Del_Record(bus_handle, g_Subsystem, param_name); \
	_ansc_memset(param_name, 0, sizeof(param_name)); \
}

#ifdef _COSA_SIM_
//RDKB-EMULATOR
//Enable and Disable options values storage of ParentalControl(ManagedSites,ManagedDevices,ManagedServices) in PSM Database
void PSM_Get_ManagedSites_RecordValues(COSA_DML_MANAGEDSITES *conf)
{
	int retPsmGet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char *param_value = NULL;
	_PSM_READ_PARAMs(PSM_PC_MS_ENABLE);
	if(strcmp(param_value,"TRUE") == 0)
		conf->Enable = TRUE;
	else
		conf->Enable = FALSE;
}
void  PSM_Get_ManagedServices_RecordValues(COSA_DML_MANAGED_SERVS *conf)
{
	int retPsmGet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char *param_value = NULL;
	_PSM_READ_PARAMs(PSM_PC_MSE_ENABLE);
	if(strcmp(param_value,"TRUE") == 0)
		conf->Enable = TRUE;
	else
		conf->Enable = FALSE;
}
void PSM_Get_ManagedDevices_RecordValues(COSA_DML_MANAGED_DEVS *conf)
{
	int retPsmGet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char *param_value = NULL;
	_PSM_READ_PARAMs(PSM_PC_MD_ENABLE);
	if(strcmp(param_value,"TRUE") == 0)
		conf->Enable = TRUE;
	else
		conf->Enable = FALSE;
	_PSM_READ_PARAMs(PSM_PC_MD_ALLOWALL);
	if(strcmp(param_value,"TRUE") == 0)
		conf->AllowAll = TRUE;
	else
		conf->AllowAll = FALSE;
}
void PSM_Set_ParentalControl_ManagedSites_RecordValues(bool Enable)
{
	int retPsmSet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char param_value[256] = {0};
	if(Enable == TRUE)
		_ansc_sprintf(param_value,"%s","TRUE");
	else
		_ansc_sprintf(param_value,"%s","FALSE");

	_PSM_WRITE_PARAMs(PSM_PC_MS_ENABLE);
}
void PSM_Set_ParentalControl_ManagedServices_RecordValues(COSA_DML_MANAGED_SERVS *Conf)
{
	int retPsmSet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char param_value[256] = {0};
	if(Conf->Enable == TRUE)
		_ansc_sprintf(param_value,"%s","TRUE");
	else
		_ansc_sprintf(param_value,"%s","FALSE");

	_PSM_WRITE_PARAMs(PSM_PC_MSE_ENABLE);
}
void PSM_Set_ParentalControl_ManagedDevices_RecordValues(COSA_DML_MANAGED_DEVS *conf)
{
	int retPsmSet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char param_value[256] = {0};
	if(conf->Enable == TRUE)
		_ansc_sprintf(param_value,"%s","TRUE");
	else
		_ansc_sprintf(param_value,"%s","FALSE");

	_PSM_WRITE_PARAMs(PSM_PC_MD_ENABLE);
	if(conf->AllowAll == TRUE)
		_ansc_sprintf(param_value,"%s","TRUE");
	else
		_ansc_sprintf(param_value,"%s","FALSE");

	_PSM_WRITE_PARAMs(PSM_PC_MD_ALLOWALL);
}
//Set and Get of ManagedSites values and TrustedUsers of ManagedSites values (in parental control features ) from PSM Database..
void PSMGetManagedSitesBlockedURLRecordValues(PCOSA_DML_BLOCKEDURL pBlockedURL,ULONG instancenum)
{
	printf("========== %s =========\n",__FUNCTION__);
	int retPsmGet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char *param_value = NULL;

	pBlockedURL->InstanceNumber=instancenum;

	_PSM_READ_PARAM(PSM_PC_MS_BLOCKMETHOD);
	if(strcmp(param_value,"URL") == 0)
		pBlockedURL->BlockMethod = BLOCK_METHOD_URL;
	else
		pBlockedURL->BlockMethod = BLOCK_METHOD_KEYWORD;

	_PSM_READ_PARAM(PSM_PC_MS_SITE);
	AnscCopyString( pBlockedURL->Site,param_value);

	_PSM_READ_PARAM(PSM_PC_MS_ALWAYSBLOCK);
	if(strcmp(param_value,"TRUE") == 0)
		pBlockedURL->AlwaysBlock = TRUE;
	else
		pBlockedURL->AlwaysBlock = FALSE;

	_PSM_READ_PARAM(PSM_PC_MS_STARTTIME);
	AnscCopyString( pBlockedURL->StartTime,param_value);

	_PSM_READ_PARAM(PSM_PC_MS_ENDTIME);
	AnscCopyString( pBlockedURL->EndTime,param_value);

	_PSM_READ_PARAM(PSM_PC_MS_BLOCKDAYS);
	AnscCopyString( pBlockedURL->BlockDays,param_value);
}

void PSMGetTrustedManagedSitesRecordValues(PCOSA_DML_TRUSTEDUSER pTuser,ULONG instancenum)
{
	printf("========== %s =========\n",__FUNCTION__);
	int retPsmGet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char *param_value = NULL;

	pTuser->InstanceNumber=instancenum;

	_PSM_READ_PARAM(PSM_PC_MS_TRUSTED_ENABLE);
	if(strcmp(param_value,"TRUE") == 0)
		pTuser->Trusted = TRUE;
	else
		pTuser->Trusted = FALSE;

	_PSM_READ_PARAM(PSM_PC_MS_TRUSTED_HOST_DESCRIPTION);
	AnscCopyString( pTuser->HostDescription,param_value);

	_PSM_READ_PARAM(PSM_PC_MS_TRUSTED_IPADDRESS);
	AnscCopyString( pTuser->IPAddress,param_value);


	_PSM_READ_PARAM(PSM_PC_MS_TRUSTED_IPADDRESS_TYPE);
	if(strcmp(param_value,"IPv4") == 0)
		pTuser->IPAddressType = IPADDR_IPV4;
	else
		pTuser->IPAddressType = IPADDR_IPV6;

}

void PSM_Set_ParentalControl_ManagedBlkURL_RecordValues(COSA_DML_BLOCKEDURL *pEntry)
{
	int retPsmSet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char param_value[256] = {0};
	ULONG instancenum = pEntry->InstanceNumber;
	if(pEntry->BlockMethod == BLOCK_METHOD_URL)
		_ansc_sprintf(param_value,"%s","URL");
	else
		_ansc_sprintf(param_value,"%s","Keyword");
	_PSM_WRITE_PARAM(PSM_PC_MS_BLOCKMETHOD);
	_ansc_sprintf(param_value,"%s",pEntry->Site);
	_PSM_WRITE_PARAM(PSM_PC_MS_SITE);
	if(pEntry->AlwaysBlock == TRUE)
		_ansc_sprintf(param_value,"%s","TRUE");
	else
		_ansc_sprintf(param_value,"%s","FALSE");
	_PSM_WRITE_PARAM(PSM_PC_MS_ALWAYSBLOCK);
	_ansc_sprintf(param_value,"%s",pEntry->StartTime);
	_PSM_WRITE_PARAM(PSM_PC_MS_STARTTIME);
	_ansc_sprintf(param_value,"%s",pEntry->EndTime);
	_PSM_WRITE_PARAM(PSM_PC_MS_ENDTIME);
	_ansc_sprintf(param_value,"%s",pEntry->BlockDays);
	_PSM_WRITE_PARAM(PSM_PC_MS_BLOCKDAYS);

}
void PSM_Set_ParentalControl_ManagedSites_Trusted_RecordValues(COSA_DML_TRUSTEDUSER *pEntry)
{
	int retPsmSet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char param_value[256] = {0};
	ULONG instancenum = pEntry->InstanceNumber;
	_ansc_sprintf(param_value,"%s",pEntry->HostDescription);
	_PSM_WRITE_PARAM(PSM_PC_MS_TRUSTED_HOST_DESCRIPTION);

	_ansc_sprintf(param_value,"%s",pEntry->IPAddress);
	_PSM_WRITE_PARAM(PSM_PC_MS_TRUSTED_IPADDRESS);
	if(pEntry->Trusted == TRUE)
		_ansc_sprintf(param_value,"%s","TRUE");
	else
		_ansc_sprintf(param_value,"%s","FALSE");

	_PSM_WRITE_PARAM(PSM_PC_MS_TRUSTED_ENABLE);
	if(pEntry->IPAddressType == IPADDR_IPV4)
		_ansc_sprintf(param_value,"%s","IPv4");
	else
		_ansc_sprintf(param_value,"%s","IPv6");
	_PSM_WRITE_PARAM(PSM_PC_MS_TRUSTED_IPADDRESS_TYPE);

}

//Set and Get of ManagedServices values and Trusted Users of ManagedServices values ( in parental control features ) from PSM Database..
void PSM_Set_ParentalControl_MSServ_RecordValues(COSA_DML_MS_SERV *pEntry)
{
	int retPsmSet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char param_value[256] = {0};
	ULONG instancenum = pEntry->InstanceNumber;
	_ansc_sprintf(param_value,"%s",pEntry->Description);
	_PSM_WRITE_PARAM(PSM_PC_MSE_DESCRIPTION);
	if(pEntry->Protocol == PROTO_TCP)
		_ansc_sprintf(param_value,"%s","TCP");
	else if(pEntry->Protocol == PROTO_UDP)
		_ansc_sprintf(param_value,"%s","UDP");
	else if(pEntry->Protocol == PROTO_BOTH)
		_ansc_sprintf(param_value,"%s","BOTH");
	_PSM_WRITE_PARAM(PSM_PC_MSE_PROTOCOL);
	_ansc_sprintf(param_value,"%ld",pEntry->StartPort);
	_PSM_WRITE_PARAM(PSM_PC_MSE_STARTPORT);
	_ansc_sprintf(param_value,"%ld",pEntry->EndPort);
	_PSM_WRITE_PARAM(PSM_PC_MSE_ENDPORT);
	if(pEntry->AlwaysBlock == TRUE)
		_ansc_sprintf(param_value,"%s","TRUE");
	else
		_ansc_sprintf(param_value,"%s","FALSE");
	_PSM_WRITE_PARAM(PSM_PC_MSE_ALWAYSBLOCK);
	_ansc_sprintf(param_value,"%s",pEntry->StartTime);
	_PSM_WRITE_PARAM(PSM_PC_MSE_STARTTIME);
	_ansc_sprintf(param_value,"%s",pEntry->EndTime);
	_PSM_WRITE_PARAM(PSM_PC_MSE_ENDTIME);
	_ansc_sprintf(param_value,"%s",pEntry->BlockDays);
	_PSM_WRITE_PARAM(PSM_PC_MSE_BLOCKDAYS);

}
void PSM_Set_ParentalControl_ManagedServices_Trusted_RecordValues(COSA_DML_MS_TRUSTEDUSER *pEntry)
{
	int retPsmSet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char param_value[256] = {0};
	ULONG instancenum = pEntry->InstanceNumber;
	_ansc_sprintf(param_value,"%s",pEntry->HostDescription);
	_PSM_WRITE_PARAM(PSM_PC_MSE_TRUSTED_HOST_DESCRIPTION);

	_ansc_sprintf(param_value,"%s",pEntry->IPAddress);
	_PSM_WRITE_PARAM(PSM_PC_MSE_TRUSTED_IPADDRESS);
	if(pEntry->Trusted == TRUE)
		_ansc_sprintf(param_value,"%s","TRUE");
	else
		_ansc_sprintf(param_value,"%s","FALSE");

	_PSM_WRITE_PARAM(PSM_PC_MSE_TRUSTED_ENABLE);
	if(pEntry->IPAddressType == IPADDR_IPV4)
		_ansc_sprintf(param_value,"%s","IPv4");
	else
		_ansc_sprintf(param_value,"%s","IPv6");
	_PSM_WRITE_PARAM(PSM_PC_MSE_TRUSTED_IPADDRESS_TYPE);

}
void PSMGetManagedServicesRecordValues(PCOSA_DML_MS_SERV pServices,ULONG instancenum)
{
	int retPsmGet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char *param_value = NULL;

	pServices->InstanceNumber=instancenum;

	_PSM_READ_PARAM(PSM_PC_MSE_DESCRIPTION);
	AnscCopyString(pServices->Description,param_value);

	_PSM_READ_PARAM(PSM_PC_MSE_STARTPORT);
	pServices->StartPort=atoi(param_value);

	_PSM_READ_PARAM(PSM_PC_MSE_ENDPORT);
	pServices->EndPort=atoi(param_value);

	_PSM_READ_PARAM(PSM_PC_MSE_ALWAYSBLOCK);
	if(strcmp(param_value,"TRUE") == 0)
		pServices->AlwaysBlock = TRUE;
	else
		pServices->AlwaysBlock = FALSE;

	_PSM_READ_PARAM(PSM_PC_MSE_STARTTIME);
	AnscCopyString(pServices->StartTime,param_value);

	_PSM_READ_PARAM(PSM_PC_MSE_ENDTIME);
	AnscCopyString(pServices->EndTime,param_value);

	_PSM_READ_PARAM(PSM_PC_MSE_BLOCKDAYS);
	AnscCopyString(pServices->BlockDays,param_value);

	_PSM_READ_PARAM(PSM_PC_MSE_PROTOCOL);
	if(strcmp(param_value,"TCP") == 0)
		pServices->Protocol =PROTO_TCP;
	else if(strcmp(param_value,"UDP") == 0)
		pServices->Protocol =PROTO_UDP;
	else if(strcmp(param_value,"BOTH") == 0)
		pServices->Protocol =PROTO_BOTH;
}
void PSMGetManagedServicesTuserRecordValues(PCOSA_DML_MS_TRUSTEDUSER pTuser,ULONG instancenum)
{
	int retPsmGet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char *param_value = NULL;

	pTuser->InstanceNumber=instancenum;

	_PSM_READ_PARAM(PSM_PC_MSE_TRUSTED_ENABLE);
	if(strcmp(param_value,"TRUE") == 0)
		pTuser->Trusted = TRUE;
	else
		pTuser->Trusted = FALSE;

	_PSM_READ_PARAM(PSM_PC_MSE_TRUSTED_HOST_DESCRIPTION);
	AnscCopyString( pTuser->HostDescription,param_value);

	_PSM_READ_PARAM(PSM_PC_MSE_TRUSTED_IPADDRESS);
	AnscCopyString( pTuser->IPAddress,param_value);


	_PSM_READ_PARAM(PSM_PC_MSE_TRUSTED_IPADDRESS_TYPE);
	if(strcmp(param_value,"IPv4") == 0)
		pTuser->IPAddressType = IPADDR_IPV4;
	else
		pTuser->IPAddressType = IPADDR_IPV6;
}
//Set and Get of ManagedDevices values ( in parental control features ) from PSM Database.
void PSM_Set_ParentalControl_MDDev_RecordValues(COSA_DML_MD_DEV *pEntry)
{
	int retPsmSet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char param_value[256] = {0};
	ULONG instancenum = pEntry->InstanceNumber;

	if(pEntry->Type == MD_TYPE_BLOCK){
		_ansc_sprintf(param_value,"%s","Block");
	}
	else{
		_ansc_sprintf(param_value,"%s","Allow");
	}
	_PSM_WRITE_PARAM(PSM_PC_MD_TYPE);
	_ansc_sprintf(param_value,"%s",pEntry->Description);
	_PSM_WRITE_PARAM(PSM_PC_MD_DESCRIPTION);
	_ansc_sprintf(param_value,"%s",pEntry->MACAddress);
	_PSM_WRITE_PARAM(PSM_PC_MD_MACADDRESS);
	if(pEntry->AlwaysBlock == TRUE)
		_ansc_sprintf(param_value,"%s","TRUE");
	else
		_ansc_sprintf(param_value,"%s","FALSE");
	_PSM_WRITE_PARAM(PSM_PC_MD_ALWAYSBLOCK);
	_ansc_sprintf(param_value,"%s",pEntry->StartTime);
	_PSM_WRITE_PARAM(PSM_PC_MD_STARTTIME);
	_ansc_sprintf(param_value,"%s",pEntry->EndTime);
	_PSM_WRITE_PARAM(PSM_PC_MD_ENDTIME);
	_ansc_sprintf(param_value,"%s",pEntry->BlockDays);
	_PSM_WRITE_PARAM(PSM_PC_MD_BLOCKDAYS);
}
void PSMGetManagedDevicesRecordValues(PCOSA_DML_MD_DEV pDevices,ULONG instancenum)
{
	int retPsmGet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char *param_value = NULL;

	pDevices->InstanceNumber=instancenum;
	_PSM_READ_PARAM(PSM_PC_MD_DESCRIPTION);
	AnscCopyString( pDevices->Description,param_value);

	_PSM_READ_PARAM(PSM_PC_MD_TYPE);
	if(strcmp(param_value,"Block") == 0)
		pDevices->Type = MD_TYPE_BLOCK;
	else
		pDevices->Type = MD_TYPE_ALLOW;

	_PSM_READ_PARAM(PSM_PC_MD_MACADDRESS);
	AnscCopyString( pDevices->MACAddress,param_value);

	_PSM_READ_PARAM(PSM_PC_MD_ALWAYSBLOCK);
	if(strcmp(param_value,"TRUE") == 0)
		pDevices->AlwaysBlock =TRUE;
	else
		pDevices->AlwaysBlock =FALSE;

	_PSM_READ_PARAM(PSM_PC_MD_STARTTIME);
	AnscCopyString( pDevices->StartTime,param_value);

	_PSM_READ_PARAM(PSM_PC_MD_ENDTIME);
	AnscCopyString( pDevices->EndTime,param_value);

	_PSM_READ_PARAM(PSM_PC_MD_BLOCKDAYS);
	AnscCopyString( pDevices->BlockDays,param_value);


}
//Deletion of PSM DataBase - ParentalControl Entries
//Managed Sites
void PSM_DEL_URL(ULONG ins)
{
	char param_name[256] = {0};
	_PSM_DEL_PARAM(PSM_PC_MS_BLOCKMETHOD);
	_PSM_DEL_PARAM(PSM_PC_MS_ALWAYSBLOCK);
	_PSM_DEL_PARAM(PSM_PC_MS_SITE);
	_PSM_DEL_PARAM(PSM_PC_MS_STARTTIME);
	_PSM_DEL_PARAM(PSM_PC_MS_ENDTIME);
	_PSM_DEL_PARAM(PSM_PC_MS_BLOCKDAYS);
}
//ManagedSites - TrustedUsers
void PSM_DEL_MSTRUSTEDUSER(ULONG ins)
{
	char param_name[256] = {0};
	_PSM_DEL_PARAM(PSM_PC_MS_TRUSTED_HOST_DESCRIPTION);
	_PSM_DEL_PARAM(PSM_PC_MS_TRUSTED_IPADDRESS);
	_PSM_DEL_PARAM(PSM_PC_MS_TRUSTED_ENABLE);
	_PSM_DEL_PARAM(PSM_PC_MS_TRUSTED_IPADDRESS_TYPE);
}
//ManagedServices
void PSM_DEL_MSServ(ULONG ins)
{
	char param_name[256] = {0};
	_PSM_DEL_PARAM(PSM_PC_MSE_DESCRIPTION);
	_PSM_DEL_PARAM(PSM_PC_MSE_STARTPORT);
	_PSM_DEL_PARAM(PSM_PC_MSE_ENDPORT);
	_PSM_DEL_PARAM(PSM_PC_MSE_ALWAYSBLOCK);
	_PSM_DEL_PARAM(PSM_PC_MSE_STARTTIME);
	_PSM_DEL_PARAM(PSM_PC_MSE_ENDTIME);
	_PSM_DEL_PARAM(PSM_PC_MSE_BLOCKDAYS);
	_PSM_DEL_PARAM(PSM_PC_MSE_PROTOCOL);
}
//ManagedServices - TrustedUsers
void PSM_Del_MSTrustedUser(ULONG ins)
{
	char param_name[256] = {0};
	_PSM_DEL_PARAM(PSM_PC_MSE_TRUSTED_HOST_DESCRIPTION);
	_PSM_DEL_PARAM(PSM_PC_MSE_TRUSTED_IPADDRESS);
	_PSM_DEL_PARAM(PSM_PC_MSE_TRUSTED_IPADDRESS_TYPE);
	_PSM_DEL_PARAM(PSM_PC_MSE_TRUSTED_ENABLE);
}
//ManagedDevices
void PSM_DEL_MDDEV_ENTRIES(ULONG ins)
{
	char param_name[256] = {0};
	_PSM_DEL_PARAM(PSM_PC_MD_DESCRIPTION);
	_PSM_DEL_PARAM(PSM_PC_MD_TYPE);
	_PSM_DEL_PARAM(PSM_PC_MD_MACADDRESS);
	_PSM_DEL_PARAM(PSM_PC_MD_ALWAYSBLOCK);
	_PSM_DEL_PARAM(PSM_PC_MD_STARTTIME);
	_PSM_DEL_PARAM(PSM_PC_MD_ENDTIME);
	_PSM_DEL_PARAM(PSM_PC_MD_BLOCKDAYS);
}

ANSC_STATUS
CosaDmlParentalControlInit(ANSC_HANDLE hDml, PANSC_HANDLE phContext)
{
	return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedSites
 */
static COSA_DML_MANAGEDSITES g_ManagedSites = {
	.Enable             = FALSE,
};

ANSC_STATUS
CosaDmlMngSites_GetConf(COSA_DML_MANAGEDSITES *conf)
{
	memcpy(conf, &g_ManagedSites, sizeof(COSA_DML_MANAGEDSITES));
	PSM_Get_ManagedSites_RecordValues(conf);//RDKB-EMULATOR
	return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedServices
 */
static COSA_DML_MANAGED_SERVS g_ManagedServs = {
	.Enable             = FALSE,
};

ANSC_STATUS
CosaDmlMngServs_GetConf(COSA_DML_MANAGED_SERVS *conf)
{
	memcpy(conf, &g_ManagedServs, sizeof(COSA_DML_MANAGED_SERVS));
	PSM_Get_ManagedServices_RecordValues(conf);//RDKB-EMULATOR
	return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedDevices
 */
static COSA_DML_MANAGED_DEVS g_ManagedDevs = {
	.Enable             = FALSE,
	.AllowAll           = FALSE,
};


ANSC_STATUS
CosaDmlMngDevs_GetConf(COSA_DML_MANAGED_DEVS *conf)
{
	memcpy(conf, &g_ManagedDevs, sizeof(COSA_DML_MANAGED_DEVS));
	PSM_Get_ManagedDevices_RecordValues(conf);//RDKB-EMULATOR
	return ANSC_STATUS_SUCCESS;
}

/*
 * TrustedUser
 */

#define  MAX_TRUSTEDUSER_ENTRY      1024

static int g_NrTrustedUser  = 0; 

static COSA_DML_TRUSTEDUSER g_TrustedUsers[MAX_TRUSTEDUSER_ENTRY] = {
	/*  {
	    .InstanceNumber = 1,
	    .Alias          = "cpe-TrustedUser-1",
	    .HostDescription = "Host-1",
	    .IPAddressType  = IPADDR_IPV4,
	    .IPAddress      = "192.168.0.1",
	    },
	    {
	    .InstanceNumber = 2,
	    .Alias          = "cpe-TrustedUser-2",
	    .HostDescription = "Host-2",
	    .IPAddressType  = IPADDR_IPV6,
	    .IPAddress      = "2012:CAFE::1",
	    },*/
};
#define  MAX_MSTRUSTEDUSER_ENTRY      4

static int g_NrMSTrustedUsers   = 0;

static COSA_DML_MS_TRUSTEDUSER g_MSTrustedUsers[MAX_MSTRUSTEDUSER_ENTRY] = {
	/* {
	   .InstanceNumber = 1,
	   .Alias          = "cpe-MSTrustedUser-1",
	   .HostDescription = "Dummy",
	   .IPAddressType  = IPADDR_IPV6,
	   .IPAddress      = "2012:cafe::3",
	   .Trusted        = FALSE,
	   },
	   {
	   .InstanceNumber = 2,
	   .Alias          = "cpe-MSTrustedUser-2",
	   .HostDescription = "Dummy",
	   .IPAddressType  = IPADDR_IPV4,
	   .IPAddress      = "192.168.0.4",
	   .Trusted        = TRUE,
	   },*/
};


/*
 * Blocked URL
 */

#define  MAX_URL_ENTRY      1024

static int g_NrBlkURL   = 0;

static COSA_DML_BLOCKEDURL g_BlockedURLs[MAX_URL_ENTRY] = {
	/*  {
	    .InstanceNumber = 1,
	    .Alias          = "cpe-BlockedSite-1",
	    .BlockMethod    = BLOCK_METHOD_URL,
	    .Site           = "http://baidu.com",
	    .AlwaysBlock    = FALSE,
	    .StartTime      = "19:00",
	    .EndTime        = "22:00",
	    .BlockDays      = "Sat,Sun",
	    },
	    {
	    .InstanceNumber = 2,
	    .Alias          = "cpe-BlockedSite-2",
	    .BlockMethod    = BLOCK_METHOD_KEYWORD,
	    .Site           = "keyword",
	    .AlwaysBlock    = FALSE,
	    .StartTime      = "10:00",
	    .EndTime        = "12:00",
	    .BlockDays      = "Mon,Wed,Fri",
	    },*/
};

ANSC_STATUS
CosaDmlMngSites_SetConf(COSA_DML_MANAGEDSITES *conf)
{
	int i;
	memcpy(&g_ManagedSites, conf, sizeof(COSA_DML_MANAGEDSITES));
	PSM_Set_ParentalControl_ManagedSites_RecordValues(conf->Enable);//RDKB-EMULATOR
	if(conf != NULL)
	{
		if(!conf->Enable)
		{
			do_parentalControl_Delrule_Sites(); //LNT_EMU
		}
		else
		{
			do_parentalControl_Addrule_Sites(); //LNT_EMU
			for(i=0;i<g_NrBlkURL;i++)
			{	    	
				do_parentalControl_Sites(ADD,&g_BlockedURLs[i]);
			}
			for(i=0;i<g_NrTrustedUser;i++)
			{	    
				CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,g_TrustedUsers[i].IPAddress,ADD);
			}	
		}
	}
	return ANSC_STATUS_SUCCESS;
}

static int
BlkURL_InsGetIndex(ULONG ins)
{
	int i;

	for (i = 0; i < g_NrBlkURL; i++)
		if (g_BlockedURLs[i].InstanceNumber == ins)
			return i;

	return -1;
}

ULONG
CosaDmlBlkURL_GetNumberOfEntries(void)
{
	char *param_value = NULL;
	PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.X_Comcast_com_ParentalControl.ManagedSites.InstanceNumber", NULL, &param_value);
	if(param_value!=NULL){
		g_NrBlkURL = atoi(param_value);
	}
	else{
		g_NrBlkURL = 0;//RDKB-EMULATOR
	}

	return g_NrBlkURL;
}

ANSC_STATUS
CosaDmlBlkURL_GetEntryByIndex(ULONG index, COSA_DML_BLOCKEDURL *pEntry)
{
	PCOSA_DML_BLOCKEDURL pBlockedURL = NULL;
	pBlockedURL = (PCOSA_DML_BLOCKEDURL) AnscAllocateMemory( sizeof( g_BlockedURLs[0])*(g_NrBlkURL));
	if (index >= g_NrBlkURL)
		return ANSC_STATUS_FAILURE;
	//RDKB-EMULATOR
	//Checking Instance Number is there or not in PSM Database
	char command[512];
	snprintf(command, sizeof(command),"cat /nvram/bbhm_cur_cfg.xml | grep dmsb.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.%ld.BlockMethod > /tmp/PSM_Check.txt",index+1);
	system(command);
	FILE *fp;
	char path[256];
	int count,flag = 0,valid_index = 0;
	fp = popen("cat /tmp/PSM_Check.txt | tail -1 ","r");
	if(fp == NULL)
	{
		printf("Failed to run command in Function %s\n",__FUNCTION__);
		return 0;
	}

	if(fgets(path, sizeof(path)-1, fp) == NULL)
	{
		index=index+1;
	}
	else
	{
		flag=1;
		valid_index=index;
	}

	if (  pBlockedURL && flag )
	{
		if(valid_index == index)
		{
			PSMGetManagedSitesBlockedURLRecordValues(pBlockedURL,index+1);
			g_BlockedURLs[index]=*pBlockedURL;
			*pEntry = g_BlockedURLs[index];
		}
	}
	pclose(fp);

	//*pEntry = g_BlockedURLs[index]; 
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_SetValues(ULONG index, ULONG ins, const char *alias)
{
	if (index >= g_NrBlkURL)
		return ANSC_STATUS_FAILURE;

	g_BlockedURLs[index].InstanceNumber = ins;
	snprintf(g_BlockedURLs[index].Alias, sizeof(g_BlockedURLs[index].Alias),
			"%s", alias);

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_AddEntry(COSA_DML_BLOCKEDURL *pEntry)
{
	if (g_NrBlkURL >= MAX_URL_ENTRY)
		return ANSC_STATUS_FAILURE;

	g_BlockedURLs[g_NrBlkURL++] = *pEntry;
	do_parentalControl_Sites(ADD,&g_BlockedURLs[g_NrBlkURL-1]); //RDKB-EMULATOR
	PSM_Set_ParentalControl_ManagedBlkURL_RecordValues(pEntry);//RDKB-EMULATOR    
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_DelEntry(ULONG ins)
{
	int i;

	if ((i = BlkURL_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	do_parentalControl_Sites(DELETE,&g_BlockedURLs[i]); //RDKB-EMULATOR
	PSM_DEL_URL(ins);//RDKB-EMULATOR
	memmove(&g_BlockedURLs[i], &g_BlockedURLs[i+1], 
			(g_NrBlkURL - i - 1) * sizeof(COSA_DML_BLOCKEDURL));
	g_NrBlkURL--;

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_GetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry)
{
	int i;

	if ((i = BlkURL_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	*pEntry = g_BlockedURLs[i];
	pEntry->InstanceNumber = ins; /* just in case */

	return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlBlkURL_SetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry)
{
	int i;

	if ((i = BlkURL_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;
	do_parentalControl_Sites(DELETE,&g_BlockedURLs[i]); //RDKB-EMULATOR
	g_BlockedURLs[i] = *pEntry;
	g_BlockedURLs[i].InstanceNumber = ins; /* just in case */
	do_parentalControl_Sites(ADD,&g_BlockedURLs[i]);    //RDKB-EMULATOR
	PSM_Set_ParentalControl_ManagedBlkURL_RecordValues(pEntry);//RDKB-EMULATOR
	return ANSC_STATUS_SUCCESS;
}

static int
TrustedUser_InsGetIndex(ULONG ins)
{
	int i;

	for (i = 0; i < g_NrTrustedUser; i++)
		if (g_TrustedUsers[i].InstanceNumber == ins)
			return i;

	return -1;
}

ULONG
CosaDmlTrustedUser_GetNumberOfEntries(void)
{
	//RDKB-EMULATOR
	char *param_value = NULL;
	PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.X_Comcast_com_ParentalControl.ManagedSites.TrustedUser.InstanceNumber", NULL, &param_value);
	if(param_value!=NULL){
		g_NrTrustedUser = atoi(param_value);
	}
	else{
		g_NrTrustedUser = 0;
	}

	return g_NrTrustedUser;
}

ANSC_STATUS
CosaDmlTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_TRUSTEDUSER *pEntry)
{
	PCOSA_DML_TRUSTEDUSER pTrustedUser = NULL;
	pTrustedUser = (PCOSA_DML_TRUSTEDUSER) AnscAllocateMemory( sizeof( g_TrustedUsers[0])*(g_NrTrustedUser));

	if (index >= g_NrTrustedUser)
		return ANSC_STATUS_FAILURE;
	//Checking Instance Number is there or not in PSM Database
	char command[512];
	snprintf(command, sizeof(command),"cat /nvram/bbhm_cur_cfg.xml | grep dmsb.X_Comcast_com_ParentalControl.ManagedSites.TrustedUser.%ld.HostDescription > /tmp/PSM_Check.txt",index+1);
	system(command);
	FILE *fp;
	char path[256];
	int count,flag = 0,valid_index = 0;
	fp = popen("cat /tmp/PSM_Check.txt | tail -1 ","r");
	if(fp == NULL)
	{
		printf("Failed to run command in Function %s\n",__FUNCTION__);
		return 0;
	}

	if(fgets(path, sizeof(path)-1, fp) == NULL)
	{
		index=index+1;
	}
	else
	{
		flag=1;
		valid_index=index;
	}

	if (  pTrustedUser && flag )
	{
		if(valid_index == index)
		{
			PSMGetTrustedManagedSitesRecordValues(pTrustedUser,index+1);
			g_TrustedUsers[index]=*pTrustedUser;
			*pEntry = g_TrustedUsers[index];
		}
	}
	pclose(fp);

	//   *pEntry = g_TrustedUsers[index];
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias)
{
	if (index >= g_NrTrustedUser)
		return ANSC_STATUS_FAILURE;

	g_TrustedUsers[index].InstanceNumber = ins;
	snprintf(g_TrustedUsers[index].Alias, sizeof(g_TrustedUsers[index].Alias),
			"%s", alias);

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_AddEntry(COSA_DML_TRUSTEDUSER *pEntry)
{
	if (g_NrTrustedUser >= MAX_TRUSTEDUSER_ENTRY)
		return ANSC_STATUS_FAILURE;

	g_TrustedUsers[g_NrTrustedUser++] = *pEntry;
	CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,g_TrustedUsers[g_NrTrustedUser-1].IPAddress,ADD);
	PSM_Set_ParentalControl_ManagedSites_Trusted_RecordValues(pEntry);//RDKB-EMULATOR
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_DelEntry(ULONG ins)
{
	int i;

	if ((i = TrustedUser_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;
	CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,g_TrustedUsers[i].IPAddress,DELETE);
	PSM_DEL_MSTRUSTEDUSER(ins);//RDKB-EMULATOR
	memmove(&g_TrustedUsers[i], &g_TrustedUsers[i+1], 
			(g_NrTrustedUser - i - 1) * sizeof(COSA_DML_TRUSTEDUSER));
	g_NrTrustedUser--;

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_GetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry)
{
	int i;

	if ((i = TrustedUser_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	*pEntry = g_TrustedUsers[i];
	pEntry->InstanceNumber = ins; /* just in case */

	return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlTrustedUser_SetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry)
{
	int i;
	if ((i = TrustedUser_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	g_TrustedUsers[i] = *pEntry;
	g_TrustedUsers[i].InstanceNumber = ins; /* just in case */
	if(pEntry->Trusted)
		CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,g_TrustedUsers[i].IPAddress,ADD); 
	else
		CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,g_TrustedUsers[i].IPAddress,DELETE); //RDKB-EMULATOR
	PSM_Set_ParentalControl_ManagedSites_Trusted_RecordValues(pEntry);//RDKB-EMULATOR
	return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedServices.Service
 */

#define  MAX_MSSERV_ENTRY      1024

static int g_NrMSServs   = 0;

static COSA_DML_MS_SERV g_MSServs[MAX_MSSERV_ENTRY] = {
	/*  {
	    .InstanceNumber = 1,
	    .Alias          = "cpe-MSService-1",
	    .Description    = "Service-1",
	    .Protocol       = PROTO_TCP,
	    .StartPort      = 100,
	    .EndPort        = 1024,
	    .AlwaysBlock   = FALSE,
	    .StartTime      = "19:00",
	    .EndTime        = "22:00",
	    .BlockDays      = "Sat,Sun",
	    },
	    {
	    .InstanceNumber = 2,
	    .Alias          = "cpe-MSService-2",
	    .Description    = "Service-2",
	    .Protocol       = PROTO_BOTH,
	    .StartPort      = 200,
	    .EndPort        = 2024,
	    .AlwaysBlock   = FALSE,
	    .StartTime      = "20:00",
	    .EndTime        = "24:00",
	    .BlockDays      = "Mon,Tue",
	    },*/
};

ANSC_STATUS
CosaDmlMngServs_SetConf(COSA_DML_MANAGED_SERVS *conf)
{
	int i;
	memcpy(&g_ManagedServs, conf, sizeof(COSA_DML_MANAGED_SERVS));
	PSM_Set_ParentalControl_ManagedServices_RecordValues(conf);//RDKB-EMULATOR
	if(!conf->Enable)
	{
		do_parentalControl_Delrule_Services(); //RDKB-EMULATOR
	}
	else
	{
		do_parentalControl_Addrule_Services();
		for(i = 0;i < g_NrMSServs; i++)
		{ 	
			do_parentalControl_Services(ADD,&g_MSServs[i]); 
		}	
		for(i=0;i<g_NrMSTrustedUsers;i++)
		{	    
			CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,g_MSTrustedUsers[i].IPAddress,ADD);	   
		}	
	}

	return ANSC_STATUS_SUCCESS;
}

static int
MSServ_InsGetIndex(ULONG ins)
{
	int i;

	for (i = 0; i < g_NrMSServs; i++)
		if (g_MSServs[i].InstanceNumber == ins)
			return i;

	return -1;
}

ULONG
CosaDmlMSServ_GetNumberOfEntries(void)
{
	//RDKB-EMULATOR
	char *param_value = NULL;
	PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.X_Comcast_com_ParentalControl.ManagedServices.InstanceNumber", NULL, &param_value);
	if(param_value!=NULL){
		g_NrMSServs = atoi(param_value);
	}
	else{
		g_NrMSServs = 0;
	}
	return g_NrMSServs;
}

ANSC_STATUS
CosaDmlMSServ_GetEntryByIndex(ULONG index, COSA_DML_MS_SERV *pEntry)
{
	//RDKB-EMULATOR
	PCOSA_DML_MS_SERV pServices = NULL;
	pServices = (PCOSA_DML_MS_SERV) AnscAllocateMemory( sizeof( g_MSServs[0])*(g_NrMSServs));

	if (index >= g_NrMSServs)
		return ANSC_STATUS_FAILURE;
	//Checking Instance Number is there or not in PSM Database
	char command[512];
	snprintf(command, sizeof(command),"cat /nvram/bbhm_cur_cfg.xml | grep dmsb.X_Comcast_com_ParentalControl.ManagedServices.Service.%ld.Description > /tmp/PSM_Check.txt",index+1);
	system(command);
	FILE *fp;
	char path[256];
	int count,flag = 0,valid_index = 0;
	fp = popen("cat /tmp/PSM_Check.txt | tail -1 ","r");
	if(fp == NULL)
	{
		printf("Failed to run command in Function %s\n",__FUNCTION__);
		return 0;
	}

	if(fgets(path, sizeof(path)-1, fp) == NULL)
	{
		index=index+1;
	}
	else
	{
		flag=1;
		valid_index=index;
	}

	if (  pServices && flag )
	{
		if(valid_index == index)
		{
			PSMGetManagedServicesRecordValues(pServices,index+1);
			g_MSServs[index]=*pServices;
			*pEntry = g_MSServs[index];
		}
	}
	pclose(fp);

	// *pEntry = g_MSServs[index];
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_SetValues(ULONG index, ULONG ins, const char *alias)
{
	if (index >= g_NrMSServs)
		return ANSC_STATUS_FAILURE;

	g_MSServs[index].InstanceNumber = ins;
	snprintf(g_MSServs[index].Alias, sizeof(g_MSServs[index].Alias),
			"%s", alias);

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_AddEntry(COSA_DML_MS_SERV *pEntry)
{
	if (g_NrMSServs >= MAX_MSSERV_ENTRY)
		return ANSC_STATUS_FAILURE;

	g_MSServs[g_NrMSServs++] = *pEntry;
	do_parentalControl_Services(ADD,&g_MSServs[g_NrMSServs-1]); //RDKB-EMULATOR
	PSM_Set_ParentalControl_MSServ_RecordValues(pEntry);
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_DelEntry(ULONG ins)
{
	int i;

	if ((i = MSServ_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	do_parentalControl_Services(DELETE,&g_MSServs[i]); //RDKB-EMULATOR
	PSM_DEL_MSServ(ins);
	memmove(&g_MSServs[i], &g_MSServs[i+1], 
			(g_NrMSServs - i - 1) * sizeof(COSA_DML_MS_SERV));
	g_NrMSServs--;

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_GetConf(ULONG ins, COSA_DML_MS_SERV *pEntry)
{
	int i;

	if ((i = MSServ_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	*pEntry = g_MSServs[i];
	pEntry->InstanceNumber = ins; /* just in case */

	return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMSServ_SetConf(ULONG ins, COSA_DML_MS_SERV *pEntry)
{
	int i;

	if ((i = MSServ_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	do_parentalControl_Services(DELETE,&g_MSServs[i]); //RDKB-EMULATOR
	g_MSServs[i] = *pEntry;
	g_MSServs[i].InstanceNumber = ins; /* just in case */
	do_parentalControl_Services(ADD,&g_MSServs[i]);    
	PSM_Set_ParentalControl_MSServ_RecordValues(pEntry);//RDKB-EMULATOR
	return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedServices.TrustedUser
 */

static int
MSTrustedUser_InsGetIndex(ULONG ins)
{
	int i;

	for (i = 0; i < g_NrMSTrustedUsers; i++)
		if (g_MSTrustedUsers[i].InstanceNumber == ins)
			return i;

	return -1;
}

ULONG
CosaDmlMSTrustedUser_GetNumberOfEntries(void)
{
	//RDKB-EMULATOR
	char *param_value = NULL;
	PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.X_Comcast_com_ParentalControl.ManagedServices.TrustedUser.InstanceNumber", NULL, &param_value);
	if(param_value!=NULL){
		g_NrMSTrustedUsers = atoi(param_value);
	}
	else{
		g_NrMSTrustedUsers = 0;
	}

	return g_NrMSTrustedUsers;
}

ANSC_STATUS
CosaDmlMSTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
//RDKB-EMULATOR
	PCOSA_DML_MS_TRUSTEDUSER pTuser = NULL;
	pTuser = (PCOSA_DML_MS_TRUSTEDUSER) AnscAllocateMemory( sizeof( g_MSTrustedUsers[0])*(g_NrMSTrustedUsers));

	if (index >= g_NrMSTrustedUsers)
		return ANSC_STATUS_FAILURE;
	//Checking Instance Number is there or not in PSM Database
	char command[512];
	snprintf(command, sizeof(command),"cat /nvram/bbhm_cur_cfg.xml | grep dmsb.X_Comcast_com_ParentalControl.ManagedServices.TrustedUser.%ld.HostDescription > /tmp/PSM_Check.txt",index+1);
	system(command);
	FILE *fp;
	char path[256];
	int count,flag = 0,valid_index = 0;
	fp = popen("cat /tmp/PSM_Check.txt | tail -1 ","r");
	if(fp == NULL)
	{
		printf("Failed to run command in Function %s\n",__FUNCTION__);
		return 0;
	}

	if(fgets(path, sizeof(path)-1, fp) == NULL)
	{
		index=index+1;
	}
	else
	{
		flag=1;
		valid_index=index;
	}

	if (  pTuser && flag )
	{
		if(valid_index == index)
		{
			PSMGetManagedServicesTuserRecordValues(pTuser,index+1);
			g_MSTrustedUsers[index]=*pTuser;
			*pEntry = g_MSTrustedUsers[index];
		}
	}
	pclose(fp);

	//*pEntry = g_MSTrustedUsers[index];
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias)
{
	if (index >= g_NrMSTrustedUsers)
		return ANSC_STATUS_FAILURE;

	g_MSTrustedUsers[index].InstanceNumber = ins;
	snprintf(g_MSTrustedUsers[index].Alias, sizeof(g_MSTrustedUsers[index].Alias),
			"%s", alias);

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_AddEntry(COSA_DML_MS_TRUSTEDUSER *pEntry)
{
	if (g_NrMSTrustedUsers >= MAX_MSTRUSTEDUSER_ENTRY)
		return ANSC_STATUS_FAILURE;

	g_MSTrustedUsers[g_NrMSTrustedUsers++] = *pEntry;
	CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,g_MSTrustedUsers[g_NrMSTrustedUsers-1].IPAddress,ADD); 
	PSM_Set_ParentalControl_ManagedServices_Trusted_RecordValues(pEntry);//RDKB-EMULATOR
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_DelEntry(ULONG ins)
{
	int i;

	if ((i = MSTrustedUser_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,g_MSTrustedUsers[i].IPAddress,DELETE);//RDKB-EMULATOR
	PSM_Del_MSTrustedUser(ins);//RDKB-EMULATOR
	memmove(&g_MSTrustedUsers[i], &g_MSTrustedUsers[i+1], 
			(g_NrMSTrustedUsers - i - 1) * sizeof(COSA_DML_MS_TRUSTEDUSER));
	g_NrMSTrustedUsers--;

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_GetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
	int i;

	if ((i = MSTrustedUser_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	*pEntry = g_MSTrustedUsers[i];
	pEntry->InstanceNumber = ins; /* just in case */

	return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMSTrustedUser_SetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
	int i;

	if ((i = MSTrustedUser_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	g_MSTrustedUsers[i] = *pEntry;
	g_MSTrustedUsers[i].InstanceNumber = ins; /* just in case */
	if(pEntry->Trusted)
		CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,g_MSTrustedUsers[i].IPAddress,ADD); // RDKB-EMULATOR
	else
		CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,g_MSTrustedUsers[i].IPAddress,DELETE); //RDKB-EMULATOR
	PSM_Set_ParentalControl_ManagedServices_Trusted_RecordValues(pEntry);//RDKB-EMULATOR
	return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedDevices.Device
 */

#define  MAX_MDDEV_ENTRY      4

static int g_NrMDDevs   = 0;
static int AllowAll = MD_TYPE_BLOCK;
static int Type = MD_TYPE_BLOCK;
static COSA_DML_MD_DEV g_MDDevs[MAX_MDDEV_ENTRY] = {
	/* {
	   .InstanceNumber = 1,
	   .Alias          = "cpe-MDDevice-1",
	   .Type           = MD_TYPE_ALLOW,
	   .Description    = "dummy-1",
	   .MACAddress     = "00:1A:2B:AA:BB:CC",
	   .AlwaysBlock   = FALSE,
	   .StartTime      = "19:00",
	   .EndTime        = "22:00",
	   .BlockDays      = "Sat,Sun",
	   },
	   {
	   .InstanceNumber = 2,
	   .Type           = MD_TYPE_BLOCK,
	   .Alias          = "cpe-MDDevice-2",
	   .Description    = "dummy-2",
	   .MACAddress     = "00:1A:2B:11:22:33",
	   .AlwaysBlock   = TRUE,
	   .StartTime      = "20:00",
	   .EndTime        = "24:00",
	   .BlockDays      = "Mon,Tue",
	   },*/
};

ANSC_STATUS
CosaDmlMngDevs_SetConf(COSA_DML_MANAGED_DEVS *conf)
{
	int i;
	memcpy(&g_ManagedDevs, conf, sizeof(COSA_DML_MANAGED_DEVS));
	PSM_Set_ParentalControl_ManagedDevices_RecordValues(conf);//RDKB-EMULATOR
	if(!conf->Enable)
	{
		do_parentalControl_Delrule_Devices();   //RDKB-EMULATOR
	}
	else
	{
		if(conf->AllowAll)
		{
			Type = MD_TYPE_BLOCK;
		}
		else
		{	
			Type = MD_TYPE_ALLOW;
		}
		if( AllowAll != Type)
		{
			do_parentalControl_Delrule_Devices();   //RDKB-EMULATOR
		}		
		do_parentalControl_Addrule_Devices();
		for( i=0; i < g_NrMDDevs; i++)
		{

			if(g_MDDevs[i].Type == Type)
			{		
				do_parentalControl_Devices(ADD,&g_MDDevs[i]);
			}	
		}
		AllowAll = Type;

	}

	return ANSC_STATUS_SUCCESS;
}

static int
MDDev_InsGetIndex(ULONG ins)
{
	int i;

	for (i = 0; i < g_NrMDDevs; i++)
		if (g_MDDevs[i].InstanceNumber == ins)
			return i;

	return -1;
}

ULONG
CosaDmlMDDev_GetNumberOfEntries(void)
{
	//RDKB-EMULATOR
	char *param_value = NULL;
	PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.InstanceNumber", NULL, &param_value);
	if(param_value!=NULL){
		g_NrMDDevs = atoi(param_value);
	}
	else{
		g_NrMDDevs = 0;
	}

	return g_NrMDDevs;
}

ANSC_STATUS
CosaDmlMDDev_GetEntryByIndex(ULONG index, COSA_DML_MD_DEV *pEntry)
{
	//RDKB-EMULATOR
	PCOSA_DML_MD_DEV pDevices = NULL;
	pDevices = (PCOSA_DML_MD_DEV) AnscAllocateMemory( sizeof( g_MDDevs[0])*(g_NrMDDevs));
	if (index >= g_NrMDDevs)
		return ANSC_STATUS_FAILURE;
	//Checking Instance Number is there or not in PSM Database
	char command[512];
	snprintf(command, sizeof(command),"cat /nvram/bbhm_cur_cfg.xml | grep dmsb.X_Comcast_com_ParentalControl.ManagedDevices.Device.%ld.Description > /tmp/PSM_Check.txt",index+1);
	system(command);
	FILE *fp;
	char path[256];
	int count,flag = 0,valid_index = 0;
	fp = popen("cat /tmp/PSM_Check.txt | tail -1 ","r");
	if(fp == NULL)
	{
		printf("Failed to run command in Function %s\n",__FUNCTION__);
		return 0;
	}

	if(fgets(path, sizeof(path)-1, fp) == NULL)
	{
		index=index+1;
	}
	else
	{
		flag=1;
		valid_index=index;
	}
	if (  pDevices && flag )
	{
		if(valid_index == index)
		{
			PSMGetManagedDevicesRecordValues(pDevices,index+1);
			g_MDDevs[index]=*pDevices;
			*pEntry = g_MDDevs[index];
		}
	}
	pclose(fp);

	//*pEntry = g_MDDevs[index];
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_SetValues(ULONG index, ULONG ins, const char *alias)
{
	if (index >= g_NrMDDevs)
		return ANSC_STATUS_FAILURE;

	g_MDDevs[index].InstanceNumber = ins;
	snprintf(g_MDDevs[index].Alias, sizeof(g_MDDevs[index].Alias),
			"%s", alias);

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_AddEntry(COSA_DML_MD_DEV *pEntry)
{
	if (g_NrMDDevs >= MAX_MDDEV_ENTRY)
		return ANSC_STATUS_FAILURE;

	g_MDDevs[g_NrMDDevs++] = *pEntry;
	do_parentalControl_Devices(ADD,&g_MDDevs[g_NrMDDevs-1]); //RDKB-EMULATOR
	PSM_Set_ParentalControl_MDDev_RecordValues(pEntry);
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_DelEntry(ULONG ins)
{
	int i;

	if ((i = MDDev_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	do_parentalControl_Devices(DELETE,&g_MDDevs[i]); //RDKB-EMULATOR
	PSM_DEL_MDDEV_ENTRIES(ins);
	memmove(&g_MDDevs[i], &g_MDDevs[i+1], 
			(g_NrMDDevs - i - 1) * sizeof(COSA_DML_MD_DEV));
	g_NrMDDevs--;

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_GetConf(ULONG ins, COSA_DML_MD_DEV *pEntry)
{
	int i;

	if ((i = MDDev_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;

	*pEntry = g_MDDevs[i];
	pEntry->InstanceNumber = ins; /* just in case */

	return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMDDev_SetConf(ULONG ins, COSA_DML_MD_DEV *pEntry)
{
	int i;

	if ((i = MDDev_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;
	do_parentalControl_Devices(DELETE,&g_MDDevs[i]);  ///RDKB-EMULATOR

	g_MDDevs[i] = *pEntry;
	g_MDDevs[i].InstanceNumber = ins; /* just in case */
	do_parentalControl_Devices(ADD,&g_MDDevs[i]);     ///RDKB-EMULATOR
	PSM_Set_ParentalControl_MDDev_RecordValues(pEntry);///RDKB-EMULATOR
	return ANSC_STATUS_SUCCESS;
}

#endif /* _COSA_SIM_ */
