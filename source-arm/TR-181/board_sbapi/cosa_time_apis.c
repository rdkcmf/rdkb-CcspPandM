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

    module: cosa_time_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlTimeInit
        *  CosaDmlTimeSetCfg
        *  CosaDmlTimeGetCfg
        *  CosaDmlTimeGetState
        *  CosaDmlTimeGetLocalTime
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

#include "cosa_time_apis.h"
#include "cJSON.h"
#include "secure_wrapper.h"

#define PARTNERS_INFO_FILE              "/nvram/partners_defaults.json"
#define BOOTSTRAP_INFO_FILE             "/nvram/bootstrap.json"

ANSC_STATUS
CosaNTPInitJournal
    (
        PCOSA_DML_TIME_CFG pTimeCfg
    );

#ifdef _COSA_SIM_

COSA_DML_TIME_CFG                   g_TimeCfg = {0};

ANSC_STATUS
CosaDmlTimeInit
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
CosaDmlTimeSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_CFG          pTimeCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (pTimeCfg)
        AnscCopyMemory(&g_TimeCfg, pTimeCfg, sizeof(COSA_DML_TIME_CFG));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_CFG          pTimeCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (pTimeCfg)
        AnscCopyMemory(pTimeCfg, &g_TimeCfg, sizeof(COSA_DML_TIME_CFG));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeGetState
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_STATUS       pStatus,
        PANSC_UNIVERSAL_TIME        pCurrLocalTime
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (pStatus && pCurrLocalTime)
    {
        *pStatus = COSA_DML_TIME_STATUS_Synchronized;
        _ansc_memset(pCurrLocalTime, 0, sizeof(ANSC_UNIVERSAL_TIME));
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeGetLocalTime
    (
       ANSC_HANDLE                 hContext,
       char                       *pCurrLocalTime
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pCurrLocalTime);
    return ANSC_STATUS_SUCCESS;
}

#elif (_COSA_INTEL_USG_ARM_ || _COSA_DRG_TPG_ || _COSA_BCM_MIPS_)

#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include "cosa_drg_common.h"
#include "platform_hal.h"
#define UTOPIA_TR181_PARAM_SIZE1   256

#define MAXBUF              512

/**
 * ntpclient config file and it's parameters
 * XXX: ntpclient can only support one NTP server.
 */
#define NTPC_CONF           "/mnt/jffs2/ntpclient.conf"
#define     NTPPAR_ENABLE       "Enable"
#define     NTPPAR_NTPSERVER1   "NTPServer"
#define     NTPPAR_NTPSERVER2   "NTPServer2"
#define     NTPPAR_TIMEZONE     "TimeZone"

#define DEF_NTPSERVER1      "ntp1.sbcglobal.net"
#define DEF_NTPSERVER2      "ntp2.sbcglobal.net"
#define DEF_TIMEZONE        "PacificTime"

#define NTPC_STATUS         "/tmp/ntpclient.log"
#define NTPC_STARSTATUS     "/tmp/ntpstatus"

#ifndef NELEMS
#define NELEMS(arr)             (sizeof(arr) / sizeof((arr)[0]))
#endif

