/************************************************************************************
  If not stated otherwise in this file or this component's Licenses.txt file the
  following copyright and licenses apply:

  Copyright 2018 RDK Management

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
**************************************************************************/

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

#ifdef CONFIG_CISCO_HOTSPOT
/**************************************************************************

    module: cosa_x_comcast_com_gre_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        07/15/2011    initial revision.

**************************************************************************/

#ifndef  _COSA_X_COMCAST_COM_GRE_INTERNAL_H
#define  _COSA_X_COMCAST_COM_GRE_INTERNAL_H
#include "cosa_apis.h"
#include "cosa_x_comcast_com_gre_apis.h"

typedef  struct
_COSA_DATAMODEL_GRE
{
    COSA_BASE_CONTENT

    COSA_DML_GRE_IF GreIf[MAX_GRE_IF];
}
COSA_DATAMODEL_GRE;

/*
    Standard function declaration
*/
ANSC_HANDLE
CosaGreCreate
    (
        VOID
    );

ANSC_STATUS
CosaGreInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaGreRemove
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
#endif
