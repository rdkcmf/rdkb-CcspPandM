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

    module: cosa_routing_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

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
#define _GNU_SOURCE
#include <string.h>
#include "cosa_apis.h"
#include "cosa_routing_dml.h"
#include "cosa_routing_apis.h"
#include "cosa_routing_internal.h"
#include "dml_tr181_custom_cfg.h"
#include "secure_wrapper.h"

#if defined (_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_) || defined (_CBR2_PRODUCT_REQ_)
#include "cosa_drg_common.h"
#endif

#include "safec_lib_common.h"

extern void* g_pDslhDmlAgent;

#if ( defined(_COSA_SIM_))

COSA_PRI_ROUTER_FULL g_RouterFull =
{ 
    {1, "Router1", TRUE},
    {COSA_DML_ROUTING_STATUS_Enabled},
    2, 
    {{1, "IPv4Forwarding1", TRUE, COSA_DML_ROUTING_STATUS_Enabled, FALSE,
      {"\x40\x40\x40\x06"}, {"\xFF\xFF\xFF\x00"}, 0, {"\x40\x40\x40\x01"}, "wan6", 1},
     {2, "IPv4Forwarding2", TRUE, COSA_DML_ROUTING_STATUS_Enabled, FALSE, 
      {"\x40\x40\x40\x07"}, {"\xFF\xFF\xFF\x00"}, 0, {"\x40\x40\x40\x01"}, "wan7", 1}},
    2,
    {{0, "", TRUE, COSA_DML_ROUTING_STATUS_Enabled, "111", 1, "111", "111", 
      COSA_DML_ROUTING_IPV6_ORIGIN_Static, 1, "111"},
     {0, "", TRUE, COSA_DML_ROUTING_STATUS_Enabled, "222", 1, "111", "222", 
      COSA_DML_ROUTING_IPV6_ORIGIN_Static, 1, "222"}}
};

COSA_DML_RIP_CFG g_RipCfg = {TRUE,  COSA_DML_RIP_MODE_Both};


COSA_DML_RIP_IF_CFG g_RipIfFull[10] =
{
    {1, 2, "InterfaceSetting1", "wan8", TRUE, TRUE, TRUE, 3, 1, 0x10101010, TRUE, 1, 3, "text1"},
    {2, 2, "InterfaceSetting2", "wan9", TRUE, TRUE, TRUE, 2, 1, 0x10101012, TRUE, 1, 3, "text2"}
};

#if 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

/*This is test part  Begin */

#define ULONG unsigned long
#define BOOL  char
#define CHAR  char
#define UCHAR unsigned char
#define PCHAR char *
#define PUCHAR unsigned char *
#define INT   int
#define _ansc_strlen   strlen
#define _ansc_strncpy  strncpy
#define _ansc_sprintf  sprintf
#define _ansc_snprintf snprintf
#define TRUE           1
#define FALSE          0
//static  USER_LOCK                   gPsmLock;

#define CONFIGFILENAMEYAN "./config_tmp_utopia.txt"

typedef struct _UtopiaContext
{
    ULONG c;
}UtopiaContext;


struct _COUPLE_LINK
{
    char * name;
    char * value;
    struct _COUPLE_LINK * next;
};

typedef struct _COUPLE_LINK COUPLE_LINK, *PCOUPLE_LINK;

static PCOUPLE_LINK pHead = NULL;
static ULONG       lastTime  = 0;

static int Utopia_Init(UtopiaContext *ctx)
{
    return 1;
}

static int utopia_init()
{
    FILE    *fileHandle  = NULL;
    char    oneLine[256] = {0};
    char    *pOneLine    = NULL;
    char    *pOneLine2   = NULL;
    PCOUPLE_LINK  pLink   = NULL;
    PCOUPLE_LINK  pLink2  = NULL;
    static   BOOL    bInitiated = false;
    INT     count        = 0;

    if ( !bInitiated )
    {
        //UserInitializeLock(&gPsmLock);
        bInitiated = true;
    }

    //This is just test.
    
    /*
    if ( pHead )
        return 0;
        */
        
    //UserAcquireLock(&gPsmLock);

    //open file 
    fileHandle = fopen(CONFIGFILENAMEYAN, "r+" );
    if ( !fileHandle )
    {
        //UserReleaseLock(&gPsmLock);
        return 0;
    }


    //get file
    pLink2 = pHead;    
    while ( (pOneLine = fgets(oneLine, sizeof(oneLine), fileHandle )) )
    {
        pLink = (PCOUPLE_LINK)malloc( sizeof(COUPLE_LINK) );
        pLink->next = NULL;
        if ( !pHead )
        {
            pHead = pLink;
            pLink2 = pHead;
        }
        else
        {
            pLink2->next = pLink;
            pLink2 = pLink;
        }

        // skip non-space in one new line 
        while ( *pOneLine != ' ' ) pOneLine++;
        pOneLine++;
        pOneLine2 = pOneLine;

        //name 
        while ( *pOneLine != ' ' ) pOneLine++;
        *pOneLine = 0;

        pLink->name = strdup(pOneLine2);

        pOneLine++;
        
        pOneLine2 = pOneLine;

        // value 
        while ( *pOneLine != ' ' ) pOneLine++;
        *pOneLine = 0;
        
        pLink->value = strdup(pOneLine2);

    }

    pLink = pHead;
    while( pLink )
    {
        count++;
        printf("%3d %s %s  \n", count, pLink->name, pLink->value );
        pLink = pLink->next;
    }

    fclose(fileHandle);

    //UserReleaseLock(&gPsmLock);

    return 0;
}

ULONG
UserGetTickInSeconds()
{
    struct timeval     tv = {0};

    gettimeofday(&tv, NULL);

    return tv.tv_sec;
}

static int utopia_save()
{
    FILE    *fileHandle  = NULL;
    char    oneLine[256] = {0};
    PCOUPLE_LINK  pLink   = NULL;
    errno_t safec_rc = -1;

    if ( ( UserGetTickInSeconds() - lastTime ) < 0 )
    {
        return 0;
    }
    else
    {
        lastTime = UserGetTickInSeconds();
    }

    if ( !pHead )
    {
        //UserReleaseLock(&gPsmLock);
        return 1;
    }
    
    //UserAcquireLock(&gPsmLock);

    //open file 
    fileHandle = fopen(CONFIGFILENAMEYAN, "w+" );
    if ( !fileHandle )
        return 0;

    pLink = pHead;
    while ( pLink )
    {
        safec_rc = sprintf_s(oneLine, sizeof(oneLine), " %s %s \n", pLink->name, pLink->value);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        fputs( oneLine, fileHandle);
        pLink = pLink->next;
    }

    fclose(fileHandle);

    //UserReleaseLock(&gPsmLock);

    return 0;
}

static int Utopia_RawSet
(
    UtopiaContext*              ctx,
    char*                       domainname,
    char*                       pRecordName,
    char*                       pValue
)
{
    char    fullname[512] = {0};
    char    *pOneLine    = NULL;
    char    *pOneLine2   = NULL;
    PCOUPLE_LINK  pLink   = NULL;
    PCOUPLE_LINK  pLink2  = NULL;
    errno_t safec_rc = -1;
    
    if ( !pHead )
        utopia_init();

    //UserAcquireLock(&gPsmLock);

    if ( domainname == NULL )
    {
        safec_rc = strcpy_s(fullname, sizeof(fullname), pRecordName);
        if(safec_rc != EOK)
        {
           ERR_CHK(safec_rc);
        }
    }
    else
    {
        safec_rc = sprintf_s(fullname, sizeof(fullname), "%s%s", domainname, pRecordName);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
    }

    pLink = pHead;
    while( pLink )
    {
        if ( !strcmp( fullname, pLink->name ) )
        {
            break;
        }
        pLink2 = pLink;
        pLink = pLink->next;
    }

    if ( !pLink )
    {
        pLink = (PCOUPLE_LINK)malloc( sizeof(COUPLE_LINK) );
        pLink->next = NULL;

        pLink->name = strdup(fullname);
        
        if ( !pHead )
        {
            pHead = pLink;
        }
        else
        {
            pLink2->next = pLink;
        }
    }
    
    pLink->value = strdup(pValue);

    //UserReleaseLock(&gPsmLock);

    utopia_save();
    
    return 0;
    
}


static int syscfg_unset
(
    char*                       domainname,
    char*                       pRecordName
)
{
    char    oneLine[256] = {0};
    char    fullname[512] = {0};
    char    *pOneLine    = NULL;
    char    *pOneLine2   = NULL;
    PCOUPLE_LINK  pLink   = NULL;
    PCOUPLE_LINK  pLink2  = NULL;
    
    if ( !pHead )
        utopia_init();

    //UserAcquireLock(&gPsmLock);

    _ansc_sprintf(fullname, "%s%s", domainname, pRecordName);

    pLink = pHead;
    pLink2 = pLink;
    while( pLink )
    {
        if ( !strcmp( fullname, pLink->name ) )
        {
            break;
        }

        pLink2 = pLink;
        pLink = pLink->next;
    }


    if ( !pLink )
    {
        //UserReleaseLock(&gPsmLock);

        return 1;
    }
    else
    {
        if ( pLink2 == pLink )
        {
            pHead = pHead->next;
        }
        else
        {
            pLink2->next = pLink->next;
        }

        free( pLink->name);
        free( pLink->value);
        free( pLink);
        
    }
    //UserReleaseLock(&gPsmLock);

    utopia_save();

    return 0;

}


static int Utopia_RawGet
(
    UtopiaContext*              ctx,
    char*                       domainname,
    char*                       pRecordName,
    char*                       pValue,
    ULONG                       size
)
{
    char    oneLine[256] = {0};
    char    fullname[512] = {0};
    char    *pOneLine    = NULL;
    char    *pOneLine2   = NULL;
    PCOUPLE_LINK  pLink   = NULL;
    PCOUPLE_LINK  pLink2  = NULL;

    if ( !pHead )
        utopia_init();

    //UserAcquireLock(&gPsmLock);

    safec_rc = sprintf_s(fullname, sizeof(fullname), "%s%s", domainname, pRecordName);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }

    pLink = pHead;
    while( pLink )
    {
        if ( !strcmp( fullname, pLink->name ) )
        {
            break;
        }

        pLink = pLink->next;

    }

    //UserReleaseLock(&gPsmLock);

    if ( pLink  )
    {
        _ansc_snprintf(pValue, size, "%s", pLink->value);
        return 1;
    }
    else
        return 0;

}

static void Utopia_Free(UtopiaContext * ctx, ULONG commit)
{
    return;
}

#endif

#elif ( defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_MIPS_))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <net/route.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "syscfg/syscfg.h"

#endif /*_COSA_ARM_*/


/* The following code is used for SIM and other system */


/*This section is used to do integration for Quagga */
/* ripd.conf
!
hostname ripd_simu
password yan
log stdout
!
 debug rip events
 debug rip packet
!
key chain yan2
 key 2
  key-string cisco
!
interface eth0
 no ip rip split-horizon
 ip rip send version 1 2
 ip rip receive version 1 2
! ip rip authentication mode text
! ip rip authentication string cisco 
 ip rip authentication mode md5 auth-length rfc
 ip rip authentication key-chain yan2
! ip rip authentication mode text
! ip rip authentication string cisco
!
router rip
 version 2
 timers basic 10 180 120
 redistribute connected metric 3
 network eth0
!if:eth0
 neighbor 86.96.241.17
 default-metric 4
 distribute-list 1 out eth0
 distribute-list 1 in eth0
!
access-list 1 deny any
!
line vty
 exec-timeout 0 0
*/

ULONG g_NumOfRipIFs = 1;

COSA_DML_RIPD_CONF CosaDmlRIPDefaultConfig = 
{
    /* global */
    FALSE,
    COSA_RIP_VERSION_2,
    5,
    180,
    120,
    1,
    1,

    /*If1*/
    TRUE,
    FALSE,
    TRUE,
    "Ethernet",
    COSA_RIPD_IF1_NAME,
    "comcast",
    "comcast",
    "keychain1",
    COSA_RIP_AUTHEN_NONE, /*COSA_RIP_AUTHEN_TEXT,*/
    1,
    COSA_RIP_VERSION_2,
    COSA_RIP_VERSION_2,
    0x0,
    
};

COSA_DML_RIPD_CONF CosaDmlRIPCurrentConfig = {0};


void _get_shell_output3(FILE *fp, char *buf, int len)
{
    char * p;

    if (fp)
    {
        if(fgets (buf, len-1, fp) != NULL)
        {
            buf[len-1] = '\0';
            if ((p = strchr(buf, '\n'))) {
                *p = '\0';
            }
        }
    v_secure_pclose(fp);
    }
}

#define RIPD_PID_FILE "/var/tmp/ripd.pid"
static int CosaRipdOperation(char * arg)
{
    char *base = basename(COSA_RIPD_BIN);
    
    if (!strncmp(arg, "stop", 4))
    {
        /* Zebra's configuration is controlled by Utopia. So we need not restart this one */
        v_secure_system("killall %s", base);
    }
    else if (!strncmp(arg, "start", 5))
    {
        if ( CosaDmlRIPCurrentConfig.Enable && (CosaDmlRIPCurrentConfig.If1ReceiveEnable || CosaDmlRIPCurrentConfig.If1SendEnable ) )
        {
            v_secure_system(COSA_RIPD_BIN " -d -f " COSA_RIPD_CUR_CONF " -u root -g root -i " RIPD_PID_FILE " &");
        }
    }
    else if (!strncmp(arg, "restart", 7))
    {
        CosaRipdOperation("stop");
        CosaRipdOperation("start");
    }
    else if (!strncmp(arg, "update", 6))
    {
        v_secure_system("killall -SIGHUP %s", base);
    }

    return 0;
}


/* set a ulong type value into utopia */
void CosaDmlSetRipd_into_utopia( PCHAR uniqueName, PCHAR table1Name, ULONG table1Index, PCOSA_DML_RIPD_CONF pEntry )
{
    UtopiaContext utctx = {0};
    CHAR  Value1[12]  = {0};

    if (!Utopia_Init(&utctx))
        return;
    
    /*Global parameters */
    /*SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "Enable", pEntry->Enable)*/
    _ansc_snprintf(&Value1[0], sizeof(Value1)-1, "%d", pEntry->Enable );
    Utopia_RawSet(&utctx, NULL,   "rip_enabled", Value1);
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "Version", pEntry->Version)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "UpdateTime", pEntry->UpdateTime)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "TimoutTime", pEntry->TimoutTime)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "CollectionTime", pEntry->CollectionTime)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "ConnectedMetric", pEntry->ConnectedMetric)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "DefaultMetric", pEntry->DefaultMetric)

    /*Interface 1*/
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1Enable", pEntry->If1Enable)
    SETS_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1Name", pEntry->If1Name)
    SETS_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1Alias", pEntry->If1Alias)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1AuthenticateType", pEntry->If1AuthenticateType)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1KeyID", pEntry->If1KeyID)
    SETS_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1Md5KeyValue", pEntry->If1Md5KeyValue)
    SETS_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1SimplePassword", pEntry->If1SimplePassword)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1SendVersion", pEntry->If1SendVersion)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1ReceiveVersion", pEntry->If1ReceiveVersion)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1SendEnable", pEntry->If1SendEnable)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1ReceiveEnable", pEntry->If1ReceiveEnable)
    SETI_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1Neighbor", pEntry->If1Neighbor)
    SETS_INTO_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1KeyChainName", pEntry->If1KeyChainName)


    Utopia_Free(&utctx,1);

    return;
}

void CosaDmlGetRipd_from_utopia( PCHAR uniqueName, PCHAR table1Name, ULONG table1Index, PCOSA_DML_RIPD_CONF pEntry )
{
    UtopiaContext utctx = {0};
    CHAR  Value1[12]  = {0};

    if (!Utopia_Init(&utctx))
        return;
    
    /*Global parameters */
    /*GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "Enable", pEntry->Enable)*/
    Utopia_RawGet(&utctx, NULL,  "rip_enabled", Value1, sizeof(Value1));
    pEntry->Enable = (Value1[0]!='0')?TRUE:FALSE;
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "Version", pEntry->Version)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "UpdateTime", pEntry->UpdateTime)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "TimoutTime", pEntry->TimoutTime)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "CollectionTime", pEntry->CollectionTime)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "ConnectedMetric", pEntry->ConnectedMetric)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "DefaultMetric", pEntry->DefaultMetric)
    
    /*Interface 1*/
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1Enable", pEntry->If1Enable)
    GETS_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1Name", pEntry->If1Name)
    GETS_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1Alias", pEntry->If1Alias)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1AuthenticateType", pEntry->If1AuthenticateType)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1KeyID", pEntry->If1KeyID)
    GETS_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1Md5KeyValue", pEntry->If1Md5KeyValue)
    GETS_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1SimplePassword", pEntry->If1SimplePassword)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1SendVersion", pEntry->If1SendVersion)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1ReceiveVersion", pEntry->If1ReceiveVersion)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1SendEnable", pEntry->If1SendEnable)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1ReceiveEnable", pEntry->If1ReceiveEnable)
    GETI_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1Neighbor", pEntry->If1Neighbor)
    GETS_FROM_UTOPIA(uniqueName, table1Name, table1Index, "", 0, "If1KeyChainName", pEntry->If1KeyChainName)

    Utopia_Free(&utctx,0);

    return;
}

/* Get configuration from Utapi system */
void CosaDmlGetRipdConfiguration()
{
    UtopiaContext utctx        = {0};
    CHAR  Namespace[128]      = {0};
    CHAR  Out[12]             = {0};
    ULONG  ret                 = 0;
    PCOSA_DML_RIPD_CONF pEntry = &CosaDmlRIPCurrentConfig;

    /* Try getting a parameter to judge */
    if (!Utopia_Init(&utctx))
        return;
    
    _ansc_snprintf(Namespace, sizeof(Namespace)-1, "%s%s%lu%s%lu", "cosa_usgv2_", "rip", (ULONG)0, "", (ULONG)0 );
    ret = Utopia_RawGet(&utctx, Namespace, "Version", Out,sizeof(Out));

    Utopia_Free(&utctx,0);

    if ( ret == FALSE )
    {
        /* There is not parameters in utapi config system */
        *pEntry = CosaDmlRIPDefaultConfig;
        CosaDmlSetRipd_into_utopia("cosa_usgv2_", "rip", 0, pEntry);
    }
    else
    {
        CosaDmlGetRipd_from_utopia("cosa_usgv2_", "rip", 0, pEntry);
    }

    return;

}

/* Save configuration into Utapi system */
void CosaDmlSaveRipdConfiguration()
{
    PCOSA_DML_RIPD_CONF pEntry = &CosaDmlRIPCurrentConfig;

    CosaDmlSetRipd_into_utopia("cosa_usgv2_", "rip", 0, pEntry);
    return;
}

