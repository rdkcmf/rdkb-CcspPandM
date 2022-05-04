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

    module: cosa_common_util.c

        Common utilities

    -------------------------------------------------------------------

    description:

        This file implements common back-end apis for the COSA Data Model Library
    -------------------------------------------------------------------

    author:

        Chen Changchun <chchangc@cisco.com>

    -------------------------------------------------------------------

    revision:

        11/01/2013    initial revision.

**************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <utapi.h>
#include "cosa_common_util.h"
#include "cosa_apis_util.h"
#ifdef _HUB4_PRODUCT_REQ_
#include "cosa_lanmanagement_apis.h"
#include "ccsp_psm_helper.h"

#define PSM_LANMANAGEMENTENTRY_LAN_ULA_ENABLE  "dmsb.lanmanagemententry.lanulaenable"
#define PSM_LANMANAGEMENTENTRY_LAN_IPV6_ENABLE "dmsb.lanmanagemententry.lanipv6enable"
#define PSM_LANMANAGEMENTENTRY_LAN_ULA  "dmsb.lanmanagemententry.lanula"
#define PSM_LANMANAGEMENTENTRY_LAN_ULA_PREFIX  "dmsb.lanmanagemententry.lanulaprefix"

extern ANSC_HANDLE bus_handle;
extern void* g_pDslhDmlAgent;
void ValidUlaHandleEventAsync(void);
#endif

/**
 * common callback function dispatcher for event
 */


EVT_DISPTER_CB_LIST g_CallbackFuncList = NULL;

/*Initialize the event dispatcher callback list */
EVT_DISPTER_CB_LIST
EvtDispterInitCallbackList
    (
        EVT_DISPTER_CB_LIST *pCbList
    )
{
    EVT_DISPTER_CB_LIST       listHead = NULL;

    listHead = (EVT_DISPTER_CB_LIST)malloc(sizeof(EVT_DISPTER_CB_LIST_ENTRY));
    if(NULL == listHead)
        return NULL;

    listHead->EventCbList = NULL;
    listHead->Next = NULL;

    *pCbList = (EVT_DISPTER_CB_LIST)listHead;

    return *pCbList;
}

/*get the callback function list based on the specified event name*/
static EVT_DISPTER_EVENT_CB_LIST 
EvtDispterFindListByEventName
    (
        EVT_DISPTER_CB_LIST     cbList,
        char *                  eventName
    )
{
    PEVT_DISPTER_CB_LIST_ENTRY      pCbListEntry = NULL;
    PEVT_DISPTER_CB_LIST_ENTRY      tmpListEntry = NULL;
    PEVT_DISPTER_METHOD_NODE        pMethodNode = NULL;
    
    pCbListEntry = (PEVT_DISPTER_CB_LIST_ENTRY)cbList->Next;
    tmpListEntry = (PEVT_DISPTER_CB_LIST_ENTRY)cbList;
    /*find the event list by name ,if not exist, create it*/
    while((NULL != pCbListEntry))
    {
        if(0 == strcmp(pCbListEntry->EventName, eventName)) {
            return pCbListEntry->EventCbList;
        }
        else {
            tmpListEntry = pCbListEntry;
            pCbListEntry = pCbListEntry->Next;
        }
    }

    /*create new callback list entry by the new event name*/
    pCbListEntry = (PEVT_DISPTER_CB_LIST_ENTRY)malloc(sizeof(EVT_DISPTER_CB_LIST_ENTRY));
    if(NULL == pCbListEntry)
        return NULL;
    
    pCbListEntry->Next = NULL;
    strncpy(pCbListEntry->EventName, eventName, sizeof(pCbListEntry->EventName)-1);
    
    /*Create event callback list for this new event*/
    pMethodNode = (PEVT_DISPTER_METHOD_NODE)malloc(sizeof(EVT_DISPTER_METHOD_NODE));
    if(NULL != pMethodNode) {
        pMethodNode->CallbackFunc = NULL;
        pMethodNode->Next = NULL;
    }
    pCbListEntry->EventCbList = (EVT_DISPTER_EVENT_CB_LIST)pMethodNode;

    tmpListEntry->Next = pCbListEntry;

    return pCbListEntry->EventCbList;
}

