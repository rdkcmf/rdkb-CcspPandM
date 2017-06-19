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

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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
#include "cosa_apis.h"
#include "cosa_routing_dml.h"
#include "cosa_routing_apis.h"
#include "cosa_routing_internal.h"

extern void* g_pDslhDmlAgent;

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
        sprintf(oneLine, " %s %s \n\0", pLink->name, pLink->value);
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
    
    if ( !pHead )
        utopia_init();

    //UserAcquireLock(&gPsmLock);

    if ( domainname == NULL )
    {
        _ansc_sprintf(fullname, "%s", pRecordName);
    }
    else
    {
        _ansc_sprintf(fullname, "%s%s", domainname, pRecordName);
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

    _ansc_sprintf(fullname, "%s%s", domainname, pRecordName);

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
    30,
    180,
    120,
    1,
    1,

    /*If1*/
    TRUE,
    FALSE,
    TRUE,
    COSA_RIPD_IF1_NAME,
    COSA_RIPD_IF1_NAME,
    "cisco1",
    "cisco1",
    "keychain1",
    COSA_RIP_AUTHEN_NONE, /*COSA_RIP_AUTHEN_TEXT,*/
    1,
    COSA_RIP_VERSION_1_2,
    COSA_RIP_VERSION_1_2,
    0x0,
    
};

COSA_DML_RIPD_CONF CosaDmlRIPCurrentConfig = {0};


void _get_shell_output3(char * cmd, char * out, int len)
{
    FILE * fp;
    char   buf[256];
    char * p;

    fp = popen(cmd, "r");

    if (fp)
    {
        fgets(buf, sizeof(buf), fp);
        
        /*we need to remove the \n char in buf*/
        if ((p = strchr(buf, '\n'))) *p = 0;

        strncpy(out, buf, len-1);

        pclose(fp);        
    }

}

#define RIPD_PID_FILE "/var/ripd.pid"
static int CosaRipdOperation(char * arg)
{
    char cmd[256] = {0};
    char out[256] = {0};
    ULONG Index  = 0;
    
    if (!strncmp(arg, "stop", 4))
    {
        /* Zebra's configuration is controlled by Utopia. So we need not restart this one */
        /*
        sprintf(cmd, "killall %s", basename(COSA_ZEBRA_BIN));
        system(cmd);
         */

        sprintf(cmd, "kill `cat %s`", RIPD_PID_FILE);
        system(cmd);
    }
    else if (!strncmp(arg, "start", 5))
    {
        /* We must be sure Dibbler-server is not up currently.
                    If it is up, we need not start it. */
        sprintf(cmd, COSA_DML_CMD_PS, basename(COSA_ZEBRA_BIN));
        _get_shell_output3(cmd, out, sizeof(out));
        if ( !strstr(out, basename(COSA_ZEBRA_BIN)))
        {
            sprintf(cmd, "%s -d -f %s -u root -g root -i %s &", COSA_RIPD_BIN, COSA_RIPD_CUR_CONF, RIPD_PID_FILE);
            AnscTraceWarning(("CosaRipdOperation -- run cmd:%s\n", cmd));
            system(cmd);
        }

        sprintf(cmd, COSA_DML_CMD_PS, basename(COSA_RIPD_BIN));
        _get_shell_output3(cmd, out, sizeof(out));
        if ( !strstr(out, basename(COSA_RIPD_BIN)))
        {
            if ( CosaDmlRIPCurrentConfig.Enable )
            {
                sprintf(cmd, "%s -d -f %s -u root -g root", COSA_RIPD_BIN, COSA_RIPD_CUR_CONF);
                AnscTraceWarning(("CosaRipdOperation -- run cmd:%s\n", cmd));
                system(cmd);
            }
        }
    }
    else if (!strncmp(arg, "restart", 7))
    {
        CosaRipdOperation("stop");
        CosaRipdOperation("start");
    }
    else if (!strncmp(arg, "update", 6))
    {
        sprintf(cmd, "killall -SIGHUP %s", basename(COSA_RIPD_BIN));
        system(cmd);
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
    pEntry->Enable = Value1[0]?TRUE:FALSE;
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
    CHAR  Version[128]        = {0};
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
    
    if ( inet_aton(Address, &addr) ){
        ulAddr = addr.s_addr;
        while( ulAddr & 0x80000000 ){
            bits++;
            ulAddr = ulAddr<<1;
        }
    }

    return bits;
}

/* Generate configuration file */
void CosaDmlGenerateRipdConfigFile(ANSC_HANDLE  hContext )
{
    PCOSA_DML_RIPD_CONF pConf = &CosaDmlRIPCurrentConfig;
    FILE * fp                 = fopen(COSA_RIPD_TMP_CONF, "w+");
    char *pstaticRoute        = NULL;
    char cmd[256]             = {0};
    if (fp)
    {
        /*we need this to get IANA IAPD info from dibbler*/
        fprintf(fp, "!\n");
        fprintf(fp, "hostname Router\n");
        fprintf(fp, "password cisco\n");
        fprintf(fp, "log stdout\n");
        fprintf(fp, "!\n");
        fprintf(fp, " debug rip events\n");
        fprintf(fp, " debug rip packet\n");
        fprintf(fp, "!\n");
        
        if (( _ansc_strlen(pConf->If1KeyChainName) > 0 ) && ( _ansc_strlen(pConf->If1Md5KeyValue) > 0 ))
        {
            fprintf(fp, "key chain %s\n", pConf->If1KeyChainName);
            fprintf(fp, " key %lu\n", pConf->If1KeyID);
            fprintf(fp, "  key-string %s\n", pConf->If1Md5KeyValue);
        }
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
            }
            fprintf(fp, " ip rip split-horizon\n");
            fprintf(fp, "!\n");
        }

        fprintf(fp, "router rip\n");
        fprintf(fp, " version %s\n", (pConf->Version==COSA_RIP_VERSION_1)?"1":((pConf->Version==COSA_RIP_VERSION_2)?"2":"2") );
        fprintf(fp, " timers basic %lu %lu %lu\n", pConf->UpdateTime, pConf->TimoutTime, pConf->CollectionTime);
        fprintf(fp, " default-metric %lu\n", pConf->DefaultMetric );
        fprintf(fp, " no redistribute connected\n");

        if ( pConf->Enable && pConf->If1Enable && (_ansc_strlen(pConf->If1Name) > 0 ))
        {
            fprintf(fp, " network %s\n", pConf->If1Name);
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

        /*this part is for distribute static route ending */

        if ( pConf->Enable && pConf->If1Enable && !pConf->If1SendEnable && (_ansc_strlen(pConf->If1Name) > 0) )
            fprintf(fp, " distribute-list 1 out %s\n", pConf->If1Name );
        if ( pConf->Enable && pConf->If1Enable && !pConf->If1ReceiveEnable && (_ansc_strlen(pConf->If1Name) > 0) )
            fprintf(fp, " distribute-list 1 in %s\n", pConf->If1Name );

        fprintf(fp, "!\n");
        fprintf(fp, "access-list 1 deny any\n");
        fprintf(fp, "!\n");
        fprintf(fp, " line vty\n");
        fprintf(fp, " exec-timeout 0 0\n");

        fclose(fp);

    }
    
    sprintf(cmd, "cp "COSA_RIPD_TMP_CONF"  "COSA_RIPD_CUR_CONF );
    AnscTraceWarning(("Run command:%s\n", cmd));
    system(cmd);

    return;
}

