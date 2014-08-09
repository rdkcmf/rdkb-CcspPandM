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
#include "cosa_x_cisco_com_mld_apis.h"
#include "cosa_x_cisco_com_mld_internal.h"
#include "plugin_main_apis.h"

#if ( defined _COSA_SIM_ )

ANSC_STATUS
CosaDmlMldInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    )
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMldSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_INFO          pInfo
    )    
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetGroup
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_MLD_GROUP         pMldGroupArray        
    )    
{
    return ANSC_STATUS_SUCCESS;
}


#elif ( defined _COSA_DRG_TPG_ )

ANSC_STATUS
CosaDmlMldInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )    
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    )    
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    )    
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_INFO          pInfo
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetGroup
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_MLD_GROUP         pMldGroupArray        
    )
{
    return ANSC_STATUS_SUCCESS;
}


#elif ( defined _COSA_DRG_CNS_ )

ANSC_STATUS
CosaDmlMldInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    )
{
    FILE *cfg_f, *pid_f, *fd;
    char buffer[256];
    pid_t pid;

    if (!pCfg)
    {
        return ANSC_STATUS_FAILURE;
    }

    pCfg->bSnoopingEnabled = FALSE;

    fd = fopen("/sys/class/net/brlan0/bridge/multicast_snooping", "r");

    if (fd)
    {
        if ( 1 == fread(buffer, 1, 1, fd) )
        {
            if ( buffer[0] == '1' )
            {
                pCfg->bSnoopingEnabled = TRUE;
            }
        }
        fclose(fd);        
    }

    if ( (pid_f = fopen("/var/run/ecmh.pid", "r")) != NULL )
    {
        if(fgets(buffer, 255,pid_f) != '\0')
        {
            fclose(pid_f);
            sscanf( buffer, "%d", &pid);
            CcspTraceWarning(("Sending SIGUSR1 to proccess %d...\n", pid));
            if ( kill(pid,SIGUSR1) == -1 )
            {
                pCfg->bEnabled = FALSE;
                return ANSC_STATUS_SUCCESS;
            }
            else
            {
                pCfg->bEnabled = TRUE;
            }
        }
    }
    else
    {
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    )
{
    FILE *cfg_f, *pid_f, *fd;
    char buffer[256];
    pid_t pid;
    COSA_DML_MLD_CFG Cfg2;

    if (!pCfg)
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( pCfg->bSnoopingEnabled )
    {
        _write_sysctl_file("/sys/class/net/brlan0/bridge/multicast_snooping", 1);
    }
    else
    {
        _write_sysctl_file("/sys/class/net/brlan0/bridge/multicast_snooping", 0);
    }

    if ( (pid_f = fopen("/var/run/ecmh.pid", "r")) != NULL )
    {
        if(fgets(buffer, 255,pid_f) != '\0')
        {
            fclose(pid_f);
            sscanf( buffer, "%d", &pid);
            CcspTraceWarning(("Sending SIGUSR1 to proccess %d...\n", pid));
            if ( kill(pid,SIGUSR1) == -1 )
            {
                if ( pCfg->bEnabled == TRUE )
                {
                    sprintf(buffer, "/usr/sbin/ecmh -i wan0");
                    system(buffer);
                }
            }
            else
            {
                if ( pCfg->bEnabled == FALSE )
                {
                    kill(pid,SIGKILL);
                }
            }
        }
    }
    else
    {
        if ( pCfg->bEnabled == TRUE )
        {
            sprintf(buffer, "/usr/sbin/ecmh -i wan0");
            system(buffer);
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_INFO          pInfo
    )
{
    FILE *pid_f;
    char buffer[256];
    pid_t pid;

    if (!pInfo)
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( (pid_f = fopen("/var/run/ecmh.pid", "r")) != NULL )
    {
        if(fgets(buffer, 255,pid_f) != '\0')
        {
            fclose(pid_f);
            sscanf( buffer, "%d", &pid);
            CcspTraceWarning(("Sending SIGUSR1 to proccess %d...\n", pid));
            if ( kill(pid,SIGUSR1) == -1 )
            {
                pInfo->Status = COSA_DML_MLD_STATUS_Disabled;
            }
            else
            {
                pInfo->Status = COSA_DML_MLD_STATUS_Enabled;
            }
        }
    }
    else
    {
        pInfo->Status = COSA_DML_MLD_STATUS_Disabled;
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetGroup
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_MLD_GROUP         pMldGroupArray        
    )
{
    FILE *stat_f, *pid_f;
    char buffer[1024];
    pid_t pid;
    char *pch;
    long count;

    if (!pulCount || !pMldGroupArray)
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( (pid_f = fopen("/var/run/ecmh.pid", "r")) != NULL )
    {
        if(fgets(buffer, 255,pid_f) != '\0')
        {
            fclose(pid_f);
            sscanf( buffer, "%d", &pid);
            CcspTraceWarning(("Sending SIGUSR1 to proccess %d...\n", pid));
            if ( kill(pid,SIGUSR1) == -1 )
            {
                return ANSC_STATUS_FAILURE;
            }
        }
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    sleep(1);   /* waits until the stat file is generated */

    *pulCount = 0;
    count = -1;

    if ( (stat_f = fopen("/var/run/ecmh.dump", "r")) != NULL )
    {
        fgets(buffer, 255, stat_f); /* Ignore the first line */
        
        while(fgets(buffer, 255,stat_f) != '\0')
        {
            pch = strtok(buffer, " \t\r\n");
            if (pch == NULL)
            {
                continue;
            }

            if (strcmp(pch, "Group") == 0)
            {
                pch = strtok(NULL, " \t\r\n");
                while(pch)
                {
                    if (*pch != ':')
                    {
                        count++;
                        if (count >= MLD_MAXINSTANCE)
                        {
                            *pulCount = count;
                            return ANSC_STATUS_SUCCESS;
                        }
                        
                        strcpy(pMldGroupArray[count].GroupAddress, pch);
                        pMldGroupArray[count].Interfaces[0] = '\0';

                        break;
                    }
                    pch = strtok(NULL, " \t\r\n");
                }
            }
            else if (strcmp(pch, "Interface:") == 0)
            {
                pch =strtok(NULL, " \t\r\n");
                if (pch)
                {
                    strcat(pMldGroupArray[count].Interfaces, pch);
                    strcat(pMldGroupArray[count].Interfaces, ",");
                }
            }
            else if (strcmp(pch, "***") == 0)
            {
                break;
            }
        }

        fclose(stat_f);
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    *pulCount = count+1;
    return ANSC_STATUS_SUCCESS;
}


#endif