ULONG CosaDmlGetBitsNumFromNetMask(char * Address)
{
    struct in_addr addr = {0};
    ULONG ulAddr = 0;
    ULONG bits = 0;
 #if defined (_CBR_PRODUCT_REQ_)
    ULONG mul = 0;
 #endif   
    if ( inet_aton(Address, &addr) ){
        ulAddr = addr.s_addr;
#if defined (_CBR_PRODUCT_REQ_)
        if((ulAddr > 0xffffff) && (ulAddr <= 0xffffffff))
        {
           mul = 1;
        }
        else if((ulAddr >= 0xffff) && (ulAddr <= 0xffffff))
        {
           ulAddr = ulAddr*0x100;
            mul = 2;
        }
        else if((ulAddr >= 0xff) && (ulAddr < 0xffff))
        {
           ulAddr = ulAddr*0x10000;
           mul = 3;
        }
        else
        {
           ulAddr = ulAddr*0x1000000;
           mul = 4;
        }
#endif
        while( ulAddr & 0x80000000 ){
            bits++;
            ulAddr = ulAddr<<1;
        }
    }
#if defined (_CBR_PRODUCT_REQ_)
    bits = 32 - (8*mul - bits);
#endif
    return bits;
}
/* Generate configuration file */
void CosaDmlGenerateRipdConfigFile(ANSC_HANDLE  hContext )
{
    PCOSA_DML_RIPD_CONF pConf = &CosaDmlRIPCurrentConfig;
    #if defined (_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_) || defined (_CBR2_PRODUCT_REQ_)
    FILE * pFile              = NULL;
    #endif
    FILE * fp                 = fopen(COSA_RIPD_CUR_CONF, "w+");
    char *pstaticRoute        = NULL;
    BOOL bTrueStaticIP        = TRUE;
    errno_t safec_rc = -1;

    AnscTraceWarning(("CosaDmlGenerateRipdConfigFile -- starts.\n"));

    bTrueStaticIP  = g_GetParamValueBool(g_pDslhDmlAgent, "Device.X_CISCO_COM_TrueStaticIP.Enable");
    #if defined (_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_) || defined (_CBR2_PRODUCT_REQ_)
	commonSyseventSet("ripd_conf-status","empty");
    #endif
    if (fp)
    {
        /*we need this to get IANA IAPD info from dibbler*/
        fprintf(fp, "!\n");
        fprintf(fp, "!log stdout\n");
        fprintf(fp, "!\n");
        fprintf(fp, "! debug rip events\n");
        fprintf(fp, "! debug rip packet\n");
        fprintf(fp, "!\n");
        
        if (( _ansc_strlen(pConf->If1KeyChainName) > 0 ) && ( _ansc_strlen(pConf->If1Md5KeyValue) > 0 ))
        {
            fprintf(fp, "key chain %s\n", pConf->If1KeyChainName);
            fprintf(fp, " key %lu\n", pConf->If1KeyID);
            fprintf(fp, "  key-string %s\n", pConf->If1Md5KeyValue);
	}
        #if defined (_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_) || defined (_CBR2_PRODUCT_REQ_)	
	if (( pConf->If1Md5KeyValue == NULL )  || (_ansc_strlen(pConf->If1Md5KeyValue) == 0 )){
            AnscTraceWarning(("Static IP: MD5 Key Values are Empty"));
	}
	#endif
        fprintf(fp, "!\n");

        if ( pConf->Enable && pConf->If1Enable && (_ansc_strlen(pConf->If1Name) > 0 ) )
        {
            fprintf(fp, "interface %s\n", pConf->If1Name);
            fprintf(fp, " ip rip send version %s\n", (pConf->If1SendVersion==COSA_RIP_VERSION_1)?"1":((pConf->If1SendVersion==COSA_RIP_VERSION_2)?"2":"1 2") );
            fprintf(fp, " ip rip receive version %s\n", (pConf->If1ReceiveVersion==COSA_RIP_VERSION_1)?"1":((pConf->If1ReceiveVersion==COSA_RIP_VERSION_2)?"2":"1 2") );
            {
                if ( pConf->If1AuthenticateType == COSA_RIP_AUTHEN_MD5 )
                {
                    fprintf(fp, " ip rip authentication mode md5 auth-length rfc\n");
                    fprintf(fp, " ip rip authentication key-chain %s\n",pConf->If1KeyChainName);
                }else if ( pConf->If1AuthenticateType == COSA_RIP_AUTHEN_TEXT )
                {
                    fprintf(fp, " ip rip authentication mode text\n");
                    fprintf(fp, " ip rip authentication string %s\n",pConf->If1SimplePassword);
                }
	#if defined (_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_) || defined (_CBR2_PRODUCT_REQ_)
		else {
		    AnscTraceWarning(("Static IP: Unknown RIP Authentication Mode\n"));
		}
	#endif
            }
            fprintf(fp, " ip rip split-horizon\n");
            fprintf(fp, "!\n");
        }

        //we put brlan0 in  configuretion for ever following If1's configuration
        if ( bTrueStaticIP && pConf->Enable && pConf->If1Enable){
            fprintf(fp, "interface %s\n", "brlan0");
            fprintf(fp, " ip rip send version %s\n", (pConf->If1SendVersion==COSA_RIP_VERSION_1)?"1":((pConf->If1SendVersion==COSA_RIP_VERSION_2)?"2":"1 2") );
            fprintf(fp, " ip rip receive version %s\n", (pConf->If1ReceiveVersion==COSA_RIP_VERSION_1)?"1":((pConf->If1ReceiveVersion==COSA_RIP_VERSION_2)?"2":"1 2") );
            {
                if ( pConf->If1AuthenticateType == COSA_RIP_AUTHEN_MD5 )
                {
                    fprintf(fp, " ip rip authentication mode md5 auth-length rfc\n");
                    fprintf(fp, " ip rip authentication key-chain %s\n",pConf->If1KeyChainName);
                }else if ( pConf->If1AuthenticateType == COSA_RIP_AUTHEN_TEXT )
                {
                    fprintf(fp, " ip rip authentication mode text\n");
                    fprintf(fp, " ip rip authentication string %s\n",pConf->If1SimplePassword);
                }
	#if defined (_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_) || defined (_CBR2_PRODUCT_REQ_)
		else {
		    AnscTraceWarning(("Static IP: Unknown RIP Authentication Mode\n"));
		}
	#endif
            }
            fprintf(fp, " ip rip split-horizon\n"); 
            fprintf(fp, "!\n");
        }

        fprintf(fp, "router rip\n");
        /* CID: 58193 Identical code for different branches*/
        fprintf(fp, " version %s\n", (pConf->Version==COSA_RIP_VERSION_1)?"1":((pConf->Version==COSA_RIP_VERSION_2)?"2":"1 2") );
        fprintf(fp, " timers basic %lu %lu %lu\n", pConf->UpdateTime, pConf->TimoutTime, pConf->CollectionTime);
        fprintf(fp, " default-metric %lu\n", pConf->DefaultMetric );
        fprintf(fp, " no redistribute connected\n");

        //this is for true static feature
        if ( bTrueStaticIP ){
            fprintf(fp, " passive-interface brlan0\n");
        }
        
        if ( pConf->Enable && pConf->If1Enable && (_ansc_strlen(pConf->If1Name) > 0 ))
        {
            fprintf(fp, " network %s\n", pConf->If1Name);
        }
        
        //this is for true static ip subnet
        if ( bTrueStaticIP ){
            char Address[32] = {0};
            char SubnetMask[32] = {0};
            ULONG bits = 0;
            ULONG subnetCount = 0;
            ULONG i = 0;
            ULONG len = 0;
            ULONG instance = 0;
            char buf1[128] = {0};
            char buf2[128] = {0};
            ANSC_STATUS ret = ANSC_STATUS_SUCCESS;

            //this part need to change to use psm interface
            do{
                len = sizeof(Address);
                ret = g_GetParamValueString(g_pDslhDmlAgent, "Device.X_CISCO_COM_TrueStaticIP.IPAddress", Address, &len );
                if ( ret != ANSC_STATUS_SUCCESS )
                    break;
                
                len = sizeof(SubnetMask);
                ret = g_GetParamValueString(g_pDslhDmlAgent, "Device.X_CISCO_COM_TrueStaticIP.SubnetMask", SubnetMask, &len );
                if ( ret != ANSC_STATUS_SUCCESS )
                    break;

                bits = CosaDmlGetBitsNumFromNetMask(SubnetMask);
                
                if ( Address[0] && (bits>0) )
                    fprintf(fp, " network %s/%lu\n", Address, bits);
                
                subnetCount = g_GetParamValueUlong(g_pDslhDmlAgent, "Device.X_CISCO_COM_TrueStaticIP.AdditionalSubnetNumberOfEntries");

                for (i=0; i<subnetCount; i++)
                {
                    instance = g_GetInstanceNumberByIndex(g_pDslhDmlAgent, "Device.X_CISCO_COM_TrueStaticIP.AdditionalSubnet.", i);
                    
                    if ( instance )
                    {
                        safec_rc = sprintf_s(buf1, sizeof(buf1), "Device.X_CISCO_COM_TrueStaticIP.AdditionalSubnet.%lu.IPAddress", instance);
                        if(safec_rc < EOK)
                        {
                           ERR_CHK(safec_rc);
                        }
                        safec_rc = sprintf_s(buf2, sizeof(buf2), "Device.X_CISCO_COM_TrueStaticIP.AdditionalSubnet.%lu.SubnetMask", instance);
                        if(safec_rc < EOK)
                        {
                           ERR_CHK(safec_rc);
                        }

                        len = sizeof(Address);
                        ret = g_GetParamValueString(g_pDslhDmlAgent, buf1, Address, &len );
                        if ( ret != ANSC_STATUS_SUCCESS )
                            break;

                        len = sizeof(SubnetMask);
                        ret = g_GetParamValueString(g_pDslhDmlAgent, buf2, SubnetMask, &len );
                        if ( ret != ANSC_STATUS_SUCCESS )
                            break;
                        
                        bits = CosaDmlGetBitsNumFromNetMask(SubnetMask);

                        fprintf(fp, " network %s/%lu\n", Address, bits);
                    }
                }

            }while(0);
            
        }

        if ( pConf->Enable && pConf->If1Enable && (  pConf->If1Neighbor > 0 ) )
        {
            fprintf(fp, " neighbor %s\n", inet_ntoa(*((struct in_addr *)&(pConf->If1Neighbor))));
        }

        /*this part is for distribute static route beginning */
        pstaticRoute = CosaTimeGetRipdConfStaticPart(hContext);
        if ( pstaticRoute )
        {   
            fprintf(fp, "%s", pstaticRoute);
            AnscFreeMemory(pstaticRoute);
        }

        if ( pConf->Enable && pConf->If1Enable && !pConf->If1SendEnable && (_ansc_strlen(pConf->If1Name) > 0) )
            fprintf(fp, " distribute-list 1 out %s\n", pConf->If1Name );
        if ( pConf->Enable && pConf->If1Enable && !pConf->If1ReceiveEnable && (_ansc_strlen(pConf->If1Name) > 0) )
            fprintf(fp, " distribute-list 1 in %s\n", pConf->If1Name );

        if ( bTrueStaticIP )
            fprintf(fp, " distribute-list 2 in %s\n", "brlan0" );

        fprintf(fp, "!\n");
        fprintf(fp, "access-list 1 deny any\n");

        //this is for true static ip feature        
        if ( bTrueStaticIP ){
            char buf1[128] = {0};
            char buf2[128] = {0};
            char Address[32] = {0};
            char Netmask[32] = {0};
            ULONG len = 0;

            len = sizeof(buf1);
            if (g_GetParamValueString(g_pDslhDmlAgent, "com.cisco.spvtg.ccsp.pam.Helper.FirstDownstreamIpInterface", buf1, &len ) == ANSC_STATUS_SUCCESS){
                safec_rc = strcpy_s(buf2, sizeof(buf2), buf1);
                if(safec_rc != EOK)
                {
                   ERR_CHK(safec_rc);
                }
                _ansc_strcat(buf1, "IPv4Address.1.IPAddress");
                _ansc_strcat(buf2, "IPv4Address.1.SubnetMask");   
                
                len = sizeof(Address);
                g_GetParamValueString(g_pDslhDmlAgent, buf1, Address, &len );
                len = sizeof(Netmask);
                g_GetParamValueString(g_pDslhDmlAgent, buf2, Netmask, &len );

                fprintf(fp, "access-list 2 deny %s %s\n", Address, Netmask);
            }

        }
        
        fprintf(fp, "!\n");
        fprintf(fp, " line vty\n");
        fprintf(fp, " exec-timeout 0 0\n");

        fclose(fp);

    }
	#if defined (_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_) || defined (_CBR2_PRODUCT_REQ_)
	pFile=fopen("/tmp/pam_ripd_config_completed","a+");
	    if(pFile)
	    {
	        fclose(pFile);
	    }
	commonSyseventSet("ripd_conf-status","ready");
	#endif
    AnscTraceWarning(("CosaDmlGenerateRipdConfigFile -- exits.\n"));

    return;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipGetCfg
            (
                ANSC_HANDLE                    hContext,
                PCOSA_DML_RIP_CFG           pCfg
            );


    Description:
    
          This function get the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                            hContext,
                
                PCOSA_DML_RIP_CFG                   pCfg

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_CFG           pCfg
    )

{
    UNREFERENCED_PARAMETER(hContext);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    pCfg->Enable = CosaDmlRIPCurrentConfig.Enable;
    pCfg->Mode   = COSA_DML_RIP_MODE_Both;
    pCfg->X_CISCO_COM_UpdateInterval = CosaDmlRIPCurrentConfig.UpdateTime;
    pCfg->X_CISCO_COM_DefaultMetric  = CosaDmlRIPCurrentConfig.DefaultMetric;

#if 1
    AnscTraceWarning(("pCfg->Enable :%s\n", pCfg->Enable?"TRUE":"FALSE"));    
    AnscTraceWarning(("pCfg->Mode :%u\n", pCfg->Mode));
    AnscTraceWarning(("pCfg->X_CISCO_COM_UpdateInterval :%lu\n", pCfg->X_CISCO_COM_UpdateInterval));
    AnscTraceWarning(("pCfg->X_CISCO_COM_DefaultMetric :%lu\n", pCfg->X_CISCO_COM_DefaultMetric));
#endif

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipSetCfg
            (
                ANSC_HANDLE                    hContext,
                PCOSA_DML_RIP_CFG           pCfg
            );


    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                            hContext,
                
                PCOSA_DML_RIP_CFG                   pCfg

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_CFG           pCfg
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hContext);
    AnscTraceWarning(("CosaDmlRipSetCfg -- starts.\n"));

    CosaDmlRIPCurrentConfig.Enable        = pCfg->Enable;
    CosaDmlRIPCurrentConfig.UpdateTime    = pCfg->X_CISCO_COM_UpdateInterval;
    CosaDmlRIPCurrentConfig.DefaultMetric = pCfg->X_CISCO_COM_DefaultMetric;

#if 1
    AnscTraceWarning(("CosaDmlRIPCurrentConfig.Enable :%s\n", CosaDmlRIPCurrentConfig.Enable?"TRUE":"FALSE"));    
    AnscTraceWarning(("CosaDmlRIPCurrentConfig.X_CISCO_COM_UpdateInterval :%lu\n", CosaDmlRIPCurrentConfig.UpdateTime));
    AnscTraceWarning(("CosaDmlRIPCurrentConfig.DefaultMetric :%lu\n", CosaDmlRIPCurrentConfig.DefaultMetric));
#endif

#if defined (_BWG_PRODUCT_REQ_)
    if(!pCfg->Enable)
    {
        CosaRipdOperation("stop");
        CosaDmlSaveRipdConfiguration();
    }
    else
    {
        CosaDmlSaveRipdConfiguration();
        CosaDmlGenerateRipdConfigFile(NULL);
        CosaRipdOperation("restart");
    }
#else
        CosaDmlSaveRipdConfiguration();
        CosaDmlGenerateRipdConfigFile(NULL);
        CosaRipdOperation("restart");
#endif

    AnscTraceWarning(("CosaDmlRipSetCfg -- exits.\n"));
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipGetNumberOfIfEntries
            (
                ANSC_HANDLE                    hContext
            );


    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                            hContext,
                

    Return:
        

**********************************************************************/
ULONG
CosaDmlRipGetNumberOfIfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);   
    return g_NumOfRipIFs;
}
   
/**********************************************************************

   caller:     self

   prototype:

       ANSC_STATUS
       CosaDmlRipGetNumberOfIfEntries
           (
               ANSC_HANDLE                    hContext
           );


   Description:
   
         This function set the router rip configuration.
       
   Arguments:
               ANSC_HANDLE                            hContext,
               

   Return:
       

**********************************************************************/
ANSC_STATUS
CosaDmlRipIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_RIP_IF_CFG        pEntry
    )
{
    PCOSA_DML_RIPD_CONF pConf       = &CosaDmlRIPCurrentConfig;
    errno_t             rc          = -1;
    UNREFERENCED_PARAMETER(hContext);
    if ( (ulIndex+1) > g_NumOfRipIFs )
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( ulIndex == 0 )
    {
        pEntry->InstanceNumber               = ulIndex+1;                   
        pEntry->Enable                       = pConf->If1Enable;                         
        /*pEntry->Version                      = COSA_RIP_VERSION_2;          */
        pEntry->AcceptRA                     = pConf->If1ReceiveEnable;                      
        pEntry->SendRA                       = pConf->If1SendEnable;                           
        pEntry->X_CISCO_COM_ReceiveVersion   = pConf->If1ReceiveVersion;
        pEntry->X_CISCO_COM_SendVersion      = pConf->If1SendVersion;                          
	//Stauts will be based on either SendEnable or ReceiveEnable enabled.(TCCBR-2764)
	pEntry->Status                       = (( pConf->Enable && pConf->If1Enable ) && ( pConf->If1ReceiveEnable == TRUE || pConf->If1SendEnable == TRUE )) ? 2 : 1;
        pEntry->X_CISCO_COM_Neighbor         = pConf->If1Neighbor;

        pEntry->X_CISCO_COM_AuthenticationType   = pConf->If1AuthenticateType;                      
        pEntry->X_CISCO_COM_Md5KeyID             = pConf->If1KeyID;                           
        rc = strcpy_s(pEntry->X_CISCO_COM_Md5KeyValue,sizeof(pEntry->X_CISCO_COM_Md5KeyValue), pConf->If1Md5KeyValue);
        ERR_CHK(rc);
        rc = strcpy_s(pEntry->X_CISCO_COM_SimplePassword,sizeof(pEntry->X_CISCO_COM_SimplePassword), pConf->If1SimplePassword);
        ERR_CHK(rc);
        rc = strcpy_s(pEntry->Alias,sizeof(pEntry->Alias), pConf->If1Alias);
        ERR_CHK(rc);
        rc = strcpy_s(pEntry->Interface,sizeof(pEntry->Interface), pConf->If1Name);
        ERR_CHK(rc);
    }
    else
    {
        AnscTraceWarning(("This RIP interface is wrong:%lu\n", pEntry->InstanceNumber));
    }

#if 1
    AnscTraceWarning(("pEntry->InstanceNumber :%lu\n", pEntry->InstanceNumber));
    AnscTraceWarning(("pEntry->Enable :%s\n", pEntry->Enable?"TRUE":"FALSE"));
    
    AnscTraceWarning(("pEntry->AcceptRA :%s\n", pEntry->AcceptRA?"TRUE":"FALSE"));
    AnscTraceWarning(("pEntry->SendRA :%s\n", pEntry->SendRA?"TRUE":"FALSE"));
    AnscTraceWarning(("pEntry->X_CISCO_COM_ReceiveVersion :%d\n", pEntry->X_CISCO_COM_ReceiveVersion));
    AnscTraceWarning(("pEntry->X_CISCO_COM_SendVersion :%d\n",pEntry->X_CISCO_COM_SendVersion));
    AnscTraceWarning(("pEntry->Status :%d\n", pEntry->Status));
    AnscTraceWarning(("pEntry->X_CISCO_COM_Neighbor :0X%lx\n", pEntry->X_CISCO_COM_Neighbor));

    AnscTraceWarning(("pEntry->X_CISCO_COM_AuthenticationType :%lu\n", pEntry->X_CISCO_COM_AuthenticationType));    
    AnscTraceWarning(("pEntry->X_CISCO_COM_Md5KeyID :%lu\n", pEntry->X_CISCO_COM_Md5KeyID));
    AnscTraceWarning(("pEntry->X_CISCO_COM_Md5KeyValue :%s\n", pEntry->X_CISCO_COM_Md5KeyValue));
//    AnscTraceWarning(("pEntry->X_CISCO_COM_SimplePassword :%s\n", pEntry->X_CISCO_COM_SimplePassword));
    AnscTraceWarning(("pEntry->Alias :%s\n", pEntry->Alias));
    AnscTraceWarning(("pEntry->Interface :%s\n", pEntry->Interface));
#endif
    
    return ANSC_STATUS_SUCCESS;
}    

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipIfAddCfg
            (
                ANSC_HANDLE                      hContext,
                PCOSA_DML_RIP_IF_CFG        pEntry
            )



    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                      hContext,
                
                PCOSA_DML_RIP_IF_CFG        pEntry                  

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipIfAddCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}




