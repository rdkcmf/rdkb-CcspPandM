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

    module: cosa_x_cisco_com_ddns_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "cosa_x_cisco_com_ddns_apis.h"

#if defined _COSA_INTEL_USG_ARM_ || defined _COSA_DRG_TPG_ ||_COSA_BCM_MIPS_
#include "utctx/utctx_api.h"
#include <utapi.h>
#include "dml_tr181_custom_cfg.h" 

COSA_DML_DDNS_SERVICE g_DdnsService[10] = 
{
    {FALSE, 1, "DdnsService1", "dyndns", "admin1", "admin1", "cisco.com", "", COSA_DML_DDNS_STATE_Idle},
    {FALSE, 2, "DdnsService2", "tzo",    "admin2", "admin2", "cisco.com", "", COSA_DML_DDNS_STATE_Idle},
    {FALSE, 3, "DdnsService3", "changeip", "admin3", "admin3", "cisco.com", "", COSA_DML_DDNS_STATE_Idle},
    {FALSE, 4, "DdnsService4", "afraid", "admin4", "admin4", "cisco.com", "", COSA_DML_DDNS_STATE_Idle}
};

ULONG    g_DdnsServiceNum = 4;
UCHAR    g_bEnabled       = TRUE;

static int loadID(char* ServiceName, ULONG* ulInstanceNumber, char* pAlias);
static int saveID(char* Namespace, char* ServiceName, ULONG ulInstanceNumber,char* pAlias);
#endif

#ifdef _COSA_SIM_

COSA_DML_DDNS_SERVICE g_DdnsService[10] = 
{
    {TRUE, 2, "DdnsService1", "DynDns.org", "admin1", "admin1", "cisco.com", "Conn1", COSA_DML_DDNS_STATE_Idle},
    {TRUE, 1, "DdnsService2", "TZO.com",    "admin2", "admin2", "cisco.com", "Conn2", COSA_DML_DDNS_STATE_Idle}
};