void CosaDmlGenerateZebraConfigFile()
{
    PCOSA_DML_RIPD_CONF pConf = &CosaDmlRIPCurrentConfig;
    FILE * fp                 = fopen(COSA_ZEBRA_CUR_CONF, "r");
    char cmd[256]             = {0};

    if ( fp == NULL )
    {
        fp                 = fopen(COSA_ZEBRA_TMP_CONF, "w+");

        if (fp)
        {
            /*we need this to get IANA IAPD info from dibbler*/
            fprintf(fp, "!\n");
            fprintf(fp, "hostname Router\n");
            fprintf(fp, "password cisco\n");

            fclose(fp);

            sprintf(cmd, "cp  "COSA_ZEBRA_TMP_CONF"  "COSA_ZEBRA_CUR_CONF );
            AnscTraceWarning(("Run command:%s\n", cmd));
            system(cmd);
        }
    }
    else
    {
        fclose(fp);
    }

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

    CosaDmlRIPCurrentConfig.Enable        = pCfg->Enable;
    CosaDmlRIPCurrentConfig.UpdateTime    = pCfg->X_CISCO_COM_UpdateInterval;
    CosaDmlRIPCurrentConfig.DefaultMetric = pCfg->X_CISCO_COM_DefaultMetric;

#if 1
    AnscTraceWarning(("CosaDmlRIPCurrentConfig.Enable :%s\n", CosaDmlRIPCurrentConfig.Enable?"TRUE":"FALSE"));    
    AnscTraceWarning(("CosaDmlRIPCurrentConfig.X_CISCO_COM_UpdateInterval :%lu\n", CosaDmlRIPCurrentConfig.UpdateTime));
    AnscTraceWarning(("CosaDmlRIPCurrentConfig.DefaultMetric :%lu\n", CosaDmlRIPCurrentConfig.DefaultMetric));
#endif

    CosaDmlSaveRipdConfiguration();
    CosaDmlGenerateZebraConfigFile();
    CosaDmlGenerateRipdConfigFile(NULL);
    CosaRipdOperation("restart");
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
        pEntry->Status                       = ( pConf->Enable && pConf->If1Enable ) ? 2 : 1;                           
        pEntry->X_CISCO_COM_Neighbor         = pConf->If1Neighbor;

        pEntry->X_CISCO_COM_AuthenticationType   = pConf->If1AuthenticateType;                      
        pEntry->X_CISCO_COM_Md5KeyID             = pConf->If1KeyID;                           
        AnscCopyString(pEntry->X_CISCO_COM_Md5KeyValue, pConf->If1Md5KeyValue);
        AnscCopyString(pEntry->X_CISCO_COM_SimplePassword, pConf->If1SimplePassword);
        
        AnscCopyString(pEntry->Alias, pConf->If1Alias);
        AnscCopyString(pEntry->Interface, pConf->If1Name);  
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
    AnscTraceWarning(("pEntry->X_CISCO_COM_Neighbor :0X%x\n", pEntry->X_CISCO_COM_Neighbor));

    AnscTraceWarning(("pEntry->X_CISCO_COM_AuthenticationType :%lu\n", pEntry->X_CISCO_COM_AuthenticationType));    
    AnscTraceWarning(("pEntry->X_CISCO_COM_Md5KeyID :%lu\n", pEntry->X_CISCO_COM_Md5KeyID));
    AnscTraceWarning(("pEntry->X_CISCO_COM_Md5KeyValue :%s\n", pEntry->X_CISCO_COM_Md5KeyValue));
    AnscTraceWarning(("pEntry->X_CISCO_COM_SimplePassword :%s\n", pEntry->X_CISCO_COM_SimplePassword));
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

    if ( pEntry->InstanceNumber == 1 )
    {
        pConf->If1Enable           = pEntry->Enable;        
        pConf->If1SendVersion      = pEntry->X_CISCO_COM_SendVersion;      
        pConf->If1ReceiveVersion   = pEntry->X_CISCO_COM_ReceiveVersion;      
        pConf->If1SendEnable       = pEntry->SendRA;      
        pConf->If1ReceiveEnable    = pEntry->AcceptRA;      
        pConf->If1Neighbor         = pEntry->X_CISCO_COM_Neighbor;

        pConf->If1AuthenticateType = pEntry->X_CISCO_COM_AuthenticationType;                      
        pConf->If1KeyID            = pEntry->X_CISCO_COM_Md5KeyID;                           
        AnscCopyString(pConf->If1Md5KeyValue, pEntry->X_CISCO_COM_Md5KeyValue );
        AnscCopyString(pConf->If1SimplePassword, pEntry->X_CISCO_COM_SimplePassword );

        //this is for true static ip feature.
        if ( pEntry->X_CISCO_COM_AuthenticationType == COSA_RIP_AUTHEN_md5 )
            pConf->If1AuthenticateType = COSA_RIP_AUTHEN_MD5;                      

        AnscCopyString(pConf->If1Alias, pEntry->Alias);
        if ((_ansc_strcmp(pEntry->Alias, COSA_RIPD_IF1_NAME ) == 0 ) ||
            (_ansc_strcmp(pEntry->Alias, "cpe" ) == 0 ) ||
            (_ansc_strcmp(pEntry->Alias, "Ethernet" ) == 0 ) )
            AnscCopyString(pConf->If1Name, COSA_RIPD_IF1_NAME);
        else
            AnscCopyString(pConf->If1Name, COSA_RIPD_IF2_NAME);

    }
    else
    {
        AnscTraceWarning(("This RIP interface is wrong:%d.", pEntry->InstanceNumber));
    }

#if 1
    AnscTraceWarning(("pConf->If1Enable :%s\n", pConf->If1Enable?"TRUE":"FALSE"));
    AnscTraceWarning(("pConf->If1SendVersion :%lu\n", pConf->If1SendVersion));
    AnscTraceWarning(("pConf->If1ReceiveVersion :%lu\n", pConf->If1ReceiveVersion));
    
    AnscTraceWarning(("pConf->If1SendEnable :%s\n", pConf->If1SendEnable?"TRUE":"FALSE"));
    AnscTraceWarning(("pConf->If1ReceiveEnable :%s\n", pConf->If1ReceiveEnable?"TRUE":"FALSE"));
    AnscTraceWarning(("pConf->If1Neighbor :0X%x\n", pConf->If1Neighbor));
    AnscTraceWarning(("pConf->If1AuthenticateType :%lu\n", pConf->If1AuthenticateType));
    AnscTraceWarning(("pConf->If1KeyID :%lu\n", pConf->If1KeyID));
    AnscTraceWarning(("pConf->If1Md5KeyValue :%s\n", pConf->If1Md5KeyValue));
    AnscTraceWarning(("pConf->If1SimplePassword :%s\n", pConf->If1SimplePassword));
    AnscTraceWarning(("pConf->If1Alias :%s\n", pConf->If1Alias));
    AnscTraceWarning(("pConf->If1Name :%s\n", pConf->If1Name));
#endif

    CosaDmlSaveRipdConfiguration();
    CosaDmlGenerateZebraConfigFile();
    CosaDmlGenerateRipdConfigFile(NULL);
    CosaRipdOperation("restart");

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
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS 
CosaDmlStaticRouteDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS 
CosaDmlStaticRouteAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    return ANSC_STATUS_FAILURE;
}
ANSC_STATUS 
CosaDmlStaticRouteSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
  return ANSC_STATUS_FAILURE;
}

