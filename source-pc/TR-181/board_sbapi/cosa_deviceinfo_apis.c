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
#include <unistd.h>

#ifdef _COSA_SIM_

// this file is in integration_src.pc directory

#include "dmsb_tr181_psm_definitions.h" // for DMSB_TR181_PSM_DeviceInfo_Root/ProductClass
#include "ccsp_psm_helper.h"            // for PSM_Get_Record_Value2
#define PARTNERS_INFO_FILE              "/nvram/partners_defaults.json"

#define _ERROR_ "NOT SUPPORTED"
#define _SSH_ERROR_ "NOT SET"
#define PARTNER_ID_LEN 64

extern void* g_pDslhDmlAgent;

static const int OK = 1 ;
static const int NOK = 0 ;
static char reverseSSHArgs[255] = { "\0" };
const char* sshCommand = "/lib/rdk/startTunnel.sh";
const char* rsshPidFile = "/var/tmp/rssh.pid";


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
CosaDmlDiGetBootloaderVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetFirmwareName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    char Firmware_Name[256] = {0};
    platform_hal_GetFirmwareName(Firmware_Name,0);//RDKB-EMU
    AnscCopyString(pValue,Firmware_Name);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetFirmwareBuildTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetBaseMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetHardware
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetAdvancedServices
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    AnscCopyString(pValue, "AdvancedServices");
    *pulSize = AnscSizeOfString(pValue);
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
    AnscCopyString(pValue, "ProcessorSpeed");
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
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
	 AnscCopyString(pValue, "Cisco");
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

	/*
	   AnscCopyString(pValue, "18550F");
	 *pulSize = AnscSizeOfString(pValue);
	 return ANSC_STATUS_SUCCESS;
	 */
}

ANSC_STATUS
CosaDmlDiGetModelName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{    
    //    AnscCopyString(pValue, _ERROR_);
    AnscCopyString(pValue, "PcSim");
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetDescription
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
	// AnscCopyString(pValue, _ERROR_);//RDKB-EMU
	AnscCopyString(pValue, "Emulates the Functionality of XB3 Router");//RDKB-EMU
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
    char val[64] = {0};
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
}

ANSC_STATUS
CosaDmlDiGetSerialNumber
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
	//    AnscCopyString(pValue, _ERROR_);//RDKB-EMU
	char serial_no[256] ={0};
	int serial_no_status;
	serial_no_status=platform_hal_GetSerialNumber(serial_no);
	if(serial_no_status == 0)
		AnscCopyString(pValue,serial_no );
	else
		AnscCopyString(pValue,"RDKBEMULATOR-012345");
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
    //AnscCopyString(pValue, _ERROR_);//RDKB-EMU
    AnscCopyString(pValue, "1");
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiGetSoftwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    //AnscCopyString(pValue, _ERROR_);//RDKB-EMU
    char software_version[256] = {0};
    platform_hal_GetSoftwareVersion(software_version,0);//RDKB-EMU
    AnscCopyString(pValue,software_version);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
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

char    g_DeviceInfo_ProvisioningCode[64] = {0};

ANSC_STATUS
CosaDmlDiGetProvisioningCode
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
	//AnscCopyString(pValue,g_DeviceInfo_ProvisioningCode);
	char ProvisioningCode[256] ={0};//RDKB-EMU
	int ProvisioningCode_status;
	ProvisioningCode_status=platform_hal_GetSerialNumber(ProvisioningCode);
	if(ProvisioningCode_status == 0)
		AnscCopyString(pValue,ProvisioningCode);
	else
		AnscCopyString(pValue,"RDKBEMULATOR-012345");

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
    if (pProvisioningCode)
        AnscCopyString(g_DeviceInfo_ProvisioningCode, pProvisioningCode);

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
    if (pValue)
        AnscCopyString(pValue,"1970-01-01T00:00:01");
    *pulSize = AnscSizeOfString(pValue);
    
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlDiGetUpTime
    (
        ANSC_HANDLE                 Context
    )
{
//#if defined _ANSC_LINUX_ //RDKB-EMU
    struct sysinfo s_info;

    if(sysinfo(&s_info))
    {
        return 0;
    }
    else
    {
        return s_info.uptime;
    }
//#endif 
   // return 0;
}



