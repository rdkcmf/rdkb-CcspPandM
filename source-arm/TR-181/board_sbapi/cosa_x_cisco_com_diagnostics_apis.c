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

    module: cosa_x_cisco_com_mld_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlNatGetPortMappingNumber
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
#include "cosa_apis.h"
#include "cosa_x_cisco_com_diagnostics_apis.h"
#include "cosa_x_cisco_com_diagnostics_internal.h"
#include "plugin_main_apis.h"

#if ( defined _COSA_SIM_ )

#define COSA_DML_DIAGNOSTICS_WARNING       "jRecord/dslh_objro_management.c:265 com.cisco.spvtg.ccsp.pam End of the repopulation in 'Client'"
#define COSA_DML_DIAGNOSTICS_ERROR         "com.cisco.spvtg.ccsp.pam CcspCcMbi_GetParameterValues -- Something has screwed up. Failure status returned"
#define COSA_DML_DIAGNOSTICS_NOTICE        "cmd deconfig interface erouter0 ip broadcast subnet router."

COSA_DML_DIAGNOSTICS_ENTRY  g_Entry[3] = 
    {
        {"2012-08-10T10:36:20", "1233", COSA_DML_DIAGNOSTICS_LEVEL_WARNING, NULL},
        {"2012-08-10T10:36:21", "1234", COSA_DML_DIAGNOSTICS_LEVEL_ERROR  , NULL},
        {"2012-08-10T10:36:22", "1235", COSA_DML_DIAGNOSTICS_LEVEL_NOTICE , NULL}
    };


ANSC_STATUS
CosaDmlDiagnosticsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiagnosticsGetEntry
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_DIAGNOSTICS_ENTRY *ppDiagnosticsEntry        
    )    
{
    ULONG                           index = 0;
    
    *pulCount                = sizeof(g_Entry)/sizeof(COSA_DML_DIAGNOSTICS_ENTRY);
    *ppDiagnosticsEntry      = AnscAllocateMemory(sizeof(g_Entry));
    AnscCopyMemory(*ppDiagnosticsEntry, &g_Entry, sizeof(g_Entry));

    for(index =0; index < *pulCount; index++)
    {
        if ( (*ppDiagnosticsEntry)[index].Level == COSA_DML_DIAGNOSTICS_LEVEL_WARNING )
        {
            (*ppDiagnosticsEntry)[index].pMessage = strdup(COSA_DML_DIAGNOSTICS_WARNING);
        }
        else if ( (*ppDiagnosticsEntry)[index].Level == COSA_DML_DIAGNOSTICS_LEVEL_ERROR )
        {
            (*ppDiagnosticsEntry)[index].pMessage = strdup(COSA_DML_DIAGNOSTICS_ERROR);
        }
        else if ( (*ppDiagnosticsEntry)[index].Level == COSA_DML_DIAGNOSTICS_LEVEL_NOTICE )
        {
            (*ppDiagnosticsEntry)[index].pMessage = strdup(COSA_DML_DIAGNOSTICS_NOTICE);
        }
    }

    return ANSC_STATUS_SUCCESS;
}


#elif ( defined _COSA_INTEL_USG_ARM_ )
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syscfg/syscfg.h"
#define printf(x, argv...)  
#define COSA_DML_DIAGNOSTICS_WARNING       "jRecord/dslh_objro_management.c:265 com.cisco.spvtg.ccsp.pam End of the repopulation in 'Client'"
#define COSA_DML_DIAGNOSTICS_ERROR         "com.cisco.spvtg.ccsp.pam CcspCcMbi_GetParameterValues -- Something has screwed up. Failure status returned"
#define COSA_DML_DIAGNOSTICS_NOTICE        "cmd deconfig interface erouter0 ip broadcast subnet router."

#define MAX_ENTRY_NUM 500   
#define LOG_FILE_NAME "/var/log/messages" 
#define LOG_FILE_NAME1 "/var/log/messages.0" 
#define LOG_TIME_SIZE 15 
#define LOG_INFO_SIZE 5
#define LOG_LINE_MIN_SIZE LOG_TIME_SIZE + 1 + LOG_INFO_SIZE 
#define MAX_MSG_LEN 256
#define SYS_LOG_TEMP_DIR "/tmp/ssfs8s9808_syslog" 
#define EVT_LOG_TEMP_DIR "/tmp/asioiwoDf_evtlog" 
#define EVT_SYSLOG_USER "local4."
#define SYS_SYSLOG_USER "local5."

