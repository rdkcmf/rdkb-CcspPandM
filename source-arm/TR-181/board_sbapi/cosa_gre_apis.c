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
/****************************************************************************
  Copyright 2016-2018 Intel Corporation

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
******************************************************************************/

#ifdef INTEL_GRE_HOTSPOT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sysevent/sysevent.h>

#include "ccsp_psm_helper.h"
#include "ansc_platform.h"
#include "syscfg.h"
#include "cosa_gre_apis.h"
#include "ccsp_psm_helper.h"
#include "sys_definitions.h"


#define DMSB_GRE_TUNNEL "dmsb.gre.tunnel."
#define GRE_INTERFACE   "interface."

#define GRE_TUNNEL_ENABLE                            DMSB_GRE_TUNNEL "%d.Enable"
#define GRE_TUNNEL_ALIAS                             DMSB_GRE_TUNNEL "%d.Alias"
#define GRE_TUNNEL_REMOTEENDPOINTS                   DMSB_GRE_TUNNEL "%d.RemoteEndpoints"
#define GRE_TUNNEL_KEEPALIVEPOLICY                   DMSB_GRE_TUNNEL "%d.KeepAlivePolicy"
#define GRE_TUNNEL_KEEPALIVETIMEOUT                  DMSB_GRE_TUNNEL "%d.KeepAliveTimeout"
#define GRE_TUNNEL_KEEPALIVETHRESHOLD                DMSB_GRE_TUNNEL "%d.KeepAliveThreshold"
#define GRE_TUNNEL_DELIVERYHEADERPROTOCOL            DMSB_GRE_TUNNEL "%d.DeliveryHeaderProtocol"
#define GRE_TUNNEL_DEFAULTDSCPMARK                   DMSB_GRE_TUNNEL "%d.DefaultDSCPMark"
#define GRE_TUNNEL_KEEPALIVECOUNT                    DMSB_GRE_TUNNEL "%d.KeepAliveCount"
#define GRE_TUNNEL_KEEPALIVEINTERVAL                 DMSB_GRE_TUNNEL "%d.KeepAliveInterval"
#define GRE_TUNNEL_KEEPALIVEFAILUREINTERVAL          DMSB_GRE_TUNNEL "%d.KeepAliveFailureInterval"
#define GRE_TUNNEL_KEEPALIVERECOVERINTERVAL          DMSB_GRE_TUNNEL "%d.KeepAliveRecoverInterval"
#define GRE_TUNNEL_MSSCLAMPINGVALUE                  DMSB_GRE_TUNNEL "%d.MSSClampingValue"

/* For GRE Hotspot */
#define GRE_TUNNEL_WIFI_INTFS                        DMSB_GRE_TUNNEL "%d.WiFiInterfaces"
#define GRE_TUNNEL_SNOOP_CIRCUIT_ENABLE              DMSB_GRE_TUNNEL "%d.SnoopCircuitEnable"
#define GRE_TUNNEL_SNOOP_REMOTE_ENABLE               DMSB_GRE_TUNNEL "%d.SnoopRemoteEnable"
#define GRE_TUNNEL_MAX_CLIENTS                       DMSB_GRE_TUNNEL "%d.MaxClients"

#define GRE_TUNNEL_IF_ENABLE                         DMSB_GRE_TUNNEL "%d." GRE_INTERFACE "%d.Enable"
#define GRE_TUNNEL_IF_ALIAS                          DMSB_GRE_TUNNEL "%d." GRE_INTERFACE "%d.Alias"
#define GRE_TUNNEL_IF_LOWERLAYERS                    DMSB_GRE_TUNNEL "%d." GRE_INTERFACE "%d.LowerLayers"
#define GRE_TUNNEL_IF_PROTOCOLIDOVERRIDE             DMSB_GRE_TUNNEL "%d." GRE_INTERFACE "%d.ProtocolIdOverride"
#define GRE_TUNNEL_IF_USECHECKSUM                    DMSB_GRE_TUNNEL "%d." GRE_INTERFACE "%d.UseCheckSum"
#define GRE_TUNNEL_IF_KEYIDENTIFIERGENERATIONPOLICY  DMSB_GRE_TUNNEL "%d." GRE_INTERFACE "%d.KeyIdentifierGenerationPolicy"
#define GRE_TUNNEL_IF_KEYIDENTIFIER                  DMSB_GRE_TUNNEL "%d." GRE_INTERFACE "%d.KeyIdentifier"
#define GRE_TUNNEL_IF_USESEQUENCENUMBER              DMSB_GRE_TUNNEL "%d." GRE_INTERFACE "%d.UseSequenceNumber"
#define GRE_TUNNEL_IF_NAME                           DMSB_GRE_TUNNEL "%d." GRE_INTERFACE "%d.Name"

#define HOTSPOT_GRE_ALIAS "Intel_Hotspot_GRE_Tunnel"
#define NONE "None"
#define DEFAULT_WIFI "xfinitywifi"

#define MAX_PARAM_LEN 64
#define MAX_REC_LEN 256
#define MAX_VAL_LEN 1024

#define  PSM_SUCCESS 0
#define  PSM_FAIL    -1

#define  INTF_STATUS_SUCCESS 0
#define  INTF_STATUS_FAIL   -1

#define RET_FAIL -1

/* Duration to wait before calling the thread handler for dhcp snooping for hotspot */
#define INITIAL_CIRCUIT_ID_SLEEP 5

/* Duration to poll and call thread handler for dhcp snooping for hotspot */
#define POLL_CIRCUIT_ID_SLEEP 3

/* Initial Queue ID of the net filter to be provided to dhcp snooping for hotspot */
#define INITIAL_SNOOPER_QUEUE 1

int hotspotGREInst = 0;

extern void* g_pDslhDmlAgent;
extern ANSC_HANDLE bus_handle;

static hotspotfd_statistics_s *g_hsfdStat = NULL;
static snooper_statistics_s *g_snstat = NULL;

int sysevent_fd;
token_t sysevent_token;

/**********************************************************************

    caller:     COSA DML

    prototype:

        int
        _PSM_GRE_Get
        (
            const char *param,
            char *value,
            int size
        )

    Description:

        Function to call PSM and get the values of the record

    Arguments:    const char  *param
                  The parameter name to be obtained form PSM

                  char *value
                  The pointer in which the value obtained will be stored

                  int size
                  The size of the array which is going to hold the value obtained

    Return:       The status of the operation.

**********************************************************************/
int
_PSM_GRE_Get(
    const char *param, char *value, int size)
{
    char *val = NULL;
    CCSP_MESSAGE_BUS_INFO *businfo;

    if (PSM_Get_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, NULL, &val) != CCSP_SUCCESS)
        return PSM_FAIL;

    snprintf(value, size, "%s", val);

    businfo = g_MessageBusHandle;
    businfo->freefunc(val);
    return PSM_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        int
        _PSM_GRE_Set
        (
            const char *param,
            const char *value
        )

    Description:

        Function to call the PSM to store values into it

    Arguments:    const char  *param
                  The parameter name to which the value will be stored

                  char *value
                  The value to be saved into PSM

    Return:       The status of the operation.

**********************************************************************/
int
_PSM_GRE_Set(const char *param, const char *value)
{
    if (PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, ccsp_string, (char *)value) != CCSP_SUCCESS)
        return PSM_FAIL;
    return PSM_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        int
        _PSM_Num_Cmp_
        (
            const int  *num1,
            const int  *num2
        )

    Description:

        Function to compare two numbers for sorting

    Arguments:    const int  *num1
                  The first number to be compared

                  const int  *num2
                  The second number to be compared

    Return:       The result of the operation will be non-zero
                  if first number is greater than second number
                  and zero otherwise.

**********************************************************************/
int
_PSM_Num_Cmp(const void *num1, const void *num2)
{
    return *(const int*)num1 > *(const int*)num2;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        void
        _get_shell_output
        (
            char* cmd,
            char* out,
            int   len
        )

    Description:

        Function to read data from command shell

    Arguments:    char *cmd
                  The command to send to the shell prompt

                  char* out
                  The output from the above command

                  int len
                  Size of output

   Return:       Void

**********************************************************************/
static void _get_shell_output(char * cmd, char * out, int len)
{
    FILE      *fp              = NULL;
    char      buf[MAX_REC_LEN] = {0};
    char      *p               = NULL;

    fp = popen(cmd, "r");

    if (fp)
    {
        fgets(buf, sizeof(buf), fp);

        /*we need to remove the \n char in buf*/
        if ((p = strchr(buf, '\n')))
            *p = 0;

        strncpy(out, buf, len-1);
        pclose(fp);
    }
}
/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGREInit
        (
            ANSC_HANDLE                 hDml,
            PANSC_HANDLE                phContext
        )

    Description:

        Initialization to backend context to handle the GRE object.

    Arguments:    ANSC_HANDLE                 hDml

                  PANSC_HANDLE                phContext
                  Linking the current GRE conext to the backend context

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGREInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    PDMSB_TR181_GRE_CONTEXT    pGREContext = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pGREContext = (PDMSB_TR181_GRE_CONTEXT)AnscAllocateMemory(sizeof(DMSB_TR181_GRE_CONTEXT));

    if ( pGREContext )
    {
        DMSB_TR181_GRE_CONTEXT_Init(pGREContext);
        *phContext = (ANSC_HANDLE)pGREContext;

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        return ANSC_STATUS_RESOURCES;
    }
}