/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipIfDelEntry
            (
                ANSC_HANDLE                    hContext,
                PCOSA_DML_RIP_IF_CFG      pEntry     
            )


    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                      hContext,
                
                PCOSA_DML_RIP_IF_CFG        pEntry  

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipIfDelCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry     
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipIfSetCfg
            (
                ANSC_HANDLE                      hContext,
                PCOSA_DML_RIP_IF_CFG        pEntry     
            )

    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                      hContext,

                PCOSA_DML_RIP_IF_CFG        pEntry 

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry     
    )
{
    PCOSA_DML_RIPD_CONF pConf       = &CosaDmlRIPCurrentConfig;
    errno_t             rc          = -1;
    UNREFERENCED_PARAMETER(hContext);

    AnscTraceWarning(("CosaDmlRipIfSetCfg -- starts.\n"));

    if ( pEntry->InstanceNumber == 1 )
    {
        pConf->If1Enable           = pEntry->Enable;        
        pConf->If1SendVersion      = pEntry->X_CISCO_COM_SendVersion;      
        pConf->If1ReceiveVersion   = pEntry->X_CISCO_COM_ReceiveVersion;      
        pConf->If1SendEnable       = pEntry->SendRA;      
        pConf->If1ReceiveEnable    = pEntry->AcceptRA;      
        pConf->If1Neighbor         = pEntry->X_CISCO_COM_Neighbor;

	// Status should reflect on runtime itself instead of after reboot(TCCBR-2764)
	pEntry->Status =( (pEntry->Enable) && (pEntry->SendRA == TRUE || pEntry->AcceptRA == TRUE)) ? 2 : 1;
        pConf->If1AuthenticateType = pEntry->X_CISCO_COM_AuthenticationType;                      
        pConf->If1KeyID            = pEntry->X_CISCO_COM_Md5KeyID;                           
        rc = strcpy_s(pConf->If1Md5KeyValue,sizeof(pConf->If1Md5KeyValue), pEntry->X_CISCO_COM_Md5KeyValue );
        ERR_CHK(rc);
        rc = strcpy_s(pConf->If1SimplePassword,sizeof(pConf->If1SimplePassword), pEntry->X_CISCO_COM_SimplePassword );
        ERR_CHK(rc);

        //this is for true static ip feature.
        rc = strcpy_s(pConf->If1Alias,sizeof(pConf->If1Alias), pEntry->Alias);
        ERR_CHK(rc);
        if ((_ansc_strcmp(pEntry->Alias, COSA_RIPD_IF1_NAME ) == 0 ) ||
            (_ansc_strcmp(pEntry->Alias, "cpe" ) == 0 ) ||
            (_ansc_strcmp(pEntry->Alias, "Ethernet" ) == 0 ) )
        {
            rc = strcpy_s(pConf->If1Name,sizeof(pConf->If1Name), COSA_RIPD_IF1_NAME);
            ERR_CHK(rc);
        }
            
        else
        {
            rc = strcpy_s(pConf->If1Name,sizeof(pConf->If1Name), COSA_RIPD_IF2_NAME);
            ERR_CHK(rc);
        }

    }
    else
    {
        AnscTraceWarning(("This RIP interface is wrong:%lu.", pEntry->InstanceNumber));
    }

#if 1
    AnscTraceWarning(("pConf->If1Enable :%s\n", pConf->If1Enable?"TRUE":"FALSE"));
    AnscTraceWarning(("pConf->If1SendVersion :%lu\n", pConf->If1SendVersion));
    AnscTraceWarning(("pConf->If1ReceiveVersion :%lu\n", pConf->If1ReceiveVersion));
    
    AnscTraceWarning(("pConf->If1SendEnable :%s\n", pConf->If1SendEnable?"TRUE":"FALSE"));
    AnscTraceWarning(("pConf->If1ReceiveEnable :%s\n", pConf->If1ReceiveEnable?"TRUE":"FALSE"));
    AnscTraceWarning(("pConf->If1Neighbor :0X%lx\n", pConf->If1Neighbor));
    AnscTraceWarning(("pConf->If1AuthenticateType :%lu\n", pConf->If1AuthenticateType));
    AnscTraceWarning(("pConf->If1KeyID :%lu\n", pConf->If1KeyID));
    AnscTraceWarning(("pConf->If1Md5KeyValue :%s\n", pConf->If1Md5KeyValue));
//    AnscTraceWarning(("pConf->If1SimplePassword :%s\n", pConf->If1SimplePassword));
    AnscTraceWarning(("pConf->If1Alias :%s\n", pConf->If1Alias));
    AnscTraceWarning(("pConf->If1Name :%s\n", pConf->If1Name));
#endif

#if defined (_BWG_PRODUCT_REQ_)
    if(!pConf->If1Enable)
    {
        CosaRipdOperation("stop");
        CosaDmlSaveRipdConfiguration();
    }
    else
    {
        CosaDmlSaveRipdConfiguration();
        CosaDmlGenerateRipdConfigFile(NULL);
        CosaRipdOperation("restart");
    }
#else
        CosaDmlSaveRipdConfiguration();
        CosaDmlGenerateRipdConfigFile(NULL);
        CosaRipdOperation("restart");
#endif

    AnscTraceWarning(("CosaDmlRipIfSetCfg -- exits.\n"));

    return ANSC_STATUS_CANT_FIND;   
}    


#if ( defined(_COSA_SIM_))

PCOSA_DML_STATICROUTE_CFG 
CosaDmlStaticRouteGetEntries
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    UNREFERENCED_PARAMETER(hContext);
    *pulCount  = 0;
    return NULL;
}

ANSC_STATUS 
CosaDmlStaticRouteGetEntryByName
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS 
CosaDmlStaticRouteDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS 
CosaDmlStaticRouteAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}
ANSC_STATUS 
CosaDmlStaticRouteSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}

/*
 *  Routing Router -- assuming there is only one router in the system
 */


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
        );


    Description:
    
        This function set the configuration of the router.
        
    Arguments:
                ANSC_HANDLE                     hDml,
                
               PANSC_HANDLE                     phContext

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
)
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(phContext);

    AnscTraceWarning(("CosaDmlRipInit -- starts.\n"));

    CosaDmlGetRipdConfiguration();
    CosaDmlGenerateRipdConfigFile(hDml);
    //CosaRipdOperation("restart");
    
    AnscTraceWarning(("CosaDmlRipInit -- exits.\n"));
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingRouterSetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_ROUTER_CFG        pCfg
            );

    Description:
    
        This function set the configuration of the router.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_ROUTER_CFG        pCfg

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingRouterSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_CFG        pCfg
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hContext);
    g_RouterFull.Cfg.InstanceNumber = pCfg->InstanceNumber;  
    g_RouterFull.Cfg.bEnabled       = pCfg->bEnabled;        
    errno_t          rc             = -1;
    rc = strcpy_s(g_RouterFull.Cfg.Alias,sizeof(g_RouterFull.Cfg.Alias), pCfg->Alias);
    ERR_CHK(rc);

    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingRouterGetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_ROUTER_CFG        pCfg
            );

    Description:
    
        This function get the router configuration.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_ROUTER_CFG        pCfg

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingRouterGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_CFG        pCfg
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hContext);
    pCfg->InstanceNumber = g_RouterFull.Cfg.InstanceNumber; 
    pCfg->bEnabled       = g_RouterFull.Cfg.bEnabled;
    errno_t           rc = -1;
    rc = strcpy_s(pCfg->Alias,sizeof(pCfg->Alias), g_RouterFull.Cfg.Alias);
    ERR_CHK(rc);
    
    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingRouterGetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_ROUTER_CFG        pCfg
            );

    Description:
    
         This function get the infomation of the router.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_ROUTER_CFG        pCfg

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingRouterGetInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_INFO       pInfo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pInfo);
    return returnStatus;
}

/*
 *  Routing IPv4 Forwarding
 */

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingGetNumberOfV4Entries
            (
                ANSC_HANDLE                 hContext
            );

    Description:
    
        This function get the number of IPv4 entries .
        
    Arguments:
                ANSC_HANDLE                 hContext

    Return:
        

**********************************************************************/
ULONG
CosaDmlRoutingGetNumberOfV4Entries
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return g_RouterFull.ulNumOfForward;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingGetV4Entry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry
            );

    Description:

        The API retrieves the complete info of one IPv4 Forwarding entry designated
        by index. The usual process is the caller gets the total number of entries, then 
        iterate through those by calling this API.

    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                       ulIndex,
                Indicates the index number of the entry.
                
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry
                To receive the complete info of the entry.
    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingGetV4Entry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hContext);
    pEntry->InstanceNumber   = g_RouterFull.V4ForwardList[ulIndex].InstanceNumber;
    pEntry->Enable           = g_RouterFull.V4ForwardList[ulIndex].Enable;
    pEntry->Status           = g_RouterFull.V4ForwardList[ulIndex].Status;
    pEntry->StaticRoute      = g_RouterFull.V4ForwardList[ulIndex].StaticRoute;
    pEntry->ForwardingPolicy = g_RouterFull.V4ForwardList[ulIndex].ForwardingPolicy;
    pEntry->ForwardingMetric = g_RouterFull.V4ForwardList[ulIndex].ForwardingMetric;

    pEntry->DestIPAddress.Value    = g_RouterFull.V4ForwardList[ulIndex].DestIPAddress.Value;
    pEntry->DestSubnetMask.Value   = g_RouterFull.V4ForwardList[ulIndex].DestSubnetMask.Value;
    pEntry->GatewayIPAddress.Value = g_RouterFull.V4ForwardList[ulIndex].GatewayIPAddress.Value;
    errno_t                     rc = -1;
    rc = strcpy_s(pEntry->Alias,sizeof(pEntry->Alias), g_RouterFull.V4ForwardList[ulIndex].Alias);
    ERR_CHK(rc);
    rc = strcpy_s(pEntry->Interface,sizeof(pEntry->Interface), g_RouterFull.V4ForwardList[ulIndex].Interface);
    ERR_CHK(rc);
    
    return returnStatus;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingSetV4EntryValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIndex,
                ULONG                           ulInstanceNumber,
                char*                             pAlias
            );


    Description:
    
        This function set the configuration of the router.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                           ulIndex,
                
                ULONG                           ulInstanceNumber,
                
                char*                             pAlias

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingSetV4EntryValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hContext);
    g_RouterFull.V4ForwardList[ulIndex].InstanceNumber = ulInstanceNumber;
    errno_t rc = -1;
    rc = strcpy_s(g_RouterFull.V4ForwardList[ulIndex].Alias,sizeof(g_RouterFull.V4ForwardList[ulIndex].Alias), pAlias);
    ERR_CHK(rc);
        
    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingAddV4Entry
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry
            );

    Description:
    
        The API adds one IPv4 forwarding entry into the router. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry
                Caller pass in the configuration. Read-only fields should be ignored.

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingAddV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )
{
    ULONG                           ulIndex = 0;
    errno_t                         rc      = -1;
    ulIndex = g_RouterFull.ulNumOfForward;
    UNREFERENCED_PARAMETER(hContext);
    if ( ulIndex >= 10 )
    {
        return ANSC_STATUS_FAILURE;
    }

    g_RouterFull.V4ForwardList[ulIndex].InstanceNumber   = pEntry->InstanceNumber;   
    g_RouterFull.V4ForwardList[ulIndex].Enable           = pEntry->Enable;   
    g_RouterFull.V4ForwardList[ulIndex].Status           = pEntry->Status;   
    g_RouterFull.V4ForwardList[ulIndex].StaticRoute      = pEntry->StaticRoute;   
    g_RouterFull.V4ForwardList[ulIndex].ForwardingPolicy = pEntry->ForwardingPolicy;   
    g_RouterFull.V4ForwardList[ulIndex].ForwardingMetric = pEntry->ForwardingMetric;   
    g_RouterFull.V4ForwardList[ulIndex].DestIPAddress.Value    = pEntry->DestIPAddress.Value;
    g_RouterFull.V4ForwardList[ulIndex].DestSubnetMask.Value   = pEntry->DestSubnetMask.Value;
    g_RouterFull.V4ForwardList[ulIndex].GatewayIPAddress.Value = pEntry->GatewayIPAddress.Value;
    
    rc = STRCPY_S_NOCLOBBER(g_RouterFull.V4ForwardList[ulIndex].Alias,sizeof(g_RouterFull.V4ForwardList[ulIndex].Alias), pEntry->Alias);
    ERR_CHK(rc);
    rc = STRCPY_S_NOCLOBBER(g_RouterFull.V4ForwardList[ulIndex].Interface,sizeof(g_RouterFull.V4ForwardList[ulIndex].Interface), pEntry->Interface);
    ERR_CHK(rc);
    g_RouterFull.ulNumOfForward++;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingDelV4Entry
        (
            ANSC_HANDLE                 hContext,
            PCOSA_DML_ROUTING_V4_ENTRY  pEntry
        );

    Description:
    
        The API deletes one IPv4 forwarding entry from the router. The forwarding entry
        is identified by the combination of the configuration.
        
    Arguments:
            ANSC_HANDLE                 hContext,
            PCOSA_DML_ROUTING_V4_ENTRY  pEntry

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingDelV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ULONG                           i = 0;
    ULONG                           j = 0;

     for ( i = 0; i < g_RouterFull.ulNumOfForward; i++ )
    {
        if ( g_RouterFull.V4ForwardList[i].InstanceNumber == pEntry->InstanceNumber )
        {
            for ( j = i; j < g_RouterFull.ulNumOfForward; j++ )
            {
                AnscCopyMemory
                (
                    &g_RouterFull.V4ForwardList[j], 
                    &g_RouterFull.V4ForwardList[j+1],
                    sizeof(COSA_DML_ROUTING_V4_ENTRY)
                );
            }

            g_RouterFull.ulNumOfForward--;

            return ANSC_STATUS_SUCCESS;
        }
    }
             
    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingSetV4Entry
        (
            ANSC_HANDLE                 hContext,
            PCOSA_DML_ROUTING_V4_ENTRY  pEntry
        );

    Description:
    
        The API re-configures one IPv4 forwarding entry in the router. The forwarding entry is identified by the combination of the configuration.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingSetV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ULONG                           i       = 0;
    errno_t                         rc      = -1;
     for ( i = 0; i < g_RouterFull.ulNumOfForward; i++ )
    {
        if ( g_RouterFull.V4ForwardList[i].InstanceNumber == pEntry->InstanceNumber )
        {
            g_RouterFull.V4ForwardList[i].InstanceNumber   = pEntry->InstanceNumber;   
            g_RouterFull.V4ForwardList[i].Enable           = pEntry->Enable;   
            g_RouterFull.V4ForwardList[i].Status           = pEntry->Status;   
            g_RouterFull.V4ForwardList[i].StaticRoute      = pEntry->StaticRoute;   
            g_RouterFull.V4ForwardList[i].ForwardingPolicy = pEntry->ForwardingPolicy;   
            g_RouterFull.V4ForwardList[i].ForwardingMetric = pEntry->ForwardingMetric;   
            g_RouterFull.V4ForwardList[i].DestIPAddress.Value    = pEntry->DestIPAddress.Value;
            g_RouterFull.V4ForwardList[i].DestSubnetMask.Value   = pEntry->DestSubnetMask.Value;
            g_RouterFull.V4ForwardList[i].GatewayIPAddress.Value = pEntry->GatewayIPAddress.Value;
            
            rc = STRCPY_S_NOCLOBBER(g_RouterFull.V4ForwardList[i].Alias,sizeof(g_RouterFull.V4ForwardList[i].Alias), pEntry->Alias);
            ERR_CHK(rc);
            rc = STRCPY_S_NOCLOBBER(g_RouterFull.V4ForwardList[i].Interface,sizeof(g_RouterFull.V4ForwardList[i].Interface), pEntry->Interface);
            ERR_CHK(rc);

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingGetV4Entry2
            (
                ANSC_HANDLE                           hContext,
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry      
            )

    description:

        This routine is to retrieve the config of Routing ipv4forwarding.

    argument:  
                    ANSC_HANDLE                          hContext,

                   PCOSA_DML_ROUTING_V4_ENTRY  pEntry  

    return:    operation status.  

**********************************************************************/

ANSC_STATUS
CosaDmlRoutingGetV4Entry2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry      
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);   
    return returnStatus;
}

ULONG
CosaDmlRoutingGetNumberOfV6Entries
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return g_RouterFull.ulNumOfIPv6Forward;
}

ANSC_STATUS
CosaDmlRoutingGetV6Entry
    (
        ANSC_HANDLE                     hContext,
        ULONG                           ulIndex,
        PCOSA_DML_ROUTING_V6_ENTRY      pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    AnscCopyMemory(pEntry, &g_RouterFull.V6ForwardList[ulIndex], sizeof(COSA_DML_ROUTING_V6_ENTRY));
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingSetV6EntryValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
    g_RouterFull.V6ForwardList[ulIndex].InstanceNumber = ulInstanceNumber;
    errno_t rc = -1;
    rc = STRCPY_S_NOCLOBBER(g_RouterFull.V6ForwardList[ulIndex].Alias,sizeof(g_RouterFull.V6ForwardList[ulIndex].Alias), pAlias);
    ERR_CHK(rc);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingAddV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ULONG   ulIndex;
    
    ulIndex = g_RouterFull.ulNumOfIPv6Forward;

    if ( ulIndex >= 10 )
    {
        return ANSC_STATUS_FAILURE;
    }

    AnscCopyMemory(&g_RouterFull.V6ForwardList[ulIndex], pEntry, sizeof(COSA_DML_ROUTING_V6_ENTRY));

    g_RouterFull.ulNumOfForward++;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingDelV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ULONG                           i = 0;
    ULONG                           j = 0;

     for ( i = 0; i < g_RouterFull.ulNumOfIPv6Forward; i++ )
    {
        if ( g_RouterFull.V6ForwardList[i].InstanceNumber == pEntry->InstanceNumber )
        {
            for ( j = i; j < g_RouterFull.ulNumOfIPv6Forward; j++ )
            {
                AnscCopyMemory
                (
                    &g_RouterFull.V6ForwardList[j], 
                    &g_RouterFull.V6ForwardList[j+1],
                    sizeof(COSA_DML_ROUTING_V6_ENTRY)
                );
            }

            g_RouterFull.ulNumOfIPv6Forward--;

            return ANSC_STATUS_SUCCESS;
        }
    }
             
    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlRoutingSetV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ULONG                           i       = 0;

    for ( i = 0; i < g_RouterFull.ulNumOfIPv6Forward; i++ )
    {
        if ( g_RouterFull.V6ForwardList[i].InstanceNumber == pEntry->InstanceNumber )
        {
            AnscCopyMemory(&g_RouterFull.V6ForwardList[i], pEntry, sizeof(COSA_DML_ROUTING_V6_ENTRY));

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlRoutingGetV6Entry2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingRemove
            (
                ANSC_HANDLE                     hContext,
                ULONG                                ulInstanceNumber,
                PCOSA_DML_RIP_IF_INFO       pInfo     
            );

    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                      hContext
                
                ULONG                                ulInstanceNumber
                
                PCOSA_DML_RIP_IF_INFO       pInfo 
                

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingRemove
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hContext);    
    return returnStatus;
}    

static BOOLEAN g_routeinfo_enabled = TRUE; 
ANSC_STATUS
CosaDmlRouteInfoSetEnabled
    (
        BOOLEAN value
    )
{
    g_routeinfo_enabled = value;
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRouteInfoGetEnabled
    (
        BOOLEAN * pBool
    )
{
    *pBool = g_routeinfo_enabled;
    return ANSC_STATUS_SUCCESS;
}
/*
 *  Device.Routing.RouteInformation.
 */
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRoutingGetRouteInfoIf
           (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount

           );

    Description:
    
        This routine is to retrieve the complete list of currently routeinfo interfaceSettings,
        which is a dynamic table.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PULONG                      pulCount
    Return:
        The pointer to the array of routeInfo interfaceSettings, allocated by callee. 
        If no entry is found, NULL is returned.

**********************************************************************/
COSA_DML_ROUTEINFO_IF_INFO g_routeinfo_ifs[] =
{
    {"Interface.1", "2001::11", "2001::/64", "0001-01-01T00:00:00Z", COSA_DML_ROUTEINFO_PRF_High, COSA_DML_ROUTEINFO_STATUS_ForwardingEntryCreated},
    {"Interface.2", "2002::11", "2002::/64", "2001-01-01T00:00:00Z", COSA_DML_ROUTEINFO_PRF_Low,  COSA_DML_ROUTEINFO_STATUS_Error},
};
PCOSA_DML_ROUTEINFO_IF_INFO
CosaDmlRoutingGetRouteInfoIf
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    UNREFERENCED_PARAMETER(hContext);
    PCOSA_DML_ROUTEINFO_IF_INFO        pEntry = NULL;
    int                                num = sizeof(g_routeinfo_ifs)/sizeof(COSA_DML_ROUTEINFO_IF_INFO);

    pEntry = (PCOSA_DML_ROUTEINFO_IF_INFO)AnscAllocateMemory( sizeof(*pEntry)*num );

    if ( !pEntry )
    {
        *pulCount = 0;
        return NULL;
    }
    else
    {
        *pulCount = num;
    }
    
    AnscCopyMemory(pEntry, &g_routeinfo_ifs, sizeof(g_routeinfo_ifs));

    return pEntry;
}

#elif ( defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_MIPS_))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <net/route.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "syscfg/syscfg.h"
#include "cosa_common_util.h"
/* for static route begin */

PCOSA_DML_STATICROUTE_CFG 
CosaDmlStaticRouteGetEntries
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    int                             ulCount = 0;
    int                             i   = 0;
    int                             rc = 0;
    UtopiaContext                   ctx;
    PCOSA_DML_STATICROUTE_CFG       pStaticRoute = NULL;
    routeStatic_t                   *sroutes = NULL;
    CHAR            name[64]   = {0};
    CHAR            value[64]  = {0};
    UNREFERENCED_PARAMETER(hContext);
    errno_t safec_rc  = -1;
    /* Initialize a Utopia Context */
    if (!Utopia_Init(&ctx))
    {
        *pulCount = 0;
        return NULL;
    }
    
    /* To get static routes */
    rc = Utopia_GetStaticRoutes(&ctx, &ulCount, &sroutes);

    /* Add the value ripadvertise for this entry from utapi */
    for ( i =0; i<ulCount; i++)
    {
        safec_rc = sprintf_s(name, sizeof(name), "%s_RIPAdvertise", sroutes[i].name);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        value[0] = '2';
        Utopia_RawSet( &ctx, "cosa_usgv2_", name, value );
    }
    
        
    Utopia_Free(&ctx,1);

    if (rc != 0)
    {
        *pulCount = 0;
        return NULL;
    }

    pStaticRoute = (PCOSA_DML_STATICROUTE_CFG)AnscAllocateMemory(sizeof(COSA_DML_STATICROUTE_CFG) * ulCount);
    if ( !pStaticRoute )
    {
        *pulCount = 0;
        return NULL;
    }

    for (i = 0; i < ulCount; i++)
    {
        pStaticRoute[i].RIPAdvertise            =   FALSE;
        pStaticRoute[i].DestIPAddress           =   _ansc_inet_addr(sroutes[i].dest_lan_ip);
        pStaticRoute[i].DestSubnetMask          =   _ansc_inet_addr(sroutes[i].netmask);
        pStaticRoute[i].GatewayIPAddress        =   _ansc_inet_addr(sroutes[i].gateway);    
        safec_rc = strcpy_s( pStaticRoute[i].Name, sizeof(pStaticRoute[i].Name), sroutes[i].name);
        if(safec_rc != EOK)
        {
           ERR_CHK(safec_rc);
        }
    }

    if ( sroutes )
        free(sroutes);

    *pulCount = ulCount;
    
    return pStaticRoute;
}