ANSC_STATUS
CosaDmlGetTCPImplementation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize    
    )
{
#if defined _ANSC_LINUX
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
#endif
    AnscCopyString(pValue, _ERROR_);
    *pulSize = AnscSizeOfString(pValue);
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

#ifdef _ANSC_LINUX
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
#endif

void COSADmlGetProcessInfo(PCOSA_DATAMODEL_PROCSTATUS p_info)
{
    PCOSA_PROCESS_ENTRY        p_proc = NULL;

#ifdef _ANSC_WINDOWSNT       
    p_info->ProcessNumberOfEntries = 2;
    p_info->CPUUsage = 20;
    p_info->pProcTable = AnscAllocateMemory(sizeof(COSA_PROCESS_ENTRY)*p_info->ProcessNumberOfEntries);

    p_proc = p_info->pProcTable;
    AnscCopyString(p_proc->Command, "test_cmd1");
    p_proc->CPUTime = 10000;
    p_proc->Pid = 1;
    p_proc->Priority = 1;
    p_proc->Size = 100;
    p_proc->State = COSA_DML_PROC_STATUS_Running;
    
    p_proc = p_info->pProcTable+1;
    AnscCopyString(p_proc->Command, "test_cmd2");
    p_proc->CPUTime = 20000;
    p_proc->Pid = 2;
    p_proc->Priority = 2;
    p_proc->Size = 200;
    p_proc->State = COSA_DML_PROC_STATUS_Sleeping;
#elif defined(_ANSC_LINUX)    
    {
        static ULONG                    ProcessTimeStamp;
        ULONG                           ProcessNumber       = 0;
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

    }
#endif
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
     
#ifdef _ANSC_WINDOWSNT
     return 50;
#elif defined(_ANSC_LINUX)
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
               fclose(fp);
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
#endif
}

ULONG COSADmlGetMemoryStatus(char * ParamName)
{
#ifdef _ANSC_WINDOWSNT
     if( AnscEqualString(ParamName, "Total", TRUE))
     {
          return 128;
     }
     else if(AnscEqualString(ParamName, "Free", TRUE))
     {
          return 64;
     }
     else 
     {
          return 0;
     }
#elif defined(_ANSC_LINUX)
     struct sysinfo si;
     if (sysinfo(&si))
     {
          /*Error*/
          return 0;
     }
     if( AnscEqualString(ParamName, "Total", TRUE))
     {
          return si.totalram*si.mem_unit/1024;
     }
     else if(AnscEqualString(ParamName, "Free", TRUE))
     {
         return si.freeram*si.mem_unit/1024;
     }
     else 
     {
          return 0;
     }
#endif     
}


ULONG COSADmlGetMaxWindowSize()
{
#if defined(_ANSC_LINUX)
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
#endif
   return 0;
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

#endif

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
    return retVal;
}

#define DMSB_TR181_PSM_Syndication_Tr069CertLocation "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_Syndication.TR69CertLocation"
#define DMSB_TR181_PSM_Syndication_Enable                        "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_Syndication.enable"

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
                printf("%s - Failed Get for '%s' \n", __FUNCTION__, DMSB_TR181_PSM_Syndication_Tr069CertLocation);
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
                printf("%s - Failed Set for '%s' \n", __FUNCTION__, DMSB_TR181_PSM_Syndication_Tr069CertLocation);
                return ANSC_STATUS_FAILURE;
        }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDiGetSyndicationEnable
    (
        ANSC_HANDLE                  hContext,
                BOOL                                            *pbEnable
    )
{
        char val[ 16 ] = {0};

        if ( PsmGet( DMSB_TR181_PSM_Syndication_Enable, val, sizeof( val ) ) != 0 )
        {
                *pbEnable = 0;
                printf("%s - Failed Get for '%s' \n", __FUNCTION__, DMSB_TR181_PSM_Syndication_Enable);
                return ANSC_STATUS_FAILURE;
        }
        else
        {
                *pbEnable = ( strcmp ( val, "TRUE" ) ? 0 : 1 );
        }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDeriveSyndicationPartnerID(char *Partner_ID)
{
	char PartnerID[PARTNER_ID_LEN];
	ULONG size = PARTNER_ID_LEN - 1;
	memset(PartnerID, 0, sizeof(PartnerID));
	// Check for PartnerID available in RDKB-build, if not then return default
	if(ANSC_STATUS_FAILURE == CosaDmlDiGetSyndicationPartnerId(NULL,&PartnerID, &size))
	{
		printf("%s - Failed to get PartnerID available in build \n", __FUNCTION__ );
	}
	strncpy(Partner_ID,PartnerID,sizeof(PartnerID));
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiSetSyndicationEnable
    (
        ANSC_HANDLE                  hContext,
                BOOL                                             bEnable
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
                printf("%s - Failed Set for '%s' \n", __FUNCTION__, DMSB_TR181_PSM_Syndication_Enable);
                return ANSC_STATUS_FAILURE;
        }

    return ANSC_STATUS_SUCCESS;
}

void FillPartnerIDValues(cJSON *json , char *partnerID , PCOSA_DATAMODEL_RDKB_UIBRANDING        PUiBrand)
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
	char *pauseScreenFileLocation = NULL;
	char *partnerText = NULL;
	char *userGuideText = NULL;
	char *customerCentralText = NULL;
	char *wifiTitle = NULL;
	char *wifiWelcomeMessage = NULL;

	partnerObj = cJSON_GetObjectItem( json, partnerID );
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



	if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.PauseScreenFileLocation") != NULL )
	{
		pauseScreenFileLocation = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.PauseScreenFileLocation")->valuestring;

		if (pauseScreenFileLocation != NULL)
		{
			AnscCopyString(PUiBrand->PauseScreenFileLocation, pauseScreenFileLocation);
			pauseScreenFileLocation = NULL;
		}
		else
		{
			printf("%s - PauseScreenFileLocation Value is NULL\n", __FUNCTION__ );
		}

	}
	else
	{
		CcspTraceWarning(("%s - support Object is NULL\n", __FUNCTION__ ));
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
		//Check whether this is comcast partner or not
		if( 0 == strcmp( "comcast", partnerID ) )
		{
			syscfg_get(NULL, "user_name_3", PUiBrand->LocalUI.DefaultLoginUsername, sizeof(PUiBrand->LocalUI.DefaultLoginUsername));
		}
		else
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
			syscfg_get(NULL, "user_password_3", PUiBrand->LocalUI.DefaultLoginPassword, sizeof(PUiBrand->LocalUI.DefaultLoginPassword));
		}
		else
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
	}

	else
	{
		CcspTraceWarning(("%s - DefaultLoginPassword Object is NULL\n", __FUNCTION__ ));
	}


	if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.MSOLogoTitle") != NULL )
	{
		char *MSOLogoTitle = NULL;
		MSOLogoTitle = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.MSOLogoTitle")->valuestring;

		if (MSOLogoTitle != NULL)
		{
			AnscCopyString(PUiBrand->LocalUI.MSOLogoTitle, MSOLogoTitle);
			MSOLogoTitle = NULL;
		}
		else
		{
			CcspTraceWarning(("%s - MSOLogoTitle Value is NULL\n", __FUNCTION__ ));
		}

	}

	else
	{
		CcspTraceWarning(("%s - MSOLogoTitle Object is NULL\n", __FUNCTION__ ));
	}

	if ( cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.HomeNetworkControl") != NULL )
	{
		char *HomeNetworkControl = NULL;
		HomeNetworkControl = cJSON_GetObjectItem( partnerObj, "Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.HomeNetworkControl")->valuestring;

		if (HomeNetworkControl != NULL)
		{
			PUiBrand->LocalUI.HomeNetworkControl = FALSE;
			if ( strcmp(HomeNetworkControl,"true") == 0)
			{
				PUiBrand->LocalUI.HomeNetworkControl= TRUE;
				HomeNetworkControl = NULL;
			}
		}
		else
		{
			CcspTraceWarning(("%s - HomeNetworkControl Value is NULL\n", __FUNCTION__ ));
		}

	}

	else
	{
		CcspTraceWarning(("%s - HomeNetworkControl Object is NULL\n", __FUNCTION__ ));
	}
}