/**********************************************************************

    caller:     COSA GRE API

    prototype:

        int
        GRE_hotspot_update_circuit_ids
            (
                int tunnelInst,
                int queuestart
            );

    Description:

       Function to check the WiFi SSIDs
       and update the circuit ids needed for hotspot

    Arguments:    tunnelInst         GRE Tunnel Instance
                  queuestart         Initial ID of the snooper queue


    Return:       The final ID of the snooper queue

**********************************************************************/
int GRE_hotspot_update_circuit_ids(int tunnelInst, int queuestart) {
    int  retval                       = 0;
    char rec[MAX_REC_LEN]             = {0};
    char localinterfaces[MAX_REC_LEN] = {0};
    char paramname[MAX_PARAM_LEN]     = {0};
    char circuitid[MAX_REC_LEN]       = {0};
    char outdata[MAX_REC_LEN]         = {0};
    char* save                        = NULL;
    char* curInt                      = NULL;
    int nameSave                      = 0;
    int circuitSave                   = 0;
    int ssidInst                      = 0;
    int size                          = 0;
    int inst                          = 0;
    parameterValStruct_t varStruct;

    varStruct.parameterName  = paramname;
    varStruct.parameterValue = outdata;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_WIFI_INTFS, tunnelInst);
    if (_PSM_GRE_Get(rec, localinterfaces, sizeof(localinterfaces)) != PSM_SUCCESS)
         return PSM_FAIL;

    curInt = strtok_r(localinterfaces, ",", &save);

    while (curInt)
    {
        circuitSave=0;
        //Trim off the trailing dot if it exists
        size = strlen(curInt);
        if (curInt[size -1] == '.')
            curInt[size - 1]='\0';

        inst = atoi(strrchr(curInt,'.')+1);

        memset(paramname,0,sizeof(paramname));
        memset(outdata,0,sizeof(outdata));

        size = sizeof(outdata);

        if (syscfg_get(NULL, "wan_physical_ifname", paramname, sizeof(paramname)) != 0)
        {
            AnscTraceWarning(("fail to get wan_physical_ifname\n"));
            snprintf(paramname, sizeof(paramname), "erouter0");
        }
        if (get_if_hwaddr(paramname, circuitid, sizeof(circuitid)) != 0)
        {
            AnscTraceWarning(("fail to get HW Addr for %s\n", paramname));
            snprintf(circuitid, sizeof(circuitid), "00:00:00:00:00:00");
        }

        circuitSave = strlen(circuitid);
        circuitSave += snprintf(circuitid + circuitSave, sizeof(circuitid) - circuitSave, ";");

        memset(paramname,0,sizeof(paramname));
        memset(outdata,0,sizeof(outdata));

        size = sizeof(outdata);

        /*Fetch the value of Device.WiFi.x.SSID */
        snprintf(paramname, sizeof(paramname),"%s.%s", curInt, "SSID");
        retval = COSAGetParamValueByPathName(bus_handle, &varStruct, &size);

        if ( retval != ANSC_STATUS_SUCCESS)
            return RET_FAIL;

        /*If the value of SSID from the above is NULL assign default SSID name */
        if(strlen(varStruct.parameterValue)==0)
        {
            strncpy(varStruct.parameterValue, DEFAULT_WIFI, sizeof(DEFAULT_WIFI));
        }

        circuitSave += snprintf(circuitid + circuitSave, sizeof(circuitid) - circuitSave, "%s;", varStruct.parameterValue);

        memset(paramname,0,sizeof(paramname));
        memset(outdata,0,sizeof(outdata));

        size = sizeof(outdata);

        /*Check if the WiFi security mode is secure or open */
        snprintf(paramname, sizeof(paramname), "Device.WiFi.AccessPoint.%d.Security.ModeEnabled", inst);
        retval = COSAGetParamValueByPathName(bus_handle, &varStruct, &size);

        if ( retval != ANSC_STATUS_SUCCESS)
            return RET_FAIL;

        if(strncmp(NONE, varStruct.parameterValue, sizeof(NONE)))
        {
            snprintf(circuitid + circuitSave, sizeof(circuitid) - circuitSave, "s");
        }
        else
        {
            snprintf(circuitid + circuitSave, sizeof(circuitid) - circuitSave, "o");
        }

        /*Set the sysevents for DHCP Snooper to pick up when the hotspot process comes up */
        snprintf(paramname, sizeof(paramname), "snooper-queue%d-circuitID", queuestart);

        sysevent_set(sysevent_fd, sysevent_token, paramname, circuitid, 0);

        snprintf(paramname, sizeof(paramname), "snooper-ssid%d-index", queuestart++);
        snprintf(outdata, sizeof(outdata), "%d", inst);

        sysevent_set(sysevent_fd, sysevent_token, paramname, outdata, 0);

        curInt = strtok_r(NULL, ",", &save);
    }
    return queuestart;
}

