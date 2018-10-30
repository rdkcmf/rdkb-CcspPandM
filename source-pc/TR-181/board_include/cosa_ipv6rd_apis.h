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

    module: cosa_ipv6rd_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        leichen2@cisco.com

    -------------------------------------------------------------------

    revision:

        05/08/2011    initial revision.
        02/09/2011    Define APIs.

**************************************************************************/

#ifndef _COSA_IPV6RD_INTERNEL_H_
#define _COSA_IPV6RD_INTERNEL_H_

// max interface instances (is it enough?)
#define MAX_6RDIF_INS              64

#define IPV6RD_DEF_STRLEN          512

typedef struct _COSA_DML_IPV6RD_IF
{
    /* configrable params */
    ULONG               InstanceNumber;     /* start counting from 1 */
    BOOL                Enable;
    char                Alias[64 + 1];
    char                BorderRelayIPv4Addr[IPV6RD_DEF_STRLEN + 1];
    BOOL                AllTrafficToBorderRelay;
    char                SPIPv6Prefix[IPV6RD_DEF_STRLEN + 1];
    ULONG               IPv4MaskLength;
    char                AddressSource[256 + 1];

    /* readonly (runtime stauts) params */
    char                Status[IPV6RD_DEF_STRLEN + 1];
    char                TunnelInterface[256 + 1];
    char                TunneledInterface[256 + 1];
} COSA_DML_IPV6RD_IF, *PCOSA_DML_IPV6RD_IF;

ANSC_STATUS
CosaDml_IPv6rdInit(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext
        );

ANSC_STATUS
CosaDml_IPv6rdFinalize(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext
        );

ANSC_STATUS
CosaDml_IPv6rdGetEnable(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext,
        BOOL            *bEnable
        );

ANSC_STATUS
CosaDml_IPv6rdSetEnable(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext,
        BOOL            bEnable
        );

ANSC_STATUS
CosaDml_IPv6rdGetNumberOfEntries(
        ANSC_HANDLE     hContext,
        ULONG           *NumEntries, /* input-output */
        ULONG           InsNumArray[]
        );

ANSC_STATUS
CosaDml_IPv6rdGetEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber,
        PCOSA_DML_IPV6RD_IF     pEntry
        );

ANSC_STATUS
CosaDml_IPv6rdSetEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber,
        PCOSA_DML_IPV6RD_IF     pEntry
        );


ANSC_STATUS
CosaDml_IPv6rdAddEntry(
        ANSC_HANDLE             hContext,
        PCOSA_DML_IPV6RD_IF     pEntry
        );

ANSC_STATUS
CosaDml_IPv6rdDelEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber
        );

ANSC_STATUS
CosaDml_Ifname2Addr(
		const char *ifname, 
		char *addr, 
		unsigned int size
		);

#endif /* _COSA_IPV6RD_INTERNEL_H_ */
