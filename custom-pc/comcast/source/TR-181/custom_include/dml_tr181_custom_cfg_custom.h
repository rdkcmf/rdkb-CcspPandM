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

        Customer - Comcast

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        09/15/2013  initial revision.

**********************************************************************/

#ifndef  _DML_TR181_CUSTOM_CFG_CUSTOM_H_
#define  _DML_TR181_CUSTOM_CFG_CUSTOM_H_

#define  CONFIG_CISCO_FILE_TRANSFER                 1

/*
 *  Configuration for True Static IP is moved to board specific
 *  build configuration file included by dml_tr181_custom_cfg.h
 *
 *  Force CONFIG_CISCO_TRUE_STATIC_IP for now
 */
#ifndef  CONFIG_CISCO_TRUE_STATIC_IP
    #define  CONFIG_CISCO_TRUE_STATIC_IP                1
#endif

#define  TR181_ParentalControlCreate()              CosaParentalControlCreate()
#define  TR181_ParentalControlRemove(hContext)      CosaParentalControlRemove(hContext)

#define  CONFIG_CCSP_WAN_MGMT_PORT

#endif   /*_DML_TR181_CUSTOM_CFG_CUSTOM_H_*/