/**********************************************************************

    caller:     COSA GRE API

    prototype:

    static void*
    GRE_HS_circuit_id_init_thread
    (
       void* arg
    )

    Description:

       This is the thread handler which polls if the WiFi SSID's are up
       to build circuit IDs for Hotspot

    Arguments:    arg         Value passed by pthread call

    Return:       NULL

**********************************************************************/
static void* GRE_HS_circuit_id_init_thread(void* arg) {
    int ret = RET_FAIL;

    sleep(INITIAL_CIRCUIT_ID_SLEEP);

    if(hotspotGREInst)
    {
        ret = GRE_hotspot_update_circuit_ids(hotspotGREInst, INITIAL_SNOOPER_QUEUE);

        while (ret < 0)
        {
            sleep(POLL_CIRCUIT_ID_SLEEP);
            ret = GRE_hotspot_update_circuit_ids(hotspotGREInst, INITIAL_SNOOPER_QUEUE);
        }
    }
    return NULL;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

    ANSC_STATUS
    CosaDmlGREInit_HotspotParams
    (
       ANSC_HANDLE                 hContext
     )

    Description:

       Function to intialize hotspot parameters

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

    Return:       The status of the operation

**********************************************************************/
ANSC_STATUS
CosaDmlGREInit_HotspotParams(
    ANSC_HANDLE                 hContext
                             )
{
    PDMSB_TR181_GRE_CONTEXT    pGREContext    = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PSINGLE_LINK_ENTRY         pSLinkEntry    = NULL;
    PDMSB_TR181_GRE_TUNNEL     pGRETunnel     = NULL;
    void*                      params         = NULL;
    int                        shmid          = 0;
    int                        shmid_snooper  = 0;
    int                        hotspotPresent = 0;

    pSLinkEntry = AnscSListGetFirstEntry(&pGREContext->GRETunnelTR181List);

    while ( pSLinkEntry )
    {
        pGRETunnel  = ACCESS_DMSB_TR181_GRE_TUNNEL(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if(!(strncmp(pGRETunnel->Cfg.Alias, HOTSPOT_GRE_ALIAS, sizeof(HOTSPOT_GRE_ALIAS))))
        {
            if(pGRETunnel->Cfg.Enable == TRUE)
            {
            hotspotGREInst = pGRETunnel->Cfg.TunnelInstanceNumber;
            hotspotPresent = 1;
            }
        }
    }

        // For sysevents to Hotspot Process
        sysevent_fd = sysevent_open(
                "127.0.0.1", SE_SERVER_WELL_KNOWN_PORT,
                SE_VERSION, kHotspotfd_events,
                &sysevent_token
        );

        if (sysevent_fd < 0)
        {
            AnscTraceError(("sysevent_open failed\n"));
             return ANSC_STATUS_FAILURE;
        }
        else
        {
            AnscTraceWarning(("sysevent_open success\n"));
        }

        //Shared memory for Hotpsot Process
        if ((shmid = shmget(kKeepAlive_Statistics, kKeepAlive_SharedMemSize, IPC_CREAT | 0666)) < 0)
        {
            AnscTraceError(("shmget: %s\n", strerror(errno)));
            return ANSC_STATUS_FAILURE;
        }

        if ((g_hsfdStat = shmat(shmid, NULL, 0)) == (hotspotfd_statistics_s *)-1)
        {
            AnscTraceError(("shmat: %s\n", strerror(errno)));
            g_hsfdStat = NULL;
            return ANSC_STATUS_FAILURE;
        }

        if ((shmid_snooper = shmget(kSnooper_Statistics, kSnooper_SharedMemSize, IPC_CREAT | 0666)) < 0)
        {
            AnscTraceError(("shmget: %s\n", strerror(errno)));
            return ANSC_STATUS_FAILURE;
        }

        if ((g_snstat = shmat(shmid_snooper, NULL, 0)) == (snooper_statistics_s *)-1)
        {
            AnscTraceError(("shmat: %s\n", strerror(errno)));
            g_snstat = NULL;
            return ANSC_STATUS_FAILURE;
        }

    if(hotspotPresent)
    {
        AnscCreateTask(GRE_HS_circuit_id_init_thread, USER_DEFAULT_TASK_STACK_SIZE, USER_DEFAULT_TASK_PRIORITY, params, "GRE_HS_CircuitIDInitThread");
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        static int
        getInterfaceStatus
            (
                const char  *ifname,
                int         *up
            );

    Description:

       Function to find the status of the interfaces

    Arguments:    const char *ifname         GRE Tunnel Interface name
                  int        *up             Status of the interface


    Return:       The status of the operation

**********************************************************************/
static int getInterfaceStatus(const char *ifname, int *up)
{
    UINT sockfd;
    struct ifreq ifr;

    if (!ifname || !up)
        return INTF_STATUS_FAIL;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        return INTF_STATUS_FAIL;
    }

    memset(&ifr, 0, sizeof(ifr));
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", ifname);

    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0)
    {
        perror("socket");
        close(sockfd);
        return INTF_STATUS_FAIL;
    }

    close(sockfd);
    *up = ifr.ifr_flags & IFF_UP;
    return INTF_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        COSA_DML_GRE_TUNNEL_IF_STATUS
        CosaDmlGRETunnelIfGetStatus
            (
                const char *ifname,
                const char* lowlayers
            );

    Description:

       Function to set the status of the Tunnel Interface

    Arguments:    const char *ifname         GRE Tunnel Interface name
                  const char* lowerlayers    Lowerlayers associated with the Tunnel Interface

    Return:       The status of GRE Tunnel Interface

**********************************************************************/
static COSA_DML_GRE_TUNNEL_IF_STATUS CosaDmlGRETunnelIfGetStatus(const char *ifname, const char* lowerlayers)
{
    UCHAR buf[MAX_VAL_LEN] = {0};
    UCHAR *lowerIntf = NULL;
    CHAR *sp = NULL;
    UINT  up;

    if (getInterfaceStatus(ifname, &up) != INTF_STATUS_SUCCESS)
        return COSA_DML_TUNNEL_IF_ERROR;
    if (!up)
        return COSA_DML_TUNNEL_IF_DOWN;

    snprintf(buf, sizeof(buf), "%s", lowerlayers);

    if ((lowerIntf = strtok_r(buf, ",", &sp)) == NULL) /* no lower if */
        return COSA_DML_TUNNEL_IF_ERROR;
    if (getInterfaceStatus(lowerIntf, &up) != INTF_STATUS_SUCCESS)
        return COSA_DML_TUNNEL_IF_ERROR;
    if (!up)
        return COSA_DML_TUNNEL_IF_LOWERLAYERDOWN;

    while ((lowerIntf = strtok_r(NULL, ",", &sp)) != NULL)
    {
        if (getInterfaceStatus(lowerIntf, &up) != INTF_STATUS_SUCCESS)
            return COSA_DML_TUNNEL_IF_ERROR;
        if (!up)
            return COSA_DML_TUNNEL_IF_LOWERLAYERDOWN;
    }
    return COSA_DML_TUNNEL_IF_UP;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGRETunnelGetNumberOfEntries
            (
                ANSC_HANDLE                 hContext,
            );

    Description:

        Backend implementation to get the number of GRE Tunnels.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

    Return:       The number of GRE Tunnel entries.

**********************************************************************/
ULONG CosaDmlGRETunnelGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    PDMSB_TR181_GRE_CONTEXT    pGREContext = (PDMSB_TR181_GRE_CONTEXT)hContext;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pGREContext->GRETunnelInit )
    {
        CosaDmlGRETunnelLoadConf(pGREContext);
        pGREContext->GRETunnelInit = TRUE;
    }
    return AnscSListQueryDepth(&pGREContext->GRETunnelTR181List);
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGRETunnelIfGetNumberOfEntries
            (
                ANSC_HANDLE                 hContext,
                ULONG                       TunnelInstanceNumber
            );

    Description:

        Backend implementation to get the number of GRE Tunnel
        Interface entries

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       TunnelInstanceNumber
                  Instance number of the Tunnel of the interface

    Return:       The number of GRE Tunnel Interface entries

**********************************************************************/
ULONG CosaDmlGRETunnelIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       TunnelInstanceNumber
    )
{
    PDMSB_TR181_GRE_CONTEXT    pGREContext = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PSINGLE_LINK_ENTRY         pSLinkEntry;
    PDMSB_TR181_GRE_TUNNEL     pGRETunnel;

    AnscTraceFlow(("%s...\n", __FUNCTION__));
    pSLinkEntry = AnscSListGetEntryByIndex(&pGREContext->GRETunnelTR181List, TunnelInstanceNumber);
    if ( pSLinkEntry )
    {
        pGRETunnel = ACCESS_DMSB_TR181_GRE_TUNNEL(pSLinkEntry);
        return  AnscSListQueryDepth(&pGRETunnel->InterfaceTR181List);
    }
    else
    {
        return 0;
    }
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGRETunnelLoadConf
        (
            PDMSB_TR181_GRE_CONTEXT pGREContext
        )

    Description:

        Read the default Tunnel and Interface configuration
        from PSM module

    Arguments:    PDMSB_TR181_GRE_CONTEXT pGREContext
                  Backend context of the GRE object

    Return:       The status of the operation

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelLoadConf
    (
        PDMSB_TR181_GRE_CONTEXT pGREContext
    )
{
    char rec[MAX_REC_LEN] = {0};
    char val[MAX_VAL_LEN] = {0};
    int tunnelInst = 0;
    int ifInstNumber = 0;
    int                               iReturnValue    = CCSP_SUCCESS;
    int                               iIfReturnValue  = CCSP_SUCCESS;
    int                               iNumInst        = 0;
    UINT*                             pInstArray      = NULL;
    int                               iNumIfInst      = 0;
    UINT*                             pInstIfArray    = NULL;
    ULONG                             ulIndex         = 0;
    ULONG                             ulIfIndex       = 0;
    char                              pParamPath[MAX_PARAM_LEN]  = {0};
    PDMSB_TR181_GRE_TUNNEL            pGRETunnel      = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF         pGRETunnelIf    = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    iReturnValue =
        PsmGetNextLevelInstances
            (
                g_MessageBusHandle,
                g_SubsystemPrefix,
                DMSB_GRE_TUNNEL,
                &iNumInst,
                &pInstArray
            );

    if ( iReturnValue != CCSP_SUCCESS )
    {
        AnscTraceWarning(("%s -- PsmGetNextLevelInstances failed, error code = %d!\n", __FUNCTION__, iReturnValue));
        goto  EXIT;
    }
    //Sort the array from PSMGetNextLevelInstances to give the correct order of GRE Instances created
    qsort (pInstArray , iNumInst, sizeof(int), _PSM_Num_Cmp);

    for ( ulIndex = 0; ulIndex < iNumInst; ulIndex++ )
    {
        pGRETunnel = (PDMSB_TR181_GRE_TUNNEL)AnscAllocateMemory(sizeof(DMSB_TR181_GRE_TUNNEL));

        if ( !pGRETunnel )
        {
            AnscTraceWarning(("%s -- insufficient resources, GRE Tunnel instance %lu\n", __FUNCTION__, ulIndex));
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pGRETunnel->Cfg.TunnelInstanceNumber = pInstArray[ulIndex];
        }
        tunnelInst = pInstArray[ulIndex];

        snprintf(rec, sizeof(rec), GRE_TUNNEL_ENABLE, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.Enable = atoi(val);

        snprintf(rec, sizeof(rec), GRE_TUNNEL_ALIAS, tunnelInst);
        if (_PSM_GRE_Get(rec, pGRETunnel->Cfg.Alias, sizeof(pGRETunnel->Cfg.Alias)) != PSM_SUCCESS)
            return PSM_FAIL;

        snprintf(rec, sizeof(rec), GRE_TUNNEL_REMOTEENDPOINTS, tunnelInst);
        if (_PSM_GRE_Get(rec, pGRETunnel->Cfg.RemoteEndpoints, sizeof(pGRETunnel->Cfg.RemoteEndpoints)) != PSM_SUCCESS)
            return PSM_FAIL;

        snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVEPOLICY, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.KeepAlivePolicy = atoi(val);

        snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVETIMEOUT, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.KeepAliveTimeout = atoi(val);

        snprintf(rec, sizeof(rec),GRE_TUNNEL_KEEPALIVETHRESHOLD , tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.KeepAliveThreshold = atoi(val);

        snprintf(rec, sizeof(rec), GRE_TUNNEL_DELIVERYHEADERPROTOCOL, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.DeliveryHeaderProtocol = atoi(val);

        snprintf(rec, sizeof(rec),GRE_TUNNEL_DEFAULTDSCPMARK, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.DefaultDSCPMark = atoi(val);

        snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVECOUNT, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.KeepAliveCount = atoi(val);

        snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVEINTERVAL, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.KeepAliveInterval = atoi(val);

        snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVEFAILUREINTERVAL, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.KeepAliveFailureInterval = atoi(val);

        snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVERECOVERINTERVAL, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.KeepAliveRecoverInterval = atoi(val);

        snprintf(rec, sizeof(rec), GRE_TUNNEL_MSSCLAMPINGVALUE, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.MSSClampingValue = atoi(val);

        snprintf(rec, sizeof(rec),GRE_TUNNEL_SNOOP_CIRCUIT_ENABLE, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.SnoopCircuitEnable = atoi(val);

        snprintf(rec, sizeof(rec),GRE_TUNNEL_SNOOP_REMOTE_ENABLE, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.SnoopRemoteEnable = atoi(val);

        snprintf(rec, sizeof(rec), GRE_TUNNEL_MAX_CLIENTS, tunnelInst);
        if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
            return PSM_FAIL;
        pGRETunnel->Cfg.MaxClients = atoi(val);

        _ansc_sprintf
            (
                pParamPath,
                DMSB_GRE_TUNNEL "%d." GRE_INTERFACE,
                tunnelInst
            );
        iIfReturnValue =
            PsmGetNextLevelInstances
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    &iNumIfInst,
                    &pInstIfArray
                );

        if ( iIfReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning(("%s -- PsmGetNextLevelInstances failed, error code = %d!\n", __FUNCTION__, iIfReturnValue));
        }
        else
        {
            AnscSListInitializeHeader(&pGRETunnel->InterfaceTR181List);

            for ( ulIfIndex = 0; ulIfIndex < iNumIfInst; ulIfIndex++ )
            {
                pGRETunnelIf = (PDMSB_TR181_GRE_TUNNEL_IF)AnscAllocateMemory(sizeof(DMSB_TR181_GRE_TUNNEL_IF));
                if ( !pGRETunnelIf )
                {
                    AnscTraceWarning(("%s -- insufficient resources, GRE Tunnel Interface instance %lu\n", __FUNCTION__, ulIfIndex));
                    return  ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pGRETunnelIf->Cfg.IfInstanceNumber = pInstIfArray[ulIfIndex];
                }
                ifInstNumber = pInstIfArray[ulIfIndex];

               snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_ENABLE, tunnelInst, ifInstNumber);
               if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
                   return PSM_FAIL;
               pGRETunnelIf->Cfg.Enable = atoi(val);

               snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_ALIAS , tunnelInst, ifInstNumber);
               if (_PSM_GRE_Get(rec, pGRETunnelIf->Cfg.Alias, sizeof(pGRETunnelIf->Cfg.Alias)) != PSM_SUCCESS)
                   return PSM_FAIL;

               snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_LOWERLAYERS, tunnelInst, ifInstNumber);
               if (_PSM_GRE_Get(rec, pGRETunnelIf->Cfg.LowerLayers, sizeof(pGRETunnelIf->Cfg.LowerLayers)) != PSM_SUCCESS)
                   return PSM_FAIL;

               snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_PROTOCOLIDOVERRIDE, tunnelInst, ifInstNumber);
               if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
                   return PSM_FAIL;
               pGRETunnelIf->Cfg.ProtocolIdOverride = atoi(val);

               snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_USECHECKSUM , tunnelInst, ifInstNumber);
               if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
                   return PSM_FAIL;
               pGRETunnelIf->Cfg.UseChecksum = atoi(val);

               snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_KEYIDENTIFIERGENERATIONPOLICY , tunnelInst, ifInstNumber);
               if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
                   return PSM_FAIL;
               pGRETunnelIf->Cfg.KeyIdentifierGenerationPolicy = atoi(val);

               snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_KEYIDENTIFIER, tunnelInst, ifInstNumber);
               if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
                   return PSM_FAIL;
               pGRETunnelIf->Cfg.KeyIdentifier = atoi(val);

               snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_USESEQUENCENUMBER, tunnelInst, ifInstNumber);
               if (_PSM_GRE_Get(rec, val, sizeof(val)) != PSM_SUCCESS)
                   return PSM_FAIL;
               pGRETunnelIf->Cfg.UseSequenceNumber = atoi(val);

               snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_NAME, tunnelInst, ifInstNumber);
               if (_PSM_GRE_Get(rec, pGRETunnelIf->Info.Name, sizeof(pGRETunnelIf->Info.Name)) != PSM_SUCCESS)
                   return PSM_FAIL;

               pGRETunnelIf->Info.LastChange = AnscGetTickInSeconds();

               AnscSListPushEntryAtBack(&pGRETunnel->InterfaceTR181List, &pGRETunnelIf->Linkage);
            }
        }
    AnscSListPushEntryAtBack(&pGREContext->GRETunnelTR181List, &pGRETunnel->Linkage);
    }

EXIT:
    if ( pInstArray )
    {
        AnscFreeMemory(pInstArray);
        pInstArray = NULL;
    }
    if ( pInstIfArray )
    {
        AnscFreeMemory(pInstIfArray);
        pInstIfArray = NULL;
    }

    return  iReturnValue;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelSaveConf
        (
            PDMSB_TR181_GRE_CONTEXT     pGREContext,
            COSA_DML_GRE_TUNNEL_CFG *cfg
        )

    Description:

        Save the Tunnel configuration into PSM module

    Arguments:    PDMSB_TR181_GRE_CONTEXT pGREContext
                  Backend context of the GRE object

                  COSA_DML_GRE_TUNNEL_CFG     *cfg
                  Configuartion which has to be stored in PSM

    Return:       The status of the operation

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelSaveConf
    (
            PDMSB_TR181_GRE_CONTEXT     pGREContext,
            COSA_DML_GRE_TUNNEL_CFG     *cfg
    )
{
    char rec[MAX_REC_LEN] = {0};
    char val[MAX_VAL_LEN] = {0};
    int tunnelInst        = 0;
    int iReturnValue      = CCSP_SUCCESS;

    tunnelInst = cfg->TunnelInstanceNumber;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_ENABLE, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->Enable);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_ALIAS, tunnelInst);
    if (_PSM_GRE_Set(rec, cfg->Alias) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_REMOTEENDPOINTS,tunnelInst);
    if (_PSM_GRE_Set(rec, cfg->RemoteEndpoints) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVEPOLICY, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->KeepAlivePolicy);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVETIMEOUT, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->KeepAliveTimeout);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVETHRESHOLD, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->KeepAliveThreshold);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_DELIVERYHEADERPROTOCOL, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->DeliveryHeaderProtocol);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_DEFAULTDSCPMARK, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->DefaultDSCPMark);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVECOUNT, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->KeepAliveCount);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVEINTERVAL, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->KeepAliveInterval);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVEFAILUREINTERVAL, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->KeepAliveFailureInterval);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_KEEPALIVERECOVERINTERVAL, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->KeepAliveRecoverInterval);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_MSSCLAMPINGVALUE, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->MSSClampingValue);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec),GRE_TUNNEL_SNOOP_CIRCUIT_ENABLE, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->SnoopCircuitEnable);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec),GRE_TUNNEL_SNOOP_REMOTE_ENABLE, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->SnoopRemoteEnable);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec),GRE_TUNNEL_MAX_CLIENTS, tunnelInst);
    snprintf(val, sizeof(val), "%d", cfg->MaxClients);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    return iReturnValue;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelSaveConf
        (
            ULONG                       TunnelInstanceNum,
            COSA_DML_GRE_TUNNEL_IF_CFG  *cfg
        )

    Description:

        Save the Tunnel configuration into PSM module

    Arguments:    ULONG                       TunnelInstanceNum
                  Instance number of the Tunnel of the interface

                  COSA_DML_GRE_TUNNEL_IF_CFG     *cfg
                  Configuartion which has to be stored in PSM

    Return:       The status of the operation

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelIfSaveConf
    (
        ULONG                         TunnelInstanceNum,
        PCOSA_DML_GRE_TUNNEL_IF_FULL  pGRETunnelIf
    )
{
    char rec[MAX_REC_LEN] = {0};
    char val[MAX_VAL_LEN] = {0};
    int  ifInstNum        = 0;
    int  iReturnValue     = CCSP_SUCCESS;

    ifInstNum = pGRETunnelIf->Cfg.IfInstanceNumber;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_ENABLE, TunnelInstanceNum, ifInstNum);
    snprintf(val, sizeof(val), "%d", pGRETunnelIf->Cfg.Enable);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_ALIAS , TunnelInstanceNum, ifInstNum);
    if (_PSM_GRE_Set(rec, pGRETunnelIf->Cfg.Alias) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_LOWERLAYERS, TunnelInstanceNum, ifInstNum);
    if (_PSM_GRE_Set(rec, pGRETunnelIf->Cfg.LowerLayers) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_PROTOCOLIDOVERRIDE, TunnelInstanceNum, ifInstNum);
    snprintf(val, sizeof(val), "%d", pGRETunnelIf->Cfg.ProtocolIdOverride);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_USECHECKSUM , TunnelInstanceNum, ifInstNum);
    snprintf(val, sizeof(val), "%d", pGRETunnelIf->Cfg.UseChecksum);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_KEYIDENTIFIERGENERATIONPOLICY , TunnelInstanceNum, ifInstNum);
    snprintf(val, sizeof(val), "%d", pGRETunnelIf->Cfg.KeyIdentifierGenerationPolicy);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_KEYIDENTIFIER, TunnelInstanceNum, ifInstNum);
    snprintf(val, sizeof(val), "%d", pGRETunnelIf->Cfg.KeyIdentifier);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_USESEQUENCENUMBER, TunnelInstanceNum, ifInstNum);
    snprintf(val, sizeof(val), "%d", pGRETunnelIf->Cfg.UseSequenceNumber);
    if (_PSM_GRE_Set(rec, val) != PSM_SUCCESS)
        return PSM_FAIL;

    snprintf(rec, sizeof(rec), GRE_TUNNEL_IF_NAME , TunnelInstanceNum, ifInstNum);
    if (_PSM_GRE_Set(rec, pGRETunnelIf->Info.Name) != PSM_SUCCESS)
        return PSM_FAIL;

    return iReturnValue;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        PDMSB_TR181_GRE_TUNNEL
        CosaDmlGRETunnelFindByInstNum
        (
            PDMSB_TR181_GRE_CONTEXT                 pGREContext,
            ULONG                                   ulInstNum
        )

    Description:

        Find the GRE Tunnel structure based on the Instance number

    Arguments:    PDMSB_TR181_GRE_CONTEXT pGREContext
                  Backend context of the GRE object

                  ULONG                       ulInstNum
                  Instance number of the Tunnel

    Return:       The structure of the GRE Tunnel needed

