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

    File: dml_tr181_custom_cfg.h

    For Data Model Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        This header holds the auto-configured (or manually configured)
        TR-181 data model related defintions specific to each platform

    ---------------------------------------------------------------

    environment:

        Linux PC sim

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        09/15/2013  initial revision.

**********************************************************************/

#ifndef  _DML_TR181_CUSTOM_CFG_H_
#define  _DML_TR181_CUSTOM_CFG_H_

#include "dml_tr181_custom_cfg_custom.h"            /* include customer specific configurations */

#define  CONFIG_SYSTEM_MOCA                         1

/*#define  CONFIG_CISCO_FILE_TRANSFER                 1*/
/*#define  CONFIG_CISCO_TRUE_STATIC_IP                1*/
/*#define  CONFIG_CISCO_GRE                           1*/   /* Cisco GRE code is covered in CISCO_HOTSPOT flag!!! */
/*#define  CONFIG_CISCO_HOTSPOT                       1*/

/*#define  CONFIG_CUSTOMER_GRE                         1*/   /* Customer GRE code is covered in CISCO_HOTSPOT flag, this is ugly! */

#define  CFG_TR181_BRIDGE_ETH4_SYSCFG               0
#define  CFG_TR181_DHCPv4_CLIENT_IfName             "eth0"
#define  CFG_TR181_DHCPv6_CLIENT_IfName             "eth0"
#define  CFG_TR181_DHCPv6_SERVER_IfName             "eth0"
#define  CFG_TR181_NeighborDiscovery_IfName         "eth0"

#define  CFG_TR181_ps_no_arg                        0
#define  CFG_TR181_RouteInfo_IfName                 "eth0"

#define  CFG_TR181_6rd_IfName                       "eth0"
#define  CFG_TR181_NO_CosaDml_Ifname2Addr           1

#define  CFG_TR181_Ddns_IfName                      "eth0"

#define  CFG_USE_CCSP_SYSLOG                        0
#define  CFG_USE_Event_Displatch                    0
#define  CFG_USE_Common_Util                        0

#define  TR181_Mlan_Sysevent_ResyncAll()
#define  TR181_Mlan_Sysevent_Resync(IfIndex)

#define  CFG_TR181_ETH_BORROW_MAC                   0

#endif   /*_DML_TR181_CUSTOM_CFG_H_*/
