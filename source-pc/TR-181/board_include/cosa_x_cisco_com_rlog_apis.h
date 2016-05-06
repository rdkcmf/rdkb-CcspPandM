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

#ifndef _COSA_RLOG_APIS_H
#define _COSA_RLOG_APIS_H
#include "cosa_apis.h"

typedef enum 
_COSA_DML_RLOG_PROTO
{
    COSA_DML_RLOG_PROTO_TCP     = 1,
    COSA_DML_RLOG_PROTO_UDP     = 2,
}
COSA_DML_RLOG_PROTO;

typedef struct
_COSA_DML_RLOG
{
    BOOL                Enable;
    char                Host[256];
    ULONG               Port;
    COSA_DML_RLOG_PROTO Protocol;
    char                Patterns[65];
}
COSA_DML_RLOG, *PCOSA_DML_RLOG;

ANSC_STATUS 
CosaDmlRLog_Init(void);

ANSC_STATUS 
CosaDmlRLog_Term(void);

ANSC_STATUS 
CosaDmlRLog_Validate(PCOSA_DML_RLOG pRLog);

ANSC_STATUS 
CosaDmlRLog_SetConf(PCOSA_DML_RLOG pRLog);

ANSC_STATUS 
CosaDmlRLog_GetConf(PCOSA_DML_RLOG pRLog);

#endif /* _COSA_RLOG_APIS_H */