**********************************************************************/
PDMSB_TR181_GRE_TUNNEL
CosaDmlGRETunnelFindByInstNum
    (
        PDMSB_TR181_GRE_CONTEXT                 pGREContext,
        ULONG                                   ulInstNum
    )
{
    PSINGLE_LINK_ENTRY        pSLinkEntry = NULL;
    PDMSB_TR181_GRE_TUNNEL    pGRETunnel  = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pSLinkEntry = AnscSListGetFirstEntry(&pGREContext->GRETunnelTR181List);

    while ( pSLinkEntry )
    {
        pGRETunnel  = ACCESS_DMSB_TR181_GRE_TUNNEL(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pGRETunnel->Cfg.TunnelInstanceNumber == ulInstNum )
        {
            return  pGRETunnel;
        }
    }
    AnscTraceWarning(("%s -- failed to find %lu!\n", __FUNCTION__, ulInstNum));
    return  NULL;
}
/**********************************************************************

    caller:     COSA DML

    prototype:

        PDMSB_TR181_GRE_TUNNEL_IF
        CosaDmlGRETunnelIfFindByInstNum
        (
            PDMSB_TR181_GRE_TUNNEL                  pGRETunnel,
            ULONG                                   ulInstNum
        )

    Description:

        Find the GRE Tunnel Interface structure based on the Instance number

    Arguments:    PDMSB_TR181_GRE_TUNNEL  pGRETunel
                  Tunnel from which interface has to be found

                  ULONG                       ulInstNum
                  Instance number of the Tunnel Interface

    Return:       The structure of the GRE Tunnel Interface needed

**********************************************************************/

