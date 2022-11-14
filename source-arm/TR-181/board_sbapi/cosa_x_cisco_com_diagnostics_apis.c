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

    module: cosa_x_cisco_com_mld_apis.c

        For COSA Data Model Library Development

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

#include <unistd.h>

#include "cosa_apis.h"
#include "cosa_x_cisco_com_diagnostics_apis.h"
#include "cosa_x_cisco_com_diagnostics_internal.h"
#include "plugin_main_apis.h"
#include "secure_wrapper.h"
#include "cosa_drg_common.h"
#include "safec_lib_common.h"

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
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
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


#elif ( defined _COSA_INTEL_USG_ARM_ ) || ( defined _COSA_BCM_MIPS_ )
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "syscfg/syscfg.h"
#include "sysevent/sysevent.h"
#define printf(x, argv...)  
#define COSA_DML_DIAGNOSTICS_WARNING       "jRecord/dslh_objro_management.c:265 com.cisco.spvtg.ccsp.pam End of the repopulation in 'Client'"
#define COSA_DML_DIAGNOSTICS_ERROR         "com.cisco.spvtg.ccsp.pam CcspCcMbi_GetParameterValues -- Something has screwed up. Failure status returned"
#define COSA_DML_DIAGNOSTICS_NOTICE        "cmd deconfig interface erouter0 ip broadcast subnet router."

#define MAX_ENTRY_NUM 500   
#define LOG_FILE_NAME "/var/log/messages" 
#define LOG_FILE_NAME1 "/var/log/messages.0" 
/* Time format Jan  1 00:00:00 1970, LOG_TIME_SIZE not include null*/
#define LOG_TIME_SIZE 20
#define LOG_TIME_SIZE_WO_Y 15
#define LOG_INFO_SIZE 5
#define LOG_LINE_MIN_SIZE LOG_TIME_SIZE + 1 + LOG_INFO_SIZE
#define MAX_MSG_LEN 256
#define SYS_LOG_TEMP_DIR "/tmp/ssfs8s9808_syslog"
#define EVT_LOG_TEMP_DIR "/tmp/asioiwoDf_evtlog"
#define EVT_SYSLOG_USER "local4."
#define SYS_SYSLOG_USER "local5."
#define MERGED_LOG_FILE "/tmp/mergeLog.txt"
#define SORT_MERGE_LOG_FILE "/tmp/sortLog.txt"

ANSC_STATUS
CosaDmlDiagnosticsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
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

static inline int _month2i(char *m)
{
    if(0 == strncmp("Jan", m, 3))
        return 1;
    else if(0 == strncmp("Feb", m, 3))
        return 2;
    else if(0 == strncmp("Mar", m, 3))
        return 3;
    else if(0 == strncmp("Apr", m, 3))
        return 4;
    else if(0 == strncmp("May", m, 3))
        return 5;
    else if(0 == strncmp("Jun", m, 3))
        return 6;
    else if(0 == strncmp("Jul", m, 3))
        return 7;
    else if(0 == strncmp("Aug", m, 3))
        return 8;
    else if(0 == strncmp("Sep", m, 3))
        return 9;
    else if(0 == strncmp("Oct", m, 3))
        return 10;
    else if(0 == strncmp("Nov", m, 3))
        return 11;
    else if(0 == strncmp("Dec", m, 3))
        return 12;
    else
        return 0;
}

static inline int date2i(char *date)
{
    char mon[3];
    int day,h,m,s;
    int value = 0;

    if( 5 == sscanf(date, "%3s %02d %02d:%02d:%02d", mon, &day, &h, &m, &s))
    {
        value = (_month2i(mon)*10000000) + (day *100000) + (h * 3600) + (m * 60) + s;
    }
    return value;
}

