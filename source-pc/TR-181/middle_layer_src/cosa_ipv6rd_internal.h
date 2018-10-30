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

    module: cosa_ipv6rd_internal.h

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

**************************************************************************/

#ifndef _COSA_IPV6RD_INTERNAL_H_
#define _COSA_IPV6RD_INTERNAL_H_

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "cosa_ipv6rd_apis.h"

#define COSA_IREP_FOLDER_NAME_IPV6RDIF          "IPv6rdIF"

#define COSA_DML_RR_NAME_IPV6RDIFAlias          "Alias"
#define COSA_DML_RR_NAME_IPV6RDIFInsNum         "InstanceNumber"
#define COSA_DML_RR_NAME_IPV6RDIFNextInsNumber  "NextInstanceNumber"
#define COSA_DML_RR_NAME_IPV6RDIFNewlyAdded     "NewlyAdded"

#define COSA_DATAMODEL_IPV6RD_CLASS_CONTENT         \
    COSA_BASE_OBJECT                                \
    /* param of object Device.IPv6rd. */            \
    BOOL                    Enable;                 \
    ULONG                   NumOfInterface;         \
    /* interface list (Device.IPv6rd.InterfaceSetting. */   \
    SLIST_HEADER            IfList;                 \
    ULONG                   ulIfNextInstance;       \
    ANSC_HANDLE             hIrepFolderCOSA;        \
    ANSC_HANDLE             hIrepFolderIPv6rdIF;    \


typedef struct 
_COSA_DATAMODEL_IPV6RD
{
#if 0
    COSA_DATAMODEL_IPV6RD_CLASS_CONTENT
#else
    ULONG                   Oid;
    PFN_COSADM_CREATE       Create;
    PFN_COSADM_REMOVE       Remove;
    PFN_COSADM_INITIALIZE   Initialize;
    /* param of object Device.IPv6rd. */            
    BOOL                    Enable;                 
    ULONG                   NumOfInterface;         
    /* interface list (Device.IPv6rd.InterfaceSetting. */ 
    SLIST_HEADER            IfList;                 
    ULONG                   ulIfNextInstance;       
    ANSC_HANDLE             hIrepFolderCOSA;        
    ANSC_HANDLE             hIrepFolderIPv6rdIF;    
#endif
}
COSA_DATAMODEL_IPV6RD, *PCOSA_DATAMODEL_IPV6RD;

ANSC_HANDLE
CosaIPv6rdCreate(
        VOID
        );

ANSC_STATUS
CosaIPv6rdInitialize(
        ANSC_HANDLE hThisObject
        );

ANSC_STATUS
CosaIPv6rdRemove(
        ANSC_HANDLE hThisObject
        );

ANSC_STATUS
CosaIPv6rdIfRegGetInfo(
        ANSC_HANDLE hThisObject
        );

ANSC_STATUS
CosaIPv6rdIfRegAddInfo(
        ANSC_HANDLE hThisObject,
        ANSC_HANDLE hCosaContext
        );

ANSC_STATUS
CosaIPv6rdIfRegDelInfo(
        ANSC_HANDLE hThisObject,
        ANSC_HANDLE hCosaContext
        );
#endif /* _COSA_IPV6RD_INTERNAL_H_ */