PDMSB_TR181_GRE_TUNNEL_IF
CosaDmlGRETunnelIfFindByInstNum
    (

        PDMSB_TR181_GRE_TUNNEL                  pGRETunnel,
        ULONG                                   ulInstNum
    )
{
    PSINGLE_LINK_ENTRY           pSLinkEntry   = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF    pGRETunnelIf  = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pSLinkEntry = AnscSListGetFirstEntry(&pGRETunnel->InterfaceTR181List);

    while ( pSLinkEntry )
    {
        pGRETunnelIf  = (PDMSB_TR181_GRE_TUNNEL_IF)pSLinkEntry;
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
        if ( pGRETunnelIf->Cfg.IfInstanceNumber == ulInstNum )
        {
            return  pGRETunnelIf;
        }
    }
    AnscTraceWarning(("%s -- failed to find %lu!\n", __FUNCTION__, ulInstNum));
    return  NULL;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelGetCfg
        (
            ANSC_HANDLE                 hContext,
            PCOSA_DML_GRE_TUNNEL_CFG    pCfg
        )

    Description:

        Backend implementation to get GRE Tunnel configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_GRE_TUNNEL_CFG    pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_GRE_TUNNEL_CFG    pCfg
    )
{
    PDMSB_TR181_GRE_CONTEXT    pGREContext = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PDMSB_TR181_GRE_TUNNEL     pGRETunnel = NULL;
    char                       buf[MAX_REC_LEN]        = {0};
    char                       param_name[MAX_REC_LEN] = {0};

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pGRETunnel = CosaDmlGRETunnelFindByInstNum(pGREContext, pCfg->TunnelInstanceNumber);

    if ( !pGRETunnel )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        _ansc_sprintf(param_name,"sysevent get %lu_current_remoteEP", pCfg->TunnelInstanceNumber);
        _get_shell_output(param_name, buf, sizeof(buf));
        _ansc_strcpy(pGRETunnel->Cfg.ConnectedRemoteEndpoint, buf);

        AnscCopyMemory(pCfg, &pGRETunnel->Cfg, sizeof(pGRETunnel->Cfg));
        return  ANSC_STATUS_SUCCESS;
    }
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelSetCfg
        (
        ANSC_HANDLE                 hContext,
        COSA_DML_GRE_TUNNEL_CFG     *greTunnel
    )

    Description:

        Backend implementation to set the new configuration entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  COSA_DML_GRE_TUNNEL_CFG    pCfg
                  Pointer of the GRE Tunnel configuation

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelSetCfg
    (
        ANSC_HANDLE                 hContext,
        COSA_DML_GRE_TUNNEL_CFG     *greTunnel
    )
{
    PDMSB_TR181_GRE_CONTEXT pGREContext = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PDMSB_TR181_GRE_TUNNEL  pGRETunnel  = NULL;

    pGRETunnel = CosaDmlGRETunnelFindByInstNum(pGREContext, greTunnel->TunnelInstanceNumber);

    if ( !pGRETunnel )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        AnscCopyMemory(&pGRETunnel->Cfg, greTunnel, sizeof(pGRETunnel->Cfg));
    }

    if (CosaDmlGRETunnelSaveConf(pGREContext, greTunnel) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelGetEntry
        (
            ANSC_HANDLE                      hContext,
            ULONG                            ulIndex,
            PCOSA_DML_GRE_TUNNEL_FULL        pEntry
        )

    Description:

        Backend implementation to get a GRE Tunnel entry.

    Arguments:    ANSC_HANDLE                    hContext
                  Reserved.

                  ULONG                          ulIndex
                  Index of the tunnel entry to be retrieved.

                  PCOSA_DML_GRE_TUNNEL_FULL   pEntry
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_GRE_TUNNEL_FULL   pEntry
    )
{
    PDMSB_TR181_GRE_CONTEXT    pGREContext = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PSINGLE_LINK_ENTRY         pSLinkEntry = NULL;
    PDMSB_TR181_GRE_TUNNEL     pGRETunnel  = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pEntry )
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( ulIndex < AnscSListQueryDepth(&pGREContext->GRETunnelTR181List) )
    {
        pSLinkEntry = AnscSListGetEntryByIndex(&pGREContext->GRETunnelTR181List, ulIndex);

        if ( pSLinkEntry )
        {
            pGRETunnel = ACCESS_DMSB_TR181_GRE_TUNNEL(pSLinkEntry);
            AnscCopyMemory(&pEntry->Cfg, &pGRETunnel->Cfg, sizeof(pGRETunnel->Cfg));
            AnscCopyMemory(&pEntry->Info, &pGRETunnel->Info, sizeof(pGRETunnel->Info));
            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            return  ANSC_STATUS_CANT_FIND;
        }
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelIfGetEntry
        (
            ANSC_HANDLE                      hContext,
            ULONG                            ulGRETunnelInstanceNumber,
            ULONG                            ulIndex,
            PCOSA_DML_GRE_TUNNEL_IF_FULL     pEntry
        )

    Description:

        Backend implementation to get a ddns service entry.

    Arguments:    ANSC_HANDLE                    hContext
                  Reserved.

                  ULONG                          ulGRETunnelInstanceNumber
                  Instance of the tunnel

                  ULONG                          ulIndex
                  Index of the tunnel interface entry to be retrieved.

                  PCOSA_DML_GRE_TUNNEL_IF_FULL   pEntry
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelIfGetEntry
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulGRETunnelInstanceNumber,
        ULONG                            ulIndex,
        PCOSA_DML_GRE_TUNNEL_IF_FULL     pEntry
    )
{
    PDMSB_TR181_GRE_CONTEXT             pGREContext  = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PDMSB_TR181_GRE_TUNNEL              pGRETunnel   = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF           pGRETunnelIf = NULL;
    PSINGLE_LINK_ENTRY                  pSLinkEntry  = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));
    pSLinkEntry = AnscSListGetEntryByIndex(&pGREContext->GRETunnelTR181List, ulGRETunnelInstanceNumber);
    if ( pSLinkEntry )
    {
        pGRETunnel = ACCESS_DMSB_TR181_GRE_TUNNEL(pSLinkEntry);
        pGRETunnelIf = (PDMSB_TR181_GRE_TUNNEL_IF)AnscSListGetEntryByIndex(&pGRETunnel->InterfaceTR181List, ulIndex);
        if (pGRETunnelIf)
        {
            AnscCopyMemory(&pEntry->Cfg, &pGRETunnelIf->Cfg, sizeof(pGRETunnelIf->Cfg));
            AnscCopyMemory(&pEntry->Info, &pGRETunnelIf->Info, sizeof(pGRETunnelIf->Info));
        }
    }
        return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelAddEntry
            (
                ANSC_HANDLE                   hContext,
                PCOSA_DML_GRE_TUNNEL_FULL     pEntry
            );

    Description:

        Backend implementation to add a GRE service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_GRE_TUNNEL_FULL   pEntry
                  Pointer of the service to be added.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelAddEntry
    (
        ANSC_HANDLE                   hContext,
        PCOSA_DML_GRE_TUNNEL_FULL     pEntry
    )
{
    int                       iReturnValue = CCSP_SUCCESS;
    PDMSB_TR181_GRE_CONTEXT   pGREContext  = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PSINGLE_LINK_ENTRY        pSLinkEntry  = NULL;
    PDMSB_TR181_GRE_TUNNEL    pGRETunnel   = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pEntry )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pGRETunnel = (PDMSB_TR181_GRE_TUNNEL)AnscAllocateMemory(sizeof(DMSB_TR181_GRE_TUNNEL));

        if ( !pGRETunnel )
        {
            AnscTraceWarning(("%s -- insufficient resources, GRE Tunnel instance %lu\n", __FUNCTION__, pEntry->Cfg.TunnelInstanceNumber));
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscSListInitializeHeader(&pGRETunnel->InterfaceTR181List);

            AnscCopyMemory(&pGRETunnel->Cfg, &pEntry->Cfg, sizeof(pGRETunnel->Cfg));
            AnscCopyMemory(&pGRETunnel->Info, &pEntry->Info, sizeof(pGRETunnel->Info));

            AnscSListPushEntryAtBack(&pGREContext->GRETunnelTR181List, &pGRETunnel->Linkage);
            CosaDmlGRETunnelSaveConf(pGREContext, &pGRETunnel->Cfg);
        }
    }
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelIfAddEntry
            (
                ANSC_HANDLE                   hContext,
                ULONG                         ulGRETunnelInstanceNumber,
                PCOSA_DML_GRE_TUNNEL_IF_FULL  pEntry
            );

    Description:

        Backend implementation to add a GRE service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                         ulGRETunnelInstanceNumber
                  The instance number of the Tunnel to which the
                  interface is to be added

                  PCOSA_DML_GRE_TUNNEL_IF_FULL   pEntry
                  Pointer of the service to be added.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelIfAddEntry
    (
        ANSC_HANDLE                   hContext,
        ULONG                         ulGRETunnelInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_IF_FULL  pEntry
    )
{
    PDMSB_TR181_GRE_CONTEXT             pGREContext  = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PDMSB_TR181_GRE_TUNNEL              pGRETunnel   = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF           pGRETunnelIf = NULL;
    PSINGLE_LINK_ENTRY                  pSLinkEntry  = NULL;
    UCHAR                               param_name[MAX_REC_LEN] = {0};
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pSLinkEntry = AnscSListGetEntryByIndex(&pGREContext->GRETunnelTR181List, (ulGRETunnelInstanceNumber-1));
    if ( pSLinkEntry )
    {
        pGRETunnel = ACCESS_DMSB_TR181_GRE_TUNNEL(pSLinkEntry);
        pGRETunnelIf = (PDMSB_TR181_GRE_TUNNEL_IF)AnscAllocateMemory(sizeof(DMSB_TR181_GRE_TUNNEL_IF));
        if ( !pGRETunnelIf )
        {
            AnscTraceWarning(("%s -- insufficient resources, GRE Tunnel Interface instance %lu\n", __FUNCTION__, ulGRETunnelInstanceNumber));
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pEntry->Info.LastChange = AnscGetTickInSeconds();

            AnscCopyMemory(&pGRETunnelIf->Cfg, &pEntry->Cfg, sizeof(pGRETunnelIf->Cfg));
            AnscCopyMemory(&pGRETunnelIf->Info, &pEntry->Info, sizeof(pGRETunnelIf->Info));

            AnscSListPushEntryAtBack(&pGRETunnel->InterfaceTR181List, &pGRETunnelIf->Linkage);
            CosaDmlGRETunnelIfSaveConf(ulGRETunnelInstanceNumber, pEntry);
        }
    }
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelSetValues
        (
            ANSC_HANDLE                 hContext,
            ULONG                       ulIndex,
            char*                       pAlias
        )

    Description:

        Backend implementation to set values for a GRE entry and
        save the configuration into PSM

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                         ulIndex
                  The instance number of the Tunnel

                  char*                       pAlias
                  Alias value to be added to the configuration

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        char*                       pAlias
    )
{
    PDMSB_TR181_GRE_CONTEXT         pGREContext = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;
    PDMSB_TR181_GRE_TUNNEL          pGRETunnel  = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pSLinkEntry = AnscSListGetEntryByIndex(&pGREContext->GRETunnelTR181List, ulIndex);

    if ( pSLinkEntry )
    {
        pGRETunnel = ACCESS_DMSB_TR181_GRE_TUNNEL(pSLinkEntry);
        _ansc_strcpy(pGRETunnel->Cfg.Alias, pAlias);
        CosaDmlGRETunnelSaveConf(pGREContext, &pGRETunnel->Cfg);
    }
    else
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

    ANSC_STATUS
    CosaDmlGRETunnelIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulTunnelInstanceNumber,
        ULONG                       ulIfInstanceNumber,
        char*                       pAlias
    )

    Description:

        Backend implementation to set values for a GRE Interfce entry and
        save the configuration into PSM

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                         ulIndex
                  The instance number of the Tunnel

                  ULONG                       ulIfInstanceNumber
                  Instance number of Tunnel Interface

                  char*                       pAlias
                  Alias value to be added to the configuration

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulTunnelInstanceNumber,
        ULONG                       ulIfInstanceNumber,
        char*                       pAlias
    )
{
    PDMSB_TR181_GRE_CONTEXT         pGREContext  = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    PDMSB_TR181_GRE_TUNNEL          pGRETunnel   = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF       pGRETunnelIf = NULL;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pEntry       = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pSLinkEntry = AnscSListGetEntryByIndex(&pGREContext->GRETunnelTR181List, ulTunnelInstanceNumber);

    if ( pSLinkEntry )
    {
        pGRETunnel = ACCESS_DMSB_TR181_GRE_TUNNEL(pSLinkEntry);
        pGRETunnelIf = (PDMSB_TR181_GRE_TUNNEL_IF)AnscSListGetEntryByIndex(&pGRETunnel->InterfaceTR181List, ulIfInstanceNumber);
        if (pGRETunnelIf)
        {
            _ansc_strcpy(pGRETunnelIf->Cfg.Alias, pAlias);
            AnscCopyMemory(&pEntry->Cfg, &pGRETunnelIf->Cfg, sizeof(pGRETunnelIf->Cfg));
            AnscCopyMemory(&pEntry->Info, &pGRETunnelIf->Info, sizeof(pGRETunnelIf->Info));
            CosaDmlGRETunnelIfSaveConf(ulTunnelInstanceNumber, pEntry);
        }
    }
    else
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelIfSetCfg
        (
            ANSC_HANDLE                   hContext,
            ULONG                         ulGRETunnelInstanceNumber,
            PCOSA_DML_GRE_TUNNEL_IF_FULL  pEntry
        )

    Description:

        Backend implementation to set the new configuration entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  ULONG                         ulGRETunnelInstanceNumber
                  Instance of the Tunnel of the interface

                  PCOSA_DML_GRE_TUNNEL_IF_FULL    pEntry
                  Pointer of the GRE Tunnel Interface structure

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelIfSetCfg
    (
        ANSC_HANDLE                   hContext,
        ULONG                         ulGRETunnelInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_IF_FULL  pEntry
    )
{
    PDMSB_TR181_GRE_CONTEXT             pGREContext               = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PDMSB_TR181_GRE_TUNNEL              pGRETunnel                = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF           pGRETunnelIf              = NULL;
    PSINGLE_LINK_ENTRY                  pSLinkEntry               = NULL;
    char                                param_name[MAX_REC_LEN]   = {0};

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pEntry->Info.LastChange = AnscGetTickInSeconds();

    pGRETunnel = CosaDmlGRETunnelFindByInstNum(pGREContext, ulGRETunnelInstanceNumber);

    if ( !pGRETunnel )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        pGRETunnelIf = CosaDmlGRETunnelIfFindByInstNum(pGRETunnel, pEntry->Cfg.IfInstanceNumber);
        if (pGRETunnelIf)
        {
            AnscCopyMemory(&pGRETunnelIf->Cfg, &pEntry->Cfg, sizeof(pEntry->Cfg));
        }
    }

    CosaDmlGRETunnelIfSaveConf(ulGRETunnelInstanceNumber, pEntry);

    if (pEntry->Cfg.Enable == true)
    {
        //Calling the service_gre.sh script to create a GRE tunnel
        _ansc_sprintf(param_name, "sysevent set igre-start %d", ulGRETunnelInstanceNumber);
        system(param_name);
    }
    else
    {
        _ansc_sprintf(param_name, "sysevent set igre-stop %d", ulGRETunnelInstanceNumber);
        system(param_name);
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelIfGetCfg
        (
            ANSC_HANDLE                  hContext,
            ULONG                        ulGRETunnelInstanceNumber,
            PCOSA_DML_GRE_TUNNEL_IF_FULL pEntry
        )

    Description:

        Backend implementation to get GRE Tunnel configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                         ulGRETunnelInstanceNumber
                  Instance of the Tunnel of the interface

                  PCOSA_DML_GRE_TUNNEL_IF_FULL    pEntry
                  Pointer of the Tunnel Interface structure to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelIfGetCfg
    (
        ANSC_HANDLE                  hContext,
        ULONG                        ulGRETunnelInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_IF_FULL pEntry
    )
{
    PDMSB_TR181_GRE_CONTEXT             pGREContext  = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PDMSB_TR181_GRE_TUNNEL              pGRETunnel   = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF           pGRETunnelIf = NULL;
    PSINGLE_LINK_ENTRY                  pSLinkEntry  = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pGRETunnel = CosaDmlGRETunnelFindByInstNum(pGREContext, ulGRETunnelInstanceNumber);

    if ( !pGRETunnel )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        pGRETunnelIf = CosaDmlGRETunnelIfFindByInstNum(pGRETunnel, pEntry->Cfg.IfInstanceNumber);
        if (pGRETunnelIf)
        {
            AnscCopyMemory(&pEntry->Cfg, &pGRETunnelIf->Cfg, sizeof(pGRETunnelIf->Cfg));
        }
    }
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelGetInfo
        (
            ANSC_HANDLE                  hContext,
            ULONG                        ulGRETunnelInstanceNumber,
            PCOSA_DML_GRE_TUNNEL_INFO    pEntry
        )

    Description:

        Backend implementation to get GRE Tunnel configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                         ulGRETunnelInstanceNumber
                  Instance of the Tunnel

                  PCOSA_DML_GRE_TUNNEL_INFO    pEntry
                  Pointer of the Tunnel Info structure to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulGRETunnelInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_INFO   pEntry
    )
{
    PDMSB_TR181_GRE_CONTEXT    pGREContext = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PDMSB_TR181_GRE_TUNNEL     pGRETunnel = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pGRETunnel = CosaDmlGRETunnelFindByInstNum(pGREContext, ulGRETunnelInstanceNumber);

    if ( !pGRETunnel )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        if (pGRETunnel->Cfg.Enable == true)
        {
            pGRETunnel->Info.Status = COSA_DML_GRE_TUNNEL_ENABLED;
        }
        else
        {
            pGRETunnel->Info.Status = COSA_DML_GRE_TUNNEL_DISABLED;
        }
        AnscCopyMemory(pEntry, &pGRETunnel->Info, sizeof(pGRETunnel->Info));
        return  ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

       ANSC_STATUS
       CosaDmlGRETunnelGetStats
           (
            ANSC_HANDLE                          hContext,
            ULONG                                ulIndex,
            PCOSA_DML_GRE_TUNNEL_STATS           pStats
           );

    Description:

        Get the GRE Tunnel Statistics

    Arguments:
              ANSC_HANDLE                          hContext,
              Reserved

              ULONG                                ulIndex,
              GRE Tunnel instance

              PCOSA_DML_GRE_TUNNEL_STATS           pStats
              GRE Tunnel Statistics
    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelGetStats
    (
        ANSC_HANDLE                          hContext,
        ULONG                                ulIndex,
        PCOSA_DML_GRE_TUNNEL_STATS           pStats
    )
{
    PDMSB_TR181_GRE_CONTEXT          pGREContext             = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PDMSB_TR181_GRE_TUNNEL           pGRETunnel              = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF        pIfCurObj               = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF        pIfNextObj              = NULL;
    UCHAR                            intfName[MAX_REC_LEN]   = {0};
    PCOSA_DML_IF_STATS               stats                   = NULL;
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pGRETunnel = CosaDmlGRETunnelFindByInstNum(pGREContext, ulIndex);
    if ( !pGRETunnel )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        pIfCurObj = (PDMSB_TR181_GRE_TUNNEL_IF)AnscSListGetFirstEntry(&pGRETunnel->InterfaceTR181List);
        stats = (PCOSA_DML_IF_STATS)AnscAllocateMemory(sizeof(COSA_DML_IF_STATS));

        _ansc_memset(pStats, 0, sizeof(COSA_DML_GRE_TUNNEL_STATS));

        pStats->KeepAliveSent = 0;
        pStats->KeepAliveReceived = 0;

        /* These two stats will be assigned if the GRE Tunnel is used for Hotspot process */
        if(!(strncmp(pGRETunnel->Cfg.Alias, HOTSPOT_GRE_ALIAS, sizeof(HOTSPOT_GRE_ALIAS))))
        {
             pStats->KeepAliveSent = g_hsfdStat->keepAlivesSent;
             pStats->KeepAliveReceived = g_hsfdStat->keepAlivesReceived;
        }

        while(pIfCurObj != NULL)
        {
            strncpy(intfName,pIfCurObj->Info.Name,MAX_REC_LEN);
            if (CosaUtilGetIfStats(intfName,  stats))
            {
                 pStats->BytesSent += stats->BytesSent;
                 pStats->BytesReceived += stats->BytesReceived;
                 pStats->PacketsSent += stats->PacketsSent;
                 pStats->PacketsReceived += stats->PacketsReceived;
                 pStats->ErrorsSent += stats->ErrorsSent;
                 pStats->ErrorsReceived += stats->ErrorsReceived;
                 pStats->MulticastPacketsReceived += stats->MulticastPacketsReceived;
                 pStats->DiscardPacketsSent += stats->DiscardPacketsSent;
                 pStats->DiscardPacketsReceived += stats->DiscardPacketsReceived;
            }
            pIfNextObj = (PDMSB_TR181_GRE_TUNNEL_IF)pIfCurObj->Linkage.Next;
            pIfCurObj = pIfNextObj;
        }
        AnscFreeMemory(stats);
        return ANSC_STATUS_SUCCESS;
    }
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelGetInfo
        (
            ANSC_HANDLE                    hContext,
            ULONG                          ulGRETunnelInstanceNumber,
            ULONG                          ulIfInstanceNumber,
            PCOSA_DML_GRE_TUNNEL_IF_INFO   pEntry
        )

    Description:

        Backend implementation to get GRE Tunnel configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                         ulGRETunnelInstanceNumber
                  Instance of the Tunnel of the interface

                  ULONG                          ulIfInstanceNumber
                  Instance of the Tunnel interface

                  PCOSA_DML_GRE_TUNNEL_INFO    pEntry
                  Pointer of the Tunnel Info structure to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelIfGetInfo
    (
        ANSC_HANDLE                    hContext,
        ULONG                          ulGRETunnelInstanceNumber,
        ULONG                          ulIfInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_IF_INFO   pEntry
    )
{
    PDMSB_TR181_GRE_CONTEXT             pGREContext  = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PDMSB_TR181_GRE_TUNNEL              pGRETunnel   = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF           pGRETunnelIf = NULL;
    PSINGLE_LINK_ENTRY                  pSLinkEntry  = NULL;
    ULONG                               ulIndex;
    UCHAR                               ucEntryParamName[MAX_REC_LEN]       = {0};
    UCHAR                               ucEntryNameValue[MAX_REC_LEN]       = {0};
    ULONG                               ulEntryNameLen = MAX_REC_LEN;
    UCHAR                               lowerLayers[MAX_REC_LEN];
    COSA_DML_LINK_TYPE                  linkType;
    UINT                                linkInstNum;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pGRETunnel = CosaDmlGRETunnelFindByInstNum(pGREContext, ulGRETunnelInstanceNumber);

    if ( !pGRETunnel )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        pGRETunnelIf = CosaDmlGRETunnelIfFindByInstNum(pGRETunnel, ulIfInstanceNumber);

        if (pGRETunnelIf)
        {
            AnscCopyMemory(pEntry, &pGRETunnelIf->Info, sizeof(pGRETunnelIf->Info));
            /* Retrieve  Interface Name */
            _ansc_snprintf(ucEntryParamName, sizeof(ucEntryParamName), "%s.%s", pGRETunnelIf->Cfg.LowerLayers, "Name");
            ulEntryNameLen = sizeof(ucEntryNameValue);

            if ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen))
            {
                AnscCopyString(lowerLayers, ucEntryNameValue);
            }
            pEntry->Status = CosaDmlGRETunnelIfGetStatus(pEntry->Name, lowerLayers);
        }
    }
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

       ANSC_STATUS
       CosaDmlGRETunnelIfGetStats
           (
                ANSC_HANDLE                          hContext,
                UCHAR                                intfName,
                PCOSA_DML_GRE_TUNNEL_IF_STATS        pStats
           );

    Description:

        Get the GRE Tunnel Interface statistics.

    Arguments:
                ANSC_HANDLE                             hContext,
                Reserved

                UCHAR                                   intfName,
                Name of the tunnel interface

                PCOSA_DML_GRE_TUNNEL_IF_STATS           pStats
                Tunnel interface statistics

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelIfGetStats
    (
        ANSC_HANDLE                          hContext,
        UCHAR*                               intfName,
        PCOSA_DML_GRE_TUNNEL_IF_STATS        pStats
    )
{
    FILE *fp                                   = NULL;
    int DiscardChecksumReceived                = 0;
    int DiscardSequenceNumberReceived          = 0;
    UCHAR stats_file_crc_errors[MAX_VAL_LEN]   = {0};
    UCHAR stats_file_fifo_errors[MAX_VAL_LEN]  = {0};
    PCOSA_DML_IF_STATS stats                   = (PCOSA_DML_IF_STATS)AnscAllocateMemory(sizeof(COSA_DML_IF_STATS));

    _ansc_memset(pStats, 0, sizeof(COSA_DML_GRE_TUNNEL_IF_STATS));

    snprintf(stats_file_crc_errors,MAX_VAL_LEN,"/sys/class/net/%s/statistics/rx_crc_errors",intfName);
    snprintf(stats_file_fifo_errors,MAX_VAL_LEN,"/sys/class/net/%s/statistics/rx_fifo_errors",intfName);

    fp = fopen(stats_file_crc_errors,"r");
    if(fp)
    {
        fscanf(fp,"%d",&DiscardChecksumReceived);
        fclose(fp);
    }

    fp = fopen(stats_file_fifo_errors,"r");
    if(fp)
    {
        fscanf(fp,"%d",&DiscardSequenceNumberReceived);
        fclose(fp);
    }

    pStats->DiscardChecksumReceived =  DiscardChecksumReceived;
    pStats->DiscardSequenceNumberReceived = DiscardSequenceNumberReceived;

    if (CosaUtilGetIfStats(intfName,  stats))
    {
        pStats->BytesSent = stats->BytesSent;
        pStats->BytesReceived = stats->BytesReceived;
        pStats->PacketsSent = stats->PacketsSent;
        pStats->PacketsReceived = stats->PacketsReceived;
        pStats->ErrorsSent = stats->ErrorsSent;
        pStats->ErrorsReceived = stats->ErrorsReceived;
        pStats->MulticastPacketsReceived = stats->MulticastPacketsReceived;
        pStats->DiscardPacketsSent = stats->DiscardPacketsSent;
        pStats->DiscardPacketsReceived = stats->DiscardPacketsReceived;

        AnscFreeMemory(stats);

        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        AnscFreeMemory(stats);
        return ANSC_STATUS_CANT_FIND;
    }
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelDelEntry
        (
            ANSC_HANDLE                 hContext,
            ULONG                       ulInstanceNumber
        )

    Description:

        Backend implementation to delete a GRE Tunnel entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  ULONG                      ulGRETunnelInstanceNumber
                  Instance number of the tunnel to be deleted

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    PDMSB_TR181_GRE_CONTEXT          pGREContext = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PDMSB_TR181_GRE_TUNNEL           pGRETunnel  = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF        pIfCurObj   = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF        pIfNextObj  = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pGRETunnel = CosaDmlGRETunnelFindByInstNum(pGREContext, ulInstanceNumber);
   if ( !pGRETunnel )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        CosaDmlGRETunnelDelPsm(&pGRETunnel->Cfg);
        pIfCurObj = (PDMSB_TR181_GRE_TUNNEL_IF)AnscSListGetFirstEntry(&pGRETunnel->InterfaceTR181List);
        while(pIfCurObj != NULL)
        {
            pIfNextObj = (PDMSB_TR181_GRE_TUNNEL_IF)pIfCurObj->Linkage.Next;
            AnscSListPopEntryByLink(&pGRETunnel->InterfaceTR181List, &pIfCurObj->Linkage);
            pIfCurObj = pIfNextObj;
        }
        pIfCurObj = NULL;
        AnscSListPopEntryByLink(&pGREContext->GRETunnelTR181List, &pGRETunnel->Linkage);
        AnscFreeMemory(pGRETunnel);
        return  ANSC_STATUS_SUCCESS;
    }
}