/*register the new callback function to the cbList*/
ANSC_STATUS
EvtDispterRgstCbFuncToList
    (
        EVT_DISPTER_CB_LIST     cbList,
        char *                  eventName,
        CALLBACK_FUNC_NAME      func,
        void *                  arg 
    )
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    PEVT_DISPTER_CALLBACK_FUNC  pCbFunc = NULL;
    PEVT_DISPTER_METHOD_NODE    pNewMethodNode = NULL;
    EVT_DISPTER_EVENT_CB_LIST   eventCbList = NULL;
    PEVT_DISPTER_METHOD_NODE    tmpNode = NULL;

    if ( (NULL == eventName) || (NULL == func))
    {
        CcspTraceWarning(("event/function name should not be null!\n"));
        return ANSC_STATUS_FAILURE;
    }

    pCbFunc = (PEVT_DISPTER_CALLBACK_FUNC)malloc(sizeof(EVT_DISPTER_CALLBACK_FUNC));
    if(NULL != pCbFunc) {
        pCbFunc->MethodName = NULL; /*currently, methodname not used*/
        pCbFunc->Func = func;
        pCbFunc->Arg = arg;
    }
    else /*RDKB-6747, CID-32958, return if malloc failed */
    {
        returnStatus = ANSC_STATUS_FAILURE;
        return returnStatus;
    }

    /*create the new callback function node*/
    pNewMethodNode = (PEVT_DISPTER_METHOD_NODE)malloc(sizeof(EVT_DISPTER_METHOD_NODE));
    if(NULL == pNewMethodNode) {
        free(pCbFunc);  /*RDKB-6747, CID-32958, free unused resources */
        returnStatus = ANSC_STATUS_FAILURE;
        return returnStatus;
    }
    pNewMethodNode->CallbackFunc = pCbFunc;
    pNewMethodNode->Next = NULL;

    /*insert the node into the right event list*/
    eventCbList = EvtDispterFindListByEventName(cbList, eventName);
    tmpNode = eventCbList;
    while((NULL != tmpNode) && (NULL != tmpNode->Next))
    {
        tmpNode = tmpNode->Next;
    }

    tmpNode->Next = pNewMethodNode;

    return returnStatus;
}

/*call the callback functin step by step from cbList*/
ANSC_STATUS
EvtDispterCallFuncFromList
    (
        EVT_DISPTER_CB_LIST     cbList,
        char *                  eventName           
    )
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    EVT_DISPTER_EVENT_CB_LIST   eventCbList = NULL;
    PEVT_DISPTER_METHOD_NODE    pMethodNode = NULL;
    PEVT_DISPTER_CALLBACK_FUNC  pCallbackFunc = NULL;
    int                         err = -1; 

    if(NULL == eventName)
        return returnStatus;

    eventCbList = EvtDispterFindListByEventName(cbList, eventName);
    
    /*call the callback function step by step in the eventCbList*/
    pMethodNode = (PEVT_DISPTER_METHOD_NODE)eventCbList->Next;

    while(NULL != pMethodNode)
    {
        pCallbackFunc = pMethodNode->CallbackFunc;
        pMethodNode = pMethodNode->Next;
        
        if((NULL == pCallbackFunc) || (NULL == pCallbackFunc->Func))
            continue;

        err = pCallbackFunc->Func(pCallbackFunc->Arg);
        if(0 != err) {
            returnStatus = ANSC_STATUS_FAILURE;
        }
    }

    return returnStatus;
}

