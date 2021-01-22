/**********************************************************************
 * Copyright 2017-2019 ARRIS Enterprises, LLC.
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
**********************************************************************/

#ifndef  _COSA_DSLITE_APIS_H
#define  _COSA_DSLITE_APIS_H

#include "../middle_layer_src/cosa_apis.h"

struct _COSA_DML_DSLITE
{
    unsigned long  InstanceNumber;
    int            active;
    int            status;
    char           alias[64+1];
    int            mode;
    int            addr_type;
    char           addr_inuse[256+1];
    char           addr_fqdn[256+1];
    char           addr_ipv6[256+1];
    int            origin;
    char           tunnel_interface[256+1];
    char           tunneled_interface[256+1];
    int            mss_clamping_enable;
    unsigned long  tcpmss;
    int            ipv6_frag_enable;
    char           tunnel_v4addr[64+1];
}_struct_pack_;

typedef struct _COSA_DML_DSLITE COSA_DML_DSLITE,  *PCOSA_DML_DSLITE;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/
ANSC_STATUS
CosaDmlDsliteInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlGetDsliteEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *bEnabled
    );

ANSC_STATUS
CosaDmlSetDsliteEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

ANSC_STATUS
CosaDmlDsliteAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DSLITE            pEntry
    );

ANSC_STATUS
CosaDmlDsliteDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDsliteSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DSLITE            pEntry
    );

ANSC_STATUS
CosaDmlDsliteGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DSLITE            pEntry
    );

ANSC_STATUS
CosaDmlDsliteGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pNum
    );

ANSC_STATUS
CosaDmlDsliteGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DSLITE            pEntry
    );

ANSC_STATUS
CosaDmlDsliteSetInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber
    );

#endif
