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

    module: cosa_moca_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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

#ifndef  _COSA_MOCA_INTERNAL_H
#define  _COSA_MOCA_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_moca_apis.h"

#define MOCA_REFRESH_INTERVAL   5
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))

struct
_COSA_DML_MOCA_IF_FULL_TABLE
{
    COSA_DML_MOCA_IF_FULL                   MoCAIfFull;
    COSA_DML_MOCA_QOS                       MoCAIfQos;
    PCOSA_DML_MOCA_ASSOC_DEVICE             pMoCAAssocDevice;
    ULONG                                   ulMoCAAssocDeviceCount;
    
    SLIST_HEADER                            pMoCAExtCounterTable;
    SLIST_HEADER                            pMoCAExtAggrCounterTable;
    
    PCOSA_DML_MOCA_PEER                     pMoCAPeerTable;
    ULONG                                   ulMoCAPeerTableCount;
    
    SLIST_HEADER                            MoCAMeshTxNodeTable;

    PCOSA_DML_MOCA_FLOW                     pMoCAFlowTable;
    ULONG                                   ulMoCAFlowTableCount;
}_struct_pack_ ;
typedef struct _COSA_DML_MOCA_IF_FULL_TABLE COSA_DML_MOCA_IF_FULL_TABLE,  *PCOSA_DML_MOCA_IF_FULL_TABLE;

#define  COSA_DATAMODEL_MOCA_CLASS_CONTENT                                  \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \
    /* start of Time object class content */                                \
    ANSC_HANDLE                             pPoamMoCADm;                    \
    ANSC_HANDLE                             pSlapMoCADm;                    \
    COSA_DML_MOCA_CFG                       MoCACfg;                        \
    COSA_DML_MOCA_IF_FULL_TABLE             MoCAIfFullTable[MOCA_INTEFACE_NUMBER];   \

typedef  struct
_COSA_DATAMODEL_MOCA_CLASS_CONTENT
{
    COSA_DATAMODEL_MOCA_CLASS_CONTENT
}
COSA_DATAMODEL_MOCA, *PCOSA_DATAMODEL_MOCA;

/*
*  This struct is for creating entry context link in writable table when call GetEntry()
*/
#define  COSA_CONTEXT_MOCA_LINK_CLASS_CONTENT                                      \
        COSA_CONTEXT_LINK_CLASS_CONTENT                                            \
        ULONG                            InterfaceIndex;                           \
        ULONG                            Index;                                    \

typedef  struct
_COSA_CONTEXT_MOCA_LINK_OBJECT
{
    COSA_CONTEXT_MOCA_LINK_CLASS_CONTENT
}
COSA_CONTEXT_MOCA_LINK_OBJECT,  *PCOSA_CONTEXT_MOCA_LINK_OBJECT;

/*
    Standard function declaration
*/
ANSC_HANDLE
CosaMoCACreate
    (
        VOID
    );

ANSC_STATUS
CosaMoCAInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaMoCARemove
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
