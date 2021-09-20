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

    module: cosa_FileTransfer_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**************************************************************************/

#include "cosa_x_cisco_com_filetransfer_dml.h"
#include "dml_tr181_custom_cfg.h"
#include "safec_lib_common.h"

#ifdef   CONFIG_CISCO_FILE_TRANSFER

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        FileTransfer_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
FileTransfer_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_FILETRANSFER    pMyObject = (PCOSA_DATAMODEL_FILETRANSFER)g_pCosaBEManager->hFileTransfer;
    PCOSA_DML_FILETRANSFER_CFG      pCfg      = (PCOSA_DML_FILETRANSFER_CFG)&pMyObject->Cfg;
    
    UNREFERENCED_PARAMETER(hInsContext);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Server", TRUE))
    {
        /* collect value */
        *puLong = pCfg->Server;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Protocol", TRUE))
    {
        /* collect value */
        *puLong = pCfg->Protocol;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AuthType", TRUE))
    {
        /* collect value */
        *puLong = pCfg->AuthType;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Action", TRUE))
    {
        /* collect value */
        *puLong = pCfg->Action;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        *puLong = CosaDmlFileTransferGetStatus((ANSC_HANDLE)pMyObject);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        FileTransfer_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
FileTransfer_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    PCOSA_DATAMODEL_FILETRANSFER    pMyObject = (PCOSA_DATAMODEL_FILETRANSFER)g_pCosaBEManager->hFileTransfer;
    PCOSA_DML_FILETRANSFER_CFG      pCfg      = (PCOSA_DML_FILETRANSFER_CFG)&pMyObject->Cfg;
    errno_t                         rc        = -1;
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pulSize);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "FileName", TRUE))
    {
        /* collect value */
        rc = strcpy_s(pValue,*pulSize, (char*)pCfg->FileName);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if( AnscEqualString(ParamName, "Username", TRUE))
    {
        /* collect value */
        rc = strcpy_s(pValue,*pulSize, (char*)pCfg->Username);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if( AnscEqualString(ParamName, "Password", TRUE))
    {
        /* collect value */
        rc = strcpy_s(pValue,*pulSize, (char*)pCfg->Password);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        FileTransfer_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
FileTransfer_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_FILETRANSFER    pMyObject = (PCOSA_DATAMODEL_FILETRANSFER)g_pCosaBEManager->hFileTransfer;
    PCOSA_DML_FILETRANSFER_CFG      pCfg      = (PCOSA_DML_FILETRANSFER_CFG)&pMyObject->Cfg;
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Server", TRUE))
    {
        /* save update to backup */
        pCfg->Server = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Protocol", TRUE))
    {
        /* save update to backup */
        pCfg->Protocol = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AuthType", TRUE))
    {
        /* save update to backup */
        pCfg->AuthType = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Action", TRUE))
    {
        /* save update to backup */
        pCfg->Action = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        FileTransfer_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
FileTransfer_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_FILETRANSFER    pMyObject = (PCOSA_DATAMODEL_FILETRANSFER)g_pCosaBEManager->hFileTransfer;
    PCOSA_DML_FILETRANSFER_CFG      pCfg      = (PCOSA_DML_FILETRANSFER_CFG)&pMyObject->Cfg;
    errno_t                         rc        = -1;
    UNREFERENCED_PARAMETER(hInsContext);   
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "FileName", TRUE))
    {
        /* save update to backup */
        rc = strcpy_s((char*)pCfg->FileName, sizeof(pCfg->FileName),pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Username", TRUE))
    {
        /* save update to backup */
        rc = strcpy_s((char*)pCfg->Username,sizeof(pCfg->Username), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Password", TRUE))
    {
        /* save update to backup */
        rc = strcpy_s((char*)pCfg->Password,sizeof(pCfg->Password), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        FileTransfer_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
FileTransfer_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        FileTransfer_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
FileTransfer_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_FILETRANSFER    pMyObject = (PCOSA_DATAMODEL_FILETRANSFER)g_pCosaBEManager->hFileTransfer;
    PCOSA_DML_FILETRANSFER_CFG      pCfg      = (PCOSA_DML_FILETRANSFER_CFG)&pMyObject->Cfg;
    UNREFERENCED_PARAMETER(hInsContext);    
    if ( ANSC_STATUS_SUCCESS != CosaDmlFileTransferSetCfg((ANSC_HANDLE)pMyObject, pCfg) )
    {
        FileTransfer_Rollback(NULL);
    }
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        FileTransfer_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
FileTransfer_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_FILETRANSFER    pMyObject = (PCOSA_DATAMODEL_FILETRANSFER)g_pCosaBEManager->hFileTransfer;
    PCOSA_DML_FILETRANSFER_CFG      pCfg      = (PCOSA_DML_FILETRANSFER_CFG)&pMyObject->Cfg;

    UNREFERENCED_PARAMETER(hInsContext);
    CosaDmlFileTransferGetCfg(NULL, pCfg);
    
    return 0;
}

#endif