ANSC_STATUS updateTimeZone(const char *timezone)
{
    if(timezone == NULL)
        return ANSC_STATUS_FAILURE;

    FILE *fp = NULL;
    char zone[MAXBUF] = {0};

    if ((fp = fopen("/etc/TZ", "w")) == NULL)
    {
        AnscTraceWarning(("%s: cannot open file /etc/TZ\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    snprintf(zone, sizeof(zone), "%s\n", timezone);
    if (fwrite(zone, strlen(zone), 1, fp) != 1)
    {
        AnscTraceWarning(("%s: fail to write\n", __FUNCTION__));
        fclose(fp);
        return ANSC_STATUS_FAILURE;
    }
    fclose(fp);
    return ANSC_STATUS_SUCCESS;
}

#if defined (_COSA_BCM_MIPS_)
/* The XF3 and CFG3 use the thin client version of ntpd called timesyncd. This version uses a
 * config file at /etc/systemd/timesyncd.conf. If the enable option is set, we'll update the value
 * in the conf file and restart the timesyncd service to pick up the changes.
 */
/* The /etc/systemd/timesyncd.conf file is mounted copy bind to /tmp/systemd-timesyncd.conf so
 * we will need to edit the file there.
 */
const char timesyncConfFile[] = "/tmp/systemd-timesyncd.conf";
const char updateTimesyncConf[] = "/usr/ccsp/updateTimesyncdConf.sh";

/*
 * Timesyncd uses "timedatectl status" to fetch whether or not we are synced.
 *
 * From the output of the above command, we are looking for one of these lines:
 * "NTP synchronized: yes"
 * "NTP synchronized: no"
 */
BOOL isTimeSynced()
{
     char buf[MAXBUF] = {0};
     BOOL isSync = FALSE;

     AnscTraceWarning(("%s: isTimeSynced.\n", __FUNCTION__));

     FILE *fp = popen("timedatectl status", "r");

     if ( fp != NULL){
         while (fgets(buf, sizeof(buf), fp) != NULL)
         {
             if (strstr(buf, "NTP synchronized: yes") != NULL) {
                 isSync = TRUE;
                 break;
             } else if (strstr(buf, "NTP synchronized: no") != NULL) {
                 break;
             }
         }

         pclose(fp);
     }

     return isSync;
}


void* startNTP(void* arg)
{
    char buf[MAXBUF]= {0};
    char server[MAXBUF] = {0};
    int  i = 0;
    PCOSA_DML_TIME_CFG pTimeCfg = (PCOSA_DML_TIME_CFG)arg;

    AnscTraceWarning(("%s: Start Function...\n", __FUNCTION__));
    if (pTimeCfg->bEnabled)
        snprintf(buf, sizeof(buf), "1");
    else
        snprintf(buf, sizeof(buf), "0");

    /*update time zone*/
    if(ANSC_STATUS_SUCCESS != updateTimeZone(pTimeCfg->LocalTimeZone))
    {
        AnscTraceWarning(("%s: Fail to update time zone!\n", __FUNCTION__));
        return NULL;
    }

    /* Timesynd supports multiple NTP Servers, so we'll build up a list */
    if (pTimeCfg->bEnabled)
    {
        for (i=0;i<=5;i++) {
            switch (i) {
            case 1:
                if (pTimeCfg->NTPServer1.ActiveValue && strlen(pTimeCfg->NTPServer1.ActiveValue) > 0)
                {
                    if (server[0] != '\0') {
                        strcat(server, " "); // add spacer
                    }
                    strcat(server, pTimeCfg->NTPServer1.ActiveValue);
                }
                break;
            case 2:
                if (pTimeCfg->NTPServer2.ActiveValue && strlen(pTimeCfg->NTPServer2.ActiveValue) > 0)
                {
                    if (server[0] != '\0') {
                        strcat(server, " "); // add spacer
                    }
                    strcat(server, pTimeCfg->NTPServer2.ActiveValue);
                }
                break;
            case 3:
                if (pTimeCfg->NTPServer3.ActiveValue && strlen(pTimeCfg->NTPServer3.ActiveValue) > 0)
                {
                    if (server[0] != '\0') {
                        strcat(server, " "); // add spacer
                    }
                    strcat(server, pTimeCfg->NTPServer3.ActiveValue);
                }
                break;
            case 4:
                if (pTimeCfg->NTPServer4.ActiveValue && strlen(pTimeCfg->NTPServer4.ActiveValue) > 0)
                {
                    if (server[0] != '\0') {
                        strcat(server, " "); // add spacer
                    }
                    strcat(server, pTimeCfg->NTPServer4.ActiveValue);
                }
                break;
            case 5:
                if (pTimeCfg->NTPServer5.ActiveValue && strlen(pTimeCfg->NTPServer5.ActiveValue) > 0) {
                    if (server[0] != '\0') {
                        strcat(server, " "); // add spacer
                    }
                    strcat(server, pTimeCfg->NTPServer5.ActiveValue);
                }
                break;
            default:
                break;
            }
        }

        if (strlen(server) != 0)
        {
            /**
             * Kill the old timesyncd process and unmount the timesyncd.conf file
             */
            AnscTraceWarning(("%s: stopping ntpclient \n", __FUNCTION__));
            v_secure_system("systemctl stop tmp-systemd-timesyncd.conf.service");
            v_secure_system("systemctl stop systemd-timesyncd");

            // Copy the new server(s) into the temp config file
            v_secure_system("sed -i '/^[#\\s]*NTP=/s/.*/NTP=%s/' %s", server, timesyncConfFile);

            // remount the modified timesyncd.conf file
            v_secure_system("systemctl start tmp-systemd-timesyncd.conf.service");

            // Restart timesyncd
            AnscTraceWarning(("%s: starting ntpclient with host %s,command:%s\n", __FUNCTION__, server, buf));
            sleep(2);
            if (v_secure_system("systemctl start systemd-timesyncd") != 0)
            {
                AnscTraceWarning(("%s: fail to execute ntpclient\n", __FUNCTION__));
            }
        }
    } else {
        if (access(updateTimesyncConf, F_OK) == 0)
        {
            AnscTraceWarning(("%s: Set NTP Server via default method\n", __FUNCTION__));
            // Update the timesyncd.conf file using the default method
            v_secure_system("/usr/ccsp/updateTimesyncdConf.sh");
        }
    }
    return NULL;
}

#else
/* TURE means synced, FALSE means fail or un-synced */
BOOL isTimeSynced()
{
     FILE *fp = NULL;
     char buf[128] = {0};
     BOOL isSync = FALSE;

     AnscTraceWarning(("%s: isTimeSynced.\n", __FUNCTION__));
     
     if (access(NTPC_STARSTATUS, F_OK) == 0)
     {
          AnscTraceWarning(("%s: open NTPC_STARSTATUS,.\n", __FUNCTION__));
	  fp = fopen(NTPC_STARSTATUS , "r");
	  if(fp == NULL) {
	       return FALSE;
	  }

	  fread(buf, 1, sizeof(buf), fp);
	  if (strncmp(buf, "Synchronized", 12) == 0)
	       isSync = TRUE;
	  else
	       isSync = FALSE;
	  fclose(fp);
     }
     else
     {
         AnscTraceWarning(("%s: open NTPC_STATUS,,.\n", __FUNCTION__));
	 fp = fopen(NTPC_STATUS, "r");
	 if(fp == NULL) {
	      isSync = FALSE;
	      return isSync;
	 }

	 fread(buf, 1, sizeof(buf), fp);
	 if (strstr(buf, "Synchronized")!= NULL) 
	      isSync = TRUE;
	 else
	      isSync = FALSE;

	 fclose(fp);
     }

     return isSync;
}

void* startNTP(void* arg)
{
    char buf[MAXBUF]= {0};
    char *server = NULL;
    char *back_server = NULL;
    int  i = 0;
    char wan_interface[32] = {0};
    PCOSA_DML_TIME_CFG pTimeCfg = (PCOSA_DML_TIME_CFG)arg;

    AnscTraceWarning(("%s: Start Function...\n", __FUNCTION__));
    if (pTimeCfg->bEnabled)
        snprintf(buf, sizeof(buf), "1");
    else
        snprintf(buf, sizeof(buf), "0");

    /*update time zone*/
    if(ANSC_STATUS_SUCCESS != updateTimeZone(pTimeCfg->LocalTimeZone))
    {
        AnscTraceWarning(("%s: Fail to update time zone!\n", __FUNCTION__));
        return NULL;
    }

    /**
     * to kill the old process whenever NTP is enabled or not, 
     * since NTPServer may changed.
     */
    v_secure_system("killall ntpclient >/dev/null 2>&1");
    v_secure_system("rm -rf /tmp/ntpstatus");
    v_secure_system("rm -rf /tmp/ntpclient.log");
    AnscTraceWarning(("%s: stopping ntpclient \n", __FUNCTION__));

    /*get current eRT interface*/
    commonSyseventGet("current_wan_ifname", wan_interface, sizeof(wan_interface));
    if('\0' == wan_interface[0])
    {
        /*default wan interface*/
        commonSyseventGet("wan_ifname", wan_interface, sizeof(wan_interface));
    }

    /* XXX: ntpclient only support one NTP Server */
    if (pTimeCfg->bEnabled)
    {
        if ( pTimeCfg->NTPServer1.ActiveValue && strlen(pTimeCfg->NTPServer1.ActiveValue) > 0)
        {
            server = pTimeCfg->NTPServer1.ActiveValue;
            //back_server = pTimeCfg->NTPServer2;
        }
        else if (pTimeCfg->NTPServer2.ActiveValue && strlen(pTimeCfg->NTPServer2.ActiveValue) > 0) {
            server = pTimeCfg->NTPServer2.ActiveValue;
        }
        else if (pTimeCfg->NTPServer3.ActiveValue && strlen(pTimeCfg->NTPServer3.ActiveValue) > 0) {
            server = pTimeCfg->NTPServer3.ActiveValue;
        }
        else if (pTimeCfg->NTPServer4.ActiveValue && strlen(pTimeCfg->NTPServer4.ActiveValue) > 0) {
            server = pTimeCfg->NTPServer4.ActiveValue;
        }
        else if (pTimeCfg->NTPServer5.ActiveValue && strlen(pTimeCfg->NTPServer5.ActiveValue) > 0) {
            server = pTimeCfg->NTPServer5.ActiveValue;
        }
        else
            server = NULL;

        if (server)
        {
            /* XXX: sleep a while to prevent be killed by "killall" above.
             * if we can found PID for kill instead of using killall 
             * (e.g., ps | awk '/ntpclient/ {print $1}' | xargs kill )
             * then we needn't sleep here. */

            AnscTraceWarning(("%s: starting ntpclient with host %s,command:%s\n", __FUNCTION__, server, buf));
            sleep(1);
            if (v_secure_system("ntpclient -i 2 -s -h %s -I %s 2>%s", server, wan_interface, NTPC_STATUS) != 0)
            {
                AnscTraceWarning(("%s: fail to execute ntpclient\n", __FUNCTION__));
            }

            for(i=1;i<=5;i++) {
                if (i == 1 && pTimeCfg->NTPServer1.ActiveValue && strlen(pTimeCfg->NTPServer1.ActiveValue) > 0) { 
                    back_server = pTimeCfg->NTPServer1.ActiveValue;
                } else if(i == 2 && pTimeCfg->NTPServer2.ActiveValue && strlen(pTimeCfg->NTPServer2.ActiveValue) > 0){
                    back_server = pTimeCfg->NTPServer2.ActiveValue;
                } else if(i == 3 && pTimeCfg->NTPServer3.ActiveValue && strlen(pTimeCfg->NTPServer3.ActiveValue) > 0){
                    back_server = pTimeCfg->NTPServer3.ActiveValue;
                } else if(i == 4 && pTimeCfg->NTPServer4.ActiveValue && strlen(pTimeCfg->NTPServer4.ActiveValue) > 0){
                    back_server = pTimeCfg->NTPServer4.ActiveValue;
                } else if(i == 5 && pTimeCfg->NTPServer5.ActiveValue && strlen(pTimeCfg->NTPServer5.ActiveValue) > 0){
                    back_server = pTimeCfg->NTPServer5.ActiveValue;
                } 
                /* try the back up ntp server */
                if (back_server && strcmp(back_server,server)!=0)
                {
                    AnscTraceWarning(("%s: trying backup server:%s\n", __FUNCTION__,back_server));
                    sleep(2); /* Wait ntpclient finished */

                    if (!isTimeSynced())
                    {
                        AnscTraceWarning(("%s: backup server not synced.\n", __FUNCTION__));
                        /**
                         * to kill the old process whenever NTP is enabled or not, 
                         * since NTPServer may changed.
                         */
                        v_secure_system("killall ntpclient >/dev/null 2>&1");
                        v_secure_system("rm -rf /tmp/ntpstatus");
                        v_secure_system("rm -rf /tmp/ntpclient.log");
                        AnscTraceWarning(("%s: stopping ntpclient 2\n", __FUNCTION__));

                        /* XXX: sleep a while to prevent be killed by "killall" above.
                         * if we can found PID for kill instead of using killall 
                         * (e.g., ps | awk '/ntpclient/ {print $1}' | xargs kill )
                         * then we needn't sleep here. */

                        AnscTraceWarning(("%s: starting ntpclient with host %s\n", __FUNCTION__, back_server));
                        sleep(1);
                        if (v_secure_system("ntpclient -i 2 -s -h %s -I %s 2>%s", back_server, wan_interface, NTPC_STATUS) != 0)
                        {
                            AnscTraceWarning(("%s: fail to execute ntpclient\n", __FUNCTION__));
                        }
                    }
                }
            }
        }
    }
    return NULL;
}
#endif

ANSC_STATUS
CosaDmlTimeInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    PCOSA_DML_TIME_CFG          pTimeCfg=(PCOSA_DML_TIME_CFG)phContext;

    CosaDmlTimeGetCfg(NULL, pTimeCfg);

/*
    if(ANSC_STATUS_SUCCESS != updateTimeZone(pTimeCfg->LocalTimeZone))
    {
        AnscTraceWarning(("%s: Fail to update time zone!\n", __FUNCTION__));
    }
*/
    return ANSC_STATUS_SUCCESS;
}



ANSC_STATUS
CosaDmlTimeSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_CFG          pTimeCfg
    )
{
    UtopiaContext ctx;
    char buf[32] = {0};
    int rc = 0;
    UNREFERENCED_PARAMETER(hContext);
    if (!pTimeCfg)
        return ANSC_STATUS_FAILURE;

//    startNTP(pTimeCfg);

    if (pTimeCfg)
    {
       /* Initialize a Utopia Context */
       if(!Utopia_Init(&ctx))
          return ERR_UTCTX_INIT;
       /* Set Local TZ to SysCfg */
       rc = Utopia_Set_DeviceTime_LocalTZ(&ctx, (char*)&(pTimeCfg->LocalTimeZone));

       /*set city index */
       sprintf(buf,"%lu",pTimeCfg->cityIndex);
       rc = Utopia_RawSet(&ctx, NULL, "ntp_cityindex", buf);

       /*Set NTP Server 1 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx, (char*)&(pTimeCfg->NTPServer1.ActiveValue), 1);

       /*Set NTP Server 2 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx, (char*)&(pTimeCfg->NTPServer2.ActiveValue), 2);

       /*Set NTP Server 3 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx, (char*)&(pTimeCfg->NTPServer3.ActiveValue), 3);

        /*Set NTP Server 4 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx, (char*)&(pTimeCfg->NTPServer4.ActiveValue), 4);

       /*Set NTP Server 5 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx, (char*)&(pTimeCfg->NTPServer5.ActiveValue), 5);
       
       /*Set NTP DaylightSaving Enabled or not to SysCfg */
       rc = Utopia_Set_DeviceTime_DaylightEnable(&ctx, pTimeCfg->bDaylightSaving);

       /*Set NTP DaylightSaving Offset to SysCfg */
       rc = Utopia_Set_DeviceTime_DaylightOffset(&ctx,pTimeCfg->DaylightSavingOffset);

       /*Set NTP Enabled or not to SysCfg */
       rc = Utopia_Set_DeviceTime_Enable(&ctx,pTimeCfg->bEnabled);

       /* Free Utopia Context */
       Utopia_Free(&ctx,!rc);
     }

#ifdef NTPD_ENABLE
       CcspTraceWarning(("%s: Triggering event to restart ntpd \n", __FUNCTION__));
	commonSyseventSet("ntpd-restart", "");
#else
    pthread_t ntp_thread;
    int err = 0;
    err = pthread_create(&ntp_thread, NULL, startNTP, (void *)pTimeCfg);

    if(0 != err)
    {
        CcspTraceError(("%s: create the ntp syn thread error!\n", __FUNCTION__));
    }
    else
        pthread_detach(ntp_thread);

#endif

     if (rc != 0)
       return ERR_SYSCFG_FAILED;
     else
       return ANSC_STATUS_SUCCESS;
}

int checkIfUTCEnabled(const char *fname)
{
#if 0
    FILE *file;
    if (file = fopen(fname, "r"))
    {
        fclose(file);
        return 0;
    }
    return 1;
#endif


       FILE *fp;
       char temp[32]={0};
       char *str="UTC_ENABLE=true";
       if((fp = fopen(fname, "r")) == NULL) {
		
                return 1;
       }

        while(fgets(temp, 32, fp) != NULL) {
                if((strstr(temp, str)) != NULL) {
			fclose(fp);
			return 0;
                }
        }

        if(fp) {
                fclose(fp);
        }

        return 1;

}

ANSC_STATUS
CosaDmlTimeGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_CFG          pTimeCfg
    )
{
    UtopiaContext ctx;
    int rc = 0;
    int utc_enabled=0;
    char val[UTOPIA_TR181_PARAM_SIZE1];
    UNREFERENCED_PARAMETER(hContext);
    if (pTimeCfg)
    {
       /* Initialize a Utopia Context */
       if(!Utopia_Init(&ctx))
           return ERR_UTCTX_INIT;
       _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);

       /* Fill Local TZ from SysCfg */
       if( (Utopia_Get_DeviceTime_LocalTZ(&ctx,val)) == UT_SUCCESS)
       {
          AnscCopyString(pTimeCfg->LocalTimeZone,val);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       rc = Utopia_RawGet(&ctx, NULL, "ntp_cityindex", val, sizeof(val)-1 );
       pTimeCfg->cityIndex = atoi(val);

       /*Fill NTP Server 1 from SysCfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,1)) == UT_SUCCESS)
       {
          AnscCopyString(pTimeCfg->NTPServer1.ActiveValue,val);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill NTP Server 2 from Syscfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,2)) == UT_SUCCESS)
       {
          AnscCopyString(pTimeCfg->NTPServer2.ActiveValue,val);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill NTP Server 3 from Syscfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,3)) == UT_SUCCESS)
       {
          AnscCopyString(pTimeCfg->NTPServer3.ActiveValue,val);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill NTP Server 4 from Syscfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,4)) == UT_SUCCESS)
       {
          AnscCopyString(pTimeCfg->NTPServer4.ActiveValue,val);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill NTP Server 5 from Syscfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,5)) == UT_SUCCESS)
       {
          AnscCopyString(pTimeCfg->NTPServer5.ActiveValue,val);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill DaylightSaving Enabled or not from syscfg */
       pTimeCfg->bDaylightSaving = Utopia_Get_DeviceTime_DaylightEnable(&ctx);

       /* Fill DaylightSaving Offset from syscfg */
       rc = Utopia_Get_DeviceTime_DaylightOffset(&ctx, (int*)&(pTimeCfg->DaylightSavingOffset));

       /* Fill NTP Enabled or not from syscfg */
       pTimeCfg->bEnabled = Utopia_Get_DeviceTime_Enable(&ctx);

       /* Free Utopia Context */
       Utopia_Free(&ctx,0);

       CosaNTPInitJournal(pTimeCfg);
     }
     
    utc_enabled = checkIfUTCEnabled(DEV_PROPERTIES_FILE);
     if (0 == utc_enabled)
     {
       /*   CcspTraceWarning(("%s: UTC Enable file exists\n", __FUNCTION__));
            printf("%s: UTC Enable file exists\n", __FUNCTION__);  */
        pTimeCfg->bUTCEnabled = TRUE;
      }
    else
      {
        /*  CcspTraceWarning(("%s: UTC Enable file not exists\n", __FUNCTION__));
            printf("%s: UTC Enable file not exists\n", __FUNCTION__); */
	 pTimeCfg->bUTCEnabled = FALSE;
       }

     if (rc != 0)
       return ERR_SYSCFG_FAILED;    
     else
       return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeGetState
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_STATUS       pStatus,
        PANSC_UNIVERSAL_TIME        pCurrLocalTime
    )
{
    UtopiaContext ctx;
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pCurrLocalTime);
    /* Initialize a Utopia Context */
    if(!Utopia_Init(&ctx))
        return ERR_UTCTX_INIT;

    *pStatus = Utopia_Get_DeviceTime_Status(&ctx);

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    return ANSC_STATUS_SUCCESS;
}

