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

    module: cosa_FileTransfer_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaFileTransferCreate
        *  CosaFileTransferInitialize
        *  CosaFileTransferRemove
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "cosa_x_cisco_com_filetransfer_internal.h"
#include "dml_tr181_custom_cfg.h"

#ifdef   CONFIG_CISCO_FILE_TRANSFER

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaFileTransferCreate
            (
            );

    description:

        This function constructs cosa FileTransfer object and return handle.

    argument:  

    return:     newly created FileTransfer object.

**********************************************************************/

ANSC_HANDLE
CosaFileTransferCreate
    (
        VOID
    )
{
    ANSC_STATUS                  returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_FILETRANSFER pMyObject    = (PCOSA_DATAMODEL_FILETRANSFER)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_FILETRANSFER)AnscAllocateMemory(sizeof(COSA_DATAMODEL_FILETRANSFER));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /* Initiation all data */
    _ansc_memset(pMyObject, 0, sizeof(COSA_DATAMODEL_FILETRANSFER));

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_FILETRANSFER_OID;
    pMyObject->Create            = CosaFileTransferCreate;
    pMyObject->Remove            = CosaFileTransferRemove;
    pMyObject->Initialize        = CosaFileTransferInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaFileTransferInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa FileTransfer object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaFileTransferInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_FILETRANSFER    pMyObject    = (PCOSA_DATAMODEL_FILETRANSFER)hThisObject;
    PCOSA_DML_FILETRANSFER_CFG      pCfg         = (PCOSA_DML_FILETRANSFER_CFG)&pMyObject->Cfg;

    CosaDmlFileTransferInit  (NULL, NULL);
    CosaDmlFileTransferGetCfg(NULL, pCfg);
    pCfg->Server      = COSA_DML_FILETRANSFER_SERVER_TFTP1;
    pCfg->Action      = COSA_DML_FILETRANSFER_ACTION_Download;
    pCfg->Protocol    = COSA_DML_FILETRANSFER_PROTOCOL_TFTP;
    pMyObject->Status = COSA_DML_FILETRANSFER_STATUS_Failed;

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaFileTransferRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa FileTransfer object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaFileTransferRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_FILETRANSFER    pMyObject    = (PCOSA_DATAMODEL_FILETRANSFER)hThisObject;

    /* Remove necessary resounce */


    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

#endif