ANSC_STATUS 
CosaDmlStaticRouteGetEntryByName
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    int             count      = 0;
    int             index      = 0;
    ANSC_STATUS     rc         = ANSC_STATUS_FAILURE;
    routeStatic_t   *sroutes   = NULL;
    UtopiaContext   ctx;
    ULONG           ret        = 0;
    CHAR            name[64]   = {0};
    CHAR            value[64]  = {0};
    UNREFERENCED_PARAMETER(hContext);
    errno_t safec_rc = -1;
    /* Initialize a Utopia Context */
    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( (UT_SUCCESS == (rc = Utopia_GetStaticRoutes(&ctx, &count, &sroutes))) && (count > 0) ) 
    { 
        if (-1 != (index = Utopia_FindStaticRoute(count, sroutes, pEntry->Name)))
        {
            safec_rc = strcpy_s(pEntry->Name, sizeof(pEntry->Name), sroutes[index-1].name );
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
            pEntry->DestIPAddress = _ansc_inet_addr(sroutes[index-1].dest_lan_ip);
            pEntry->DestSubnetMask = _ansc_inet_addr(sroutes[index-1].netmask);
            pEntry->GatewayIPAddress = _ansc_inet_addr(sroutes[index-1].gateway);

            safec_rc = sprintf_s(name, sizeof(name), "%s_RIPAdvertise", pEntry->Name);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            ret = Utopia_RawGet( &ctx, "cosa_usgv2_", name, value, sizeof(value) );

            if (ret == 0 )
                pEntry->RIPAdvertise = ( (value[0]=='1')?TRUE:FALSE );
            else
            {
                pEntry->RIPAdvertise = FALSE;
                
                /* Add the value ripadvertise for this entry from utapi */
                safec_rc = sprintf_s(name,sizeof(name), "%s_RIPAdvertise", pEntry->Name);
                if(safec_rc < EOK)
                {
                   ERR_CHK(safec_rc);
                }
                if ( pEntry->RIPAdvertise )
                    value[0] = '1';
                else
                    value[0] = '2';
                ret = Utopia_RawSet( &ctx, "cosa_usgv2_", name, value );
            }
            
            rc = ANSC_STATUS_SUCCESS;
        }

    } 

    /*RDKB-6844, CID-33204, free unused resources before exit*/
    if (sroutes) 
    {
        free(sroutes);
    }

    Utopia_Free(&ctx,1);

    return rc;
}

ANSC_STATUS 
CosaDmlStaticRouteDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    int             rc         = 0;
    UtopiaContext   ctx;
    CHAR            name[64]   = {0};
    CHAR            value[64]  = {0};
    ULONG           ret        = 0;
    errno_t    safec_rc = -1;

    UNREFERENCED_PARAMETER(hContext);
    /* Initialize a Utopia Context */
    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Remove the value ripadvertise for this entry from utapi */
    safec_rc = sprintf_s(name, sizeof(name), "%s_RIPAdvertise", pEntry->Name);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    ret = Utopia_RawSet( &ctx, "cosa_usgv2_", name, value );
    if ( ret != 0 )
        AnscTraceWarning(("CosaDmlStaticRouteDelEntry -- return fail from Utopia_RawSet().\n"));


    rc = Utopia_DeleteStaticRouteName( &ctx, pEntry->Name);

    Utopia_Free(&ctx,1);

    if ( rc == 0 )
        return ANSC_STATUS_SUCCESS;
    else
        return ANSC_STATUS_FAILURE;
}


ANSC_STATUS 
CosaDmlStaticRouteAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    routeStatic_t   staticRoute;
    UtopiaContext   ctx;
    ULONG           rc         = 0;
    CHAR            name[64]   = {0};
    CHAR            value[64]  = {0};
    ULONG           ret        = 0;
    UNREFERENCED_PARAMETER(hContext);
    errno_t safec_rc = -1;

    safec_rc = strcpy_s(staticRoute.name, sizeof(staticRoute.name), pEntry->Name );
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(staticRoute.dest_lan_ip, sizeof(staticRoute.dest_lan_ip), _ansc_inet_ntoa(*((struct in_addr*)&pEntry->DestIPAddress)) );
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(staticRoute.netmask, sizeof(staticRoute.netmask), _ansc_inet_ntoa(*((struct in_addr*)&pEntry->DestSubnetMask) ));
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(staticRoute.gateway, sizeof(staticRoute.gateway), _ansc_inet_ntoa(*((struct in_addr*)&pEntry->GatewayIPAddress) ));
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }

    /* Initialize a Utopia Context */
    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }

    rc = Utopia_AddStaticRoute(&ctx, &staticRoute);


    /* Add the value ripadvertise for this entry from utapi */
    safec_rc = sprintf_s(name, sizeof(name), "%s_RIPAdvertise", pEntry->Name);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    if ( pEntry->RIPAdvertise )
        value[0] = '1';
    else
        value[0] = '2';
    ret = Utopia_RawSet( &ctx, "cosa_usgv2_", name, value );

    Utopia_Free(&ctx,1);

    if ( ret != 0 )
        AnscTraceWarning(("CosaDmlStaticRouteAddEntry -- return fail from Utopia_RawSet().\n"));

    if ( rc == 0 )
        return ANSC_STATUS_SUCCESS;
    else
        return ANSC_STATUS_FAILURE;
}

ANSC_STATUS 
CosaDmlStaticRouteSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    int             count           = 0;
    int             index           = 0;
    ANSC_STATUS     rc              = ANSC_STATUS_FAILURE;
    routeStatic_t   sroutes2;
    routeStatic_t   *sroutes        = NULL;
    UtopiaContext   ctx;
    CHAR            name[64]   = {0};
    CHAR            value[64]  = {0};
    ULONG           ret        = 0;
    UNREFERENCED_PARAMETER(hContext);
    errno_t safec_rc = -1;
    /* Initialize a Utopia Context */
    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( (UT_SUCCESS == (rc = Utopia_GetStaticRoutes(&ctx, &count, &sroutes))) && (count > 0) ) 
    { 
        if (-1 != (index = Utopia_FindStaticRoute(count, sroutes, pEntry->Name)))
        {
            safec_rc = strcpy_s(sroutes2.name, sizeof(sroutes2.name), pEntry->Name );
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
            safec_rc = strcpy_s(sroutes2.dest_lan_ip, sizeof(sroutes2.dest_lan_ip), _ansc_inet_ntoa(*((struct in_addr*)&pEntry->DestIPAddress)));
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
            safec_rc = strcpy_s(sroutes2.netmask, sizeof(sroutes2.netmask), _ansc_inet_ntoa(*((struct in_addr*)&pEntry->DestSubnetMask)));
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
            safec_rc = strcpy_s(sroutes2.gateway, sizeof(sroutes2.gateway), _ansc_inet_ntoa(*((struct in_addr*)&pEntry->GatewayIPAddress)));
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }

            /* ??? how about dest_intf: INTERFACE_WAN */

            rc = Utopia_EditStaticRoute(&ctx, index, &sroutes2);
            rc = ANSC_STATUS_SUCCESS;
        }

        if (sroutes) 
        {
            free(sroutes);
        }
    } 


    /* Update the value ripadvertise for this entry from utapi .
           Normally speaking, this name will not be changed. We need not update this value. */
    safec_rc = sprintf_s(name, sizeof(name), "%s_RIPAdvertise", pEntry->Name);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    if ( pEntry->RIPAdvertise )
        value[0] = '1';
    else
        value[0] = '2';
    ret = Utopia_RawSet( &ctx, "cosa_usgv2_", name, value );

    Utopia_Free(&ctx,1);

    if ( ret != 0 )
        AnscTraceWarning(("CosaDmlStaticRouteSetEntry -- return fail from Utopia_RawSet().\n"));

    return rc;
}


/* for static route end */



ULONG g_NumOfRipEntry = 2;

/**
 * IPv6 Forwarding:
 *
 *   internal structure and functions
 */
#define MAX_RTENTRY         256
#define NELEM(a)            (sizeof(a) / sizeof((a)[0]))

/*we don't use it now*/
//#define USE_TR181_PATH

/**
 * PSM parameters:
 *
 *   tr_routing_v4entry.<name>.instance
 *   tr_routing_v6entry.<name>.alias
 *   tr_routing_v6entry.<name>.enabled
 *
 * the "key" is name, whitch is consit of FREFIX/GW/DEV
 */
#define TR_RT6_PREF         "tr_routing_v6entry"
#define RT6_INS             "instance"
#define RT6_ALIAS           "alias"
#define RT6_ENABLED         "enabled"

typedef struct RouteAlias6_s
{
    /* generated */
    char        	name[256];

    /* stored in PSM */
    ULONG       	insNum;
    char        	alias[256];
    BOOL        	enabled;
} RouteAlias6_t;

typedef struct RouteInfo6_s 
{
	/* these params get from kernel */
    char        	prefix[64];
    char        	gateway[64];
    char        	interface[64];
    char        	proto[64]; /* from which protocol */
    int         	metric;

	/* user attached params */
	RouteAlias6_t	alias6;
} RouteInfo6_t;

typedef struct ProtoOrigMap_s 
{
    char        	*proto;
    COSA_DML_ROUTING_V6_ORIGIN    origin;
} ProtoOrigMap_t;

typedef struct RouteInfo6_KeyParam_s 
{
    char        	prefix[64];
    char        	gateway[64];
    char        	interface[64];

} RouteInfo6_KeyParam_t;

static RouteInfo6_KeyParam_t result_arr[MAX_RTENTRY];

/* XXX: we may use variable length array ? */
static int          g_numRtInfo6;
static RouteInfo6_t g_routeInfos6[MAX_RTENTRY];

static ProtoOrigMap_t protoOrigMap[] = 
{
    {"dhcp",    	COSA_DML_ROUTING_IPV6_ORIGIN_DHCPv6},
    {"",        	COSA_DML_ROUTING_IPV6_ORIGIN_OSPF},
    {"ra",      	COSA_DML_ROUTING_IPV6_ORIGIN_RA},
    {"",        	COSA_DML_ROUTING_IPV6_ORIGIN_RIPng},
    {"kernel",  	COSA_DML_ROUTING_IPV6_ORIGIN_Static},
    {"boot",    	COSA_DML_ROUTING_IPV6_ORIGIN_Static},
    {"static",  	COSA_DML_ROUTING_IPV6_ORIGIN_Static},
};

static COSA_DML_ROUTING_V4_ORIGIN
Route4_Proto2Origin(char *proto)
{
	if (!proto || !strlen(proto))
	{
		return COSA_DML_ROUTING_IPV4_ORIGIN_Static;
	}

    if (strcmp(proto, "kernel") == 0)
    {
        return COSA_DML_ROUTING_IPV4_ORIGIN_Static;
    }
    else if (strcmp(proto, "boot") == 0)
    {
        return COSA_DML_ROUTING_IPV4_ORIGIN_Static;
    }
    else if (strcmp(proto, "static") == 0)
    {
        return COSA_DML_ROUTING_IPV4_ORIGIN_Static;
    }
    else if (strcmp(proto, "dhcp") == 0)
    {
        return COSA_DML_ROUTING_IPV4_ORIGIN_DHCPv4;
    }
    else if (strcmp(proto, "ipcp") == 0)
    {
        return COSA_DML_ROUTING_IPV4_ORIGIN_IPCP;
    }
    else if (strcmp(proto, "gated/ospf") == 0)
    {
        return COSA_DML_ROUTING_IPV4_ORIGIN_OSPF;
    }
    else if (strcmp(proto, "gated/rip") == 0)
    {
        return COSA_DML_ROUTING_IPV4_ORIGIN_RIP;
    }

    return COSA_DML_ROUTING_IPV6_ORIGIN_Static;
}

/**
 * @number is an value-result param.
 */
static int
Route6_GetRouteTable(const char *ifname, RouteInfo6_t infos[], int *numInfo)
{
    FILE *fp;
    char cmd[256], line[256];
    int entryCnt, i;
    char *prefix, *key, *val;
    char *delim = " \t\r\n", *saveptr;
    RouteInfo6_t *info6;
	BOOL bFound = FALSE;
    errno_t safec_rc = -1;

    if (!ifname || !infos || !numInfo)
        return -1;

    /**
     * because of "proto" (orig) info,
     * we use "ip -6 route" instead of "route -A inet6".
     */
    if ((fp = v_secure_popen("r", "ip -6 route show dev %s", ifname)) == NULL)
        return -1;

    entryCnt = g_numRtInfo6;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (entryCnt >= *numInfo)
            break;

        /* first token is prefix */
        if ((prefix = strtok_r(line, delim, &saveptr)) == NULL)
            continue;

        info6 = &infos[entryCnt];
        bzero(info6, sizeof(RouteInfo6_t));

        if (strcmp(prefix, "default") == 0)
            //snprintf(info6->prefix, sizeof(info6->prefix), "::/0");
			continue;
        else
            snprintf(info6->prefix, sizeof(info6->prefix), "%s", prefix);

        /* record the interface */
        safec_rc = sprintf_s(info6->interface, sizeof(info6->interface), "%s", ifname);
        if(safec_rc < EOK) ERR_CHK(safec_rc);

        /* key/val pairs */
        while ((key = strtok_r(NULL, delim, &saveptr)) != NULL
                && (val = strtok_r(NULL, delim, &saveptr)) != NULL)
        {
            if (strcmp(key, "dev") == 0) /* should not appear, just put it here*/
                snprintf(info6->interface, sizeof(info6->interface), "%s", val);
            else if (strcmp(key, "via") == 0) 
                snprintf(info6->gateway, sizeof(info6->gateway), "%s", val);
            else if (strcmp(key, "proto") == 0) 
                snprintf(info6->proto, sizeof(info6->proto), "%s", val);
            else if (strcmp(key, "metric") == 0) 
                info6->metric = atoi(val);
            
            /* skip unneeded keys. */
        }
		bFound = FALSE;

		for(i=0; i < g_numRtInfo6; i++)
		{
			if( (strcmp(result_arr[i].prefix,info6->prefix) == 0) && \
				(strcmp(result_arr[i].gateway,info6->gateway) == 0)&& \
				(strcmp(result_arr[i].interface,info6->interface) == 0)) 
			{
					
				bFound = TRUE;
				break;
			}
		}
	
		if (g_numRtInfo6 == 0 || bFound == FALSE)
		{
			safec_rc = strcpy_s(result_arr[g_numRtInfo6].prefix, sizeof(result_arr[g_numRtInfo6].prefix), info6->prefix);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
			safec_rc = strcpy_s(result_arr[g_numRtInfo6].gateway, sizeof(result_arr[g_numRtInfo6].gateway), info6->gateway);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
			safec_rc = strcpy_s(result_arr[g_numRtInfo6].interface, sizeof(result_arr[g_numRtInfo6].interface), info6->interface);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
			g_numRtInfo6++;
			entryCnt ++;
		}
    }
        v_secure_pclose(fp);
	//Fix for issue RDKB-367
#if defined(_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
    snprintf(cmd, sizeof(cmd), "ip -6 route list table main");
#else 
    snprintf(cmd, sizeof(cmd), "ip -6 route list table erouter");
#endif
    if ((fp = popen(cmd, "r")) == NULL)
        return -1;
	
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (entryCnt >= *numInfo) 
            break;

        /* first token is prefix */
        if ((prefix = strtok_r(line, delim, &saveptr)) == NULL)
            continue;

        info6 = &infos[entryCnt];
        bzero(info6, sizeof(RouteInfo6_t));

        if (strcmp(prefix, "default") == 0) {
            snprintf(info6->prefix, sizeof(info6->prefix), "::/0");
	}
        else
            snprintf(info6->prefix, sizeof(info6->prefix), "%s", prefix);

        /* record the interface */
        safec_rc = sprintf_s(info6->interface, sizeof(info6->interface), "%s", ifname);
        if(safec_rc < EOK) ERR_CHK(safec_rc);

        /* key/val pairs */
        while ((key = strtok_r(NULL, delim, &saveptr)) != NULL
                && (val = strtok_r(NULL, delim, &saveptr)) != NULL)
        {
            if (strcmp(key, "dev") == 0) /* should not appear, just put it here*/
                snprintf(info6->interface, sizeof(info6->interface), "%s", val);
            else if (strcmp(key, "via") == 0) 		
                snprintf(info6->gateway, sizeof(info6->gateway), "%s", val);
            else if (strcmp(key, "proto") == 0) 
                snprintf(info6->proto, sizeof(info6->proto), "%s", val);
            else if (strcmp(key, "metric") == 0) 
                info6->metric = atoi(val);
            
            /* skip unneeded keys. */
        }
		 bFound = FALSE;
		 
		 for(i=0; i < g_numRtInfo6; i++)
		 {
			 if( (strcmp(result_arr[i].prefix,info6->prefix) == 0) && \
				 (strcmp(result_arr[i].gateway,info6->gateway) == 0)&& \
				 (strcmp(result_arr[i].interface,info6->interface) == 0)) 
			 {
				 
				 bFound = TRUE;
				 break;
			 }
		 }
		 
		 if (g_numRtInfo6 == 0 || bFound == FALSE)
		 {
			safec_rc = strcpy_s(result_arr[g_numRtInfo6].prefix, sizeof(result_arr[g_numRtInfo6].prefix), info6->prefix);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
			safec_rc = strcpy_s(result_arr[g_numRtInfo6].gateway, sizeof(result_arr[g_numRtInfo6].gateway), info6->gateway);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
			safec_rc = strcpy_s(result_arr[g_numRtInfo6].interface, sizeof(result_arr[g_numRtInfo6].interface), info6->interface);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
			 g_numRtInfo6++;
			 entryCnt ++;
		 }

     }
	pclose(fp);
  //Fix ends
    *numInfo = g_numRtInfo6;
     
    return 0;
}

