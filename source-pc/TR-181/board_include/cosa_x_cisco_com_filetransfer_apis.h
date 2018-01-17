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

#ifndef  _COSA_FILETRANSFER_APIS_H
#define  _COSA_FILETRANSFER_APIS_H

#include "../middle_layer_src/cosa_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "../middle_layer_src/plugin_main_apis.h"

#define  TRUE_STATIC_IP_CONFIG_PATH  "/var/"

/***********************************
    Actual definition declaration
************************************/

typedef  enum
_COSA_DML_FILETRANSFER_PROTOCOL
{
    COSA_DML_FILETRANSFER_PROTOCOL_TFTP   = 1,
    COSA_DML_FILETRANSFER_PROTOCOL_FTP,
    COSA_DML_FILETRANSFER_PROTOCOL_HTTP
}
COSA_DML_FILETRANSFER_PROTOCOL, *PCOSA_DML_FILETRANSFER_PROTOCOL;

typedef  enum
_COSA_DML_FILETRANSFER_AUTHTYPE
{
    COSA_DML_FILETRANSFER_AUTHTYPE_Basic   = 1,
    COSA_DML_FILETRANSFER_AUTHTYPE_Digest
}
COSA_DML_FILETRANSFER_AUTHTYPE, *PCOSA_DML_FILETRANSFER_AUTHTYPE;

typedef  enum
_COSA_DML_FILETRANSFER_ACTION
{
    COSA_DML_FILETRANSFER_ACTION_Download   = 1,
    COSA_DML_FILETRANSFER_ACTION_Upload
}
COSA_DML_FILETRANSFER_ACTION, *PCOSA_DML_FILETRANSFER_ACTION;

typedef  enum
_COSA_DML_FILETRANSFER_STATUS
{
    COSA_DML_FILETRANSFER_STATUS_InProgress   = 1,
    COSA_DML_FILETRANSFER_STATUS_Complete,
    COSA_DML_FILETRANSFER_STATUS_Failed
}
COSA_DML_FILETRANSFER_STATUS, *PCOSA_DML_FILETRANSFER_STATUS;

/*
 *  Structure definitions
 */
typedef  struct
_COSA_DML_FILETRANSFER_CFG
{
    ULONG               Server;
    ULONG               Protocol;                                           
    UCHAR               FileName[128];                                      
    UCHAR               Username[128];                                      
    UCHAR               Password[128];                                      
    ULONG               AuthType;                                           
    ULONG               Action;                                                
}
COSA_DML_FILETRANSFER_CFG,  *PCOSA_DML_FILETRANSFER_CFG;

ANSC_STATUS
CosaDmlFileTransferInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
);

ANSC_STATUS
CosaDmlFileTransferSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FILETRANSFER_CFG  pCfg
    );

ANSC_STATUS
CosaDmlFileTransferGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FILETRANSFER_CFG  pCfg
    );

ULONG
CosaDmlFileTransferGetStatus
    (
        ANSC_HANDLE                 hContext
    );

#endif