#ifdef _XF3_PRODUCT_REQ_
ANSC_STATUS
CosaDmlTimeGetTimeOffset
    (
       ANSC_HANDLE                 hContext,
       char                       *pTimeOffset
    );
#endif

ANSC_STATUS
CosaDmlTimeGetLocalTime
    (
       ANSC_HANDLE                 hContext,
       char                       *pCurrLocalTime
    )
{
    UNREFERENCED_PARAMETER(hContext);
    time_t t;
#if defined(UTC_ENABLE) && !defined(_XF3_PRODUCT_REQ_)
struct tm *pLcltime, temp;
   time(&t);
   t = t + getOffset();
   localtime_r(&t, &temp);
   pLcltime = &temp;
#else
    struct tm *pLcltime;
#ifdef _XF3_PRODUCT_REQ_
    char timeOffset[256];
    int offset;
    t = time(NULL);
    CosaDmlTimeGetTimeOffset((ANSC_HANDLE)NULL, timeOffset);
    offset = atoi(timeOffset);
    t += (time_t)offset;
#else
    t = time(NULL);
#endif
    pLcltime = localtime(&t);
#endif
    _ansc_sprintf(pCurrLocalTime, "%.4u-%.2u-%.2u %.2u:%.2u:%.2u",
            (pLcltime->tm_year)+1900,
            (pLcltime->tm_mon)+1,
            pLcltime->tm_mday,
            pLcltime->tm_hour,
            pLcltime->tm_min,
            pLcltime->tm_sec);

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlTimeGetTimeOffset
    (
       ANSC_HANDLE                 hContext,
       char                       *pTimeOffset
    )
{
    UNREFERENCED_PARAMETER(hContext);
    char offset_value[100]={0};
    memset(offset_value,0,sizeof(offset_value));
    if(!access("/nvram/ETHWAN_ENABLE", 0))
    {
        commonSyseventGet("ipv4-timeoffset", offset_value, sizeof(offset_value));
        if ( ('\0' != offset_value[0] ) && ( 0 != strlen(offset_value) ) ) 
        {
             CcspTraceWarning(("%s: offset_value received from ipv4-timeoffset is %s \n", __FUNCTION__,offset_value));
             if ( offset_value[0] == '@' )
             {
                strcpy(pTimeOffset,offset_value+1);
             }
             else
             {
                strcpy(pTimeOffset,offset_value);

             }
             return ANSC_STATUS_SUCCESS;       
        }

    }
    
    platform_hal_getTimeOffSet(pTimeOffset);

    return ANSC_STATUS_SUCCESS;
}


#define PARTNER_ID_LEN 64
void FillParamUpdateSourceNTP(cJSON *partnerObj, char *key, char *paramUpdateSource)
{
    cJSON *paramObj = cJSON_GetObjectItem( partnerObj, key);
    if ( paramObj != NULL )
    {
        char *valuestr = NULL;
        cJSON *paramObjVal = cJSON_GetObjectItem(paramObj, "UpdateSource");
        if (paramObjVal)
            valuestr = paramObjVal->valuestring;
        if (valuestr != NULL)
        {
            AnscCopyString(paramUpdateSource, valuestr);
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

void FillPartnerIDNTPJournal
    (
        cJSON *json ,
        char *partnerID ,
        PCOSA_DML_TIME_CFG pTimeCfg
    )
{
                cJSON *partnerObj = cJSON_GetObjectItem( json, partnerID );
                if( partnerObj != NULL)
                {
                      FillParamUpdateSourceNTP(partnerObj, "Device.Time.NTPServer1", (char*)&pTimeCfg->NTPServer1.UpdateSource);
                      FillParamUpdateSourceNTP(partnerObj, "Device.Time.NTPServer2", (char*)&pTimeCfg->NTPServer2.UpdateSource);
                      FillParamUpdateSourceNTP(partnerObj, "Device.Time.NTPServer3", (char*)&pTimeCfg->NTPServer3.UpdateSource);
                      FillParamUpdateSourceNTP(partnerObj, "Device.Time.NTPServer4", (char*)&pTimeCfg->NTPServer4.UpdateSource);
                      FillParamUpdateSourceNTP(partnerObj, "Device.Time.NTPServer5", (char*)&pTimeCfg->NTPServer5.UpdateSource);
                }
                else
                {
                      CcspTraceWarning(("%s - PARTNER ID OBJECT Value is NULL\n", __FUNCTION__ ));
                }
}

//Get the UpdateSource info from /nvram/bootstrap.json. This is needed to know for override precedence rules in set handlers
ANSC_STATUS
CosaNTPInitJournal
    (
        PCOSA_DML_TIME_CFG pTimeCfg
    )
{
        char *data = NULL;
        cJSON *json = NULL;
        FILE *fileRead = NULL;
        char PartnerID[PARTNER_ID_LEN] = {0};
        ULONG size = PARTNER_ID_LEN - 1;
        int len;
        if (!pTimeCfg)
        {
                CcspTraceWarning(("%s-%d : NULL param\n" , __FUNCTION__, __LINE__ ));
                return ANSC_STATUS_FAILURE;
        }

        if (access(BOOTSTRAP_INFO_FILE, F_OK) != 0)
        {
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
                                        FillPartnerIDNTPJournal(json, PartnerID, pTimeCfg);
                                }
                                else
                                {
                                        CcspTraceWarning(( "Reading Deafult PartnerID Values \n" ));
                                        strcpy(PartnerID, "comcast");
                                        FillPartnerIDNTPJournal(json, PartnerID, pTimeCfg);
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
         else
         {
                CcspTraceWarning(("BOOTSTRAP_INFO_FILE %s is empty\n", BOOTSTRAP_INFO_FILE));
                return ANSC_STATUS_FAILURE;
         }
         return ANSC_STATUS_SUCCESS;
}

#endif
