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

#elif (_COSA_DRG_CNS_ || _COSA_DRG_TPG_)

#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#define UTOPIA_TR181_PARAM_SIZE1   256

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
    UtopiaContext ctx;
    int rc = 0;

    if (pTimeCfg)
    {
       /* Initialize a Utopia Context */
       if(!Utopia_Init(&ctx))
          return ERR_UTCTX_INIT;
       /* Set Local TZ to SysCfg */
       rc = Utopia_Set_DeviceTime_LocalTZ(&ctx,&(pTimeCfg->LocalTimeZone));

       /*Set NTP Server 1 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx,&(pTimeCfg->NTPServer1),1);

       /*Set NTP Server 2 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx,&(pTimeCfg->NTPServer2),2);

       /*Set NTP Server 3 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx,&(pTimeCfg->NTPServer3),3);

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
    _ansc_sprintf(pCurrLocalTime, "%.4u-%.2u-%.2uT%.2u:%.2u:%.2u",
            (pLcltime->tm_year)+1900,
            (pLcltime->tm_mon)+1,
            pLcltime->tm_mday,
            pLcltime->tm_hour,
            pLcltime->tm_min,
            pLcltime->tm_sec);

    return ANSC_STATUS_SUCCESS;
}

#endif
