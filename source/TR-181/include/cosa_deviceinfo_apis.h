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

    module: cosa_deviceinfo_apis.h

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


#ifndef  _COSA_DEVICEINFO_APIS_H
#define  _COSA_DEVICEINFO_APIS_H

#include "../middle_layer_src/cosa_apis.h"
#include "../middle_layer_src/plugin_main_apis.h"

#if defined(_ANSC_LINUX)
    #include <sys/sysinfo.h>
#endif

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

enum
{
    CNS_FLASH_SerialNo = 1,
    CNS_FLASH_Description,
    CNS_FLASH_ModelNo,
    CNS_FLASH_NP_ID,
    CNS_FLASH_Version,
    CNS_FLASH_MFG_Date,
    CNS_FLASH_HW_Feature
};

enum
{
    COSA_DML_PROC_STATUS_Running = 1,
    COSA_DML_PROC_STATUS_Sleeping,
    COSA_DML_PROC_STATUS_Stopped,
    COSA_DML_PROC_STATUS_Idle,
    COSA_DML_PROC_STATUS_Uninterruptible,
    COSA_DML_PROC_STATUS_Zombie,
};

typedef  struct
_COSA_PROCESS_ENTRY
{
    ULONG                           Pid;
    char                            Command[256];
    ULONG                           Size;
    ULONG                           Priority;
    ULONG                           CPUTime;
    ULONG                           State;
}
COSA_PROCESS_ENTRY, *PCOSA_PROCESS_ENTRY;

#define  COSA_DATAMODEL_PROCESS_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    COSA_BASE_CONTENT                                                                       \
    ULONG                           ProcessNumberOfEntries;                             \
    ULONG                           CPUUsage;                                           \
    PCOSA_PROCESS_ENTRY             pProcTable;                                       \

typedef  struct
_COSA_DATAMODEL_PROCESS                                               
{
    COSA_DATAMODEL_PROCESS_CLASS_CONTENT
}
COSA_DATAMODEL_PROCSTATUS,  *PCOSA_DATAMODEL_PROCSTATUS;

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_HANDLE CosaProcStatusCreate();
void COSADmlRemoveProcessInfo(PCOSA_DATAMODEL_PROCSTATUS pObj);
void COSADmlGetProcessInfo(PCOSA_DATAMODEL_PROCSTATUS p_info);
ULONG COSADmlGetCpuUsage();
ULONG COSADmlGetMemoryStatus(char * ParamName);
ULONG COSADmlGetMaxWindowSize();

ANSC_STATUS
CosaDmlDiInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlDiGetManufacturer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetManufacturerOUI
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetModelName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetDescription
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetProductClass
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetSerialNumber
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetHardwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetSoftwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetAdditionalHardwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetAdditionalSoftwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetProvisioningCode
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiSetProvisioningCode
    (
        ANSC_HANDLE                 hContext,
        char*                       pProvisioningCode
    );

ULONG
CosaDmlDiGetUpTime
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlDiGetFirstUseDate
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetHardware
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetBootloaderVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetFirmwareName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetFirmwareBuildTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetBaseMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );
    
ANSC_STATUS
CosaDmlGetTCPImplementation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize    
    );

ANSC_STATUS
CosaDmlDiGetAdvancedServices
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetProcessorSpeed
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

#endif
