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


/**********************************************************************

    File: cosa_ip_ethernet_multilan.h

    For Data Model SBAPI Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        This file defines the APIs servicing TR-181
        data model Ethernet Interface and Link SBAPI integration.

    ---------------------------------------------------------------

    environment:

        Platform Independent, but with multi-LAN support

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        02/28/2013  initial revision.

**********************************************************************/

#ifndef _COSA_ETHERNET_APIS_MULTILAN_
#define _COSA_ETHERNET_APIS_MULTILAN_

#include "cosa_ethernet_apis.h"

/**************************************************************************
                        DATA STRUCTURE DEFINITIONS
**************************************************************************/

typedef struct
_DMSB_TR181_ETH_LINK
{
     SINGLE_LINK_ENTRY              Linkage;
     COSA_DML_ETH_LINK_CFG          Cfg;
     COSA_DML_ETH_LINK_SINFO        StaticInfo;
     COSA_DML_ETH_LINK_DINFO        DynamicInfo;

     COSA_DML_IF_STATS              LastStats;
}
DMSB_TR181_ETH_LINK, *PDMSB_TR181_ETH_LINK;

#define  ACCESS_DMSB_TR181_ETH_LINK(p)          ACCESS_CONTAINER(p, DMSB_TR181_ETH_LINK, Linkage)


typedef struct
_DMSB_TR181_ETH_CONTEXT
{
    /* Ethernet Port */

    /* Ethernet Link */
    BOOLEAN                         EthLinkInit;
    SLIST_HEADER                    EthLinkList;
}
DMSB_TR181_ETH_CONTEXT, *PDMSB_TR181_ETH_CONTEXT;

#define  DMSB_TR181_ETH_CONTEXT_Init(p)                         \
            {                                                   \
                p->EthLinkInit      = FALSE;                    \
                AnscSListInitializeHeader(&p->EthLinkList);     \
            }

/**************************************************************************
                        ROUTINE PROTOTYPES
**************************************************************************/

ANSC_STATUS
CosaDmlEthMlanInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );


#endif