ANSC_STATUS UpdateJsonParam
        (
                char*                       pKey,
                char*                   PartnerId,
                char*                   pValue
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
                 printf("%s-%d : Error in opening JSON file\n" , __FUNCTION__, __LINE__ );
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
                 printf("%s-%d : Memory allocation failed \n", __FUNCTION__, __LINE__);
                 fclose( fileRead );
                 return ANSC_STATUS_FAILURE;
         }

         fclose( fileRead );
          if( data != NULL )
	  {
                 json = cJSON_Parse( data );
                 if( !json )
                 {
                         printf(  "%s-%d : json file parser error : [%s]\n", cJSON_GetErrorPtr() ,__FUNCTION__,__LINE__);
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
                                         printf( "Updated json content is %s\n", cJsonOut);
                                         configUpdateStatus = writeToJson(cJsonOut);
                                         if ( !configUpdateStatus)
                                         {
                                                 printf( "Updated Value for %s partner\n",PartnerId);
                                                 printf( "Param:%s - Value:%s\n",pKey,pValue);
                                         }
                                         else
                                        {
                                                 printf( "Failed to update value for %s partner\n",PartnerId);
                                                 printf( "Param:%s\n",pKey);
                                                 cJSON_Delete(json);
                                                 return ANSC_STATUS_FAILURE;
                                        }
                                 }
                                else
                                {
                                        printf("%s - OBJECT  Value is NULL %s\n", pKey,__FUNCTION__ );
                                        cJSON_Delete(json);
                                        return ANSC_STATUS_FAILURE;
                                }

                         }
                         else
                         {
                                printf("%s - PARTNER ID OBJECT Value is NULL\n", __FUNCTION__ );
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
        printf("%s : %d Failed to open file %s\n", __FUNCTION__,__LINE__,PARTNERS_INFO_FILE);
        return -1;
    }

    fwrite(data, strlen(data), 1, fp);
    fclose(fp);
    return 0;
}
				