#define USE_SYSTEM

static int
Route6_Add(const char *prefix, const char *gw, const char *dev, int metric)
{
    int ret;

    if (!prefix || !strlen(prefix) || !dev || !strlen(dev)) 
    {
        CcspTraceWarning(("%s: prefix or dev is empty\n", __FUNCTION__));
        return -1;
    }

    if (gw && *gw) {
        if (metric != 0)
            ret = v_secure_system("ip -6 route add %s dev %s via %s metric %d", prefix, dev, gw, metric);
        else
            ret = v_secure_system("ip -6 route add %s dev %s via %s", prefix, dev, gw);
    }
    else {
        if (metric != 0)
            ret = v_secure_system("ip -6 route  add %s dev %s metric %d", prefix, dev, metric);
        else
            ret = v_secure_system("ip -6 route add %s dev %s", prefix, dev);
    }

    if (ret != 0)
    {
        CcspTraceWarning(("%s: fail to add route\n", __FUNCTION__));
        return -1;
    }
    return 0;
}

static int
Route6_Del(const char *prefix, const char *gw, const char *dev)
{
    int ret;

    if (!prefix || !strlen(prefix)) 
    {
        CcspTraceWarning(("%s: prefix or dev is empty\n", __FUNCTION__));
        return -1;
    }

    if (dev && *dev) {
        if (gw && *gw)
            ret = v_secure_system("ip -6 route del %s dev %s via %s", prefix, dev, gw);
        else
            ret = v_secure_system("ip -6 route del %s dev %s", prefix, dev);
    }
    else {
        if (gw && *gw)
            ret = v_secure_system("ip -6 route del %s via %s", prefix, gw);
        else
            ret = v_secure_system("ip -6 route del %s ", prefix);
    }

    if (ret != 0)
    {
        CcspTraceWarning(("%s: fail to delete route\n", __FUNCTION__));
        return -1;
    }
    return 0;
}

static BOOL
Route6_IsRouteExist(const char *prefix, const char *gw, const char *dev)
{
    int ret;

    if (!prefix || !strlen(prefix)) 
    {
        return FALSE;
    }

    if (dev && *dev) {
        if (gw && *gw)
            ret = v_secure_system("ip -6 route show %s dev %s via %s", prefix, dev, gw);
        else
            ret = v_secure_system("ip -6 route show %s dev %s", prefix, dev);
    }
    else {
        if (gw && *gw)
            ret = v_secure_system("ip -6 route show %s via %s", prefix, gw);
        else
            ret = v_secure_system("ip -6 route show %s | grep '^' ", prefix);
        /* since if no route, system will also return 0 (just no line is shown),
        * we use " | grep '^'" to ensure one line is showed to STDIN */
    }

    if (ret != 0)
    {
        return FALSE;
    }
    return TRUE;
}

#define MAX_RT6IF       16
#define IFNAME_SIZ      32

static int
Route6_GetIfNames(char iflist[][IFNAME_SIZ], int *nlist)
{
    if (!iflist || !nlist)
        return -1;

#if defined(USE_TR181_PATH)
    int ifnum = g_GetParamValueUlong(g_pDslhDmlAgent, "Device.IP.InterfaceNumberOfEntries");
    if (ifnum <= 0)
    {
#endif
        if (*nlist < 2)
            return -1;

        snprintf(iflist[0], IFNAME_SIZ, "%s", "erouter0");
        snprintf(iflist[1], IFNAME_SIZ, "%s", "brlan0");
#if defined (_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
        snprintf(iflist[2], IFNAME_SIZ, "%s", "lo");
        *nlist = 3;
#else
        snprintf(iflist[2], IFNAME_SIZ, "%s", "wan0");
        snprintf(iflist[3], IFNAME_SIZ, "%s", "lo");

        *nlist = 4;
#endif
#if defined(USE_TR181_PATH)
    }
    else
    {
        int ifcnt = 0;
        int i;
        char key[256];
        ULONG len;
        for (i = 1; i <= ifnum; i++) /* start from 1 */
        {
            if (ifcnt >= *nlist)
                break;

            len = IFNAME_SIZ;
            snprintf(key, sizeof(key), "Device.IP.Interface.%d.Name", i);
            if (g_GetParamValueString(g_pDslhDmlAgent, key, iflist[ifcnt], &len) != 0)
                continue;

            ifcnt++;
        }

        *nlist = ifcnt;
    }
#endif

    return 0;
}

static int
Route6_GenerateName(const char *prefix, const char *gateway, 
        const char *interface, char *name, int size)
{
    if (!prefix || !strlen(prefix))
        prefix = "*";
    if (!gateway || !strlen(gateway))
        gateway = "*";
    if (!interface || !strlen(interface))
        interface = "*";

    snprintf(name, size, "%s_%s_%s", prefix, gateway, interface);

    return 0;
}

static int 
Route6_LoadRouteInfo(void)
{
    int i, ifnum, rtcnt;
    char key[256], val[256];
    RouteInfo6_t *info6;
    RouteAlias6_t *alias6;
	UtopiaContext ctx;
    char iflist[MAX_RT6IF][IFNAME_SIZ] ={{0}};

    /* get interface list */
    ifnum = MAX_RT6IF;
    if (Route6_GetIfNames(iflist, &ifnum) != 0)
        return -1;

    /* get runtime routing table from kernel for each interface */
    g_numRtInfo6 = 0;
    for (i = 0; i < ifnum; i++) 
    {
        if ((rtcnt = NELEM(g_routeInfos6) - g_numRtInfo6) <= 0)
            break;

        if (Route6_GetRouteTable(iflist[i], &g_routeInfos6[0], &rtcnt) != 0)
        {
            CcspTraceWarning(("%s: Fail to get IPv6 route table for %s\n", 
                        __FUNCTION__, iflist[i]));
            continue;
        }
        g_numRtInfo6 = rtcnt;
    }

	if (!Utopia_Init(&ctx))
		return -1;

    /* for each RouteInfo6_t load RouteAlias6_t from PSM */
    for (i = 0; i < g_numRtInfo6; i++) 
    {
        info6 = &g_routeInfos6[i];
        alias6 = &info6->alias6;
        bzero(alias6, sizeof(RouteAlias6_t));

        /* generete the name */
        Route6_GenerateName(info6->prefix, info6->gateway, 
                info6->interface, alias6->name, sizeof(alias6->name));

        /* load the params from PSM by <name> */
        /* instance number */
        snprintf(key, sizeof(key), "%s.%s.%s", TR_RT6_PREF, alias6->name, RT6_INS);
        if (Utopia_RawGet(&ctx, NULL, key, val, sizeof(val))) 
        {
            alias6->insNum = atoi(val);
        }

        /* alias */
        snprintf(key, sizeof(key), "%s.%s.%s", TR_RT6_PREF, alias6->name, RT6_ALIAS);
        Utopia_RawGet(&ctx, NULL, key, alias6->alias, sizeof(alias6->alias));

        /* enabled */
        snprintf(key, sizeof(key), "%s.%s.%s", TR_RT6_PREF, alias6->name, RT6_ENABLED);
        if (Utopia_RawGet(&ctx, NULL, key, val, sizeof(val))) 
        {
            /* alias6->enabled = (atoi(val) == 1 ? TRUE : FALSE); */
            /*
             * because the route is exist in kernel, 
             * and here shows DISABLE?
             */
            alias6->enabled = TRUE;
            if (atoi(val) != 1) 
            {
                Utopia_RawSet(&ctx, NULL, key, "1");
            }
        }
    }

	Utopia_Free(&ctx, 1);

    return 0;
}

static int
Route6_UnloadRouteInfo(void)
{
    bzero(&g_routeInfos6, sizeof(g_routeInfos6));
    g_numRtInfo6 = 0;
    return 0;
}

static COSA_DML_ROUTING_V6_ORIGIN
Route6_Proto2Origin(const char *proto)
{
    ULONG i;

	if (!proto || !strlen(proto))
		return COSA_DML_ROUTING_IPV6_ORIGIN_Static;

    for (i = 0; i < NELEM(protoOrigMap); i++) 
    {
        if (strcmp(proto, protoOrigMap[i].proto) == 0)
            return protoOrigMap[i].origin;
    }

    return COSA_DML_ROUTING_IPV6_ORIGIN_Static;
}

static const char *
Route6_Origin2Proto(COSA_DML_ROUTING_V6_ORIGIN origin)
{
    ULONG i;

    for (i = 0; i < NELEM(protoOrigMap); i++) 
    {
        if (origin == protoOrigMap[i].origin)
            return protoOrigMap[i].proto;
    }

    return "static";
}

static RouteInfo6_t *
Route6_GetInfoByInsNum(int insNum, int *index)
{
    int i;

    for (i = 0; i < g_numRtInfo6; i++) 
    {
        if (g_routeInfos6[i].alias6.insNum == (ULONG)insNum) 
        {
            if (index)
                *index = i;
            return &g_routeInfos6[i];
        }
    }

    return NULL;
}

static int
Route6_SaveParams(const RouteAlias6_t *alias6)
{
	char key[256], val[256];
	UtopiaContext ctx;
	errno_t rc = -1;

	if (!alias6)
		return -1;

	if (!Utopia_Init(&ctx))
		return -1;

    rc = sprintf_s(key, sizeof(key), "%s.%s.%s", TR_RT6_PREF, alias6->name, RT6_INS);
    if( rc < EOK ) ERR_CHK(rc);
    snprintf(val, sizeof(val), "%lu", alias6->insNum);
    if (!Utopia_RawSet(&ctx, NULL, key, val))
	{
		Utopia_Free(&ctx, 0);
        return -1;
    }

    rc = sprintf_s(key, sizeof(key), "%s.%s.%s", TR_RT6_PREF, alias6->name, RT6_ALIAS);
    if( rc < EOK ) ERR_CHK(rc);
    if (!Utopia_RawSet(&ctx, NULL, key, (char*)alias6->alias))
	{
		Utopia_Free(&ctx, 0);
        return -1;
    }

    rc = sprintf_s(key, sizeof(key), "%s.%s.%s", TR_RT6_PREF, alias6->name, RT6_ENABLED);
    if( rc < EOK ) ERR_CHK(rc);
    if (!Utopia_RawSet(&ctx, NULL, key, alias6->enabled ? "1" : "0"))
	{
		Utopia_Free(&ctx, 0);
        return -1;
    }

	Utopia_Free(&ctx, 1);

	return 0;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
        );


    Description:
    
        This function set the configuration of the router.
        
    Arguments:
                ANSC_HANDLE                       hDml,
                
                PCOSA_DML_ROUTER_CFG        pCfg

    Return:
        

**********************************************************************/
static int
CosaDmlRouteInfoInit
    (
        void
    );

ANSC_HANDLE g_RoutingEntryInMiddleLayer = NULL;
ANSC_STATUS CosaDmlRipCallBack
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    AnscTraceWarning(("CosaDmlRipCallback -- starts.\n"));
    CosaDmlGenerateRipdConfigFile(g_RoutingEntryInMiddleLayer);
    //CosaRipdOperation("restart");

    AnscTraceWarning(("CosaDmlRipCallback -- exits.\n"));
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
)
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    DSLHDMAGNT_CALLBACK *  pEntry = NULL;
    UNREFERENCED_PARAMETER(phContext);
    CosaDmlRouteInfoInit();
    CosaDmlGetRipdConfiguration();

    g_RoutingEntryInMiddleLayer = hDml;
    pEntry = (PDSLHDMAGNT_CALLBACK)AnscAllocateMemory(sizeof(*pEntry));
    pEntry->func = CosaDmlRipCallBack;
    g_RegisterCallBackAfterInitDml(g_pDslhDmlAgent, pEntry);

    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingRouterSetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_ROUTER_CFG        pCfg
            );

    Description:
    
        This function set the configuration of the router.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_ROUTER_CFG        pCfg

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingRouterSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_CFG        pCfg
    )
{
    int                             rc           = -1;
    UtopiaContext                   ctx;
    char                            buf[8]       ={0};
    UNREFERENCED_PARAMETER(hContext);
    /* Initialize a Utopia Context */
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    rc = Utopia_RawSet(&ctx, NULL, "tr_routing_alias", pCfg->Alias);
    if (rc != 1)
       return ANSC_STATUS_FAILURE;
    CcspTraceWarning(("CosaDmlRoutingRouterSetCfg Alias %s\n", pCfg->Alias));

    snprintf(buf, sizeof(buf), "%d", pCfg->bEnabled);
    rc = Utopia_RawSet(&ctx, NULL, "tr_routing_enabled", buf);

    /* Free Utopia Context */
    Utopia_Free(&ctx, 1);

    if (rc != 1)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingRouterGetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_ROUTER_CFG        pCfg
            );

    Description:
    
        This function get the router configuration.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_ROUTER_CFG        pCfg

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingRouterGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_CFG        pCfg
    )
{
    char                            tmpBuf[8]   = {0};
    int                             rc           = -1;
    UtopiaContext                   ctx;
    UNREFERENCED_PARAMETER(hContext);
    
    /* Initialize a Utopia Context */
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    rc = Utopia_RawGet(&ctx, NULL, "tr_routing_alias", pCfg->Alias, sizeof(pCfg->Alias));
    CcspTraceWarning(("CosaDmlRoutingRouterGetCfg Alias %s\n", pCfg->Alias));

    if (AnscSizeOfString(pCfg->Alias))
    {
        pCfg->InstanceNumber = 1;
    }

    rc = Utopia_RawGet(&ctx, NULL, "tr_routing_enabled", tmpBuf, sizeof(tmpBuf));
    if(1 == rc) {
        pCfg->bEnabled = atoi(tmpBuf);
    }

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    if (rc != 1)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingRouterGetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_ROUTER_CFG        pCfg
            );

    Description:
    
         This function get the infomation of the router.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_ROUTER_CFG        pCfg

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingRouterGetInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_INFO       pInfo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hContext);    
    pInfo->Status = COSA_DML_ROUTING_STATUS_Enabled;

    return returnStatus;
}

/*
 *  Routing IPv4 Forwarding
 */

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingGetNumberOfV4Entries
            (
                ANSC_HANDLE                 hContext
            );

    Description:
    
        This function get the number of IPv4 entries .
        
    Arguments:
                ANSC_HANDLE                 hContext

    Return:
        

**********************************************************************/
int Num_V4Entry = 0;
StaticRoute *sroute;
int Config_Num = 0;
struct
Router_Alias
{
    ULONG                           InstanceNumber;
    ULONG                           Index;
    BOOL                            Enabled;
    BOOL                            Used;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];
    char                            Name[64];
}
Router_Alias[256];

/**
 * Count Netmask BitNum
 */
static int
Count_NetmaskBitNum(unsigned long netmask)
{
	int count = 0;
	
	while(netmask)
	{
		count++;
		netmask = netmask & (netmask - 1);
	}
	
	return count;
}

/**
 * Add Route Entry to kernel table after initialization
 */
