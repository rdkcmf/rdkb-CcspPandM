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

    module: pnm_custom.h

        For CCSP P&M Component,

    ---------------------------------------------------------------

    description:

        This header file gives the custom module definitions for
        CCSP P&M.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Ding Hua

**********************************************************************/

#ifndef  _PNM_CUSTOM_H_
#define  _PNM_CUSTOM_H_

/**********************************************************************
                      GLOBAL CUSTOM DEFINITIONS
**********************************************************************/

#define  MESSAGE_BUS_CONFIG_FILE                    "msg_daemon.cfg"

#define  CCSP_PNM_START_CFG_FILE                    "CcspPam.cfg"

/* TBD  Move to platform specific header file */
#define  CCSP_PNM_BACKTRACE_FILE                    "/nvram/pamssp_backtrace"

#endif   /*_PNM_CUSTOM_H_*/