/*clear the callback list*/
VOID
EvtDispterClearCallbackList
    (
        EVT_DISPTER_CB_LIST          cbList
    )
{
    PEVT_DISPTER_CB_LIST_ENTRY  pCbListEntry = NULL;
    EVT_DISPTER_EVENT_CB_LIST   eventCbList = NULL;
    PEVT_DISPTER_METHOD_NODE    pMethodNode = NULL;


    pCbListEntry = (PEVT_DISPTER_CB_LIST_ENTRY)cbList->Next;

    while(NULL != pCbListEntry)
    {
        eventCbList = pCbListEntry->EventCbList;
        pMethodNode = eventCbList->Next;
        
        while(NULL != pMethodNode)
        {
            eventCbList->Next = pMethodNode->Next;
            /* free current method node */
            free(pMethodNode->CallbackFunc);
            free(pMethodNode);
            
            pMethodNode = eventCbList->Next;
        }
        free(eventCbList);
        cbList->Next = pCbListEntry->Next;
        free(pCbListEntry);
        pCbListEntry = cbList->Next;
    }

}

/*Destroy the callback list after clear*/
VOID
EvtDispterDestroyCallbackList
    (
        EVT_DISPTER_CB_LIST * pCbList
    )
{
    EvtDispterClearCallbackList(*pCbList);
    free(*pCbList);
    *pCbList = NULL;
}


/********************************************************************************

    Description:
    
          This function register the callback functions for event
        
    Arguments:

          eventName: the event name, should not be null

               func: the callback function which you want register for the event

                arg: the argument of the callback function
    
    Return: ANSC_STATUS (ANSC_STATUS_SUCCESS or ANSC_STATUS_FAILURE)
        

**********************************************************************************/
ANSC_STATUS
EvtDispterRgstCallbackForEvent
    (
        char *              eventName,
        CALLBACK_FUNC_NAME  func,
        void *              arg 
    )
{
    if (NULL ==  g_CallbackFuncList)
        EvtDispterInitCallbackList(&g_CallbackFuncList);

    return EvtDispterRgstCbFuncToList(g_CallbackFuncList, eventName, func, arg);
}

/********************************************************************************

    Description:
    
          This function call the callback functions step by step when the
          specified event trigger
        
    Arguments:

          eventName: the event name, should not be null


    Return: ANSC_STATUS (ANSC_STATUS_SUCCESS or ANSC_STATUS_FAILURE)
        

**********************************************************************************/
ANSC_STATUS
EvtDispterCallFuncByEvent
    (
        char *          eventName           
    )
{
    return EvtDispterCallFuncFromList(g_CallbackFuncList, eventName);
}

/************************************************common event handler********************************************************/

static int se_fd = 0; 
static token_t token;
#ifdef _HUB4_PRODUCT_REQ_
static async_id_t async_id[4];
#else
static async_id_t async_id[3];
#endif
static short server_port;
static char  server_ip[19];
#ifdef _HUB4_PRODUCT_REQ_
enum {EVENT_ERROR=-1, EVENT_OK, EVENT_TIMEOUT, EVENT_HANDLE_EXIT, EVENT_LAN_STARTED=0x10, EVENT_LAN_STOPPED, 
        EVENT_WAN_STARTED=0x20, EVENT_WAN_STOPPED,EVENT_WAN_IPV4_RECD=0x30, EVENT_VALID_ULA_ADDRESS=0x40};
#else
enum {EVENT_ERROR=-1, EVENT_OK, EVENT_TIMEOUT, EVENT_HANDLE_EXIT, EVENT_LAN_STARTED=0x10, EVENT_LAN_STOPPED,
        EVENT_WAN_STARTED=0x20, EVENT_WAN_STOPPED,EVENT_WAN_IPV4_RECD=0x30};
#endif

static void
EvtDispterWanIpAddrsCallback(char *ip_addrs)
{
#ifdef DUAL_CORE_XB3
    CcspTraceInfo(("%s vsystem %d \n", __FUNCTION__,__LINE__)); 
    CcspTraceInfo(("EvtDispterWanIpAddrsCallback - erouter0 IP = %s\n",ip_addrs));
    vsystem("/usr/ccsp/pam/erouter0_ip_sync.sh %s &",ip_addrs);
#endif
#if defined(_XB6_PRODUCT_REQ_) && !defined(_XB7_PRODUCT_REQ_)
    CcspTraceInfo(("%s vsystem %d \n", __FUNCTION__,__LINE__));
    CcspTraceInfo(("EvtDispterWanIpAddrsCallback - erouter0 IP = %s\n",ip_addrs));
    vsystem("/usr/sbin/sec_pushown.sh --ip4 \"%s\"", ip_addrs);
#endif

     if (strcmp(ip_addrs,"0.0.0.0") != 0 ) {

           CcspTraceInfo(("%s Setting current_wan_ipaddr and restarting firewall %d \n", __FUNCTION__,__LINE__)); 
	    sysevent_set(se_fd, token, "current_wan_ipaddr", ip_addrs, 0);
	    sysevent_set(se_fd, token, "firewall-restart", NULL, 0);
    }
}

