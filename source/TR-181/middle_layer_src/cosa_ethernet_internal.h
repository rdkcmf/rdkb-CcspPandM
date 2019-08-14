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

    module: cosa_ethernet_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#ifndef  _COSA_ETHERNET_INTERNAL_H
#define  _COSA_ETHERNET_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_ethernet_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

#define  COSA_IREP_FOLDER_NAME_ETHLINK                  "EthLink"
#define  COSA_DML_RR_NAME_EthernetLinkAlias             "Alias"
#define  COSA_DML_RR_NAME_EthernetLinkInsNum            "InstanceNumber"
#define  COSA_DML_RR_NAME_EthernetLinkNextInsNunmber    "NextInstanceNumber"
#define  COSA_DML_RR_NAME_EthernetLinkNewlyAdded        "NewlyAdded"

#define  COSA_IREP_FOLDER_NAME_ETHVLANTERMINATION       "EthVlanTermination"
#define  COSA_DML_RR_NAME_EthernetVlanTerminationAlias  "Alias"
#define  COSA_DML_RR_NAME_EthernetVlanTerminationInsNum "InstanceNumber"
#define  COSA_DML_RR_NAME_EthernetVlanTerminationNextInsNunmber "NextInstanceNumber"
#define  COSA_DML_RR_NAME_EthernetVlanTerminationNewlyAdded     "NewlyAdded"

#define  COSA_DATAMODEL_ETHERNET_CLASS_CONTENT                              \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \
    /* start of DeviceInfo object class content */                          \
    COSA_DML_ETH_PORT_FULL              EthernetPortFullTable[MAXINSTANCE]; \
    SLIST_HEADER                        EthernetLinkList;                   \
    ULONG                               ulEthernetLinkNextInstance;         \
    SLIST_HEADER                        EthernetVlanTerminationList;        \
    ULONG                               ulEthernetVlanTerminationNextInstance; \
    ANSC_HANDLE                         hIrepFolderCOSA;                    \
    ANSC_HANDLE                         hIrepFolderEthLink;                 \
    ANSC_HANDLE                         hIrepFolderEthVlanTermination;      \
    
    
typedef  struct
_COSA_DATAMODEL_ETHERNET_CLASS_CONTENT
{
    COSA_DATAMODEL_ETHERNET_CLASS_CONTENT
}
COSA_DATAMODEL_ETHERNET, *PCOSA_DATAMODEL_ETHERNET;

/*
    Standard function declaration
*/
ANSC_HANDLE
CosaEthernetCreate

    (
        VOID
    );

ANSC_STATUS
CosaEthernetInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaEthernetRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaEthLinkRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaEthLinkRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
CosaEthLinkRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
CosaEthVlanTerminationRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaEthVlanTerminationRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
CosaEthVlanTerminationRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );
ANSC_STATUS
CosaEthPortGetAssocDevices
    (
        UCHAR       *mac,
        CHAR        *maclist,
        int         numMacAddr
  
    );

void * EthWan_TelementryLogger_Thread(void *data);


#endif
