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

#ifdef _COSA_SIM_

// this file is in integration_src.pc directory

#include "dmsb_tr181_psm_definitions.h" // for DMSB_TR181_PSM_DeviceInfo_Root/ProductClass
#include "ccsp_psm_helper.h"            // for PSM_Get_Record_Value2

#define _ERROR_ "NOT SUPPORTED"

extern void* g_pDslhDmlAgent;

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
        char                       *inputparam
    )
{
    char wrapstring[256]={0};
    ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;
    	
	// check for possible command injection	
    if(strstr(inputparam,";"))
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
       sprintf(wrapstring,"'%s'",inputparam);
       strcpy(inputparam,wrapstring);
    }

	return returnStatus;

}
#endif