static inline int _day_cmp(char * miss, char *base)
{
    int i_m = 0;
    int i_b = 0;
    i_m = date2i(miss);
    i_b = date2i(base);

    if(i_m < 10000000 || i_b < 10000000)
        return 0;

    if(i_m / 100000 == 101){
        if(i_b / 100000 >= 101 && i_b/100000 < 201 )
            return 0;
        else
            return 1970;
    }else if(i_m/100000 == 1231 ){
        if(i_b / 100000 >= 101 && i_b/100000 < 201 )
            return -1;
        else if(i_b / 100000 == 1231)
            return 0;
        else
            return 1969;
    }else if(i_m <= i_b){
        return 0;
    }else
       return -1;
}

static inline void _gen_year(char *miss,char **base)
{
    int time_status;
    int y;
    char *date = *base;
    time_status = _day_cmp(miss, *base);
    switch(time_status){
        default:
        case 0:
            //printf("copy year, time: %s , %s \n", miss, date);
            strncpy(miss + LOG_TIME_SIZE-5, date + LOG_TIME_SIZE-5, 5);
            break;
        case -1:
            //printf("subtract 1 year, time:  %s\n", miss);
            y = atoi(date + LOG_TIME_SIZE - 5) -1;
            sprintf(miss + LOG_TIME_SIZE - 5, " %4d", y);
            break;
        /* 12/31/1961 or 1/1/1970, box might not get the time  */
        case 1970:
        case 1969:
            //printf("this is init time:  %s\n", miss);
            sprintf(miss + LOG_TIME_SIZE - 5, " %4d", time_status);
            break;
    }
}

static inline void _gen_years(PCOSA_DML_DIAGNOSTICS_ENTRY p, char *date, int pos)
{
    int j = 0;
    char *base = date;
    for(j = pos; j >= 0 ; j--){
        if(p[j].Time[LOG_TIME_SIZE-5] == '\0')
        {
            _gen_year(p[j].Time, &base);
        }
        else if(atoi(p[j].Time + LOG_TIME_SIZE-4) <= 1971)
           continue;
        else
           break;
    }
}

static int _getLogInfo(FILE* fd, PCOSA_DML_DIAGNOSTICS_ENTRY *info, int *entry_count, char *user){
    int i = 0, count=0, c;
    char *line = NULL;
    char *tmp = NULL;
    size_t LineNum;
    char LevelStr[64];
    PCOSA_DML_DIAGNOSTICS_ENTRY p;
    int len = 0;
    int year_miss = 0;
	char *timestamp;
	time_t now;
    errno_t safec_rc = -1;
    unsigned int MessageLen;

    while( (c = fgetc(fd)) != EOF){
        if( c == '\n')
            count++;
    }
    fseek(fd, 0, SEEK_SET);
    count += *entry_count;
//    p = (PCOSA_DML_DIAGNOSTICS_ENTRY)AnscReallocMemory((*info), (*entry_count)*sizeof(COSA_DML_DIAGNOSTICS_ENTRY), (count + 1024) * sizeof(COSA_DML_DIAGNOSTICS_ENTRY));
    p = (PCOSA_DML_DIAGNOSTICS_ENTRY)realloc((*info), (count + 4) * sizeof(COSA_DML_DIAGNOSTICS_ENTRY));
    if(p == NULL){
        *entry_count = 0;
        return 0;
    }
    i = *entry_count;

    /*
       As long as the pointer to the line buffer and the variable holding its
       size are not modified between calls to getline() there's no need to free
       the line buffer after every call to getline() (the buffer will be reused
       and automatically resized as required by getline()).
    */
    while(0 < (len = getline( &line, &LineNum, fd))){
        if(len < LOG_LINE_MIN_SIZE )
            continue;

        p[i].Tag[0] = '\0';

        //Get time string
        //In R1.3 ~ R1.5 Time format is like
        // Jan  1 00:00:00
        //in R1.6 year will append in time string,like 
        // Jan  1 00:00:00 2014
#if !defined(_COSA_BCM_MIPS_)
        int year = 0;   
        memcpy(p[i].Time, line, LOG_TIME_SIZE);
        p[i].Time[LOG_TIME_SIZE] = '\0';
        /* If time format < R1.6 */
        year = atoi(p[i].Time + LOG_TIME_SIZE-5);
        if(0 == year){
            printf("Cannot get year\n");
            p[i].Time[LOG_TIME_SIZE - 5] = '\0';
            year_miss = 1;
        }else{
            /* Generate years if didn't get years before */
            if(year_miss == 1 && year >=1971){
                _gen_years(p, p[i].Time, i-1);
                year_miss = 0;
            }
        }
#else
        memcpy(p[i].Time, line, LOG_TIME_SIZE_WO_Y);
        p[i].Time[LOG_TIME_SIZE_WO_Y] = '\0';
#endif

        //Get Level
        if(NULL != (tmp = strstr(line, user))){
             if(1 != sscanf(tmp, "%*[^.].%63s", LevelStr)){
                 continue;
             }else{
                 p[i].Level = _Level_str2num(LevelStr);
                 strncpy(p[i].Tag, user, sizeof(p[i].Tag));
             }
        }else{
             continue;
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
                MessageLen = (((len + 4) >> 2) <<2); //Destination max size(MessageLen) should be unsigned for non safec platform
                //strncpy(p[i].pMessage, tmp, sizeof(p[i].pMessage) - 1);
                safec_rc = strcpy_s(p[i].pMessage, MessageLen, tmp);
                if(safec_rc != EOK)
                {
                   ERR_CHK(safec_rc);
                }
             }
        }else{
             continue;
        }
        i++;
    }

    if(line){
        free(line);
        line = NULL;
    }

    if(i == 0){
        AnscFreeMemory(p);
        p = NULL;
    }
    if(year_miss == 1){
       /* cannot find years from log, get local time */ 
		time(&now);
		timestamp = ctime(&now) + 4; /* skip day of week */
        _gen_years(p, timestamp, i - 1);
    }
    *info = p;
    *entry_count = i;
    return i;

}

