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

#include "cosa_x_cisco_com_rlog_apis.h"
#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include "secure_wrapper.h"

/* 
 * For USGv2 (_COSA_INTEL_USG_ARM_):
 *
 * syslogd is not started by cosa but by boot-up script "etc/scripts/sys_startup.sh", 
 * and no more control later. At that time, cosa/utopia are not available.
 * There is an utopia module for syslogd, unfortunately, it's disabled.
 * (see ./Utopia/source/scripts/init/service.d/service_syslog.sh)
 *
 * so let's just restart the syslogd without changing the original behaviour:
 *
 *   1. /etc/syslog.conf is used for config file and it's a symbol link
 *   2. "-l" should be used to set level (/nvram/syslog_level)
 *   3. using "-R" to enable remote log, and also "-L"to reserver local logging.
 *
 * Some limitation for remote log in uclibc-v1.19.2
 *
 *   4. do not support TCP for remote log
 *   5. do not support config file for remote log
 *   6. do not support pattern for remote log
 */

/* 
 * The reason not using utopia's "log_remote" is that
 * if we set it to "0" (disable) then we lost other info 
 */
#define RLOG_CFG_ENABLE         "rlog_enable"
#define RLOG_CFG_HOST           "rlog_host"
#define RLOG_CFG_PORT           "rlog_port"

#define SYSLOGD_PID_FILE        "/var/run/syslogd.pid"
#define SYSLOGD_LEVEL_FILE      "/nvram/syslog_level"
#define SYSLOGD_DEF_LEVEL       6

#define RLOG_DEBUG

#ifdef RLOG_DEBUG
#define RLOG_DBG(x ...)         do { fprintf(stderr, x); } while (0)
#else
#define RLOG_DBG(x ...)
#endif

static int 
RLog_LoadConf(PCOSA_DML_RLOG conf)
{
    UtopiaContext ctx;
    char param[256];

    if (!Utopia_Init(&ctx))
        return -1;

    memset(conf, 0, sizeof(*conf));

    Utopia_RawGet(&ctx, NULL, RLOG_CFG_ENABLE, param, sizeof(param));
    if (atoi(param) == 1)
        conf->Enable = TRUE;
    else
        conf->Enable = FALSE;

    Utopia_RawGet(&ctx, NULL, RLOG_CFG_HOST, conf->Host, sizeof(conf->Host));

    Utopia_RawGet(&ctx, NULL, RLOG_CFG_PORT, param, sizeof(param));
    conf->Port = atoi(param);
    
    /* could not change */
    conf->Protocol = COSA_DML_RLOG_PROTO_UDP;
    snprintf(conf->Patterns, sizeof(conf->Patterns), "*.*");

    Utopia_Free(&ctx, 0);
    return 0;
}

static int 
RLog_SaveConf(const PCOSA_DML_RLOG conf)
{
    UtopiaContext ctx;
    char param[256];

    if (!Utopia_Init(&ctx))
        return -1;

    if (conf->Enable)
        Utopia_RawSet(&ctx, NULL, RLOG_CFG_ENABLE, "1");
    else
        Utopia_RawSet(&ctx, NULL, RLOG_CFG_ENABLE, "0");

    Utopia_RawSet(&ctx, NULL, RLOG_CFG_HOST, conf->Host);

    snprintf(param, sizeof(param), "%d", conf->Port);
    Utopia_RawSet(&ctx, NULL, RLOG_CFG_PORT, param);

    Utopia_Free(&ctx, 1);
    return 0;
}

static int 
RLog_GetLevel(void)
{
    FILE *fp;
    char line[64], *ptr, *sp;
    int level = SYSLOGD_DEF_LEVEL;
    
    if ((fp = fopen(SYSLOGD_LEVEL_FILE, "rb")) == NULL)
        return SYSLOGD_DEF_LEVEL;

    if (fgets(line, sizeof(line), fp) != NULL)
    {
        if ((ptr = strtok_r(line, " \t\r\n", &sp)) != NULL)
        {
            if (atoi(ptr) >= 1 && atoi(ptr) <= 8)
                level = atoi(ptr);
        }
    }

    fclose(fp);
    return level;
}

static int
RLog_Restart(PCOSA_DML_RLOG conf)
{
    int err=-1, level;

#if 0 /* no PID file in current version */
    if (access(SYSLOGD_PID_FILE, F_OK) == 0)
        vsystem("kill `cat %s`", SYSLOGD_PID_FILE);
#else
    vsystem("kill `ps | awk '/syslogd/ && !/awk/ {print $1}'`");
#endif

    level = RLog_GetLevel();
    if (conf->Enable && strlen(conf->Host) > 0)
    {
        CcspTraceInfo(("%s vsystem %d \n", __FUNCTION__,__LINE__)); 
        if (conf->Port == 0 || conf->Port > 65535)
            err = v_secure_system("syslogd -l %d -R %s -L", level, conf->Host);
        else
            err = v_secure_system("syslogd -l %d -R %s:%lu -L", level, conf->Host, conf->Port);
    }
    else
    {
        CcspTraceInfo(("%s vsystem %d \n", __FUNCTION__,__LINE__)); 
        err = vsystem("syslogd -l %d", level);
    }

    if (err != 0)
        return -1;

    return 0;
}

ANSC_STATUS 
CosaDmlRLog_Init(void)
{
    COSA_DML_RLOG conf;
    int level;

    if (RLog_LoadConf(&conf) != 0)
    {
        RLOG_DBG("%s: fail to load config\n", __FUNCTION__);
        AnscTraceError(("%s: fail to load config\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    if (!conf.Enable && vsystem("ps | grep syslogd | grep -v grep >/dev/null") == 0)
    {
        /* do not restart syslogd */
        RLOG_DBG("%s: Nothing need to do !\n", __FUNCTION__);
        return ANSC_STATUS_SUCCESS;
    }

    if (RLog_Restart(&conf) != 0)
    {
        RLOG_DBG("%s: fail to start remote logging\n", __FUNCTION__);
        AnscTraceError(("%s: fail to start remote logging\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    RLOG_DBG("%s: start remote log success !\n", __FUNCTION__);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDmlRLog_Term()
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDmlRLog_Validate(PCOSA_DML_RLOG pRLog)
{
    if (!pRLog)
        return ANSC_STATUS_FAILURE;

    if (pRLog->Enable)
    {
        if (strlen(pRLog->Host) == 0)
            return ANSC_STATUS_FAILURE;

        if (pRLog->Port > 65535)
            return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDmlRLog_SetConf(PCOSA_DML_RLOG pRLog)
{
    if (!pRLog)
        return ANSC_STATUS_FAILURE;

    if (RLog_Restart(pRLog) != 0)
    {
        RLOG_DBG("%s: fail to restart remote logging\n", __FUNCTION__);
        AnscTraceError(("%s: fail to restart remote logging\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    if (RLog_SaveConf(pRLog) != 0)
    {
        RLOG_DBG("%s: fail to save config\n", __FUNCTION__);
        AnscTraceError(("%s: fail to save config\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    RLOG_DBG("%s: set config success !\n", __FUNCTION__);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDmlRLog_GetConf(PCOSA_DML_RLOG pRLog)
{
    if (!pRLog)
        return ANSC_STATUS_FAILURE;
    
    if (RLog_LoadConf(pRLog) != 0)
    {
        RLOG_DBG("%s: fail to read config\n", __FUNCTION__);
        AnscTraceError(("%s: fail to read config\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    RLOG_DBG("%s: get config success !\n", __FUNCTION__);
    return ANSC_STATUS_SUCCESS;
}