/*
 *  Routing Router -- assuming there is only one router in the system
 */
ANSC_HANDLE g_RoutingEntryInMiddleLayer = NULL;
ANSC_STATUS CosaDmlRipCallBack
    (
        ANSC_HANDLE                 hContext
    )
{
    CosaDmlGenerateRipdConfigFile(g_RoutingEntryInMiddleLayer);
    CosaRipdOperation("restart");

    return ANSC_STATUS_SUCCESS;
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
                ANSC_HANDLE                     hDml,
                
               PANSC_HANDLE                     phContext

    Return:
        

**********************************************************************/
static BOOLEAN g_routeinfo_enabled = TRUE; 

static int
CosaDmlRouteInfoInit
    (
        void
    )
{
    g_routeinfo_enabled = FALSE;

    return  0;
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

    CosaDmlRouteInfoInit();
    CosaDmlGetRipdConfiguration();
    CosaDmlGenerateZebraConfigFile();

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
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    g_RouterFull.Cfg.InstanceNumber = pCfg->InstanceNumber;  
    g_RouterFull.Cfg.bEnabled       = pCfg->bEnabled;        

    AnscCopyString(g_RouterFull.Cfg.Alias, pCfg->Alias);

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

    pCfg->InstanceNumber = g_RouterFull.Cfg.InstanceNumber; 
    pCfg->bEnabled       = g_RouterFull.Cfg.bEnabled;

    AnscCopyString(pCfg->Alias, g_RouterFull.Cfg.Alias);
    
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

    pEntry->InstanceNumber   = g_RouterFull.V4ForwardList[ulIndex].InstanceNumber;
    pEntry->Enable           = g_RouterFull.V4ForwardList[ulIndex].Enable;
    pEntry->Status           = g_RouterFull.V4ForwardList[ulIndex].Status;
    pEntry->StaticRoute      = g_RouterFull.V4ForwardList[ulIndex].StaticRoute;
    pEntry->ForwardingPolicy = g_RouterFull.V4ForwardList[ulIndex].ForwardingPolicy;
    pEntry->ForwardingMetric = g_RouterFull.V4ForwardList[ulIndex].ForwardingMetric;

    pEntry->DestIPAddress.Value    = g_RouterFull.V4ForwardList[ulIndex].DestIPAddress.Value;
    pEntry->DestSubnetMask.Value   = g_RouterFull.V4ForwardList[ulIndex].DestSubnetMask.Value;
    pEntry->GatewayIPAddress.Value = g_RouterFull.V4ForwardList[ulIndex].GatewayIPAddress.Value;
    
    AnscCopyString(pEntry->Alias, g_RouterFull.V4ForwardList[ulIndex].Alias);
    AnscCopyString(pEntry->Interface, g_RouterFull.V4ForwardList[ulIndex].Interface);
    
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

    g_RouterFull.V4ForwardList[ulIndex].InstanceNumber = ulInstanceNumber;
    AnscCopyString(g_RouterFull.V4ForwardList[ulIndex].Alias, pAlias);
        
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

    ulIndex = g_RouterFull.ulNumOfForward;

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
    
    AnscCopyString(g_RouterFull.V4ForwardList[ulIndex].Alias, pEntry->Alias);
    AnscCopyString(g_RouterFull.V4ForwardList[ulIndex].Interface, pEntry->Interface);

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
    ULONG                           i       = 0;

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
            
            AnscCopyString(g_RouterFull.V4ForwardList[i].Alias, pEntry->Alias);
            AnscCopyString(g_RouterFull.V4ForwardList[i].Interface, pEntry->Interface);

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
   
    return returnStatus;
}

ULONG
CosaDmlRoutingGetNumberOfV6Entries
    (
        ANSC_HANDLE                 hContext
    )
{
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
    g_RouterFull.V6ForwardList[ulIndex].InstanceNumber = ulInstanceNumber;
    
    AnscCopyString(g_RouterFull.V6ForwardList[ulIndex].Alias, pAlias);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingAddV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry
    )
{
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
    
    return returnStatus;
}    

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

#endif