static int _get_log(PCOSA_DML_DIAGNOSTICS_ENTRY *ppEntry, char *path, char *user, size_t *bufsize){
    struct dirent *result = NULL;
    DIR *dir;
    FILE* fd;
    int count=0;
    PCOSA_DML_DIAGNOSTICS_ENTRY entry = NULL;
    char HOSTNAME[64];
    UNREFERENCED_PARAMETER(bufsize);

    dir = opendir(path);
    if(dir == NULL)
        return 0;

    while((result = readdir(dir))){
        if(result == NULL)
            break;
        if(result->d_name[0] == '.')
            continue;

        v_secure_system("cat %s/%s >> " MERGED_LOG_FILE, path, result->d_name);
    }

    gethostname(HOSTNAME, sizeof(HOSTNAME));

    /* Sort the logs in descending order of timestamp*/
    v_secure_system("grep %s " MERGED_LOG_FILE " | sort -r -n -k4 > " SORT_MERGE_LOG_FILE, HOSTNAME);

    fd = fopen(SORT_MERGE_LOG_FILE, "r");
    _getLogInfo(fd, &entry, &count, user);
    fclose(fd);

    closedir(dir);

    v_secure_system("rm -f " MERGED_LOG_FILE" "SORT_MERGE_LOG_FILE);

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
    char temp[512];
    char LOGFILE[64];

    *pulCount = 0;
    *ppDiagnosticsEntry = NULL;
    
    LOGFILE[0] = '\0';
    UNREFERENCED_PARAMETER(hContext);
    
    if( (!commonSyseventGet("SYS_LOG_FILE_V2", LOGFILE, sizeof(LOGFILE))) \
        && (LOGFILE[0] == '\0'))
        return ANSC_STATUS_FAILURE;
    
    v_secure_system("mkdir -p " SYS_LOG_TEMP_DIR " ; log_handle.sh uncompress_syslog " SYS_LOG_TEMP_DIR);

    snprintf(temp, sizeof(temp), "%s.0", LOGFILE);
    if(!access(temp, 0)){
        v_secure_system("cp %s.0 " SYS_LOG_TEMP_DIR, LOGFILE);
    }

    if(!access(LOGFILE, 0)){
        v_secure_system("cp %s " SYS_LOG_TEMP_DIR, LOGFILE);
    }

    *pulCount = _get_log(ppDiagnosticsEntry, SYS_LOG_TEMP_DIR, SYS_SYSLOG_USER, NULL);
    /*snprintf(temp, sizeof(temp), "rm -rf %s", dir);
    printf("%s\n",temp);
    system(temp);
*/
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiagnosticsGetEventlog
    (
        ANSC_HANDLE                    hContext,
        PULONG                         pulCount,
        PCOSA_DML_DIAGNOSTICS_ENTRY *ppDiagnosticsEntry
    )
{
    char temp[512];
    char LOGFILE[64];

    *pulCount = 0;
    *ppDiagnosticsEntry = NULL;
    LOGFILE[0] = '\0'; 
    
    UNREFERENCED_PARAMETER(hContext);
    if( (!commonSyseventGet("EVT_LOG_FILE_V2", LOGFILE, sizeof(LOGFILE))) \
        && (LOGFILE[0] == '\0'))
        return ANSC_STATUS_FAILURE;

    v_secure_system("mkdir -p " EVT_LOG_TEMP_DIR " ; log_handle.sh uncompress_evtlog " EVT_LOG_TEMP_DIR);

    snprintf(temp, sizeof(temp), "%s.0", LOGFILE);
    if(!access(temp, 0)){
        v_secure_system("cp %s.0 " EVT_LOG_TEMP_DIR, LOGFILE);
    }

    if(!access(LOGFILE, 0)){
        v_secure_system("cp %s " EVT_LOG_TEMP_DIR, LOGFILE);
    }

    *pulCount = _get_log(ppDiagnosticsEntry, EVT_LOG_TEMP_DIR, EVT_SYSLOG_USER, NULL);
    /*snprintf(temp, sizeof(temp), "rm -rf %s", dir);
    printf("%s\n",temp);
    system(temp);
*/
    printf("*pulCount %d \n",*pulCount);
    return ANSC_STATUS_SUCCESS;
}

static PCOSA_DML_DIAGNOSTICS_ENTRY pEventLogBuf = NULL;
static int EventLogNum = 0;
static size_t EventLogBufsize = 0;
static ULONG EventLogLastTick;
static PCOSA_DML_DIAGNOSTICS_ENTRY pSystemLogBuf = NULL;
static int SystemLogNum = 0;
static size_t SystemLogBufsize = 0;
static ULONG SystemLogLastTick;

static char LOG_LEVEL_STR[][16]={"Unknown", "Emergency", "Alert", "Critical", "Error", "Warning", "Notice", "Informational", "Debug"};

#define REFRESH_INTERVAL 30
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))