ANSC_STATUS
CosaDmlDiagnosticsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{

    return ANSC_STATUS_SUCCESS;
}

static unsigned int _Level_str2num(char* str){
#define CMP_LEVEL_STR(srt, conststr)  if(!memcmp(str, conststr, strlen(conststr)))

    CMP_LEVEL_STR(str, "info"){
        return COSA_DML_DIAGNOSTICS_LEVEL_INFORMATIONAL;
    }
    CMP_LEVEL_STR(str, "notice"){
        return  COSA_DML_DIAGNOSTICS_LEVEL_NOTICE;
    }
    CMP_LEVEL_STR(str, "err"){
        return COSA_DML_DIAGNOSTICS_LEVEL_ERROR;
    }
    CMP_LEVEL_STR(str, "warn"){
        return COSA_DML_DIAGNOSTICS_LEVEL_WARNING;
    }
    CMP_LEVEL_STR(str, "emerg"){
        return COSA_DML_DIAGNOSTICS_LEVEL_EMERGENCY;
    }
    CMP_LEVEL_STR(str, "alert"){
        return COSA_DML_DIAGNOSTICS_LEVEL_ALERT;
    }
    CMP_LEVEL_STR(str, "crit"){
        return  COSA_DML_DIAGNOSTICS_LEVEL_CRITICAL;
    }
    CMP_LEVEL_STR(str, "debug"){
        return COSA_DML_DIAGNOSTICS_LEVEL_DEBUG;
    }

    return 0xFFFF;
}

static int _getLogInfo(FILE* fd, PCOSA_DML_DIAGNOSTICS_ENTRY *info, int *entry_count, char *user){
    int i = 0, count=0, c;
    char *line = NULL;
    char *tmp = NULL;
    size_t LineNum;
    char UserStr[64], LevelStr[64];
    PCOSA_DML_DIAGNOSTICS_ENTRY p;
    int len = 0;

    while( (c = fgetc(fd)) != EOF){
        if( c == '\n')
            count++;
    }
    fseek(fd, 0, SEEK_SET);
    count += *entry_count;
    p = (PCOSA_DML_DIAGNOSTICS_ENTRY)AnscReAllocateMemory((*info), count * sizeof(COSA_DML_DIAGNOSTICS_ENTRY));
    if(p == NULL){
        *entry_count = 0;
        return 0;
    }
    i = *entry_count;
    while(0 < (len = getline( &line, &LineNum, fd))){
        if(len < LOG_LINE_MIN_SIZE )
            goto CONTINUE;

        //Get time string
        memcpy(p[i].Time, line, LOG_TIME_SIZE);
        p[i].Time[LOG_TIME_SIZE] = '\0';
        //Get Level
        if(NULL != (tmp = strstr(line, user))){
             if(1 != sscanf(tmp, "%*[^.].%63s", LevelStr)){
                 goto CONTINUE;
             }else{
                 p[i].Level = _Level_str2num(LevelStr);
                 strncpy(p[i].Tag, user, sizeof(p[i].Tag));
             }
        }else{
             goto CONTINUE;
        }

        //Get Log information
        if(NULL != (tmp = strstr(tmp, LevelStr))){
             tmp += strlen(LevelStr);
             while(!isgraph(*tmp) && *tmp != '\0'){
                 tmp++;
             }

             if(line[len-1] == '\n'){
                line[len-1] = '\0';
                len--;    
             }

             p[i].pMessage = malloc((((len + 4) >> 2) <<2)); 
             if(p[i].pMessage != NULL){
                //strncpy(p[i].pMessage, tmp, sizeof(p[i].pMessage) - 1);
                strcpy(p[i].pMessage, tmp);
             }
        }else{
             goto CONTINUE;
        }
        i++;
CONTINUE:
        free(line);
        line = NULL;
    }

    if(i == 0){
        AnscFreeMemory(p);
        p = NULL;
    }
    *info = p;
    *entry_count = i;
    return i;

}