/*
 * Initialize sysevnt 
 *   return 0 if success and -1 if failture.
 */
int 
EvtDispterEventInits(void)
{
    int rc;

    snprintf(server_ip, sizeof(server_ip), "127.0.0.1");
    server_port = SE_SERVER_WELL_KNOWN_PORT;

    se_fd = sysevent_open(server_ip, server_port, SE_VERSION, "common_master", &token);
    if (!se_fd) {
        return(EVENT_ERROR);
    }

    /*you can register the event as you want*/

	//register lan-status event
    sysevent_set_options(se_fd, token, "lan-status", TUPLE_FLAG_EVENT);
    rc = sysevent_setnotification(se_fd, token, "lan-status", &async_id[0]);
    if (rc) {
       return(EVENT_ERROR);
    } 

	//register wan-status event
    sysevent_set_options(se_fd, token, "wan-status", TUPLE_FLAG_EVENT);
    rc = sysevent_setnotification(se_fd, token, "wan-status", &async_id[1]);
    if (rc) {
       return(EVENT_ERROR);
    } 

	//register ipv4_wan_ipaddr event
    //sysevent_set_options(se_fd, token, "ipv4_wan_ipaddr", TUPLE_FLAG_EVENT);
    rc = sysevent_setnotification(se_fd, token, "ipv4_wan_ipaddr", &async_id[2]);
    if (rc) {
       return(EVENT_ERROR);
    }
#ifdef _HUB4_PRODUCT_REQ_
	//register valid_ula_address event
    sysevent_set_options(se_fd, token, "valid_ula_address", TUPLE_FLAG_EVENT);
    rc = sysevent_setnotification(se_fd, token, "valid_ula_address", &async_id[3]);
    if (rc) {
       return(EVENT_ERROR);
    }
#endif
    return(EVENT_OK);
}

/*
 * Listen sysevent notification message
 */
int 
EvtDispterEventListen(void)
{
    int     ret = EVENT_TIMEOUT;
    fd_set  rfds;
    int     retval;

    FD_ZERO(&rfds);
    FD_SET(se_fd, &rfds);

    retval=select(se_fd+1, &rfds, NULL, NULL, NULL);

    if(retval) {
        se_buffer            msg_buffer;
        se_notification_msg *msg_body = (se_notification_msg *)msg_buffer;
        unsigned int         msg_size;
        token_t              from;
        int                  msg_type;

        msg_size  = sizeof(msg_buffer);
        msg_type = SE_msg_receive(se_fd, msg_buffer, &msg_size, &from);
        // if not a notification message then ignore it
        if (SE_MSG_NOTIFICATION == msg_type) {
            // extract the name and value from the return message data
            int   name_bytes;
            int   value_bytes;
            char *name_str;
            char *value_str;
            char *data_ptr;

            data_ptr   = (char *)&(msg_body->data);
            name_str   = (char *)SE_msg_get_string(data_ptr, &name_bytes);
            data_ptr  += name_bytes;
            value_str =  (char *)SE_msg_get_string(data_ptr, &value_bytes);

            if(!strcmp(name_str, "lan-status"))
            {
                if (!strncmp(value_str, "started", 7)) 
                    ret = EVENT_LAN_STARTED;
                else if (!strncmp(value_str, "stopped", 7)) 
                    ret = EVENT_LAN_STOPPED;
            }
            else if(!strcmp(name_str, "wan-status"))
            {
                if (!strncmp(value_str, "started", 7)) 
                    ret = EVENT_WAN_STARTED;
                else if (!strncmp(value_str, "stopped", 7)) 
                    ret = EVENT_WAN_STOPPED;
            }
            else if(!strcmp(name_str, "ipv4_wan_ipaddr"))
            {
                EvtDispterWanIpAddrsCallback(value_str);
                ret = EVENT_WAN_IPV4_RECD;
            }
#ifdef _HUB4_PRODUCT_REQ_
            else if(!strcmp(name_str, "valid_ula_address"))
            {
                if (!strncmp(value_str, "false", 5))
                {
                    ret = EVENT_VALID_ULA_ADDRESS;
                }
            }
#endif
        } else {
            CcspTraceWarning(("Received msg that is not a SE_MSG_NOTIFICATION (%d)\n", msg_type));
	    if ( 0 != system("pidof syseventd")) {

           	CcspTraceWarning(("%s syseventd not running ,breaking the receive notification loop \n",__FUNCTION__));
		ret = EVENT_HANDLE_EXIT;
	    }
        }
    } else {
        CcspTraceError(("%s: Received no event retval=%d\n", __FUNCTION__, retval));
    }
    return ret;
}