static void __freeDiagEntry(PCOSA_DML_DIAGNOSTICS_ENTRY *ppEntry, int *num)
{
    int index;
    if(*ppEntry != NULL)
    {
        for( index = 0; index < *num; index++ )
       {
            if ( (*ppEntry)[index].pMessage != NULL )
            {
                free((*ppEntry)[index].pMessage);
            }
        }
        AnscFreeMemory(*ppEntry);
        *ppEntry        = NULL;
        *num= 0;
    }
}

static int __is_updated(ULONG *last_tick)
{
    if ( !(*last_tick) )
    {
        *last_tick = AnscGetTickInSeconds();

        return TRUE;
    }

    if ( *last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else
    {
        *last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
}

ANSC_STATUS
CosaDmlDiagnosticsGetAllEventlog
    (
        char*                          pValue,
        ULONG*                         pUlSize
    )
{
    char temp[512];
    char LOGFILE[64];
    size_t tmpsize = 0;
    int i;
    LOGFILE[0] = '\0';

    if(__is_updated(&EventLogLastTick))
    {
        __freeDiagEntry(&pEventLogBuf, &EventLogNum);
        EventLogBufsize = 0;
        if( (!commonSyseventGet("EVT_LOG_FILE_V2", LOGFILE, sizeof(LOGFILE))) \
            && (LOGFILE[0] == '\0'))
            return ANSC_STATUS_FAILURE;

        v_secure_system("mkdir -p " EVT_LOG_TEMP_DIR " ; log_handle.sh uncompress_evtlog " EVT_LOG_TEMP_DIR);
        
        snprintf(temp, sizeof(temp), "%s.0", LOGFILE);
        if(!access(temp, 0)){
            v_secure_system("cp %s.0 " EVT_LOG_TEMP_DIR, LOGFILE);
        }

        if(!access(LOGFILE, 0)){
            v_secure_system("cp %s " EVT_LOG_TEMP_DIR, LOGFILE);
        }

        EventLogNum = _get_log(&pEventLogBuf, EVT_LOG_TEMP_DIR, EVT_SYSLOG_USER, &EventLogBufsize);
        v_secure_system("rm -rf " EVT_LOG_TEMP_DIR);
    }

    if(*pUlSize < EventLogBufsize)
    {
        *pUlSize = EventLogBufsize;
        return ANSC_STATUS_FAILURE;
    }else{
        for(i=0 ; (i< EventLogNum) && (*pUlSize > tmpsize + 1); i++){
           tmpsize += snprintf(pValue + tmpsize, *pUlSize - 1 - tmpsize , "\n%s\n%s\n%s\n%s", pEventLogBuf[i].Time, pEventLogBuf[i].Tag, LOG_LEVEL_STR[pEventLogBuf[i].Level], pEventLogBuf[i].pMessage );
        }
        pValue[tmpsize] = '\0';
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlDiagnosticsGetAllSyslog
    (
        char*                          pValue,
        ULONG*                         pUlSize
    )
{
    char temp[512];
    char LOGFILE[64];
    size_t tmpsize = 0;
    int i;
    LOGFILE[0] = '\0';

    if(__is_updated(&SystemLogLastTick))
    {
        __freeDiagEntry(&pSystemLogBuf, &SystemLogNum);
        SystemLogBufsize = 0;
        if( (!commonSyseventGet("SYS_LOG_FILE_V2", LOGFILE, sizeof(LOGFILE))) \
            && (LOGFILE[0] == '\0'))
            return ANSC_STATUS_FAILURE;

        v_secure_system("mkdir -p " SYS_LOG_TEMP_DIR " ; log_handle.sh uncompress_syslog " SYS_LOG_TEMP_DIR);
        
        snprintf(temp, sizeof(temp), "%s.0", LOGFILE);
        if(!access(temp, 0)){
            v_secure_system("cp %s.0 " SYS_LOG_TEMP_DIR, LOGFILE);
        }

        if(!access(LOGFILE, 0)){
            v_secure_system("cp %s " SYS_LOG_TEMP_DIR, LOGFILE);
        }

        SystemLogNum = _get_log(&pSystemLogBuf, SYS_LOG_TEMP_DIR, SYS_SYSLOG_USER, &SystemLogBufsize);
        v_secure_system("rm -rf " SYS_LOG_TEMP_DIR);
    }

    if(*pUlSize < SystemLogBufsize)
    {
        *pUlSize = SystemLogBufsize;
        return ANSC_STATUS_FAILURE;
    }else{
        for(i=0; (i< SystemLogNum) && (*pUlSize > tmpsize + 1); i++){
           tmpsize += snprintf(pValue + tmpsize, *pUlSize - 1 - tmpsize , "\n%s\n%s\n%s\n%s", pSystemLogBuf[i].Time, pSystemLogBuf[i].Tag, LOG_LEVEL_STR[pSystemLogBuf[i].Level], pSystemLogBuf[i].pMessage );
        }
        pValue[tmpsize] = '\0';
        return ANSC_STATUS_SUCCESS;
    }
}
#endif

