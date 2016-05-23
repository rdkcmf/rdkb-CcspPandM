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

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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

#ifdef _COSA_SIM_

COSA_DML_TIME_CFG                   g_TimeCfg = {0};

ANSC_STATUS
CosaDmlTimeInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_CFG          pTimeCfg
    )
{
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
    return ANSC_STATUS_SUCCESS;
}

#elif (_COSA_BCM_MIPS_ || _COSA_DRG_TPG_)

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

ANSC_STATUS startNTP(PCOSA_DML_TIME_CFG pTimeCfg)
{
    FILE *fp;
    char buf[MAXBUF]= {0};
    char *server = NULL;
    char *back_server = NULL;
    int  i = 0;
    char wan_interface[32] = {0};

    AnscTraceWarning(("%s: Start Function...\n", __FUNCTION__));
    if (pTimeCfg->bEnabled)
        snprintf(buf, sizeof(buf), "1");
    else
        snprintf(buf, sizeof(buf), "0");

    /*update time zone*/
    if(ANSC_STATUS_SUCCESS != updateTimeZone(pTimeCfg->LocalTimeZone))
    {
        AnscTraceWarning(("%s: Fail to update time zone!\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    /**
     * to kill the old process whenever NTP is enabled or not, 
     * since NTPServer may changed.
     */
    system("killall ntpclient >/dev/null 2>&1");
    system("rm -rf /tmp/ntpstatus");
    system("rm -rf /tmp/ntpclient.log");
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
        if ( pTimeCfg->NTPServer1 && strlen(pTimeCfg->NTPServer1) > 0)
        {
            server = pTimeCfg->NTPServer1;
            //back_server = pTimeCfg->NTPServer2;
        }
        else if (pTimeCfg->NTPServer2 && strlen(pTimeCfg->NTPServer2) > 0) {
            server = pTimeCfg->NTPServer2;
        }
        else if (pTimeCfg->NTPServer3 && strlen(pTimeCfg->NTPServer3) > 0) {
            server = pTimeCfg->NTPServer3;
        }
        else if (pTimeCfg->NTPServer4 && strlen(pTimeCfg->NTPServer4) > 0) {
            server = pTimeCfg->NTPServer4;
        }
        else if (pTimeCfg->NTPServer5 && strlen(pTimeCfg->NTPServer5) > 0) {
            server = pTimeCfg->NTPServer5;
        }
        else
            server = NULL;

        if (server)
        {
            /* XXX: sleep a while to prevent be killed by "killall" above.
             * if we can found PID for kill instead of using killall 
             * (e.g., ps | awk '/ntpclient/ {print $1}' | xargs kill )
             * then we needn't sleep here. */
            snprintf(buf, sizeof(buf), "sleep 1 && ntpclient -i 2 -s -h %s -I %s 2>%s", server, wan_interface, NTPC_STATUS);

            AnscTraceWarning(("%s: starting ntpclient with host %s,command:%s\n", __FUNCTION__, server, buf));
            if (system(buf) != 0)
            {
                AnscTraceWarning(("%s: fail to execute ntpclient\n", __FUNCTION__));
            }

            for(i=1;i<=5;i++) {
                if (i == 1 && pTimeCfg->NTPServer1 && strlen(pTimeCfg->NTPServer1) > 0) { 
                    back_server = pTimeCfg->NTPServer1;
                } else if(i == 2 && pTimeCfg->NTPServer2 && strlen(pTimeCfg->NTPServer2) > 0){
                    back_server = pTimeCfg->NTPServer2;
                } else if(i == 3 && pTimeCfg->NTPServer3 && strlen(pTimeCfg->NTPServer3) > 0){
                    back_server = pTimeCfg->NTPServer3;
                } else if(i == 4 && pTimeCfg->NTPServer4 && strlen(pTimeCfg->NTPServer4) > 0){
                    back_server = pTimeCfg->NTPServer4;
                } else if(i == 5 && pTimeCfg->NTPServer5 && strlen(pTimeCfg->NTPServer5) > 0){
                    back_server = pTimeCfg->NTPServer5;
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
                        system("killall ntpclient >/dev/null 2>&1");
                        system("rm -rf /tmp/ntpstatus");
                        system("rm -rf /tmp/ntpclient.log");
                        AnscTraceWarning(("%s: stopping ntpclient 2\n", __FUNCTION__));

                        /* XXX: sleep a while to prevent be killed by "killall" above.
                         * if we can found PID for kill instead of using killall 
                         * (e.g., ps | awk '/ntpclient/ {print $1}' | xargs kill )
                         * then we needn't sleep here. */
                        snprintf(buf, sizeof(buf), "sleep 1 && ntpclient -i 2 -s -h %s -I %s 2>%s", back_server, wan_interface, NTPC_STATUS);

                        AnscTraceWarning(("%s: starting ntpclient with host %s\n", __FUNCTION__, back_server));
                        if (system(buf) != 0)
                        {
                            AnscTraceWarning(("%s: fail to execute ntpclient\n", __FUNCTION__));
                        }
                    }
                }
            }
        }
    }
}


ANSC_STATUS
CosaDmlTimeInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    int rc = 0;
    int iEnbl = 0;
    char val[UTOPIA_TR181_PARAM_SIZE1];

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
    int err = 0;
    pthread_t ntp_thread;

    if (!pTimeCfg)
        return ANSC_STATUS_FAILURE;

    err = pthread_create(&ntp_thread, NULL, startNTP, (void *)pTimeCfg);

    if(0 != err)
    {
        CcspTraceError(("%s: create the ntp syn thread error!\n", __FUNCTION__));
    }
    else
        pthread_detach(ntp_thread);

//    startNTP(pTimeCfg);

    if (pTimeCfg)
    {
       /* Initialize a Utopia Context */
       if(!Utopia_Init(&ctx))
          return ERR_UTCTX_INIT;
       /* Set Local TZ to SysCfg */
       rc = Utopia_Set_DeviceTime_LocalTZ(&ctx,&(pTimeCfg->LocalTimeZone));

       /*set city index */
       sprintf(buf,"%d",pTimeCfg->cityIndex);
       rc = Utopia_RawSet(&ctx, NULL, "ntp_cityindex", buf);

       /*Set NTP Server 1 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx,&(pTimeCfg->NTPServer1),1);

       /*Set NTP Server 2 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx,&(pTimeCfg->NTPServer2),2);

       /*Set NTP Server 3 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx,&(pTimeCfg->NTPServer3),3);
       
       /*Set NTP DaylightSaving Enabled or not to SysCfg */
       rc = Utopia_Set_DeviceTime_DaylightEnable(&ctx,pTimeCfg->bDaylightSaving);

       /*Set NTP DaylightSaving Offset to SysCfg */
       rc = Utopia_Set_DeviceTime_DaylightOffset(&ctx,pTimeCfg->DaylightSavingOffset);

       /*Set NTP Enabled or not to SysCfg */
       rc = Utopia_Set_DeviceTime_Enable(&ctx,pTimeCfg->bEnabled);

       /* Free Utopia Context */
       Utopia_Free(&ctx,!rc);
     }
     if (rc != 0)
       return ERR_SYSCFG_FAILED;
     else
       return ANSC_STATUS_SUCCESS;
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
    int iEnbl = 0;
    char val[UTOPIA_TR181_PARAM_SIZE1];

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
          AnscCopyString(pTimeCfg->NTPServer1,val);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill NTP Server 2 from Syscfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,2)) == UT_SUCCESS)
       {
          AnscCopyString(pTimeCfg->NTPServer2,val);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill NTP Server 3 from Syscfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,3)) == UT_SUCCESS)
       {
          AnscCopyString(pTimeCfg->NTPServer3,val);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill DaylightSaving Enabled or not from syscfg */
       pTimeCfg->bDaylightSaving = Utopia_Get_DeviceTime_DaylightEnable(&ctx);

       /* Fill DaylightSaving Offset from syscfg */
       rc = Utopia_Get_DeviceTime_DaylightOffset(&ctx, &(pTimeCfg->DaylightSavingOffset));

       /* Fill NTP Enabled or not from syscfg */
       pTimeCfg->bEnabled = Utopia_Get_DeviceTime_Enable(&ctx);

       /* Free Utopia Context */
       Utopia_Free(&ctx,0);
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
    int rc = -1;

    /* Initialize a Utopia Context */
    if(!Utopia_Init(&ctx))
        return ERR_UTCTX_INIT;

    *pStatus = Utopia_Get_DeviceTime_Status(&ctx);

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeGetLocalTime
    (
       ANSC_HANDLE                 hContext,
       char                       *pCurrLocalTime
    )
{
    time_t t;
    struct tm *pLcltime;
    t = time(NULL);
    pLcltime = localtime(&t);
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
    platform_hal_getTimeOffSet(pTimeOffset);
    return ANSC_STATUS_SUCCESS;
}





#endif