int 
AddRouteEntryToKernel(void *arg)
{
    int     index, ret;
    UNREFERENCED_PARAMETER(arg);   

    for (index = 0; index < Config_Num; index++) {
        if (Router_Alias[index].Enabled) { // add route entry to kernel when initialize
#if defined (_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
                    ret = v_secure_system("ip route add %s/%d via %s ", sroute[index].dest_lan_ip, Count_NetmaskBitNum(inet_addr(sroute[index].netmask)), sroute[index].gateway);
#else
                    ret = v_secure_system("ip route add %s/%d via %s table erouter metric %d", sroute[index].dest_lan_ip, Count_NetmaskBitNum(inet_addr(sroute[index].netmask)), sroute[index].gateway, sroute[index].metric);
#endif
            if (ret != 0)
            {
                Router_Alias[index].Enabled = FALSE;
            }
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlRoutingGetNumberOfV4Entries
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    int                             index        = 0;
    ULONG                           uIndex       = 0;
    ULONG                           r_count      = 0;
    char                            cmd_buf[80]  = {0};
    char                            sys_buf[80]  = {0};
    int                             rc           = 0;
    UtopiaContext                   ctx;
    errno_t  safec_rc  = -1;
    UNREFERENCED_PARAMETER(hContext);
    /* Initialize a Utopia Context */
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    /* Fetch the Alias through syscfg_get */
    if (rc == 0)
    {
        r_count = 0;

        for(uIndex = 0; uIndex < 256 ; uIndex++)
        {
            safec_rc = sprintf_s(cmd_buf, sizeof(cmd_buf), "tr_routing_v4entry_%lu_alias", uIndex+1);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            returnStatus = Utopia_RawGet(&ctx, NULL, cmd_buf, sys_buf, sizeof(sys_buf));
            
            if (returnStatus == 1)
            {
                CcspTraceWarning(("returnStatus %lu Instance %lu, \tAlias '%s'\n", returnStatus, uIndex+1, sys_buf));
                safec_rc = strcpy_s(Router_Alias[r_count].Alias,sizeof(Router_Alias[r_count].Alias), sys_buf);
                ERR_CHK(safec_rc);

                Router_Alias[r_count].InstanceNumber = uIndex+1;

                safec_rc = sprintf_s(cmd_buf, sizeof(cmd_buf), "tr_routing_v4entry_%lu_name", uIndex+1);
                if(safec_rc < EOK)
                {
                   ERR_CHK(safec_rc);
                }
                returnStatus = Utopia_RawGet(&ctx, NULL, cmd_buf, sys_buf, sizeof(sys_buf));
                printf("name: %s\n", sys_buf);

                if (returnStatus == 1)
                {
                    safec_rc = strcpy_s(Router_Alias[r_count].Name,sizeof(Router_Alias[r_count].Name), sys_buf);
                    ERR_CHK(safec_rc);
                }
                
                safec_rc = sprintf_s(cmd_buf, sizeof(cmd_buf), "tr_routing_v4entry_%lu_enabled", uIndex+1);
                if(safec_rc < EOK)
                {
                   ERR_CHK(safec_rc);
                }
                returnStatus = Utopia_RawGet(&ctx, NULL, cmd_buf, sys_buf, sizeof(sys_buf));

                if (returnStatus == 1)
                {
                    Router_Alias[r_count].Enabled = atoi(sys_buf);
                }

                Router_Alias[r_count].Used    = TRUE;

                r_count++;
            }
        }

        Config_Num = r_count;
        Num_V4Entry = r_count;
        CcspTraceWarning(("-CosaDmlRoutingGetNumberOfV4Entries %d entry in syscfg-\n", Config_Num));
        CcspTraceWarning(("-CosaDmlRoutingGetNumberOfV4Entries %d entry in backend-\n", Num_V4Entry));

        sroute = (StaticRoute *)malloc(sizeof(StaticRoute) * Num_V4Entry);
        if (NULL == sroute) {
            Utopia_Free(&ctx,0);
            return ANSC_STATUS_FAILURE;
        }
        bzero(sroute, sizeof(StaticRoute) * Num_V4Entry);

        for (index = 0; index < Config_Num; index++) {
            sscanf(Router_Alias[index].Name, "%s %s %s %d", sroute[index].dest_lan_ip, sroute[index].netmask, sroute[index].gateway,
                   &(sroute[index].metric));
            Router_Alias[index].Index = index;
        }

        /*register callback functions for event "lan-status"*/
        EvtDispterRgstCallbackForEvent("lan-status", AddRouteEntryToKernel, NULL);
        
    }

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);
    /* CID: 63127 'Constant' variable guards dead code*/
        
    return Num_V4Entry;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingGetV4Entry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry
            );

    Description:

        The API retrieves the complete info of one IPv4 Forwarding entry designated
        by index. The usual process is the caller gets the total number of entries, then 
        iterate through those by calling this API.

    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                       ulIndex,
                Indicates the index number of the entry.
                
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry
                To receive the complete info of the entry.
    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingGetV4Entry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    int                             index        = 0;
    char                            buf[33]      = {0};
    errno_t  safec_rc = -1;
    UNREFERENCED_PARAMETER(hContext);
    pEntry->Enable           = TRUE;
    pEntry->Status           = 2;
    pEntry->StaticRoute      = TRUE;
    pEntry->ForwardingPolicy = 0;
    pEntry->Origin           = Route4_Proto2Origin(sroute[ulIndex].origin);

    pEntry->DestIPAddress.Value    = _ansc_inet_addr(sroute[ulIndex].dest_lan_ip);
    pEntry->DestSubnetMask.Value   = _ansc_inet_addr(sroute[ulIndex].netmask);
    pEntry->GatewayIPAddress.Value = _ansc_inet_addr(sroute[ulIndex].gateway);
    pEntry->ForwardingMetric       = sroute[ulIndex].metric;
    pEntry->InstanceNumber         = 0;

    safec_rc = strcpy_s(pEntry->Interface,sizeof(pEntry->Interface), sroute[ulIndex].dest_intf );
    ERR_CHK(safec_rc);
    CcspTraceWarning(("-CosaDmlRoutingGetV4Entry %lu interface is %s-\n", ulIndex, pEntry->Interface));

    safec_rc = sprintf_s(buf, sizeof(buf), "%s,%s",
            sroute[ulIndex].dest_lan_ip,
            sroute[ulIndex].netmask
            );
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    CcspTraceWarning(("-CosaDmlRoutingGetV4Entry %lu name is %s-\n", ulIndex, buf));

    for(index = 0; index < Config_Num; index++)
    {
        if (Router_Alias[index].Index != ulIndex)
            continue;

        CcspTraceWarning(("-CosaDmlRoutingGetV4Entry find cfg in syscfg %d-\n", index ));
        pEntry->InstanceNumber   = Router_Alias[index].InstanceNumber;
        pEntry->Enable           = (Router_Alias[index].Enabled)?TRUE:FALSE;
        pEntry->Status           = (Router_Alias[index].Enabled)?2:1;
        safec_rc = strcpy_s(pEntry->Alias,sizeof(pEntry->Alias), Router_Alias[index].Alias);
        ERR_CHK(safec_rc);

        break;
    }

    CcspTraceWarning(("-CosaDmlRoutingGetV4Entry failed to find cfg in syscfg -\n" ));

    return returnStatus;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingSetV4EntryValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIndex,
                ULONG                           ulInstanceNumber,
                char*                             pAlias
            );


    Description:
    
        This function set the configuration of the router.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                           ulIndex,
                
                ULONG                           ulInstanceNumber,
                
                char*                             pAlias

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingSetV4EntryValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    char                            cmd[80]      = {0};
    char                            buf[33]      = {0};
    UtopiaContext                   ctx;
    errno_t                         safec_rc = -1;
    errno_t                         rc       = -1;
    UNREFERENCED_PARAMETER(hContext);
    if (ulInstanceNumber == 0 || pAlias == NULL)
        return ANSC_STATUS_FAILURE;

    /* Initialize a Utopia Context */
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    CcspTraceWarning(("CosaDmlRoutingSetV4EntryValues %lu entry,  instance number %lu, alias %s", 
                ulIndex, ulInstanceNumber, pAlias));
    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_alias", ulInstanceNumber );
    Utopia_RawSet(&ctx, NULL, cmd, pAlias);

    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_name", ulInstanceNumber);
    rc = sprintf_s(buf, sizeof(buf), "%s %s %s %d", 
            sroute[ulIndex].dest_lan_ip,
            sroute[ulIndex].netmask,
            sroute[ulIndex].gateway,
            sroute[ulIndex].metric
            );
    if( rc < EOK ) ERR_CHK(rc);
    Utopia_RawSet(&ctx, NULL, cmd, buf);

    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_enabled", ulInstanceNumber);
    Utopia_RawSet(&ctx, NULL, cmd, "1");

    Utopia_Free(&ctx, 1);

    Router_Alias[Config_Num].Index          = ulIndex;
    Router_Alias[Config_Num].InstanceNumber = ulInstanceNumber;
    Router_Alias[Config_Num].Used           = TRUE;
    safec_rc = strcpy_s(Router_Alias[Config_Num].Alias,sizeof(Router_Alias[Config_Num].Alias), pAlias);
    ERR_CHK(safec_rc);

    Config_Num++;
    
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingAddV4Entry
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry
            );

    Description:
    
        The API adds one IPv4 forwarding entry into the router. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry
                Caller pass in the configuration. Read-only fields should be ignored.

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingAddV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )	
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    char                            cmd[80]      = {0};
    char                            buf[80]      = {0};
    StaticRoute                     *new_route   = 0;
    UtopiaContext                   ctx;
    int                             err = 0;
    errno_t  safec_rc = -1;
    UNREFERENCED_PARAMETER(hContext);    
    if (!pEntry)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Initialize a Utopia Context */
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    Num_V4Entry++;
    new_route = realloc(sroute, sizeof(StaticRoute  )*Num_V4Entry);

    if(new_route == NULL)
    {
        Utopia_Free(&ctx,0);
        Num_V4Entry--;
        return ANSC_STATUS_FAILURE;
    }

    sroute = new_route;

    safec_rc = sprintf_s(sroute[Num_V4Entry-1].dest_lan_ip, sizeof(sroute[Num_V4Entry-1].dest_lan_ip), "%d.%d.%d.%d", 
            pEntry->DestIPAddress.Dot[0],
            pEntry->DestIPAddress.Dot[1],
            pEntry->DestIPAddress.Dot[2],
            pEntry->DestIPAddress.Dot[3]
            );
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = sprintf_s(sroute[Num_V4Entry-1].netmask, sizeof(sroute[Num_V4Entry-1].netmask), "%d.%d.%d.%d", 
            pEntry->DestSubnetMask.Dot[0],
            pEntry->DestSubnetMask.Dot[1],
            pEntry->DestSubnetMask.Dot[2],
            pEntry->DestSubnetMask.Dot[3]
            );
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = sprintf_s(sroute[Num_V4Entry-1].gateway, sizeof(sroute[Num_V4Entry-1].gateway), "%d.%d.%d.%d", 
            pEntry->GatewayIPAddress.Dot[0],
            pEntry->GatewayIPAddress.Dot[1],
            pEntry->GatewayIPAddress.Dot[2],
            pEntry->GatewayIPAddress.Dot[3]
            );
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    strncpy(sroute[Num_V4Entry-1].dest_intf, pEntry->Interface, 9);
    
    sroute[Num_V4Entry-1].metric = pEntry->ForwardingMetric;

    Router_Alias[Config_Num].InstanceNumber = pEntry->InstanceNumber;
    Router_Alias[Config_Num].Index          = Num_V4Entry-1;
    Router_Alias[Config_Num].Used           = TRUE;
    safec_rc = strcpy_s(Router_Alias[Config_Num].Alias,sizeof(Router_Alias[Config_Num].Alias), pEntry->Alias);
    ERR_CHK(safec_rc);
    
    CcspTraceWarning(("---CosaDmlRoutingAddV4Entry ip is %s\n", sroute[Num_V4Entry-1].dest_lan_ip));
    CcspTraceWarning(("---CosaDmlRoutingAddV4Entry mask is %s\n", sroute[Num_V4Entry-1].netmask));
    CcspTraceWarning(("---CosaDmlRoutingAddV4Entry gateway is %s\n", sroute[Num_V4Entry-1].gateway));
    CcspTraceWarning(("---CosaDmlRoutingAddV4Entry name is %s\n", sroute[Num_V4Entry-1].name));

#if defined (_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
         /* add new route entry */
    if(AnscSizeOfString(pEntry->Interface))
    {
        if((err = v_secure_system("ip route add %d.%d.%d.%d/%d via %s %s %s ", pEntry->DestIPAddress.Dot[0], pEntry->DestIPAddress.Dot[1], pEntry->DestIPAddress.Dot[2], pEntry->DestIPAddress.Dot[3], Count_NetmaskBitNum(pEntry->DestSubnetMask.Value), inet_ntoa(*(struct in_addr *)&(pEntry->GatewayIPAddress.Value)), "dev", pEntry->Interface)) != 0) {
            returnStatus = ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        if((err = v_secure_system("ip route add %d.%d.%d.%d/%d via %s ", pEntry->DestIPAddress.Dot[0], pEntry->DestIPAddress.Dot[1], pEntry->DestIPAddress.Dot[2], pEntry->DestIPAddress.Dot[3], Count_NetmaskBitNum(pEntry->DestSubnetMask.Value), inet_ntoa(*(struct in_addr *)&(pEntry->GatewayIPAddress.Value)))) != 0) {
            returnStatus = ANSC_STATUS_FAILURE;
        }
    }
        
#else
    /* add new route entry */
    int                             metric = 0;
    metric = (pEntry->ForwardingMetric)?pEntry->ForwardingMetric+1:pEntry->ForwardingMetric;

    if(AnscSizeOfString(pEntry->Interface))
    {
        if((err = v_secure_system("ip route add %d.%d.%d.%d/%d via %s table erouter metric %d %s %s ", pEntry->DestIPAddress.Dot[0], pEntry->DestIPAddress.Dot[1], pEntry->DestIPAddress.Dot[2], pEntry->DestIPAddress.Dot[3], Count_NetmaskBitNum(pEntry->DestSubnetMask.Value), inet_ntoa(*(struct in_addr *)&(pEntry->GatewayIPAddress.Value)), metric, "dev", pEntry->Interface)) != 0) {
            returnStatus = ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        if((err = v_secure_system("ip route add %d.%d.%d.%d/%d via %s table erouter metric %d ", pEntry->DestIPAddress.Dot[0], pEntry->DestIPAddress.Dot[1], pEntry->DestIPAddress.Dot[2], pEntry->DestIPAddress.Dot[3], Count_NetmaskBitNum(pEntry->DestSubnetMask.Value), inet_ntoa(*(struct in_addr *)&(pEntry->GatewayIPAddress.Value)), metric)) != 0) {
            returnStatus = ANSC_STATUS_FAILURE;
        }
    }
#endif
    if (returnStatus == ANSC_STATUS_SUCCESS)
    {
        pEntry->Enable           = TRUE;
        pEntry->Status           = 2;
        pEntry->StaticRoute      = TRUE;
        pEntry->ForwardingPolicy = 0;
    }
    else
    {
        pEntry->Enable           = FALSE;
        pEntry->Status           = 1;
    }

    Router_Alias[Config_Num].Enabled = (returnStatus == ANSC_STATUS_SUCCESS)?1:0;
    Config_Num++;

    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_alias", 
            pEntry->InstanceNumber );
    Utopia_RawSet(&ctx, NULL, cmd, pEntry->Alias);

    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_name", pEntry->InstanceNumber);
    snprintf(buf, sizeof(buf), "%d.%d.%d.%d %d.%d.%d.%d %s %d", 
            pEntry->DestIPAddress.Dot[0],
            pEntry->DestIPAddress.Dot[1],
            pEntry->DestIPAddress.Dot[2],
            pEntry->DestIPAddress.Dot[3],
            pEntry->DestSubnetMask.Dot[0],
            pEntry->DestSubnetMask.Dot[1],
            pEntry->DestSubnetMask.Dot[2],
            pEntry->DestSubnetMask.Dot[3],
            inet_ntoa(*(struct in_addr *)&(pEntry->GatewayIPAddress.Value)),
            pEntry->ForwardingMetric
            );
    Utopia_RawSet(&ctx, NULL, cmd, buf);
    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_enabled", pEntry->InstanceNumber);
    snprintf(buf, sizeof(buf), "%d", (returnStatus == ANSC_STATUS_SUCCESS)?1:0);
    Utopia_RawSet(&ctx, NULL, cmd, buf);

    /* Free Utopia Context */
    Utopia_Free(&ctx,1);

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingDelV4Entry
        (
            ANSC_HANDLE                 hContext,
            PCOSA_DML_ROUTING_V4_ENTRY  pEntry
        );

    Description:
    
        The API deletes one IPv4 forwarding entry from the router. The forwarding entry
        is identified by the combination of the configuration.
        
    Arguments:
            ANSC_HANDLE                 hContext,
            PCOSA_DML_ROUTING_V4_ENTRY  pEntry

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingDelV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    char                            cmd[80]      = {0};
    UtopiaContext                   ctx;
    UNREFERENCED_PARAMETER(hContext);
    
    int index;
    int err = 0;
    int uindex;

    if (!pEntry)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* CID: 68647 Array compared against 0*/
    if (pEntry->InstanceNumber == 0)
        return ANSC_STATUS_FAILURE;

    CcspTraceWarning(("CosaDmlRoutingDelV4Entry entry, %lu\n", pEntry->InstanceNumber));
    /* Initialize a Utopia Context */
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
 
    for(index = 0; index < Config_Num; index++)
    {
        if (pEntry->InstanceNumber != Router_Alias[index].InstanceNumber)
            continue;

        CcspTraceWarning(("CosaDmlRoutingDelV4Entry find entry, %d\n", index));

        /* delete the previous one */
        uindex = Router_Alias[index].Index;
#if defined (_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
        if(AnscSizeOfString(sroute[uindex].dest_intf))
        {
            err = v_secure_system("ip route del %s/%d via %s %s %s ", sroute[uindex].dest_lan_ip, Count_NetmaskBitNum(inet_addr(sroute[uindex].netmask)), sroute[uindex].gateway, "dev", sroute[uindex].dest_intf);
        }
        else 
        {
            err = v_secure_system("ip route del %s/%d via %s ", sroute[uindex].dest_lan_ip, Count_NetmaskBitNum(inet_addr(sroute[uindex].netmask)), sroute[uindex].gateway);
        }
        break;
#else
        int metric = 0;
        if(AnscSizeOfString(sroute[uindex].dest_intf))
        {
            err = v_secure_system("ip route del %s/%d via %s table erouter metric %d %s %s ", sroute[uindex].dest_lan_ip, Count_NetmaskBitNum(inet_addr(sroute[uindex].netmask)), sroute[uindex].gateway, metric, "dev", sroute[uindex].dest_intf);
        }
        else
        {
            err = v_secure_system("ip route del %s/%d via %s table erouter metric %d ", sroute[uindex].dest_lan_ip, Count_NetmaskBitNum(inet_addr(sroute[uindex].netmask)), sroute[uindex].gateway, metric);
        }
        break;
#endif
    }
    if (err) {
        CcspTraceWarning(("CosaDmlRoutingDelV4Entry delete failure, %s\n", strerror(errno)));
        returnStatus = ANSC_STATUS_FAILURE;
    }
    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_alias", 
            pEntry->InstanceNumber);
    Utopia_RawSet(&ctx, NULL, cmd, NULL);
    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_name", 
            pEntry->InstanceNumber);
    Utopia_RawSet(&ctx, NULL, cmd, NULL);
    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_enabled", 
            pEntry->InstanceNumber);
    Utopia_RawSet(&ctx, NULL, cmd, NULL);
    CcspTraceWarning(("CosaDmlRoutingDelV4Entry unset tr_routing_v4entry_%lu_alias \n", pEntry->InstanceNumber));

    Utopia_Free(&ctx, 1);

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingSetV4Entry
        (
            ANSC_HANDLE                 hContext,
            PCOSA_DML_ROUTING_V4_ENTRY  pEntry
        );

    Description:
    
        The API re-configures one IPv4 forwarding entry in the router. Thetry is identified by the combination of the configuration.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingSetV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    char                            cmd[80]      = {0};
    char                            buf[80]      = {0};
    int                             index = 0;
    int                             err = 0;
    UtopiaContext                   ctx;
    errno_t  safec_rc  = -1;
#if defined (_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
#else
    int                             metric = 0;
#endif
    int                             uindex;
    UNREFERENCED_PARAMETER(hContext);

    if (!pEntry)
    {
        return ANSC_STATUS_FAILURE;
    }
    /*CID: 60087 Array compared against 0*/
    if (pEntry->InstanceNumber == 0)
        return ANSC_STATUS_FAILURE;

    /* Initialize a Utopia Context */
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    CcspTraceWarning(("CosaDmlRoutingSetV4Entry %lu \n", pEntry->InstanceNumber));
    /* set syscfg */
    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_alias", 
            pEntry->InstanceNumber );
    Utopia_RawSet(&ctx, NULL, cmd, pEntry->Alias);
    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_name", pEntry->InstanceNumber);
    snprintf(buf, sizeof(buf), "%d.%d.%d.%d %d.%d.%d.%d %s %d",
            pEntry->DestIPAddress.Dot[0],
            pEntry->DestIPAddress.Dot[1],
            pEntry->DestIPAddress.Dot[2],
            pEntry->DestIPAddress.Dot[3],
            pEntry->DestSubnetMask.Dot[0],
            pEntry->DestSubnetMask.Dot[1],
            pEntry->DestSubnetMask.Dot[2],
            pEntry->DestSubnetMask.Dot[3],
            inet_ntoa(*(struct in_addr *)&(pEntry->GatewayIPAddress.Value)),
            pEntry->ForwardingMetric
            );
    Utopia_RawSet(&ctx, NULL, cmd, buf);
    snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_enabled", pEntry->InstanceNumber);
    snprintf(buf, sizeof(buf), "%d", (pEntry->Enable)?1:0);
    Utopia_RawSet(&ctx, NULL, cmd, buf);
    Utopia_Free(&ctx, 1);

    for(index = 0; index < Config_Num; index++)
    {
        if (pEntry->InstanceNumber != Router_Alias[index].InstanceNumber)
            continue;

        CcspTraceWarning(("CosaDmlRoutingSetV4Entry find entry %d \n", index));
        if (
             pEntry->DestIPAddress.Value    == inet_addr(sroute[index].dest_lan_ip) &&
             pEntry->DestSubnetMask.Value   == inet_addr(sroute[index].netmask)     &&
             pEntry->GatewayIPAddress.Value == inet_addr(sroute[index].gateway)     &&
             pEntry->Enable                 == Router_Alias[index].Enabled          &&
             pEntry->ForwardingMetric       == sroute[index].metric                 && 
             !strcmp(pEntry->Interface, sroute[index].dest_intf)
           )
        {
            /* The same parameter, no need to change */
            return ANSC_STATUS_SUCCESS;
        }

        Router_Alias[index].Enabled = pEntry->Enable;

        /* First delete the previous one */
        uindex = Router_Alias[index].Index;
#if defined (_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
        if(AnscSizeOfString(sroute[uindex].dest_intf))
        {
            err = v_secure_system("ip route del %s/%d via %s %s %s ", sroute[uindex].dest_lan_ip, Count_NetmaskBitNum(inet_addr(sroute[uindex].netmask)), sroute[uindex].gateway, "dev", sroute[uindex].dest_intf);
        }
        else
        {
            err = v_secure_system("ip route del %s/%d via %s ", sroute[uindex].dest_lan_ip, Count_NetmaskBitNum(inet_addr(sroute[uindex].netmask)), sroute[uindex].gateway);
        }
        break;
#else
        if(AnscSizeOfString(sroute[uindex].dest_intf))
        {
            err = v_secure_system("ip route del %s/%d via %s table erouter metric %d %s %s ", sroute[uindex].dest_lan_ip, Count_NetmaskBitNum(inet_addr(sroute[uindex].netmask)), sroute[uindex].gateway, metric, "dev", sroute[uindex].dest_intf);
        }
        else
        {
            err = v_secure_system("ip route del %s/%d via %s table erouter metric %d ", sroute[uindex].dest_lan_ip, Count_NetmaskBitNum(inet_addr(sroute[uindex].netmask)), sroute[uindex].gateway, metric);
        }
        break;
#endif
        pEntry->Status           = 1;

        /* Then add the new one */
        if (!pEntry->Enable)
        {
            break;
        }

        CcspTraceWarning(("CosaDmlRoutingSetV4Entry add the new entry\n"));
        safec_rc = sprintf_s(sroute[uindex].dest_lan_ip, sizeof(sroute[uindex].dest_lan_ip), "%d.%d.%d.%d", 
                pEntry->DestIPAddress.Dot[0],
                pEntry->DestIPAddress.Dot[1],
                pEntry->DestIPAddress.Dot[2],
                pEntry->DestIPAddress.Dot[3]
                );
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        safec_rc = sprintf_s(sroute[uindex].netmask, sizeof(sroute[uindex].netmask), "%d.%d.%d.%d", 
                pEntry->DestSubnetMask.Dot[0],
                pEntry->DestSubnetMask.Dot[1],
                pEntry->DestSubnetMask.Dot[2],
                pEntry->DestSubnetMask.Dot[3]
                );
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        safec_rc = sprintf_s(sroute[uindex].gateway, sizeof(sroute[uindex].gateway), "%d.%d.%d.%d", 
                pEntry->GatewayIPAddress.Dot[0],
                pEntry->GatewayIPAddress.Dot[1],
                pEntry->GatewayIPAddress.Dot[2],
                pEntry->GatewayIPAddress.Dot[3]
                );
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        strncpy(sroute[uindex].dest_intf, pEntry->Interface, 9);
#if defined (_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
        if(AnscSizeOfString(pEntry->Interface))
        {
            err = v_secure_system("ip route add %d.%d.%d.%d/%d via %s %s %s ", pEntry->DestIPAddress.Dot[0], pEntry->DestIPAddress.Dot[1], pEntry->DestIPAddress.Dot[2], pEntry->DestIPAddress.Dot[3], Count_NetmaskBitNum(pEntry->DestSubnetMask.Value), inet_ntoa(*(struct in_addr *)&(pEntry->GatewayIPAddress.Value)), "dev", pEntry->Interface);
        }
        else
        {
            err = v_secure_system("ip route add %d.%d.%d.%d/%d via %s ", pEntry->DestIPAddress.Dot[0], pEntry->DestIPAddress.Dot[1], pEntry->DestIPAddress.Dot[2], pEntry->DestIPAddress.Dot[3], Count_NetmaskBitNum(pEntry->DestSubnetMask.Value), inet_ntoa(*(struct in_addr *)&(pEntry->GatewayIPAddress.Value)));
        }
        break;
#else
        metric = (pEntry->ForwardingMetric)?pEntry->ForwardingMetric+1:pEntry->ForwardingMetric;
        if(AnscSizeOfString(pEntry->Interface))
        {
            err = v_secure_system("ip route add %d.%d.%d.%d/%d via %s table erouter metric %d %s %s ", pEntry->DestIPAddress.Dot[0], pEntry->DestIPAddress.Dot[1], pEntry->DestIPAddress.Dot[2], pEntry->DestIPAddress.Dot[3], Count_NetmaskBitNum(pEntry->DestSubnetMask.Value), inet_ntoa(*(struct in_addr *)&(pEntry->GatewayIPAddress.Value)), metric, "dev", pEntry->Interface);
        }
        else
        {
            err = v_secure_system("ip route add %d.%d.%d.%d/%d via %s table erouter metric %d ", pEntry->DestIPAddress.Dot[0], pEntry->DestIPAddress.Dot[1], pEntry->DestIPAddress.Dot[2], pEntry->DestIPAddress.Dot[3], Count_NetmaskBitNum(pEntry->DestSubnetMask.Value), inet_ntoa(*(struct in_addr *)&(pEntry->GatewayIPAddress.Value)), metric);
        }
        break;
#endif
        if (returnStatus == ANSC_STATUS_SUCCESS)
        {
            pEntry->Status           = 2;
            pEntry->StaticRoute      = TRUE;
            pEntry->ForwardingPolicy = 0;
        }
        else
        {
            pEntry->Enable           = FALSE;
            pEntry->Status           = 1;
            pEntry->StaticRoute      = TRUE;
            Router_Alias[index].Enabled = FALSE;

            if(!Utopia_Init(&ctx))
                return ANSC_STATUS_FAILURE;

            snprintf(cmd, sizeof(cmd), "tr_routing_v4entry_%lu_enabled", pEntry->InstanceNumber);
            snprintf(buf, sizeof(buf), "%d", 0);
            Utopia_RawSet(&ctx, NULL, cmd, buf);
            Utopia_Free(&ctx, 1);
        }

        break;
    }
    if (err) {
        CcspTraceWarning(("CosaDmlRoutingSetV4Entry add failure, %s\n", strerror(errno)));
        returnStatus = ANSC_STATUS_FAILURE;
    }
    return returnStatus;
}

