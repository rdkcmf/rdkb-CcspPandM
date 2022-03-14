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


#ifndef _COSADRGCOMMON_
#if !defined(_COSA_SIM_)
#define _COSADRGCOMMON_
#include<stdint.h>
#include "sysevent/sysevent.h"

typedef struct _os_stats {
    uint64_t rxBytes;
    uint32_t rxPackets;
    uint32_t rxErrors;
    uint32_t rxDrops;
    uint32_t rxMulticast;
    uint64_t txBytes;
    uint32_t txPackets;
    uint32_t txErrors;
    uint32_t txDrops;
    uint32_t txMulticast;
} OSStats, *POSStats;

int getOSStats(char* iface, POSStats stats);



extern int commonSyseventFd;
extern token_t commonSyseventToken;


int commonSyseventSet(char* key, char* value);
int commonSyseventGet(char* key, char* value, int valLen);
int commonSyseventClose();

#define CCSP_COMMON_FIFO "/tmp/ccsp_common_fifo"

#endif
#endif
