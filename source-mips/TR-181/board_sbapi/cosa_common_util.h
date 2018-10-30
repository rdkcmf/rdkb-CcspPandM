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

    module: cosa_common_util.h

        Common utilities
    -------------------------------------------------------------------

    description:

        This file defines the common apis to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Chen Changchun <chchangc@cisco.com>
    -------------------------------------------------------------------

    revision:

        11/01/2013    initial revision.

**************************************************************************/

#ifndef _COSA_COMMON_UTIL_INC_
#define _COSA_COMMON_UTIL_INC_

#include "ansc_platform.h"
#include "sysevent/libsysevent_internal.h"

#define MAX_EVENT_NAME_LENGTH   32

typedef char * METHOD_NAME;
typedef int (*CALLBACK_FUNC_NAME)(void *);

typedef struct {
    METHOD_NAME         MethodName;
    CALLBACK_FUNC_NAME  Func;
    void                *Arg;
}EVT_DISPTER_CALLBACK_FUNC, *PEVT_DISPTER_CALLBACK_FUNC;

typedef struct method_node * PEVT_DISPTER_METHOD_NODE;
typedef struct method_node {
    EVT_DISPTER_CALLBACK_FUNC       *CallbackFunc;
    PEVT_DISPTER_METHOD_NODE         Next;   
} EVT_DISPTER_METHOD_NODE, *EVT_DISPTER_EVENT_CB_LIST;

typedef struct cbListEntry * PEVT_DISPTER_CB_LIST_ENTRY;

typedef struct cbListEntry{
    char                          EventName[MAX_EVENT_NAME_LENGTH];
    EVT_DISPTER_EVENT_CB_LIST     EventCbList;
    PEVT_DISPTER_CB_LIST_ENTRY    Next;
} EVT_DISPTER_CB_LIST_ENTRY,*EVT_DISPTER_CB_LIST;


/**************************event dispatcher functions****************************/

/*Initialize the event dispatcher callback list */
EVT_DISPTER_CB_LIST
EvtDispterInitCallbackList
    (
        EVT_DISPTER_CB_LIST *   pCbList
    );


/*register the new callback function to the cbList*/
ANSC_STATUS
EvtDispterRgstCbFuncToList
    (
        EVT_DISPTER_CB_LIST     cbList,
        char *                  eventName,
        CALLBACK_FUNC_NAME      func,
        void *                  arg 
    );


/*call the callback functin step by step from cbList*/
ANSC_STATUS
EvtDispterCallFuncFromList
    (
        EVT_DISPTER_CB_LIST     cbList,
        char *                  eventName           
    );


/*clear the callback list*/
VOID
EvtDispterClearCallbackList
    (
        EVT_DISPTER_CB_LIST     cbList
    );


/*Destroy the callback list after clear*/
VOID
EvtDispterDestroyCallbackList
    (
        EVT_DISPTER_CB_LIST * pCbList
    );


/*register the callback functions for event */
extern ANSC_STATUS
EvtDispterRgstCallbackForEvent
    (
        char *              eventName,
        CALLBACK_FUNC_NAME  func,
        void *              arg 
    );

/*call the callback functions step by step when the specified event trigger*/
extern ANSC_STATUS
EvtDispterCallFuncByEvent
    (
        char *      eventName           
    );


/*
 * sysevent related functions
 */
int EvtDispterEventInits(void);
int EvtDispterEventListen(void);
int EvtDispterEventClose(void);
void * EvtDispterEventHandler(void *arg);
int EvtDispterCheckEvtStatus(int fd, token_t token);
/*Create a thread to handle the sysevent asynchronously*/
void EvtDispterHandleEventAsync(void);

#endif