/*
 * Close sysevent
 */
int 
EvtDispterEventClose(void)
{
    /* we are done with this notification, so unregister it using async_id provided earlier */
    sysevent_rmnotification(se_fd, token, async_id[0]);
    sysevent_rmnotification(se_fd, token, async_id[1]);
    sysevent_rmnotification(se_fd, token, async_id[2]);
#ifdef _HUB4_PRODUCT_REQ_
    sysevent_rmnotification(se_fd, token, async_id[3]);
#endif

    /* close this session with syseventd */
    sysevent_close(se_fd, token);

    return (EVENT_OK);
}

/*
 * check the initialized sysevent status (happened or not happened),
 * if the event happened, call the functions registered for the events previously
 */
int 
EvtDispterCheckEvtStatus(int fd, token_t token)
{
    char evtValue[256] = {0};
    int  returnStatus = ANSC_STATUS_SUCCESS;

    /*lan-status event*/
    if( 0 == sysevent_get(fd, token, "lan-status", evtValue, sizeof(evtValue)) && '\0' != evtValue[0] )
    {
        if (0 == strncmp(evtValue, "started", strlen("started"))) {
            if (ANSC_STATUS_SUCCESS != EvtDispterCallFuncByEvent("lan-status"))
                    returnStatus = ANSC_STATUS_FAILURE;
        }
    }

    /*wan-status*/
    if ( 0 == sysevent_get(fd, token, "wan-status", evtValue, sizeof(evtValue)) && '\0' != evtValue[0])
    {
        if (0 == strncmp(evtValue, "started", strlen("started")))
            if (ANSC_STATUS_SUCCESS != EvtDispterCallFuncByEvent("wan-status"))
                returnStatus = ANSC_STATUS_FAILURE;
    }

    /*ipv4_wan_ipaddr*/
    if ( 0 == sysevent_get(fd, token, "ipv4_wan_ipaddr", evtValue, sizeof(evtValue)) && '\0' != evtValue[0])
    {
        EvtDispterWanIpAddrsCallback(evtValue);
    }

    return returnStatus;
}

/*
 * the sysevent handler in event dispatcher
 */
void *
EvtDispterEventHandler(void *arg)
{
    int ret = EVENT_ERROR;
    UNREFERENCED_PARAMETER(arg);
    while(EVENT_ERROR == EvtDispterEventInits())
    {
        CcspTraceError(("%s: sysevent init failed!\n", __FUNCTION__));
        sleep(1);
    }

    /*first check the events status*/
    EvtDispterCheckEvtStatus(se_fd, token);

    while(1)
    {
        ret = EvtDispterEventListen();
        switch (ret)
        {
            case EVENT_LAN_STARTED:
                EvtDispterCallFuncByEvent("lan-status");
                break;
            case EVENT_LAN_STOPPED:
                break;
            case EVENT_WAN_STARTED:
                EvtDispterCallFuncByEvent("wan-status");
                break;
            case EVENT_WAN_STOPPED:
                break;
            case EVENT_WAN_IPV4_RECD:
                break;
#ifdef _HUB4_PRODUCT_REQ_
            case EVENT_VALID_ULA_ADDRESS:
                ValidUlaHandleEventAsync();
                break;
#endif
            default :
                CcspTraceWarning(("The received event status is not expected!\n"));
                break;
        }

        if (EVENT_HANDLE_EXIT == ret) //end this event handling loop
            break;

        sleep(2);
    }

    EvtDispterEventClose();

    return NULL;
}