/**********************************************************************

    caller:     COSA DML

    prototype:


    ANSC_STATUS
    CosaDmlGRETunnelDelPsm
    (
        PCOSA_DML_GRE_TUNNEL_CFG     pCfg
    )

    Description:

        Backend implementation to delete GRE Tunnel entry
        from bbhm_cur_cfg.xml file

    Arguments:    PCOSA_DML_GRE_TUNNEL_CFG     pCfg
                  Configuration to be deleted from the PSM

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelDelPsm
    (
        PCOSA_DML_GRE_TUNNEL_CFG     pCfg
    )
{
    int    iReturnValue              = CCSP_SUCCESS;
    char   pParamPath[MAX_PARAM_LEN] = {0};

    AnscTraceFlow(("%s...\n", __FUNCTION__));

        _ansc_sprintf
            (
                pParamPath,
                DMSB_GRE_TUNNEL "%d.",
                pCfg->TunnelInstanceNumber
            );

        iReturnValue =
            PSM_Del_Record
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning(("%s -- failed to delete PSM records, error code %d", __FUNCTION__, iReturnValue));
        }
    return  CCSP_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlGRETunnelIfDelEntry
        (
            ANSC_HANDLE                  hContext,
            ULONG                        ulGRETunnelInstanceNumber,
            PCOSA_DML_GRE_TUNNEL_IF_FULL pInterface
        )

    Description:

        Backend implementation to delete a GRE Tunnel Interface entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  ULONG                      ulGRETunnelInstanceNumber
                  Instance number of the tunnel

                  PCOSA_DML_GRE_TUNNEL_IF_FULL pInterface
                  Pointer of the interface structure to be deleted

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelIfDelEntry
    (
        ANSC_HANDLE                  hContext,
        ULONG                        ulGRETunnelInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_IF_FULL pInterface
    )
{
    PDMSB_TR181_GRE_CONTEXT             pGREContext = (PDMSB_TR181_GRE_CONTEXT)hContext;
    PDMSB_TR181_GRE_TUNNEL              pGRETunnel  = NULL;
    PDMSB_TR181_GRE_TUNNEL_IF           pIfLinkObj  = NULL;
    PSINGLE_LINK_ENTRY                  pSLinkEntry = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));
    pGRETunnel = CosaDmlGRETunnelFindByInstNum(pGREContext, ulGRETunnelInstanceNumber);

    if ( !pGRETunnel )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        pIfLinkObj = CosaDmlGRETunnelIfFindByInstNum(pGRETunnel, pInterface->Cfg.IfInstanceNumber);
        if(pIfLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find GRE Tunnel Interface instance %lu\n", __FUNCTION__, pInterface->Cfg.IfInstanceNumber));
            return ANSC_STATUS_CANT_FIND;
        }
        AnscSListPopEntryByLink(&pGRETunnel->InterfaceTR181List, &pIfLinkObj->Linkage);
        CosaDmlGRETunnelIfDelPsm(pGRETunnel->Cfg.TunnelInstanceNumber, pInterface->Cfg.IfInstanceNumber);
        AnscFreeMemory(pIfLinkObj);
    }
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:


    ANSC_STATUS
    CosaDmlGRETunnelDelPsm
    (
        ULONG                        TunnelInstanceNumber,
        ULONG                        IfInstanceNumber
    )

    Description:

        Backend implementation to delete GRE Tunnel Interface entry
        from bbhm_cur_cfg.xml file

    Arguments:    ULONG                        TunnelInstanceNumber
                  Instance number of the tunnel

                  ULONG                        IfInstanceNumber
                  Instance number of the tunnel interface

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlGRETunnelIfDelPsm
    (
        ULONG                        TunnelInstanceNumber,
        ULONG                        IfInstanceNumber
    )
{
    int    iReturnValue               = CCSP_SUCCESS;
    char   pParamPath[MAX_PARAM_LEN]  = {0};

    AnscTraceFlow(("%s...\n", __FUNCTION__));

        _ansc_sprintf
            (
                pParamPath,
                DMSB_GRE_TUNNEL "%d." GRE_INTERFACE "%d.",
                TunnelInstanceNumber,IfInstanceNumber
            );

        iReturnValue =
            PSM_Del_Record
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath
                );
        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning(("%s -- failed to delete PSM records, error code %d", __FUNCTION__, iReturnValue));
        }
    return ANSC_STATUS_SUCCESS;
}

#endif 