ULONG    g_DdnsServiceNum = 2;
UCHAR    bEnabled         = TRUE;


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
            );

        Description:
	        This is the initialization routine for XYZ backend.

        Arguments:      ANSC_HANDLE                 hDml
                        Opaque handle from DM adapter. 
        

	                    PANSC_HANDLE                phContext		
                        Opaque handle passed back from backend, 
                        needed by CosaDmlNatXyz() routines.

        Return:         Status of operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsGetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DDNS_CFG          pCfg
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_CFG          pCfg
    )
{
    pCfg->bEnabled = bEnabled;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsSetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DDNS_CFG          pCfg
            );

    Description:

        Backend implementation to set Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the configuration to be set.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsSetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_CFG          pCfg
    )
{
    bEnabled = pCfg->bEnabled;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsGetNumberOfServices
            (
                ANSC_HANDLE                 hContext,
            );

    Description:

        Backend implementation to get the number of Ddns servies.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

    Return:       The number of Ddns servies.

**********************************************************************/
ULONG
CosaDmlDdnsGetNumberOfServices
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_DdnsServiceNum;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsGetService
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_DDNS_SERVICE      pService
            );

    Description:

        Backend implementation to get a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsGetService
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DDNS_SERVICE      pService
    )
{
    pService->bEnabled = g_DdnsService[ulIndex].bEnabled;
    pService->InstanceNumber = g_DdnsService[ulIndex].InstanceNumber;
    AnscCopyString(pService->Alias, g_DdnsService[ulIndex].Alias);
    AnscCopyString(pService->ServiceName, g_DdnsService[ulIndex].ServiceName);
    AnscCopyString(pService->Username, g_DdnsService[ulIndex].Username);
    AnscCopyString(pService->Password, g_DdnsService[ulIndex].Password);
    AnscCopyString(pService->Domain, g_DdnsService[ulIndex].Domain);
    AnscCopyString(pService->AssociatedConnection, g_DdnsService[ulIndex].AssociatedConnection);
    pService->ConnectionState = g_DdnsService[ulIndex].ConnectionState;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsServiceSetValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            );

    Description:

        Backend implementation to set back instance number and alias.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsServiceSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    g_DdnsService[ulIndex].InstanceNumber = ulInstanceNumber;
    AnscCopyString(g_DdnsService[ulIndex].Alias, pAlias);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsGetServiceByInstNum
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber,
                PCOSA_DML_DDNS_SERVICE      pService
            );

    Description:

        Backend implementation to get a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulInstanceNumber
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_SERVICE      pService
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsGetServiceByInstNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DDNS_SERVICE      pService
    )
{
    ULONG                           ulIndex = 0;

    for ( ulIndex = 0; ulIndex < g_DdnsServiceNum; ulIndex++ )
    {
        if ( g_DdnsService[ulIndex].InstanceNumber == ulInstanceNumber )
        {
            pService->bEnabled = g_DdnsService[ulIndex].bEnabled;
            AnscCopyString(pService->Alias, g_DdnsService[ulIndex].Alias);
            AnscCopyString(pService->ServiceName, g_DdnsService[ulIndex].ServiceName);
            AnscCopyString(pService->Username, g_DdnsService[ulIndex].Username);
            AnscCopyString(pService->Password, g_DdnsService[ulIndex].Password);
            AnscCopyString(pService->Domain, g_DdnsService[ulIndex].Domain);
            AnscCopyString(pService->AssociatedConnection, g_DdnsService[ulIndex].AssociatedConnection);
            pService->ConnectionState = g_DdnsService[ulIndex].ConnectionState;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsAddService
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DDNS_SERVICE      pService
            );

    Description:

        Backend implementation to add a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be added.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsAddService
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_SERVICE      pService
    )
{
    g_DdnsService[g_DdnsServiceNum].bEnabled = pService->bEnabled;
    g_DdnsService[g_DdnsServiceNum].InstanceNumber = pService->InstanceNumber;
    AnscCopyString(g_DdnsService[g_DdnsServiceNum].Alias, pService->Alias);
    AnscCopyString(g_DdnsService[g_DdnsServiceNum].ServiceName, pService->ServiceName);
    AnscCopyString(g_DdnsService[g_DdnsServiceNum].Username, pService->Username);
    AnscCopyString(g_DdnsService[g_DdnsServiceNum].Password, pService->Password);
    AnscCopyString(g_DdnsService[g_DdnsServiceNum].Domain, pService->Domain);
    AnscCopyString(g_DdnsService[g_DdnsServiceNum].AssociatedConnection, pService->AssociatedConnection);
    g_DdnsService[g_DdnsServiceNum].ConnectionState = pService->ConnectionState;

    g_DdnsServiceNum++;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsDelService
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DDNS_SERVICE      pService
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  PCOSA_DML_DDNS_CFG         pCfg
                  Pointer of the service to be deleted.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsDelService
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    ULONG                           i = 0;
    ULONG                           j = 0;

    for ( i = 0; i < g_DdnsServiceNum; i++ )
    {
        if ( g_DdnsService[i].InstanceNumber == ulInstanceNumber )
        {
            for ( j = i; j < g_DdnsServiceNum; j++ )
            {
                g_DdnsService[j].bEnabled = g_DdnsService[j + 1].bEnabled;
                AnscCopyString(g_DdnsService[j].Alias,       g_DdnsService[j + 1].Alias);
                AnscCopyString(g_DdnsService[j].ServiceName, g_DdnsService[j + 1].ServiceName);
                AnscCopyString(g_DdnsService[j].Username,    g_DdnsService[j + 1].Username);
                AnscCopyString(g_DdnsService[j].Password,    g_DdnsService[j + 1].Password);
                AnscCopyString(g_DdnsService[j].Domain,      g_DdnsService[j + 1].Domain);
                AnscCopyString(g_DdnsService[j].AssociatedConnection, g_DdnsService[j + 1].AssociatedConnection);
                g_DdnsService[j].ConnectionState = g_DdnsService[j + 1].ConnectionState;
            }

            g_DdnsServiceNum--;

            break;
        }
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsSetService
            (
                ANSC_HANDLE                 hContext,
                char*                       pServiceName,
                PCOSA_DML_DDNS_SERVICE      pService
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  char*                      pServiceName,
                  Name of the service to be modified.

                  PCOSA_DML_DDNS_CFG         pService
                  Pointer of the new service entry.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsSetService
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_SERVICE      pService
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_DdnsServiceNum; i++)
    {
        if ( g_DdnsService[i].InstanceNumber == pService->InstanceNumber )
        {
            g_DdnsService[i].bEnabled = pService->bEnabled;
            AnscCopyString(g_DdnsService[i].ServiceName, pService->ServiceName);
            AnscCopyString(g_DdnsService[i].Username,    pService->Username   );
            AnscCopyString(g_DdnsService[i].Password,    pService->Password   );
            AnscCopyString(g_DdnsService[i].Domain,      pService->Domain     );
            AnscCopyString(g_DdnsService[i].AssociatedConnection, pService->AssociatedConnection);

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************
    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsGetInfo
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber,
                PCOSA_DML_DDNS_SERVICE      pInfo
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulInstanceNumber
                  InstanceNumber.

                  PCOSA_DML_DDNS_SERVICE      pInfo
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.
**********************************************************************/

ANSC_STATUS
CosaDmlDdnsGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DDNS_SERVICE      pInfo
    )
{
    return ANSC_STATUS_SUCCESS;
}
#elif (_COSA_INTEL_USG_ARM_ || _COSA_DRG_TPG_ || _COSA_BCM_MIPS_)


static int saveID(char* Namespace, char* ServiceName, ULONG ulInstanceNumber,char* pAlias) {
    UtopiaContext utctx;
    char idStr[COSA_DML_SERVICE_NAME_LENGTH+10];
    Utopia_Init(&utctx);

    sprintf(idStr,"%s,%u", pAlias,ulInstanceNumber);
    Utopia_RawSet(&utctx, DDNS_NAMESPACE, ServiceName,idStr);
    
    Utopia_Free(&utctx,TRUE); 

    return 0;
}

static int loadID(char* ServiceName, ULONG* ulInstanceNumber, char* pAlias) {
    UtopiaContext utctx;
    char idStr[COSA_DML_SERVICE_NAME_LENGTH+10] = {0};
    char* instNumString;
    int rv;
    Utopia_Init(&utctx);
    
    rv =Utopia_RawGet(&utctx, DDNS_NAMESPACE, ServiceName, idStr, sizeof(idStr));
    if (rv == -1 || idStr[0] == '\0') {
        Utopia_Free(&utctx,0);
        return -1;
    }
    instNumString=idStr + AnscSizeOfToken(idStr, ",", sizeof(idStr))+1;
    *(instNumString-1)='\0';

    AnscCopyString(pAlias, idStr);
    *ulInstanceNumber = AnscGetStringUlong(instNumString);

    Utopia_Free(&utctx,0);
    return 0;
}

static int UtGetString(const char *path, char *pVal, ULONG length)
{
    UtopiaContext ctx;

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_RawGet(&ctx, NULL, (char *)path, pVal, length);

    Utopia_Free(&ctx, 0);
    return ANSC_STATUS_SUCCESS;
}

static int UtSetString(const char *path, char *val)
{
    UtopiaContext ctx;

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    if (val)
        Utopia_RawSet(&ctx, NULL, (char *)path, val);

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

static int UtGetUlong(const char *path, ULONG *pVal)
{
    UtopiaContext ctx;
    char buf[64] = {0};

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    Utopia_RawGet(&ctx, NULL, (char *)path, buf, sizeof(buf));
    *pVal = atoi(buf);

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

static int UtGetBool(const char *path, BOOLEAN *pVal)
{
    UtopiaContext ctx;
    char buf[64] = {0};

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    Utopia_RawGet(&ctx, NULL, (char *)path, buf, sizeof(buf));
    if (atoi(buf) == 1)
        *pVal = TRUE;
    else
        *pVal = FALSE;

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

static int UtSetBool(const char *path, BOOLEAN val)
{
    UtopiaContext ctx;

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    if (val)
        Utopia_RawSet(&ctx, NULL, (char *)path, "1");
    else
        Utopia_RawSet(&ctx, NULL, (char *)path, "0");

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

static int 
DdnsRestart(void)
{
    ULONG   i = 0;
    char    cmd[512] = {0};
    int n;
    
    if (vsystem("killall -9 ez-ipupdate") != 0)
    {
        fprintf(stderr, "%s: fail to killall ez-ipupdate\n", __FUNCTION__);
    }
    if ( g_bEnabled )
    {
        for (i=0;i<g_DdnsServiceNum;i++)
        {
            if (g_DdnsService[i].bEnabled)
            {
                if ( i == 2 )
                {
                    n=snprintf(cmd, sizeof(cmd), "http://nic.changeip.com/nic/update?u=%s&p=%s&hostname=%s",
                    g_DdnsService[i].Username,
                    g_DdnsService[i].Password,
                    g_DdnsService[i].Domain);
                    if (n >= 0 && n < sizeof(cmd))
                    {
					if(-1 == secure_system_call_vp("usr/bin/wget","-T","10",cmd,NULL))
					{
                    	fprintf(stderr, "%s: fail to start: %s\n", __FUNCTION__, cmd);
					}
                    }
                    else
                    {
                        fprintf(stderr, "%s: fail to Write formatted output to sized buffer\n", __FUNCTION__ );
                    }
                }
                else if ( i == 3 )
                {
                    n=snprintf(cmd , sizeof(cmd),"\"http://freedns.afraid.org/dynamic/update.php?%s\"",
                    g_DdnsService[i].Password);
                    if(n >=0 && n < sizeof(cmd))
                    {
					if(-1 == secure_system_call_vp("usr/bin/wget",cmd,NULL))
					{
                    	fprintf(stderr, "%s: fail to start: %s\n", __FUNCTION__, cmd);
					}
                    }
                    else
                    {
                        fprintf(stderr, "%s: fail to Write formatted output to sized buffer\n", __FUNCTION__ );
                    }
                }
                else
                {
                    char *argp[9] = {NULL};
                    int len=0;

                    argp[0] = "/usr/bin/ez-ipupdate";

                    argp[1] = "--interface=erouter0";

                    len = strlen("--cache-file=/var/ez-ipupdate.cache.") +strlen(g_DdnsService[i].ServiceName)+1;
                    if(argp[2] = malloc(len))
                    {
                        n=snprintf(argp[2], len, "--cache-file=/var/ez-ipupdate.cache.%s",g_DdnsService[i].ServiceName);
                        if(n < 0 || n >= len)
                        {
                            fprintf(stderr, "%s: fail to Write formatted output to sized buffer\n", __FUNCTION__ );
                            goto error;
                        }
                    }
                    else
                    {
                        fprintf(stderr, "malloc failed to allocate memory\n");
                        goto error;
                    }

                    argp[3] = "--daemon";

                    argp[4] = "--max-interval=2073600";

                    len = strlen("--service-type=%") + strlen(g_DdnsService[i].ServiceName) + 1;
                    if(argp[5] = malloc(len))
                    {
                        n=snprintf(argp[5], len, "--service-type=%s",g_DdnsService[i].ServiceName);
                        if(n < 0 || n >= len)
                        {
                            fprintf(stderr, "%s: fail to Write formatted output to sized buffer\n", __FUNCTION__ );
                            goto error;
                        }
                    }
                    else
                    {
                        fprintf(stderr, "malloc failed to allocate memory\n");
                        goto error;
                    }
   
                      /*add 1 byte len for ":"  for --user=%s:%s*/
                    len=strlen("--user=") + strlen(g_DdnsService[i].Username) + strlen(g_DdnsService[i].Password) + 2;
                    if(argp[6] = malloc(len))
                    {
                        n=snprintf(argp[6], len, "--user=%s:%s",g_DdnsService[i].Username,g_DdnsService[i].Password);
                        if(n < 0 || n >= len)
                        {
                            fprintf(stderr, "%s: fail to Write formatted output to sized buffer\n", __FUNCTION__ );
                            goto error;
                        }
                    }
                    else
                    {
                        fprintf(stderr, "malloc failed to allocate memory\n");
                        goto error;
                    }
 
                    len = strlen("--host=") + strlen(g_DdnsService[i].Domain) + 1;
                    if(argp[7] = malloc(len))
                    {
                        n=snprintf(argp[7], len, "--host=%s",g_DdnsService[i].Domain);
                        if(n < 0 || n >= len)
                        {
                            fprintf(stderr, "%s: fail to Write formatted output to sized buffer\n", __FUNCTION__ );    
                            goto error;
                        }
                        CcspTraceInfo(("%s : DDnsService Domain : %s\n", __FUNCTION__,argp[7]));
                    }
                    else
                    {
                        fprintf(stderr, "malloc failed to allocate memory\n");
                        goto error;
                    }
                    argp[8] = NULL; //array must be NULL terminated
                    
                     CcspTraceInfo(("fnuction secure call: DDnsService Domain : %s\n", __FUNCTION__));

                    if(-1 == secure_system_call_p(argp[0],argp))
					{
						fprintf(stderr, "%s: fail to start: %s\n", __FUNCTION__, cmd);
		    		        }
                    error:
                        if ( argp[2] != NULL ) { free( argp[2] ); }
                        if ( argp[5] != NULL ) { free( argp[5] ); }
                        if ( argp[6] != NULL ) { free( argp[6] ); }
                        if ( argp[7] != NULL ) { free( argp[7] ); }
                }
            }
        }
    }

    return 0;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
            );

        Description:
	        This is the initialization routine for XYZ backend.

        Arguments:      ANSC_HANDLE                 hDml
                        Opaque handle from DM adapter. 
        

	                    PANSC_HANDLE                phContext		
                        Opaque handle passed back from backend, 
                        needed by CosaDmlNatXyz() routines.

        Return:         Status of operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
#if 0
    UtopiaContext       utctx;
    ddnsService_t       ddnsService;
    int                 ret = 0;

    _ansc_memset(&ddnsService, 0, sizeof(ddnsService_t));

    Utopia_Init(&utctx);
    ret = Utopia_GetDDNSService(&utctx, &ddnsService);
    CcspTraceInfo(("------CosaDmlDdnsInit,ret:%d,enable:%d,provide:%d,user:%s,pass:%s,host:%s\n", ret, ddnsService.enabled,ddnsService.provider,ddnsService.username,ddnsService.password, ddnsService.hostname ));
    if (ret == 0 && ddnsService.enabled) {
        if ( ddnsService.provider == DDNS_DYNDNS ) {
            g_DdnsService[0].bEnabled = ddnsService.enabled;
            AnscCopyString(g_DdnsService[0].Username, ddnsService.username);
            AnscCopyString(g_DdnsService[0].Password, ddnsService.password);
            AnscCopyString(g_DdnsService[0].Domain, ddnsService.hostname);
            AnscCopyString(g_DdnsService[0].Mail_exch, ddnsService.mail_exch);
            g_DdnsService[0].Backup_mx = ddnsService.backup_mx;
            g_DdnsService[0].Wildcard = ddnsService.wildcard;
        }
        else if (ddnsService.provider == DDNS_TZO) {
            g_DdnsService[1].bEnabled = ddnsService.enabled;
            AnscCopyString(g_DdnsService[1].Username, ddnsService.username);
            AnscCopyString(g_DdnsService[1].Password, ddnsService.password);
            AnscCopyString(g_DdnsService[1].Domain, ddnsService.hostname);
            AnscCopyString(g_DdnsService[1].Mail_exch, ddnsService.mail_exch);
            g_DdnsService[1].Backup_mx = ddnsService.backup_mx;
            g_DdnsService[1].Wildcard = ddnsService.wildcard;
        }
    } else {
        g_bEnabled                = FALSE;
        g_DdnsService[0].bEnabled = FALSE;
        g_DdnsService[1].bEnabled = FALSE;
    }

    Utopia_Free(&utctx,0);
    loadID(g_DdnsService[0].ServiceName, &g_DdnsService[0].InstanceNumber, g_DdnsService[0].Alias);
    loadID(g_DdnsService[1].ServiceName, &g_DdnsService[1].InstanceNumber, g_DdnsService[1].Alias);
    return ANSC_STATUS_SUCCESS;
#endif
    ULONG   i;
    char    service_name_path[32]={0};
    char    username_path[32]={0};
    char    password_path[32]={0};
    char    hostname_path[32]={0};
    char    enabled_path[32]={0};

    char    service_name[32]={0};
    char    username[32]={0};
    char    password[32]={0};
    char    hostname[256]={0};
    BOOLEAN enabled = FALSE;

    if ( UtGetBool("ddns_enable", &g_bEnabled) != ANSC_STATUS_SUCCESS )
    {
        g_bEnabled = FALSE;
    }

    for (i=0; i<g_DdnsServiceNum; i++)
    {
        sprintf(service_name_path, "ddns_service%d", i+1);
        sprintf(username_path, "ddns_username%d", i+1);
        sprintf(password_path, "ddns_password%d", i+1);
        sprintf(hostname_path, "ddns_hostname%d", i+1);
        sprintf(enabled_path, "ddns_enable%d", i+1);

        _ansc_memset(service_name, 0, sizeof(service_name));

        if ( UtGetString(service_name_path, service_name, sizeof(service_name)-1) != ANSC_STATUS_SUCCESS ||
        UtGetString(username_path, username, sizeof(username)-1) != ANSC_STATUS_SUCCESS ||
        UtGetString(password_path, password, sizeof(password)-1) != ANSC_STATUS_SUCCESS ||
        UtGetString(hostname_path, hostname, sizeof(hostname)-1) != ANSC_STATUS_SUCCESS ||
        UtGetBool(enabled_path, &enabled) != ANSC_STATUS_SUCCESS )
        {
            continue;
        }

        /*fprintf(stderr, "%s: %s,%d,%s,%s,%s\n", __FUNCTION__, service_name, enabled, username, password, hostname);*/

        if (AnscEqualString(service_name, "dyndns", FALSE))
        {
            g_DdnsService[0].bEnabled = enabled;
            AnscCopyString(g_DdnsService[0].Username, username);
            AnscCopyString(g_DdnsService[0].Password, password);
            AnscCopyString(g_DdnsService[0].Domain, hostname);
        }
        else if (AnscEqualString(service_name, "tzo", FALSE))
        {
            g_DdnsService[1].bEnabled = enabled;
            AnscCopyString(g_DdnsService[1].Username, username);
            AnscCopyString(g_DdnsService[1].Password, password);
            AnscCopyString(g_DdnsService[1].Domain, hostname);
        }
        else if (AnscEqualString(service_name, "changeip", FALSE))
        {
            g_DdnsService[2].bEnabled = enabled;
            AnscCopyString(g_DdnsService[2].Username, username);
            AnscCopyString(g_DdnsService[2].Password, password);
            AnscCopyString(g_DdnsService[2].Domain, hostname);
        }
        else if (AnscEqualString(service_name, "afraid", FALSE))
        {
            g_DdnsService[3].bEnabled = enabled;
            AnscCopyString(g_DdnsService[3].Username, username);
            AnscCopyString(g_DdnsService[3].Password, password);
            AnscCopyString(g_DdnsService[3].Domain, hostname);
        }
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsGetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DDNS_CFG          pCfg
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_CFG          pCfg
    )
{
    CcspTraceInfo(("------CosaDmlDdnsGetConfig, Enabled:%d\n", g_bEnabled));
    pCfg->bEnabled = g_bEnabled;
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsSetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DDNS_CFG          pCfg
            );

    Description:

        Backend implementation to set Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the configuration to be set.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsSetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_CFG          pCfg
    )
{
#if 0
    UtopiaContext        utctx;
    ddnsService_t        ddnsService;
    int                  ret = 0;

    _ansc_memset(&ddnsService, 0, sizeof(ddnsService_t));
    CcspTraceInfo(("------CosaDmlDdnsSetConfig\n"));

    Utopia_Init(&utctx);

    ret = Utopia_GetDDNSService(&utctx, &ddnsService);
    CcspTraceInfo(("------CosaDmlDdnsSetConfig,ret:%d,enable:%d,provide:%d,user:%s,pass:%s,host:%s\n", ret, ddnsService.enabled,ddnsService.provider,ddnsService.username,ddnsService.password, ddnsService.hostname ));

    g_bEnabled = pCfg->bEnabled;
    if (g_bEnabled == FALSE) {
        ddnsService.enabled       = FALSE;
        g_DdnsService[0].bEnabled = FALSE;
        g_DdnsService[1].bEnabled = FALSE;
        Utopia_SetDDNSService(&utctx, &ddnsService);
    } else {
        ddnsService.enabled       = TRUE;
        if (g_DdnsService[0].bEnabled) {
            ddnsService.provider = DDNS_DYNDNS;
            AnscCopyString(ddnsService.username, g_DdnsService[0].Username);
            AnscCopyString(ddnsService.password, g_DdnsService[0].Password);
            AnscCopyString(ddnsService.hostname, g_DdnsService[0].Domain);
            AnscCopyString(ddnsService.mail_exch, g_DdnsService[0].Mail_exch);
            ddnsService.backup_mx = g_DdnsService[0].Backup_mx;
            ddnsService.wildcard  = g_DdnsService[0].Wildcard;
        } else {
            ddnsService.provider = DDNS_TZO;
            AnscCopyString(ddnsService.username, g_DdnsService[1].Username);
            AnscCopyString(ddnsService.password, g_DdnsService[1].Password);
            AnscCopyString(ddnsService.hostname, g_DdnsService[1].Domain);
            AnscCopyString(ddnsService.mail_exch, g_DdnsService[1].Mail_exch);
            ddnsService.backup_mx = g_DdnsService[1].Backup_mx;
            ddnsService.wildcard  = g_DdnsService[1].Wildcard;
        } 
        Utopia_SetDDNSService(&utctx, &ddnsService);
    }
    Utopia_Free(&utctx,1);
    return ANSC_STATUS_SUCCESS;
#endif
    g_bEnabled = pCfg->bEnabled;

    UtSetBool("ddns_enable", g_bEnabled);

    if (g_bEnabled == FALSE)
    {
        if (vsystem("killall ez-ipupdate") != 0)
        {
            fprintf(stderr, "%s: fail to killall ez-ipupdate\n", __FUNCTION__);
        }
    }
    else
    {
        DdnsRestart();
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsGetNumberOfServices
            (
                ANSC_HANDLE                 hContext,
            );

    Description:

        Backend implementation to get the number of Ddns servies.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

    Return:       The number of Ddns servies.

**********************************************************************/
ULONG
CosaDmlDdnsGetNumberOfServices
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_DdnsServiceNum;
}


/**********************************************************************
    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsGetInfo
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber,
                PCOSA_DML_DDNS_SERVICE      pInfo
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.
                
                  ULONG                       ulInstanceNumber
                  InstanceNumber.

                  PCOSA_DML_DDNS_SERVICE      pInfo
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DDNS_SERVICE      pInfo
    )
{
#if 0
    UtopiaContext         utctx;
    ddnsStatus_t          ddnsStatus;

    if (pInfo)
        _ansc_memset(pInfo, 0, sizeof(COSA_DML_DDNS_SERVICE));
    else
        return ANSC_STATUS_FAILURE;

    Utopia_Init(&utctx);

    if (g_DdnsService[0].InstanceNumber == ulInstanceNumber) {
        pInfo->bEnabled  = g_DdnsService[0].bEnabled;
        AnscCopyString(pInfo->Username, g_DdnsService[0].Username);
        AnscCopyString(pInfo->Password, g_DdnsService[0].Password);
        AnscCopyString(pInfo->Domain, g_DdnsService[0].Domain);
        AnscCopyString(pInfo->Mail_exch, g_DdnsService[0].Mail_exch);
        pInfo->Backup_mx = g_DdnsService[0].Backup_mx;
        pInfo->Wildcard  = g_DdnsService[0].Wildcard;
    } else if (g_DdnsService[1].InstanceNumber == ulInstanceNumber) {
        pInfo->bEnabled  = g_DdnsService[1].bEnabled;
        AnscCopyString(pInfo->Username, g_DdnsService[0].Username);
        AnscCopyString(pInfo->Password, g_DdnsService[0].Password);
        AnscCopyString(pInfo->Domain, g_DdnsService[0].Domain);
        AnscCopyString(pInfo->Mail_exch, g_DdnsService[0].Mail_exch);
        pInfo->Backup_mx = g_DdnsService[0].Backup_mx;
        pInfo->Wildcard  = g_DdnsService[0].Wildcard;
    }

    if(pInfo->bEnabled) {
        Utopia_GetDDNSServiceStatus(&utctx, &ddnsStatus);
        if ( ddnsStatus == DDNS_STATUS_UNKNOWN) {
            pInfo->ConnectionState = COSA_DML_DDNS_STATE_Idle;
        } else if ( ddnsStatus == DDNS_STATUS_FAILED) {

        } else if ( ddnsStatus == DDNS_STATUS_FAILED_CONNECT) {

        } else if ( ddnsStatus == DDNS_STATUS_FAILED_AUTH) {
            pInfo->ConnectionState = COSA_DML_DDNS_STATE_ErrorAuthentication;
        } else if ( ddnsStatus == DDNS_STATUS_SUCCESS) {
            pInfo->ConnectionState = COSA_DML_DDNS_STATE_Succeeded;
        }
    } else {
        pInfo->ConnectionState = COSA_DML_DDNS_STATE_Idle;
    }
    Utopia_Free(&utctx,0);
    return ANSC_STATUS_SUCCESS;
#endif
    CosaDmlDdnsInit(NULL, NULL);

    pInfo->bEnabled  = g_DdnsService[ulInstanceNumber-1].bEnabled;
    AnscCopyString(pInfo->ServiceName, g_DdnsService[ulInstanceNumber-1].ServiceName);
    AnscCopyString(pInfo->Username, g_DdnsService[ulInstanceNumber-1].Username);
    AnscCopyString(pInfo->Password, g_DdnsService[ulInstanceNumber-1].Password);
    AnscCopyString(pInfo->Domain, g_DdnsService[ulInstanceNumber-1].Domain);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsGetService
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_DDNS_SERVICE      pService
            );

    Description:

        Backend implementation to get a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsGetService
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DDNS_SERVICE      pService
    )
{
#if 0
    UtopiaContext         utctx;
    ddnsService_t         ddnsService;
    int                   ret = 0;

    _ansc_memset(&ddnsService, 0, sizeof(ddnsService_t));
    CcspTraceInfo(("------CosaDmlDdnsGetService\n"));
    Utopia_Init(&utctx);
    ret = Utopia_GetDDNSService(&utctx, &ddnsService);
    CcspTraceInfo(("------CosaDmlDdnsInit,ret:%d,enable:%d,provide:%d,user:%s,pass:%s,host:%s\n", ret, ddnsService.enabled,ddnsService.provider,ddnsService.username,ddnsService.password, ddnsService.hostname ));
    if (ret == 0 && ddnsService.enabled) {
        if ( ddnsService.provider == DDNS_DYNDNS ) {
            g_DdnsService[0].bEnabled = ddnsService.enabled;
            AnscCopyString(g_DdnsService[0].Username, ddnsService.username);
            AnscCopyString(g_DdnsService[0].Password, ddnsService.password);
            AnscCopyString(g_DdnsService[0].Domain, ddnsService.hostname);
            AnscCopyString(g_DdnsService[0].Mail_exch, ddnsService.mail_exch);
            g_DdnsService[0].Backup_mx = ddnsService.backup_mx;
            g_DdnsService[0].Wildcard = ddnsService.wildcard;
        }
        else if (ddnsService.provider == DDNS_TZO) {
            g_DdnsService[1].bEnabled = ddnsService.enabled;
            AnscCopyString(g_DdnsService[1].Username, ddnsService.username);
            AnscCopyString(g_DdnsService[1].Password, ddnsService.password);
            AnscCopyString(g_DdnsService[1].Domain, ddnsService.hostname);
            AnscCopyString(g_DdnsService[1].Mail_exch, ddnsService.mail_exch);
            g_DdnsService[1].Backup_mx = ddnsService.backup_mx;
            g_DdnsService[1].Wildcard = ddnsService.wildcard;
        }
    }

    Utopia_Free(&utctx,0);
    loadID(g_DdnsService[0].ServiceName, &g_DdnsService[0].InstanceNumber, g_DdnsService[0].Alias);
    loadID(g_DdnsService[1].ServiceName, &g_DdnsService[1].InstanceNumber, g_DdnsService[1].Alias);

    pService->bEnabled = g_DdnsService[ulIndex].bEnabled;
    pService->InstanceNumber = g_DdnsService[ulIndex].InstanceNumber;
    AnscCopyString(pService->Alias, g_DdnsService[ulIndex].Alias);
    AnscCopyString(pService->ServiceName, g_DdnsService[ulIndex].ServiceName);
    AnscCopyString(pService->Username, g_DdnsService[ulIndex].Username);
    AnscCopyString(pService->Password, g_DdnsService[ulIndex].Password);
    AnscCopyString(pService->Domain, g_DdnsService[ulIndex].Domain);
    AnscCopyString(pService->AssociatedConnection, g_DdnsService[ulIndex].AssociatedConnection);
    pService->ConnectionState = g_DdnsService[ulIndex].ConnectionState;
    AnscCopyString(pService->Mail_exch, g_DdnsService[ulIndex].Mail_exch);
    pService->Backup_mx = g_DdnsService[ulIndex].Backup_mx;
    pService->Wildcard  = g_DdnsService[ulIndex].Wildcard;

    return ANSC_STATUS_SUCCESS;
#endif
    CosaDmlDdnsInit(NULL, NULL);

    pService->bEnabled  = g_DdnsService[ulIndex].bEnabled;
    AnscCopyString(pService->ServiceName, g_DdnsService[ulIndex].ServiceName);
    AnscCopyString(pService->Username, g_DdnsService[ulIndex].Username);
    AnscCopyString(pService->Password, g_DdnsService[ulIndex].Password);
    AnscCopyString(pService->Domain, g_DdnsService[ulIndex].Domain);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsServiceSetValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            );

    Description:

        Backend implementation to set back instance number and alias.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsServiceSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    CcspTraceInfo(("------CosaDmlDdnsServiceSetValues...\n"));
    g_DdnsService[ulIndex].InstanceNumber = ulInstanceNumber;
    AnscCopyString(g_DdnsService[ulIndex].Alias, pAlias);
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsGetServiceByInstNum
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber,
                PCOSA_DML_DDNS_SERVICE      pService
            );

    Description:

        Backend implementation to get a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsGetServiceByInstNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DDNS_SERVICE      pService
    )
{
    CcspTraceInfo(("------CosaDmlDdnsGetServiceByInstNum...\n"));
    ULONG                           ulIndex = 0;

    for ( ulIndex = 0; ulIndex < g_DdnsServiceNum; ulIndex++ )
    {
        if ( g_DdnsService[ulIndex].InstanceNumber == ulInstanceNumber )
        {
            pService->bEnabled = g_DdnsService[ulIndex].bEnabled;
            AnscCopyString(pService->Alias, g_DdnsService[ulIndex].Alias);
            AnscCopyString(pService->ServiceName, g_DdnsService[ulIndex].ServiceName);
            AnscCopyString(pService->Username, g_DdnsService[ulIndex].Username);
            AnscCopyString(pService->Password, g_DdnsService[ulIndex].Password);
            AnscCopyString(pService->Domain, g_DdnsService[ulIndex].Domain);
            AnscCopyString(pService->AssociatedConnection, g_DdnsService[ulIndex].AssociatedConnection);
            pService->ConnectionState = g_DdnsService[ulIndex].ConnectionState;
            AnscCopyString(pService->Mail_exch, g_DdnsService[ulIndex].Mail_exch);
            pService->Backup_mx  = g_DdnsService[ulIndex].Backup_mx;
            pService->Wildcard   = g_DdnsService[ulIndex].Wildcard;
        }
    }
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsAddService
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DDNS_SERVICE      pService
            );

    Description:

        Backend implementation to add a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be added.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsAddService
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_SERVICE      pService
    )
{
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsDelService
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DDNS_SERVICE      pService
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  PCOSA_DML_DDNS_CFG         pCfg
                  Pointer of the service to be deleted.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsDelService
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDdnsSetService
            (
                ANSC_HANDLE                 hContext,
                char*                       pServiceName,
                PCOSA_DML_DDNS_SERVICE      pService
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  char*                      pServiceName,
                  Name of the service to be modified.

                  PCOSA_DML_DDNS_CFG         pService
                  Pointer of the new service entry.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlDdnsSetService
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_SERVICE      pService
    )
{
#if 0
    ULONG                           i = 0;
    UtopiaContext                   utctx;
    ddnsService_t                   ddnsService;
    COSA_DML_DDNS_CFG               DdnsCfg;
    int                             ret = 0;
    _ansc_memset(&ddnsService, 0, sizeof(ddnsService_t));
    CcspTraceInfo(("------CosaDmlDdnsSetService...\n"));
    Utopia_Init(&utctx);
    ret = Utopia_GetDDNSService(&utctx, &ddnsService); 
    CcspTraceInfo(("------CosaDmlDdnsSetService,ret:%d,enable:%d,provide:%d,user:%s,pass:%s,host:%s\n", ret, ddnsService.enabled,ddnsService.provider,ddnsService.username,ddnsService.password, ddnsService.hostname ));
    CcspTraceInfo(("------CosaDmlDdnsSetService,ServiceName:%s\n", pService->ServiceName));

    if (AnscEqualString(pService->ServiceName,g_DdnsService[0].ServiceName,FALSE)) {
        ddnsService.provider = DDNS_DYNDNS;
    } else if (AnscEqualString(pService->ServiceName,g_DdnsService[1].ServiceName,FALSE)) {
        ddnsService.provider = DDNS_TZO;
    }

    CcspTraceInfo(("------CosaDmlDdnsSetService,ServiceName:%s,0:%s,1:%s,provider:%d...\n", pService->ServiceName,g_DdnsService[0].ServiceName,g_DdnsService[1].ServiceName,ddnsService.provider));
    AnscCopyString(ddnsService.username, pService->Username );
    AnscCopyString(ddnsService.password, pService->Password );
    AnscCopyString(ddnsService.hostname, pService->Domain   );
    AnscCopyString(ddnsService.mail_exch,pService->Mail_exch);
    ddnsService.enabled   = pService->bEnabled;
    ddnsService.backup_mx = pService->Backup_mx;
    ddnsService.wildcard  = pService->Wildcard;  

    Utopia_SetDDNSService(&utctx, &ddnsService);

    Utopia_Free(&utctx,1);
    if (AnscEqualString(pService->ServiceName,g_DdnsService[0].ServiceName,FALSE)) {
        saveID(DDNS_NAMESPACE, g_DdnsService[0].ServiceName, g_DdnsService[0].InstanceNumber, pService->Alias);
    } else if (AnscEqualString(pService->ServiceName,g_DdnsService[1].ServiceName,FALSE)) {
        saveID(DDNS_NAMESPACE, g_DdnsService[1].ServiceName, g_DdnsService[1].InstanceNumber, pService->Alias);
    }
    CcspTraceInfo(("------CosaDmlDdnsSetService,Alias:%s,0:%s,1:%s...\n", pService->Alias,g_DdnsService[0].Alias,g_DdnsService[1].Alias));
    for ( i = 0; i < g_DdnsServiceNum; i++)
    {
        if ( AnscEqualString(g_DdnsService[i].Alias, pService->Alias, FALSE) )
        {
            g_DdnsService[i].bEnabled = pService->bEnabled;
            AnscCopyString(g_DdnsService[i].ServiceName, pService->ServiceName);
            AnscCopyString(g_DdnsService[i].Username,    pService->Username   );
            AnscCopyString(g_DdnsService[i].Password,    pService->Password   );
            AnscCopyString(g_DdnsService[i].Domain,      pService->Domain     );
            AnscCopyString(g_DdnsService[i].AssociatedConnection, pService->AssociatedConnection);
            AnscCopyString(g_DdnsService[i].Mail_exch,pService->Mail_exch);
            g_DdnsService[i].Backup_mx = pService->Backup_mx;
            g_DdnsService[i].Wildcard  = pService->Wildcard;  
            break;
        }
    }

    /* When we enable one, we need to disable another... */
    if (AnscEqualString(pService->ServiceName,g_DdnsService[0].ServiceName,FALSE) && pService->bEnabled) {
        g_DdnsService[1].bEnabled = FALSE;
    } else if (AnscEqualString(pService->ServiceName,g_DdnsService[1].ServiceName,FALSE) && pService->bEnabled) {
        g_DdnsService[0].bEnabled = FALSE;
    }

    if (g_DdnsService[0].bEnabled == FALSE && g_DdnsService[1].bEnabled == FALSE) {
        g_bEnabled = FALSE;
        DdnsCfg.bEnabled = FALSE;
        CosaDmlDdnsSetConfig(NULL, &DdnsCfg);
    }
    else
    {
        g_bEnabled = TRUE;
        DdnsCfg.bEnabled = TRUE;
    }
    return ANSC_STATUS_SUCCESS;
#endif
    if (AnscEqualString(pService->ServiceName, "dyndns", TRUE))
    {
        g_DdnsService[0].bEnabled = pService->bEnabled;
        AnscCopyString(g_DdnsService[0].Domain, pService->Domain);
        AnscCopyString(g_DdnsService[0].Password, pService->Password);
        AnscCopyString(g_DdnsService[0].Username, pService->Username);

        UtSetBool("ddns_enable1", g_DdnsService[0].bEnabled);
        UtSetString("ddns_hostname1", g_DdnsService[0].Domain);
        UtSetString("ddns_password1", g_DdnsService[0].Password);
        UtSetString("ddns_username1", g_DdnsService[0].Username);
        UtSetString("ddns_service1", "dyndns");
    }
    else if (AnscEqualString(pService->ServiceName, "tzo", TRUE))
    {
        g_DdnsService[1].bEnabled = pService->bEnabled;
        AnscCopyString(g_DdnsService[1].Domain, pService->Domain);
        AnscCopyString(g_DdnsService[1].Password, pService->Password);
        AnscCopyString(g_DdnsService[1].Username, pService->Username);

        UtSetBool("ddns_enable2", g_DdnsService[1].bEnabled);
        UtSetString("ddns_hostname2", g_DdnsService[1].Domain);
        UtSetString("ddns_password2", g_DdnsService[1].Password);
        UtSetString("ddns_username2", g_DdnsService[1].Username);
        UtSetString("ddns_service2", "tzo");
    }
    else if (AnscEqualString(pService->ServiceName, "changeip", TRUE))
    {
        g_DdnsService[2].bEnabled = pService->bEnabled;
        AnscCopyString(g_DdnsService[2].Domain, pService->Domain);
        AnscCopyString(g_DdnsService[2].Password, pService->Password);
        AnscCopyString(g_DdnsService[2].Username, pService->Username);

        UtSetBool("ddns_enable3", g_DdnsService[2].bEnabled);
        UtSetString("ddns_hostname3", g_DdnsService[2].Domain);
        UtSetString("ddns_password3", g_DdnsService[2].Password);
        UtSetString("ddns_username3", g_DdnsService[2].Username);
        UtSetString("ddns_service3", "changeip");
    }
    else if (AnscEqualString(pService->ServiceName, "afraid", TRUE))
    {
        g_DdnsService[3].bEnabled = pService->bEnabled;
        AnscCopyString(g_DdnsService[3].Domain, pService->Domain);
        AnscCopyString(g_DdnsService[3].Password, pService->Password);
        AnscCopyString(g_DdnsService[3].Username, pService->Username);

        UtSetBool("ddns_enable4", g_DdnsService[3].bEnabled);
        UtSetString("ddns_hostname4", g_DdnsService[3].Domain);
        UtSetString("ddns_password4", g_DdnsService[3].Password);
        UtSetString("ddns_username4", g_DdnsService[3].Username);
        UtSetString("ddns_service4", "afraid");
    }
    else
    {
        return ANSC_STATUS_SUCCESS;
    }

    DdnsRestart();

    return ANSC_STATUS_SUCCESS;
}

#endif