/**
 * IPv6 forwarding 
 */
ULONG
CosaDmlRoutingGetNumberOfV6Entries
    (
        ANSC_HANDLE                 hContext
    )
{
	/*
     * XXX: we can't use g_GetParamValueXXX before whole back-end initialized.
     * Though this function will also called when dml init, 
     * but it will also be called later (every time need the the num of entry).
     */
    UNREFERENCED_PARAMETER(hContext);
    if (Route6_LoadRouteInfo() != 0)
        return 0;
	
    return g_numRtInfo6;
}

ANSC_STATUS
CosaDmlRoutingGetV6Entry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry
    )
{
    RouteInfo6_t *info6;
    RouteAlias6_t *alias6;
    UNREFERENCED_PARAMETER(hContext);
    if (!pEntry)
        return ANSC_STATUS_FAILURE;

    bzero(pEntry, sizeof(COSA_DML_ROUTING_V6_ENTRY));

    if (ulIndex >= (ULONG)g_numRtInfo6) 
    {
        CcspTraceWarning(("%s: bad ulIndex\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    info6 = &g_routeInfos6[ulIndex];
    alias6 = &info6->alias6;

    /* get params from RouteInfo6_t */
    snprintf(pEntry->DestIPPrefix, sizeof(pEntry->DestIPPrefix), "%s", info6->prefix);
    snprintf(pEntry->NextHop, sizeof(pEntry->NextHop), "%s", info6->gateway);

#if defined(USE_TR181_PATH)
    char *path;
    if ((path = CosaUtilGetFullPathNameByKeyword("Device.IP.Interface.", 
			"Name", info6->interface)) != NULL)
		snprintf(pEntry->Interface, sizeof(pEntry->Interface), "%s", path);
	else
#endif
    	snprintf(pEntry->Interface, sizeof(pEntry->Interface), "%s", info6->interface);

    pEntry->Origin = Route6_Proto2Origin(info6->proto);
    pEntry->ForwardingMetric = info6->metric;

    /* get params from RouteAlias6_t */
    //pEntry->InstanceNumber = alias6->insNum;
    //snprintf(pEntry->Alias, sizeof(pEntry->Alias), "%s", alias6->alias);
    pEntry->Enable = alias6->enabled;

    /* other params */
    if (Route6_IsRouteExist(info6->prefix, info6->gateway, info6->interface))
        pEntry->Status = COSA_DML_ROUTING_STATUS_Enabled;
    else
        pEntry->Status = COSA_DML_ROUTING_STATUS_Disabled;
    /* XXX: on linux, how to get out the expire time (lifetime) of an prefix ? */
    snprintf(pEntry->ExpirationTime, sizeof(pEntry->ExpirationTime), "9999-12-31T23:59:59Z");
    pEntry->ForwardingPolicy = -1; /* XXX not support now */

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingSetV6EntryValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    RouteInfo6_t *info6;
    RouteAlias6_t *alias6;
    UNREFERENCED_PARAMETER(hContext);

	/**
	 * This function set RouteAlias6_t{} and PSM param **ONLY**.
	 * @ulIndex to indicate the existing RouteInfo6_t{};
	 */

    if (ulIndex >= (ULONG)g_numRtInfo6 || ulInstanceNumber == 0 || !pAlias)
        return ANSC_STATUS_FAILURE;

    info6 = &g_routeInfos6[ulIndex];
    alias6 = &info6->alias6;

	/* RouteAlias6_t{} */
    Route6_GenerateName(info6->prefix, info6->gateway, 
            info6->interface, alias6->name, sizeof(alias6->name));
    alias6->insNum = ulInstanceNumber;
    snprintf(alias6->alias, sizeof(alias6->alias), "%s", pAlias);
    alias6->enabled = TRUE;

    /* PSM parameters */
	if (Route6_SaveParams(alias6) != 0)
		return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingAddV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry
    )
{
    RouteInfo6_t *info6;
    RouteAlias6_t *alias6;
    UNREFERENCED_PARAMETER(hContext);
    errno_t rc = -1;

    if (!pEntry)
        return ANSC_STATUS_FAILURE;

    /* XXX: we may use variable length array 
     * (re-allocate if space not enough) ? */
    if (g_numRtInfo6 >= MAX_RTENTRY) 
    {
        CcspTraceWarning(("%s: Too many entries\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    /* for later use */
    info6 = &g_routeInfos6[g_numRtInfo6];
    alias6 = &info6->alias6;

    /* I.   RouteInfo6_t{} */
    bzero(info6, sizeof(RouteInfo6_t));
    snprintf(info6->prefix, sizeof(info6->prefix), "%s", pEntry->DestIPPrefix);
    rc = sprintf_s(info6->gateway, sizeof(info6->gateway), "%s", pEntry->NextHop);
    if( rc < EOK ) ERR_CHK(rc);

#if defined(USE_TR181_PATH)
    char key[256];
    ULONG len;
	if (strlen(pEntry->Interface) == 0) 
    {
		/* XXX: suitable to use brlan0 ? */
		snprintf(info6->interface, sizeof(info6->interface), "brlan0");
	} 
    else 
    {
	    snprintf(key, sizeof(key), "%sName", pEntry->Interface);
	    len = sizeof(info6->interface);
	    if (g_GetParamValueString(g_pDslhDmlAgent, key, info6->interface, &len) != 0)
	    {
	        /* info6->interface[0] = '\0'; */
	        snprintf(info6->interface, sizeof(info6->interface), "%s", pEntry->Interface);
	    }
	}
#else
    rc = sprintf_s(info6->interface, sizeof(info6->interface), "%s", pEntry->Interface);
    if( rc < EOK ) ERR_CHK(rc);
#endif
    snprintf(info6->proto, sizeof(info6->proto), "%s", Route6_Origin2Proto(pEntry->Origin));
    info6->metric = pEntry->ForwardingMetric;

    /* II.  System backend (kernel routing table) */
    if (pEntry->Enable) 
    {
        if (Route6_Add(info6->prefix, info6->gateway, 
                    info6->interface, info6->metric) != 0) 
        {
            CcspTraceWarning(("%s: Route6_Add error\n", __FUNCTION__));
            /* return ANSC_STATUS_FAILURE; */
        }
    }

    /* III. RouteAlias6_t{} */
    bzero(alias6, sizeof(RouteAlias6_t));
    Route6_GenerateName(info6->prefix, info6->gateway, 
            info6->interface, alias6->name, sizeof(alias6->name));
    alias6->insNum = pEntry->InstanceNumber;
    rc = sprintf_s(alias6->alias, sizeof(alias6->alias), "%s", pEntry->Alias);
    if( rc < EOK ) ERR_CHK(rc);
    alias6->enabled = pEntry->Enable;

    /* IV.  PSM params */
	if (Route6_SaveParams(alias6) != 0)
		return ANSC_STATUS_FAILURE;
	
    /* all settings are successfully commited, now add counter */
    g_numRtInfo6++;;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingDelV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    )
{
    int index;
    char key[256];
    RouteInfo6_t *info6;
    RouteAlias6_t *alias6;
    errno_t rc = -1;
    UNREFERENCED_PARAMETER(hContext);
    if (!pEntry)
        return ANSC_STATUS_FAILURE;

    if ((info6 = Route6_GetInfoByInsNum(pEntry->InstanceNumber, &index)) == NULL) 
    {
		CcspTraceWarning(("%s: instance not exist\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    alias6 = &info6->alias6;

    /* PSM params */
    rc = sprintf_s(key, sizeof(key), "%s.%s.%s", TR_RT6_PREF, alias6->name, RT6_INS);
    if( rc < EOK ) ERR_CHK(rc);
    syscfg_unset(NULL, key);

    rc = sprintf_s(key, sizeof(key), "%s.%s.%s", TR_RT6_PREF, alias6->name, RT6_ALIAS);
    if( rc < EOK ) ERR_CHK(rc);
    syscfg_unset(NULL, key);

    rc = sprintf_s(key, sizeof(key), "%s.%s.%s", TR_RT6_PREF, alias6->name, RT6_ENABLED);
    if( rc < EOK ) ERR_CHK(rc);
    syscfg_unset(NULL, key);

    syscfg_commit();

    /* kernel routing table */
    Route6_Del(info6->prefix, info6->gateway, info6->interface);

    /* RouteInfo6_t{} and RouteAlias6_t{} */
    memmove(&g_routeInfos6[index], &g_routeInfos6[index + 1], 
    		(g_numRtInfo6 - index - 1) * sizeof(RouteInfo6_t));
    g_numRtInfo6--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingSetV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    )
{
    RouteInfo6_t *info6;
    RouteAlias6_t *alias6;
    errno_t rc = -1;
    UNREFERENCED_PARAMETER(hContext);
    if (!pEntry)
        return ANSC_STATUS_FAILURE;

    if ((info6 = Route6_GetInfoByInsNum(pEntry->InstanceNumber, NULL)) == NULL) 
    {
        CcspTraceWarning(("%s: instance not exist\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    alias6 = &info6->alias6;

    /* delete old route first, fail is no problem */
	if (alias6->enabled)
	    Route6_Del(info6->prefix, info6->gateway, info6->interface);

    /* RouteInfo6_t{} */
    snprintf(info6->prefix, sizeof(info6->prefix), "%s", pEntry->DestIPPrefix);
    rc = sprintf_s(info6->gateway, sizeof(info6->gateway), "%s", pEntry->NextHop);
    if( rc < EOK ) ERR_CHK(rc);

#if defined(USE_TR181_PATH)
    char key[256];
    ULONG len;
	if (strlen(pEntry->Interface) == 0) 
    {
		/* XXX: suitable to use brlan0 ? */
		snprintf(info6->interface, sizeof(info6->interface), "brlan0");
	} 
    else 
    {
	    snprintf(key, sizeof(key), "%sName", pEntry->Interface);
	    len = sizeof(info6->interface);
	    if (g_GetParamValueString(g_pDslhDmlAgent, key, info6->interface, &len) != 0)
	    {
	        /* info6->interface[0] = '\0'; */
			snprintf(info6->interface, sizeof(info6->interface), "%s", pEntry->Interface);
		}
    }
#else
	rc = sprintf_s(info6->interface, sizeof(info6->interface), "%s", pEntry->Interface);
	if( rc < EOK ) ERR_CHK(rc);
#endif
    snprintf(info6->proto, sizeof(info6->proto), "%s", Route6_Origin2Proto(pEntry->Origin));
    info6->metric = pEntry->ForwardingMetric;

    /* System backend (kernel routing table) */
    if (pEntry->Enable) 
    {
        if (Route6_Add(info6->prefix, info6->gateway, info6->interface, info6->metric) != 0) 
        {
            CcspTraceWarning(("%s: fail to add new route\n", __FUNCTION__));
        }
    }

    /* RouteAlias6_t{} */
    Route6_GenerateName(info6->prefix, info6->gateway, info6->interface,
            alias6->name, sizeof(alias6->name));
    if (alias6->insNum != pEntry->InstanceNumber)
            CcspTraceWarning(("%s: instance num not match !!\n", __FUNCTION__));
    rc = sprintf_s(alias6->alias, sizeof(alias6->alias), "%s", pEntry->Alias);
    if( rc < EOK ) ERR_CHK(rc);
    alias6->enabled = pEntry->Enable;

    /* PSM params */
	if (Route6_SaveParams(alias6) != 0)
		return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

/* This function use InstanceNumber, 
 * and CosaDmlRoutingSetV6Entry() use index !! */
ANSC_STATUS
CosaDmlRoutingGetV6Entry2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    )
{
    int index;
    UNREFERENCED_PARAMETER(hContext);
    if (!pEntry)
        return ANSC_STATUS_FAILURE;

    if (Route6_GetInfoByInsNum(pEntry->InstanceNumber, &index) == NULL)
        return ANSC_STATUS_FAILURE;

    return CosaDmlRoutingGetV6Entry(NULL, index, pEntry);
}

/* Currently RIP is realized seperatedly from UTAPI.*/

#if 0

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipGetCfg
            (
                ANSC_HANDLE                    hContext,
                PCOSA_DML_RIP_CFG           pCfg
            );


    Description:
    
          This function get the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                            hContext,
                
                PCOSA_DML_RIP_CFG                   pCfg

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_CFG           pCfg
    )

{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    ULONG                           rc           = 0;
    UtopiaContext                   Ctx;

    if ( !Utopia_Init(&Ctx) )
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_GetBool(&Ctx, UtopiaValue_RIP_Enabled, (boolean_t *)&pCfg->Enable);

    pCfg->Mode   = COSA_DML_RIP_MODE_Both;
    
    Utopia_Free(&Ctx, 0); 
    
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipSetCfg
            (
                ANSC_HANDLE                    hContext,
                PCOSA_DML_RIP_CFG           pCfg
            );


    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                            hContext,
                
                PCOSA_DML_RIP_CFG                   pCfg

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_CFG           pCfg
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    ULONG                           rc           = 0;
    UtopiaContext                   Ctx;

    if ( !pCfg )
    {
        CcspTraceWarning(("%s Error Input\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    if ( !Utopia_Init(&Ctx) )
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    UTOPIA_SETBOOL(&Ctx, UtopiaValue_RIP_Enabled, pCfg->Enable);

    Utopia_Free(&Ctx, 1); 
    
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipGetNumberOfIfEntries
            (
                ANSC_HANDLE                    hContext
            );


    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                            hContext,
                

    Return:
        

**********************************************************************/
ULONG
CosaDmlRipGetNumberOfIfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
   
    return g_NumOfRipEntry;
}
   
/**********************************************************************

   caller:     self

   prototype:

       ANSC_STATUS
       CosaDmlRipGetNumberOfIfEntries
           (
               ANSC_HANDLE                    hContext
           );


   Description:
   
         This function set the router rip configuration.
       
   Arguments:
               ANSC_HANDLE                            hContext,
               

   Return:
       

**********************************************************************/
ANSC_STATUS
CosaDmlRipIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_RIP_IF_FULL       pEntry
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    char                            buf[64]      = {0};
    UtopiaContext                   Ctx;
    int                             rc = 0;
    char                            param_val[64] = {0};
    ULONG                           val_len = 0;

    if ( ulIndex >= g_NumOfRipEntry ) 
    {
        CcspTraceWarning(("%s Error Input\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    if ( !Utopia_Init(&Ctx) )
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    /*  to get Device.Routing.RIP.InterfaceSetting.{i}.Version */
    pEntry->Cfg.Version = 2;

    if ( 0 == ulIndex )
    {
        /* to get LAN interface setting for rip */
        AnscZeroMemory(buf, 64);        
        rc = Utopia_RawGet(&Ctx, NULL, "tr_rip_insnum_lan", buf, sizeof(buf)); 
        
        if ( 1 == rc )
        {
            pEntry->Cfg.InstanceNumber = atoi(buf);
        }
        
        AnscZeroMemory(buf, 64);        
        rc = Utopia_RawGet(&Ctx, NULL, "tr_rip_alias_lan", buf, sizeof(buf));  
        
        if ( 1 == rc )
        {
            AnscCopyString(pEntry->Cfg.Alias, buf);
        } 

        /*  to get Device.Routing.RIP.InterfaceSetting.{0}.Enable */
        Utopia_GetBool(&Ctx, UtopiaValue_RIP_InterfaceLAN, (boolean_t *)&pEntry->Cfg.Enable);

        /*  to get Device.Routing.RIP.InterfaceSetting.{0}.Interface */
        rc = g_GetParamValueString(g_pDslhDmlAgent, "com.cisco.spvtg.ccsp.pam.Helper.FirstUpstreamIpInterface", param_val, &val_len);
fprintf(stderr, "%s -- %d , %s\n", __FUNCTION__, __LINE__, param_val);
        AnscCopyString(pEntry->Cfg.Interface, param_val);

        /*  to get Device.Routing.RIP.InterfaceSetting.{0}.AcceptRA */
        AnscZeroMemory(buf, 64);
        syscfg_get(NULL, "rip_recv_lan", buf, sizeof(buf));
 
        pEntry->Cfg.AcceptRA = (buf[0] == '1')? TRUE : FALSE;
        
        /*  to get Device.Routing.RIP.InterfaceSetting.{0}.SendRA */
        AnscZeroMemory(buf, 64);
        syscfg_get(NULL, "rip_send_lan", buf, sizeof(buf));

        pEntry->Cfg.SendRA = (buf[0] == '1')? TRUE : FALSE;
    }
    else if ( 1 == ulIndex )
    {
        /* to get WAN interface setting for rip */
        AnscZeroMemory(buf, 64);
        rc = Utopia_RawGet(&Ctx, NULL, "tr_rip_insnum_wan", buf, sizeof(buf)); 
        
        if ( 1 == rc )
        {
            pEntry->Cfg.InstanceNumber = atoi(buf);
        }

        AnscZeroMemory(buf, 64);
        rc = Utopia_RawGet(&Ctx, NULL, "tr_rip_alias_wan", buf, sizeof(buf));  
        
        if ( 1 == rc )
        {
            AnscCopyString(pEntry->Cfg.Alias, buf);
        } 

        /*  to get Device.Routing.RIP.InterfaceSetting.{1}.Enable */
        Utopia_GetBool(&Ctx, UtopiaValue_RIP_InterfaceWAN, (boolean_t *)&pEntry->Cfg.Enable);

        /*  to get Device.Routing.RIP.InterfaceSetting.{1}.Interface */
        rc = g_GetParamValueString(g_pDslhDmlAgent, "com.cisco.spvtg.ccsp.pam.Helper.FirstDownstreamIpInterface", param_val, &val_len);
fprintf(stderr, "%s -- %d , %s\n", __FUNCTION__, __LINE__, param_val);
        AnscCopyString(pEntry->Cfg.Interface, param_val);

        /*  to get Device.Routing.RIP.InterfaceSetting.{1}.AcceptRA */
        AnscZeroMemory(buf, 64);
        syscfg_get(NULL, "rip_recv_wan", buf, sizeof(buf));
        
        pEntry->Cfg.AcceptRA = (buf[0] == '1')? TRUE : FALSE;
        
        /*  to get Device.Routing.RIP.InterfaceSetting.{1}.SendRA */
        AnscZeroMemory(buf, 64);
        syscfg_get(NULL, "rip_send_wan", buf, sizeof(buf));
        
        pEntry->Cfg.SendRA = (buf[0] == '1')? TRUE : FALSE;
    }

    Utopia_Free(&Ctx,0);

    return ANSC_STATUS_SUCCESS;
}    


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipGetNumberOfIfEntries
            (
                ANSC_HANDLE                    hContext
            );


    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                            hContext,
                

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    char                            buf[64]      = {0};
    UtopiaContext                   Ctx;
    errno_t safec_rc = -1;

    if ( (NULL == pAlias) || 
         (0 == ulInstanceNumber)|| (ulIndex >= g_NumOfRipEntry) )
    {
        CcspTraceWarning(("%s Error Input\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }    

    if ( !Utopia_Init(&Ctx) )
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    
    CcspTraceWarning(("-----CosaDmlRipIfSetValues %d insNumber %d, alias %s\n", 
                ulIndex, ulInstanceNumber, pAlias));

    safec_rc = sprintf_s(buf, sizeof(buf), "%d", ulInstanceNumber);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }

    if ( 0 == ulIndex )
    {
        Utopia_RawSet(&Ctx, NULL, "tr_rip_insnum_lan", buf);    
        Utopia_RawSet(&Ctx, NULL, "tr_rip_alias_lan", pAlias);
    }
    else if ( 1 == ulIndex )
    {
        Utopia_RawSet(&Ctx, NULL, "tr_rip_insnum_wan", buf);    
        Utopia_RawSet(&Ctx, NULL, "tr_rip_alias_wan", pAlias);
    }
    
    Utopia_Free(&Ctx, 1);
   
    return returnStatus;
}   

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipIfAddEntry
            (
                ANSC_HANDLE                      hContext,
                PCOSA_DML_RIP_IF_CFG        pEntry
            )



    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                      hContext,
                
                PCOSA_DML_RIP_IF_CFG        pEntry                  

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    
    return returnStatus;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipIfDelEntry
            (
                ANSC_HANDLE                    hContext,
                PCOSA_DML_RIP_IF_CFG      pEntry     
            )


    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                      hContext,
                
                PCOSA_DML_RIP_IF_CFG        pEntry  

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry     
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipIfSetCfg
            (
                ANSC_HANDLE                      hContext,
                PCOSA_DML_RIP_IF_CFG        pEntry     
            )

    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                      hContext,

                PCOSA_DML_RIP_IF_CFG        pEntry 

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry     
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    BOOLEAN                         bRipEnable   = FALSE;
    char                            buf[64]      = {0};
    UtopiaContext                   Ctx;
    errno_t   safec_rc = -1;

    if ( !pEntry ) 
    {
        CcspTraceWarning(("%s Error Input\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    if ( !Utopia_Init(&Ctx) )
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

     /* set cfg into backend to trigger the state machine */
    if ( 1 == pEntry->InstanceNumber )
    {
        UTOPIA_SETBOOL(&Ctx, UtopiaValue_RIP_InterfaceLAN, pEntry->Enable);

        /*  to set Device.Routing.RIP.InterfaceSetting. insnum */
        AnscZeroMemory(buf, 64);
        safec_rc = sprintf_s(buf, sizeof(buf), "%d", pEntry->InstanceNumber);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        Utopia_RawSet(&Ctx, NULL, "tr_rip_insnum_lan", buf);    

        /*  to set Device.Routing.RIP.InterfaceSetting.{0}.Alias */
        Utopia_RawSet(&Ctx, NULL, "tr_rip_alias_lan", pEntry->Alias);

        /*  to set Device.Routing.RIP.InterfaceSetting.{0}.AcceptRA */
        AnscZeroMemory(buf, 64);                
        safec_rc = sprintf_s(buf, sizeof(buf), "%d", pEntry->AcceptRA);        
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        syscfg_set(NULL, "rip_recv_lan", buf);
        
        /*  to set Device.Routing.RIP.InterfaceSetting.{0}.SendRA */
        AnscZeroMemory(buf, 64);                
        safec_rc = sprintf_s(buf, sizeof(buf), "%d", pEntry->SendRA);        
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        syscfg_set(NULL, "rip_send_lan", buf);
    }
    else if ( 2 == pEntry->InstanceNumber )
    {
        UTOPIA_SETBOOL(&Ctx, UtopiaValue_RIP_InterfaceWAN, pEntry->Enable);

        /*  to set Device.Routing.RIP.InterfaceSetting. insnum */
        AnscZeroMemory(buf, 64);
        safec_rc = sprintf_s(buf, sizeof(buf), "%d", pEntry->InstanceNumber);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        Utopia_RawSet(&Ctx, NULL, "tr_rip_insnum_wan", buf);    

        /*  to set Device.Routing.RIP.InterfaceSetting.{1}.Alias */
        Utopia_RawSet(&Ctx, NULL, "tr_rip_alias_wan", pEntry->Alias);

        /*  to set Device.Routing.RIP.InterfaceSetting.{1}.AcceptRA */
        AnscZeroMemory(buf, 64);                
        safec_rc = sprintf_s(buf, sizeof(buf), "%d", pEntry->AcceptRA);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        
        syscfg_set(NULL, "rip_recv_wan", buf);
        
        /*  to set Device.Routing.RIP.InterfaceSetting.{1}.SendRA */
        AnscZeroMemory(buf, 64);                
        safec_rc = sprintf_s(buf, sizeof(buf), "%d", pEntry->SendRA);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        
        syscfg_set(NULL, "rip_send_wan", buf);
    }  
    
    Utopia_Free(&Ctx, 1); 
    
    return ANSC_STATUS_SUCCESS;
}    

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipIfGetCfg
            (
                ANSC_HANDLE                  hContext,
                PCOSA_DML_RIP_IF_CFG    pEntry     
            )

    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                     hContext
                
                PCOSA_DML_RIP_IF_CFG        pEntry 

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry     
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    ULONG                           ulIndex      = 0;
    COSA_DML_RIP_IF_FULL            sEntryFull;

    if ( !pEntry ) 
    {
        CcspTraceWarning(("%s Error Input\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    ulIndex = pEntry->InstanceNumber - 1;
    
    returnStatus = CosaDmlRipIfGetEntry(NULL, ulIndex, &sEntryFull);

    if ( ANSC_STATUS_SUCCESS == returnStatus )
    {
        AnscCopyMemory(pEntry, &(sEntryFull.Cfg), sizeof(COSA_DML_RIP_IF_CFG));
    
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_CANT_FIND;
}    

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRipIfGetInfo
            (
                ANSC_HANDLE                     hContext,
                ULONG                                ulInstanceNumber,
                PCOSA_DML_RIP_IF_INFO       pInfo     
            );

    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                      hContext
                
                ULONG                                ulInstanceNumber
                
                PCOSA_DML_RIP_IF_INFO       pInfo 
                

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRipIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_RIP_IF_INFO       pInfo     
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    char                            buf[64]      = {0};
    token_t                         se_token;
    int                             se_fd = s_sysevent_connect(&se_token);
    UtopiaContext                   Ctx;
    BOOLEAN                         bLanEnable = FALSE;
    BOOLEAN                         bWanEnable = FALSE;

    pInfo->Status = COSA_DML_STATUS_Disabled;

    if ( se_fd < 0 )
    {
        CcspTraceWarning(("%s -- failed to connect to sysevent!\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    if ( !Utopia_Init(&Ctx) )
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    
    sysevent_get(se_fd, se_token, "rip-status", buf, sizeof(buf));

    if ( !_ansc_strncmp(buf, "started", _ansc_strlen(buf)))
    {
        if ( 1 == ulInstanceNumber )
        {
            Utopia_GetBool(&Ctx, UtopiaValue_RIP_InterfaceLAN, (boolean_t *)&bLanEnable);
            if ( TRUE == bLanEnable )
            {
                pInfo->Status = COSA_DML_STATUS_Enabled;
            }
        }
        else if ( 2 == ulInstanceNumber )
        {
            Utopia_GetBool(&Ctx, UtopiaValue_RIP_InterfaceWAN, (boolean_t *)&bWanEnable);
            if ( TRUE == bWanEnable )
            {
                pInfo->Status = COSA_DML_STATUS_Enabled;
            }
        }     
    }

    /*
     *  it appears we cannot close sysevent
     *
    sysevent_close(se_fd, se_token);
     */

    Utopia_Free(&Ctx, 0); 
        
    return returnStatus;
}    

#endif

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingGetV4Entry2
            (
                ANSC_HANDLE                           hContext,
                PCOSA_DML_ROUTING_V4_ENTRY  pEntry      
            )

    description:

        This routine is to retrieve the config of Routing ipv4forwarding.

    argument:  
                    ANSC_HANDLE                          hContext,

                   PCOSA_DML_ROUTING_V4_ENTRY  pEntry  

    return:    operation status.  

**********************************************************************/

ANSC_STATUS
CosaDmlRoutingGetV4Entry2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry      
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRoutingRemove
            (
                ANSC_HANDLE                     hContext,
                ULONG                                ulInstanceNumber,
                PCOSA_DML_RIP_IF_INFO       pInfo     
            );

    Description:
    
          This function set the router rip configuration.
        
    Arguments:
                ANSC_HANDLE                      hContext
                
                ULONG                                ulInstanceNumber
                
                PCOSA_DML_RIP_IF_INFO       pInfo 
                

    Return:
        

**********************************************************************/
ANSC_STATUS
CosaDmlRoutingRemove
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hContext);   
    if (sroute != NULL)
    {
        free(sroute);
    }

    Route6_UnloadRouteInfo();

    return returnStatus;
}    

static BOOLEAN g_routeinfo_enabled = FALSE; 

#define SYSCFG_FORMAT_IPV6_ROUTEINFO "tr_ipv6_routeinfo"

static int _set_routeinfo_to_sysctl(BOOLEAN bEnabled)
{
    if (bEnabled)
    {
        /*Enable kernel by modify sysctl params*/
        _write_sysctl_file("/proc/sys/net/ipv6/conf/"COSA_DML_ROUTEINFO_IFNAME"/forwarding", 0);
        _write_sysctl_file("/proc/sys/net/ipv6/conf/"COSA_DML_ROUTEINFO_IFNAME"/accept_ra_rtr_pref", 1);
        _write_sysctl_file("/proc/sys/net/ipv6/conf/"COSA_DML_ROUTEINFO_IFNAME"/accept_ra_rt_info_max_plen", 128);
    }
    else 
    {
        /*Disable kernel by modify sysctl params*/
        _write_sysctl_file("/proc/sys/net/ipv6/conf/"COSA_DML_ROUTEINFO_IFNAME"/accept_ra_rtr_pref", 0);
    }    
    return 0;
}

static int
CosaDmlRouteInfoInit
    (
        void
    )
{
    UtopiaContext utctx = {0};
    char out[256] = {0};
    
    if (!Utopia_Init(&utctx))
        return ANSC_STATUS_FAILURE;

    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_IPV6_ROUTEINFO"_enabled",out,sizeof(out));

    if (!out[0])
        g_routeinfo_enabled = FALSE;
    else
        g_routeinfo_enabled = (out[0] == '1') ? TRUE:FALSE;
    
    _set_routeinfo_to_sysctl(g_routeinfo_enabled);

    Utopia_Free(&utctx,0); 
    return 0;                   
}

ANSC_STATUS
CosaDmlRouteInfoSetEnabled
    (
        BOOLEAN value
    )
{
    UtopiaContext utctx = {0};
    char out[256] = {0};
    errno_t safec_rc = -1;
	
    /*handle syscfg*/
    if (!Utopia_Init(&utctx))
        return ANSC_STATUS_FAILURE;

    if (value)
    {
        safec_rc = sprintf_s(out, sizeof(out), "1");
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
    }
    else
    {
        safec_rc = sprintf_s(out, sizeof(out), "0");
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
    }
    Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_IPV6_ROUTEINFO"_enabled",out);

    Utopia_Free(&utctx,1);                    

    if (g_routeinfo_enabled != value)
        _set_routeinfo_to_sysctl(value);
    
    g_routeinfo_enabled = value;
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRouteInfoGetEnabled
    (
        BOOLEAN * pBool
    )
{
    *pBool = g_routeinfo_enabled;

    return ANSC_STATUS_SUCCESS;
}

/*
 *  Device.Routing.RouteInformation.
 */
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRoutingGetRouteInfoIf
           (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount

           );

    Description:
    
        This routine is to retrieve the complete list of currently routeinfo interfaceSettings,
        which is a dynamic table.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PULONG                      pulCount
    Return:
        The pointer to the array of routeInfo interfaceSettings, allocated by callee. 
        If no entry is found, NULL is returned.

**********************************************************************/
#define IPV6_ROUTEINFO_FILE "/proc/net/ipv6_routeinfo"
/*this file is in the format of 
fe80:0:0:0:a00:27ff:fe10:d6ac 64 00 20:03:00:00:00:00:00:00:
which means:
route_src_addr, prefix_len, PreferredRouteFlag(%02x), prefix*/
static int _routeinfo_get_prefix(char * in, int pref_len, char * out, unsigned int out_size)
{
    char * str = NULL;
    char * p_token = NULL;
    char * saveptr = NULL;
    int    val = 0;
    struct in6_addr addr6;
    int    i = 0;
    int    num = 0;
    char   addr_str[64] = {0};
    errno_t safec_rc = -1;
    
    num = pref_len%8 ? (pref_len/8+1):pref_len/8 ;
    num = (num > 16) ? 16:num;

    for (str = in; i<num ; str = NULL) 
    {
        p_token = strtok_r(str, ":", &saveptr);
        if (p_token == NULL)
            break;

        sscanf(p_token, "%02x", &val);
        addr6.s6_addr[i++] = val;
    }

    inet_ntop(AF_INET6, &addr6, addr_str, sizeof(addr_str));
    safec_rc = strcpy_s(out, out_size, addr_str);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
       return -1;
    }
    str = out + strlen(out);
    snprintf(str, sizeof(out_size)-strlen(out), "/%d", pref_len);

    return 0;
}

int _get_datetime_offset(int offset, char * buf, int len);
#define TMP_ROUTEINFO_OUTPUT  "/tmp/.routeinfo_out"
static int _routeinfo_get_lft(char * prefix, char * lft_str, int lft_str_size)
{
    FILE  * fp = NULL;
    char  line[256] = {0};
    char  buf[64] = {0};
    char  * p_str = NULL;
    int   expire_time = 0;
    int   found = 0;
    errno_t  safec_rc = -1;

    v_secure_system("ip -6 route show > " TMP_ROUTEINFO_OUTPUT);

    fp = fopen(TMP_ROUTEINFO_OUTPUT, "r");
    if (fp)
    {
        while (fgets(line, sizeof(line), fp))
        {
            safec_rc = sprintf_s(buf, sizeof(buf), "dev %s ", COSA_DML_ROUTEINFO_IFNAME);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            if (strstr(line, buf) && strstr(line, prefix) && strstr(line, "expires"))
            {
                p_str = strstr(line, "expires");
                
                if (sscanf(p_str, "expires %dsec", &expire_time) == 1)
                {
                    _get_datetime_offset(expire_time, lft_str, lft_str_size);
                    found = 1;
                    break;
                }
            }
            
        }

        fclose(fp);
    }

    /*CID:65634 Unchecked return value from library*/
    if(remove(TMP_ROUTEINFO_OUTPUT) != 0)
     CcspTraceWarning (("%s: Unable to delete a file.", __FUNCTION__));

    return found;
}

PCOSA_DML_ROUTEINFO_IF_INFO
CosaDmlRoutingGetRouteInfoIf
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    PCOSA_DML_ROUTEINFO_IF_INFO pEntry = NULL;
    PCOSA_DML_ROUTEINFO_IF_INFO pEntry1 = NULL;
    int                         num = 0;
    FILE                      * fp = fopen(IPV6_ROUTEINFO_FILE, "r+");
    char                        route_addr[64] = {0};
    int                         pref_route_flag = 0;
    int                         pref_len = 0;
    char                        prefix[64] = {0};
    char                        line[1024] = {0};
    errno_t  safec_rc = -1;
    
    UNREFERENCED_PARAMETER(hContext);
    if (!fp)
    {
        *pulCount = 0;        
        return NULL;
    }

    while(fgets(line, sizeof(line)-1, fp))
    {
        char * p1 = NULL;

        /*sanity check*/
        if ((p1 = strchr(line, ' '))) 
        {
            if (((ULONG)(p1-line)) >= sizeof(route_addr))
                continue;
        }
        else 
            continue;
        
        if ((p1 = strrchr(line, ' '))) 
        {
            if ((ULONG)(line+strlen(line)-p1) >= sizeof(prefix))
                continue;
        }
        else 
            continue;
        

        if (sscanf(line, "%s %d %02x %s", 
                   route_addr,
                   &pref_len,
                   &pref_route_flag,
                   prefix) != 4)
            continue;

        if (pref_route_flag == 0x10)
            continue;
        
        num++;
        
        pEntry = realloc(pEntry, num*sizeof(*pEntry));

        pEntry1 = pEntry + num-1;
        
        safec_rc = strcpy_s(pEntry1->SourceRouter, sizeof(pEntry1->SourceRouter), route_addr);
        if(safec_rc != EOK)
        {
           ERR_CHK(safec_rc);
        }

        if (pref_route_flag == 0x00)
            pEntry1->PreferredRouteFlag = COSA_DML_ROUTEINFO_PRF_Medium;
        else if (pref_route_flag == 0x01)
            pEntry1->PreferredRouteFlag = COSA_DML_ROUTEINFO_PRF_High;
        else 
            pEntry1->PreferredRouteFlag = COSA_DML_ROUTEINFO_PRF_Low;

        
        _routeinfo_get_prefix(prefix, pref_len, pEntry1->Prefix, sizeof(pEntry1->Prefix));

        if (_routeinfo_get_lft(pEntry1->Prefix, pEntry1->RouteLifetime, sizeof(pEntry1->RouteLifetime)))
            pEntry1->Status = COSA_DML_ROUTEINFO_STATUS_ForwardingEntryCreated;
        else
            pEntry1->Status = COSA_DML_ROUTEINFO_STATUS_NoForwardingEntry;
        
        memset(line, 0, sizeof(line));
    }

    *pulCount = num;    

    fclose(fp);

    return pEntry;
}

#endif