/*
 * Create a thread to handle the sysevent asynchronously
 */
void
EvtDispterHandleEventAsync(void)
{
    int err;
    pthread_t event_handle_thread;

    err = pthread_create(&event_handle_thread, NULL, EvtDispterEventHandler, NULL);
    if(0 != err)
    {
        CcspTraceError(("%s: create the event handle thread error!\n", __FUNCTION__));
    }
}

int executeCmd(char *cmd)
{
	int l_iSystem_Res;
	l_iSystem_Res = system(cmd);
    if (0 != l_iSystem_Res && ECHILD != errno)
    {
        CcspTraceError(("%s: %s command didnt execute successfully\n", __FUNCTION__,cmd));
        return l_iSystem_Res;
    }
    return 0;
}


#ifdef _HUB4_PRODUCT_REQ_
void* RegenerateUla(void *arg)
{
    char *pIpv6_enable = NULL;
    char *pUla_enable  = NULL;
    char *pUla_prefix  = NULL;
    char *pUla = NULL;

    UNREFERENCED_PARAMETER(arg);

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    pthread_detach(pthread_self());

    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_IPV6_ENABLE, NULL, &pIpv6_enable))
    {
    	if (pIpv6_enable != NULL) {
           bus_info->freefunc(pIpv6_enable);
    	}
        return NULL;
    }

    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_ULA_ENABLE, NULL, &pUla_enable))
    {
    	if (pUla_enable != NULL) {
           bus_info->freefunc(pUla_enable);
    	}
        return NULL;
    }

    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_ULA, NULL, &pUla))
    {
        if(pUla != NULL)
            bus_info->freefunc(pUla);
        return NULL;
    }

    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_ULA_PREFIX, NULL, &pUla_prefix))
    {
        if(pUla_prefix != NULL)
            bus_info->freefunc(pUla_prefix);
        return NULL;
    }

    if ((strncmp(pIpv6_enable, "TRUE", 4 ) == 0) && (strncmp(pUla_enable, "TRUE", 4 ) == 0))
    {
        //Check whether NULL or not
        if( ( NULL != pUla_prefix ) && ( NULL != pUla ) )
        {
            char   acTmpULAPrefix[64] = { 0 },
                   acTmpULA[64]       = { 0 };

            //Copy PSM buffer to local buffer to avoid memory overwrite in calling function
            snprintf( acTmpULAPrefix, sizeof(acTmpULAPrefix), "%s", pUla_prefix );
            snprintf( acTmpULA, sizeof(acTmpULA), "%s", pUla );

            if( ANSC_STATUS_SUCCESS == CosaDmlLanMngm_SetLanIpv6Ula( acTmpULAPrefix, acTmpULA ) )
            {
               sysevent_set( se_fd, token, "valid_ula_address", "true", 0 );
            }
        }
    }
    else
    {
        system("killall zebra");
    }

    if(pIpv6_enable != NULL)
        bus_info->freefunc(pIpv6_enable);

    if(pUla_enable != NULL)
        bus_info->freefunc(pUla_enable);

    if(pUla_prefix != NULL)
        bus_info->freefunc(pUla_prefix);

    if(pUla != NULL)
        bus_info->freefunc(pUla);
    return NULL;
}

/*
 * Create a thread to handle the sysevent asynchronously
 */
void
ValidUlaHandleEventAsync(void)
{
    int err;
    pthread_t event_handle_thread;

    err = pthread_create(&event_handle_thread, NULL, RegenerateUla, NULL);
    if(0 != err)
    {
        CcspTraceError(("%s: create the event handle thread error!\n", __FUNCTION__));
    }
}
#endif