ANSC_STATUS
CosaDmlDiUiBrandingInit
  (
        PCOSA_DATAMODEL_RDKB_UIBRANDING PUiBrand
  )
 {

        char *data = NULL;
        cJSON *json = NULL;
        FILE *fileRead = NULL;
        char PartnerID[PARTNER_ID_LEN] = {0};
        char cmd[512] = {0};
        ULONG size = PARTNER_ID_LEN - 1;
        int len;

        if (!PUiBrand)
        {
                printf("%s-%d : NULL param\n" , __FUNCTION__, __LINE__ );
                return ANSC_STATUS_FAILURE;
        }

        memset(PUiBrand, 0, sizeof(COSA_DATAMODEL_RDKB_UIBRANDING));
        if (access(PARTNERS_INFO_FILE, F_OK) != 0)
        {
                snprintf(cmd, sizeof(cmd), "cp %s %s", "/etc/partners_defaults.json", PARTNERS_INFO_FILE);
                printf("%s\n",cmd);
                system(cmd);
        }

         fileRead = fopen( PARTNERS_INFO_FILE, "r" );
         if( fileRead == NULL )
         {
                 printf("%s-%d : Error in opening JSON file\n" , __FUNCTION__, __LINE__ );
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
                 fclose( fileRead );
                 return ANSC_STATUS_FAILURE;
         }

	 fclose( fileRead );

         if( data != NULL )
         {
                 json = cJSON_Parse( data );
                 if( !json )
                 {
                         printf(  "%s-%s : json file parser error : [%d]\n", cJSON_GetErrorPtr() ,__FUNCTION__,__LINE__);
                         free(data);
                         return ANSC_STATUS_FAILURE;
                 }
                 else
                 {
                                        printf( "Reading Deafult PartnerID Values \n" );
                                        strcpy(PartnerID, "comcast");
                                        FillPartnerIDValues(json, PartnerID, PUiBrand);
                        cJSON_Delete(json);
                }
          free(data);
          data = NULL;
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

