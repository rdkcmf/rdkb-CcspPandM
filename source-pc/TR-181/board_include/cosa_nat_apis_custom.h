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

    module: cosa_nat_apis_custom.h

    For Data Model Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        The prototypes of custom data model SBAPI are defined here

    ---------------------------------------------------------------

    environment:

        Maybe platform independent, but customer specific

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        05/08/2014  initial revision.

**************************************************************************/


#ifndef  _COSA_NAT_APIS_CUSTOM_H
#define  _COSA_NAT_APIS_CUSTOM_H

#define  COSA_DML_NAT_CUSTOM                                            \
            BOOLEAN                 X_Comcast_com_EnablePortMapping;    \
            BOOLEAN                 X_Comcast_com_EnableHSPortMapping;  \
            BOOLEAN                 X_Comcast_com_EnableNATMapping;     \

#define	 COSA_DML_NAT_CUSTOM_SET_ENABLEPORTMAPPING(structure, value)	\
			(structure)->X_Comcast_com_EnablePortMapping = (value)

#define  COSA_DML_NAT_CUSTOM_GET_ENABLEPORTMAPPING(structure) 			\
			(structure)->X_Comcast_com_EnablePortMapping

#define  COSA_DML_NAT_CUSTOM_SET_ENABLEHSPORTMAPPING(structure, value)	\
			(structure)->X_Comcast_com_EnableHSPortMapping = (value)

#define  COSA_DML_NAT_CUSTOM_GET_ENABLEHSPORTMAPPING(structure)			\
			(structure)->X_Comcast_com_EnableHSPortMapping

#define  COSA_DML_NAT_CUSTOM_SET_ENABLENATMAPPING(structure, value)		\
			(structure)->X_Comcast_com_EnableNATMapping = (value)

#define COSA_DML_NAT_CUSTOM_GET_ENABLENATMAPPING(structure)				\
			(structure)->X_Comcast_com_EnableNATMapping

#endif