static int _get_log(PCOSA_DML_DIAGNOSTICS_ENTRY *ppEntry, char *path, char *user){
    struct dirent *ptr;    
    DIR *dir;
    FILE* fd;
    int count=0;
    PCOSA_DML_DIAGNOSTICS_ENTRY entry = NULL;
    char fName[64];

    dir = opendir(path);
    if(dir == NULL)
        return 0;

    while(( ptr = readdir(dir)) != NULL){
        if(ptr->d_name[0] == '.')
            continue;
        sprintf(fName, "%s/%s", path,ptr->d_name);
        fd = fopen(fName, "r");
        if(fd == NULL)
            continue;
        _getLogInfo(fd, &entry, &count, user);
    }
    closedir(dir);
    *ppEntry = entry;
    return count;
}

ANSC_STATUS
CosaDmlDiagnosticsGetEntry
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_DIAGNOSTICS_ENTRY *ppDiagnosticsEntry        
    )    
{
    char logfile[FILENAME_MAX]; 
    char temp[512];
    char dir[2*FILENAME_MAX];
    char LOGFILE[64];

    *pulCount = 0;
    *ppDiagnosticsEntry = NULL;

    syscfg_init();
    syscfg_get(NULL, "SYS_LOG_FILE", LOGFILE, sizeof(LOGFILE));

    if(LOGFILE[0] == '\0')
        return ANSC_STATUS_FAILURE;
    
    snprintf(dir, sizeof(dir), SYS_LOG_TEMP_DIR);
    snprintf(temp, sizeof(temp), "mkdir -p %s", dir);
    printf("%s/n",temp);
    system(temp);
    
    snprintf(temp, sizeof(temp), "log_handle.sh uncompress_syslog %s", dir);
    printf("%s\n",temp);
    system(temp);

    snprintf(temp, sizeof(temp), "%s.0", LOGFILE);
    if(!access(temp, 0)){
        snprintf(temp, sizeof(temp), "cp %s.0 %s", LOGFILE, dir);
        printf("%s\n",temp);
        system(temp);
    }
  
    if(!access(LOGFILE, 0)){
        snprintf(temp, sizeof(temp), "cp %s %s", LOGFILE, dir);
        printf("%s\n",temp);
        system(temp);
    }
    
    *pulCount = _get_log(ppDiagnosticsEntry, dir, SYS_SYSLOG_USER);   
    snprintf(temp, sizeof(temp), "rm -rf %s", dir);
    printf("%s\n",temp);
    system(temp);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiagnosticsGetEventlog
    (
        ANSC_HANDLE                    hContext,
        PULONG                         pulCount,
        PCOSA_DML_DIAGNOSTICS_EVENTLOG *ppDiagnosticsEntry        
    )    
{
    char logfile[FILENAME_MAX]; 
    char temp[512];
    char dir[2*FILENAME_MAX];
    char LOGFILE[64];

    *pulCount = 0;
    *ppDiagnosticsEntry = NULL;
    syscfg_init();
    syscfg_get(NULL, "EVT_LOG_FILE", LOGFILE, sizeof(LOGFILE));

    if(LOGFILE[0] == '\0')
        return ANSC_STATUS_FAILURE;

    snprintf(dir, sizeof(dir), EVT_LOG_TEMP_DIR);
    snprintf(temp, sizeof(temp), "mkdir -p %s", dir);
    printf("%s/n",temp);
    system(temp);
    
    snprintf(temp, sizeof(temp), "log_handle.sh uncompress_evtlog %s", dir);
    printf("%s\n",temp);
    system(temp);

    snprintf(temp, sizeof(temp), "%s.0", LOGFILE);
    if(!access(temp, 0)){
        snprintf(temp, sizeof(temp), "cp %s.0 %s", LOGFILE, dir);
        printf("%s\n",temp);
        system(temp);
    }
  
    if(!access(LOGFILE, 0)){
        snprintf(temp, sizeof(temp), "cp %s %s", LOGFILE, dir);
        printf("%s\n",temp);
        system(temp);
    }
    
    *pulCount = _get_log(ppDiagnosticsEntry, dir, EVT_SYSLOG_USER);   
    snprintf(temp, sizeof(temp), "rm -rf %s", dir);
    printf("%s\n",temp);
    system(temp);
    printf("*pulCount %d \n",*pulCount);    
    return ANSC_STATUS_SUCCESS;
}

#endif

