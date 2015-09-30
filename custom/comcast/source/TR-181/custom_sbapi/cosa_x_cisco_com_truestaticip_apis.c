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

    module: cosa_routing_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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

        01/11/2011    initial revision.

**************************************************************************/
#include "cosa_apis.h"
#include "cosa_x_cisco_com_truestaticip_dml.h"
#include "cosa_x_cisco_com_filetransfer_apis.h"
#include "cosa_x_cisco_com_truestaticip_apis.h"
#include "cosa_x_cisco_com_truestaticip_internal.h"
#include "dmsb_tr181_psm_definitions.h"
#include "ansc_ato_interface.h"
#include "ccsp_psm_helper.h"
#include <openssl/des.h>
#include <utctx/utctx.h>
#include <utctx/utctx_api.h>
#include <utapi/utapi.h>
#include <utapi/utapi_util.h>
#include <stdlib.h>
#include "cosa_drg_common.h"

extern void* g_pDslhDmlAgent;

static int iBlockedURLInsNum = 0;

static void
free_parameterValStruct
    (
        void *bus_handle,
        int size,
        parameterValStruct_t *val
    )
{
    int i;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
	
	if(val) 
	{	
    	if(size) 
		{
        	for(i = 0; i < size; i++)
      		{
                if(val[i].parameterName)  bus_info->freefunc(val[i].parameterName);
            	if(val[i].parameterValue) bus_info->freefunc(val[i].parameterValue);
        	}
    	}
		
		bus_info->freefunc(val);
    }
}

static
ANSC_STATUS
TriggerOtherModule
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus           = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP   )hContext;
    PCOSA_DML_TSIP_CFG              pTsipCfg               = (PCOSA_DML_TSIP_CFG     )&pMyObject->TSIPCfg;
    BOOL                            bEnabled               = FALSE;

    bEnabled = g_GetParamValueBool(g_pDslhDmlAgent, "Device.Routing.RIP.Enable");
    AnscTraceWarning(("Get and Set Device.Routing.RIP.Enable: %d\n", bEnabled));
    returnStatus = g_SetParamValueBool("Device.Routing.RIP.Enable", bEnabled);

    if ( !pTsipCfg->Enabled )
    {
        system("sysevent set wan_staticip-status stopped");
    }
    else
    {
        system("sysevent set wan_staticip-status started");
    }

    system("sysevent set firewall-restart 1");

    return returnStatus;
}

ANSC_STATUS
CosaDmlTSIPInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
)
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPLoadMappingFile
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus           = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP   )hContext;
    PANSC_ATOM_TABLE_OBJECT         pAtomNamespace         = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomNamespace;
    PANSC_TOKEN_CHAIN               pMappingTokenChain     = (PANSC_TOKEN_CHAIN      )NULL;
    PANSC_TOKEN_CHAIN               pMappingEntryChain     = (PANSC_TOKEN_CHAIN      )NULL;
    PANSC_STRING_TOKEN              pStringToken           = (PANSC_STRING_TOKEN     )NULL;
    PANSC_STRING_TOKEN              pEntryToken            = (PANSC_STRING_TOKEN     )NULL;
    PNAMESPACE_MAPPING              pMapping               = NULL;
    FILE*                           fpMapping              = NULL;
    char*                           pBuffer                = NULL;
    char*                           pSeparator             = NULL;
    char*                           pCount                 = NULL;
    char*                           pType                  = NULL;
    ULONG                           ulType                 = 0;
    ULONG                           ulCount                = 0;
    ULONG                           ulIndex                = 0;
    ULONG                           len                    = 0;
    ULONG                           len2                   = 0;

    if ( !pAtomNamespace )
    {
        AnscTraceWarning(("CosaDmlTSIPLoadMappingFile: Atom table is NOT ready!\n"));
        return ANSC_STATUS_FAILURE;
    }

    AnscTraceWarning(("!!!!!! CosaDmlTSIPLoadMappingFile 1 !!!!!!\n"));

    fpMapping = fopen("./mapping.txt"/*TRUESTATICIP_MAPPING_FILE*/, "r");
    if ( !fpMapping )
    {
        AnscTraceWarning(("!!!!!!!!!! CosaDmlTSIPLoadMappingFile: mapping file open error!!!!!!!!!\n"));
        return ANSC_STATUS_FAILURE;
    }

    fseek(fpMapping, 0L, SEEK_END);
    len = ftell(fpMapping);
    if ( len <= 0 )
    {
        AnscTraceWarning(("CosaDmlTSIPLoadMappingFile: mapping file is empty!\n"));
        fclose(fpMapping);
        return ANSC_STATUS_FAILURE;
    }

    AnscTraceWarning(("!!!!!! CosaDmlTSIPLoadMappingFile 2 len=%d !!!!!!\n", len));

    fseek(fpMapping, 0L, SEEK_SET);
    pBuffer = (char*)AnscAllocateMemory(len + 1);
    if ( !pBuffer )
    {
        fclose(fpMapping);
        return ANSC_STATUS_RESOURCES;
    }

    fseek(fpMapping, 0L, SEEK_SET);
    len2 = fread(pBuffer, 1, len, fpMapping);
    if ( len2 != len )
    {
        AnscTraceWarning(("CosaDmlTSIPLoadMappingFile: mapping file read error!\n"));
        returnStatus =  ANSC_STATUS_FAILURE;
        goto EXIT;
    }

    AnscTraceWarning(("!!!!!! CosaDmlTSIPLoadMappingFile 3 !!!!!!\n"));

    pMappingTokenChain = AnscTcAllocate(pBuffer, "\n");

    if ( !pMappingTokenChain )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        goto EXIT;
    }

    AnscTraceWarning(("!!!!!! CosaDmlTSIPLoadMappingFile 4 !!!!!!\n"));
    
    while ((pStringToken = AnscTcUnlinkToken(pMappingTokenChain)))
    {
        ulIndex = 0;
        pSeparator = _ansc_strstr(pStringToken->Name, " ");
        *pSeparator++ = '\0';
        pCount = pSeparator;
        pSeparator = _ansc_strstr(pCount, " ");
        *pSeparator++ = '\0';
        ulCount = _ansc_atoi(pCount);
        pMapping = (PNAMESPACE_MAPPING)AnscAllocateMemory(ulCount * sizeof(NAMESPACE_MAPPING));
        if ( !pMapping )
        {
            AnscFreeMemory(pStringToken);
            returnStatus = ANSC_STATUS_RESOURCES;
            goto EXIT;
        }
           
        pMappingEntryChain = AnscTcAllocate(pSeparator, ";");
        
        if ( !pMappingEntryChain )
        {
            AnscFreeMemory(pMapping    );
            AnscFreeMemory(pStringToken);
            returnStatus = ANSC_STATUS_RESOURCES;
            goto EXIT;
        }
        
        while ((pEntryToken = AnscTcUnlinkToken(pMappingEntryChain)))
        {
            pType = pEntryToken->Name;
            pSeparator = _ansc_strstr(pEntryToken->Name, " ");
            *pSeparator++ = '\0';
            ulType =_ansc_atoi(pType);
            pMapping[ulIndex].Type = ulType;
            AnscCopyString(pMapping[ulIndex++].FullPath, pSeparator);
            AnscFreeMemory(pEntryToken);
        }

        /* printf("pStringToken->Name: %s, pMapping->FullPath: %s\n", pStringToken->Name, pMapping->FullPath); */
        
        pAtomNamespace->AddAtomByName
                (
                    pAtomNamespace,
                    pStringToken->Name,
                    ulCount,
                    pMapping
                );
            
        AnscFreeMemory(pStringToken);
    }

EXIT:
    if ( pBuffer )
    {
        AnscFreeMemory(pBuffer);
    }

    fclose(fpMapping);

    return returnStatus;
}

static BOOL
CosaDmlTSIPTestFullPathName
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus           = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP      )hContext;
    char                            TestName[256]          = {0};
    char                            param_val[64]          = {0};
    ULONG                           val_len                = 0;
    int                             ret                    = 0;
    
    _ansc_sprintf(TestName, "Device.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.%d.Site", iBlockedURLInsNum);
    val_len = sizeof(param_val);

    ret = g_GetParamValueString(g_pDslhDmlAgent, TestName, param_val, &val_len);

    return (ret == 0) ? TRUE : FALSE;
}

static
ANSC_STATUS
CosaDmlTSIPApplyConfigFileTask
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus           = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP      )hContext;
    PCOSA_DML_TSIP_CFG              pTsipCfg               = (PCOSA_DML_TSIP_CFG        )&pMyObject->TSIPCfg;
    PCOSA_DML_FILETRANSFER_CFG      pFtCfg                 = (PCOSA_DML_FILETRANSFER_CFG)pMyObject->hFtCfg;
    PANSC_ATOM_TABLE_OBJECT         pAtomNamespace         = (PANSC_ATOM_TABLE_OBJECT   )pMyObject->hAtomNamespace;
    PANSC_ATOM_DESCRIPTOR           pAtomItem              = (PANSC_ATOM_DESCRIPTOR     )NULL;
    PANSC_TOKEN_CHAIN               pConfigTokenChain      = (PANSC_TOKEN_CHAIN         )NULL;
    PANSC_STRING_TOKEN              pStringToken           = (PANSC_STRING_TOKEN        )NULL;
    PANSC_STRING_TOKEN              pEntryToken            = (PANSC_STRING_TOKEN        )NULL;
    parameterValStruct_t*           pVal                   = NULL;
    PNAMESPACE_MAPPING              pMapping               = NULL;
    char                            FileName[64]           = {0};
    char                            FullName[256]          = {0};
    char*                           pBuffer                = NULL;
    char*                           pBuffer2               = NULL;
    char*                           pSeparator             = NULL;
    char*                           pValue                 = NULL;
    char*                           p                      = NULL;
    char*                           pFaultParamName        = NULL;    
    FILE*                           fpConfig               = NULL;
    ULONG                           ulParamCount           = 0;
    ULONG                           len                    = 0;
    ULONG                           len2                   = 0;
    int                             InsNum                 = 0;
    int                             i                      = 0;
    int                             ret                    = CCSP_SUCCESS;
    BOOL                            bCommit                = FALSE;
    ULONG                           ulIndex                = 0;
    SLAP_VARIABLE                   SlapValue       = {0};

    AnscTraceFlow(("%s\n", __FUNCTION__));

    if ( !pFtCfg || !pAtomNamespace )
    {
        AnscTraceWarning(("CosaDmlTSIPApplyConfigFile: Mapping file is NOT ready!\n"));
        return ANSC_STATUS_FAILURE;
    }

    if ( TRUE )
    {
        FILE* fpConfig3 = NULL;
        int len3 = 0;

        fpConfig3 = fopen("/var/cliconfig.txt", "r");
        if ( fpConfig3 )
        {
            fseek(fpConfig3, 0L, SEEK_END);
            len3 = ftell(fpConfig3);

            pBuffer2 = (char*)AnscAllocateMemory(len3 + 1);
            if ( pBuffer2 )
            {
                fseek(fpConfig3, 0L, SEEK_SET);
                fread(pBuffer2, 1, len3, fpConfig3);
            }
            fclose(fpConfig3);
            remove("/var/cliconfig.txt");
            goto Start;
        }
    }

    _ansc_sprintf(FileName, "%s%s", TRUE_STATIC_IP_CONFIG_PATH, pFtCfg->FileName);

    fpConfig = fopen(FileName, "r");
    if ( !fpConfig )
    {
        AnscTraceWarning(("CosaDmlTSIPApplyConfigFile: Config file open error!\n"));
        return ANSC_STATUS_FAILURE;
    }
    
    fseek(fpConfig, 0L, SEEK_END);
    len = ftell(fpConfig);
    if ( len <= 0 )
    {
        AnscTraceWarning(("CosaDmlTSIPApplyConfigFile: Config file is empty!\n"));
        fclose(fpConfig);
        return ANSC_STATUS_FAILURE;
    }

    pBuffer  = (char*)AnscAllocateMemory(len + 1);
    
    if ( !pBuffer )
    {
        fclose(fpConfig);
        return ANSC_STATUS_RESOURCES;
    }

    fseek(fpConfig, 0L, SEEK_SET);
    len2 = fread(pBuffer, 1, len, fpConfig);
    if ( len2 != len )
    {
        AnscTraceWarning(("CosaDmlTSIPLoadMappingFile: config file read error!\n"));
        returnStatus =  ANSC_STATUS_FAILURE;
        goto EXIT;
    }

    /* Decrypt pBuffer */
    if ( AnscSizeOfString(pTsipCfg->ConfigEncryptKey) == KEY_LENGTH_64BIT ) /* 64 bit DES key */
    {
        DES_cblock Key2 = {0}; /*= {0x57, 0x8A, 0x95, 0x8E, 0x3D, 0xD9, 0x33, 0xFC}*/
        DES_key_schedule schedule = {0};
        FILE* fpConfig2 = NULL;
        ULONG ulTmpValue = 0;
        char temp_char[3] = {0};
        int i = 0;

        pBuffer2 = (char*)AnscAllocateMemory(len + 1);
        if ( !pBuffer2 )
        {
            fclose(fpConfig);
            returnStatus = ANSC_STATUS_RESOURCES;
            goto EXIT;
        }

        for ( i = 0; i < 8; i++ )
        {
            temp_char[0] = pTsipCfg->ConfigEncryptKey[i*2 + 0];
            temp_char[1] = pTsipCfg->ConfigEncryptKey[i*2 + 1];
            temp_char[2] = 0;

            ulTmpValue = AnscGetStringUlongHex(temp_char);
            Key2[i] = (u_char)ulTmpValue;
        }

        DES_set_key( &Key2, &schedule );
        
        for (i = 0; i < len / 8; i++ )
        {
            DES_ecb_encrypt(pBuffer + (i*8), pBuffer2 + (i*8), &schedule, DES_DECRYPT);
        }

        fpConfig2 = fopen("/var/decrypted.txt", "w+");
        if ( fpConfig2 )
        {
            fwrite(pBuffer2, 1, AnscSizeOfString(pBuffer2), fpConfig2);
            fclose(fpConfig2);
        }
    }
    else
    {
        AnscTraceWarning(("!!!!!!!!!! Please set decryption key first !!!!!!!!!!\n"));
        /* goto EXIT; */
    }

Start:

    pConfigTokenChain = AnscTcAllocate(pBuffer2, "\n");

    if ( !pConfigTokenChain )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        goto EXIT;
    }

    while ((pStringToken = AnscTcUnlinkToken(pConfigTokenChain)))
    {
        printf("!!! Now processing %s !!!\n", pStringToken->Name);
        pSeparator = _ansc_strstr(pStringToken->Name, " ");
        if ( !pSeparator )
        {
            AnscFreeMemory(pStringToken);
            continue;
        }
        *pSeparator++ = '\0';
        pValue = pSeparator;
        if ( p = _ansc_strstr(pValue, "\r") )
        {
            *p = '\0';
        }

        AnscTraceWarning(("!!!!!!!!!! pValue = %s!!!!!!!!!!\n", pValue));
        
        pAtomItem = (PANSC_ATOM_DESCRIPTOR)pAtomNamespace->GetAtomByName(pAtomNamespace, pStringToken->Name);
        if ( !pAtomItem )
        {
            AnscTraceWarning(("%s is NOT found in the mapping file!\n", pStringToken->Name));
            AnscFreeMemory(pStringToken);
            continue;
        }
    
        ulParamCount = pAtomItem->AtomId;
        pMapping = (PNAMESPACE_MAPPING)pAtomItem->hContext;

        pVal = (parameterValStruct_t*)AnscAllocateMemory(ulParamCount * sizeof(parameterValStruct_t));
        if ( pVal == NULL )
        {
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT;
        }
        
        if (ulParamCount == 1)
        {
            /* Simple mapping goes here */
            pVal[0].parameterName  = AnscCloneString(pMapping[0].FullPath);
            if ( AnscEqualString(pStringToken->Name, "wan_ip_method", FALSE) )
            {
                pVal[0].parameterValue = AnscEqualString(pValue, "true_static", FALSE) ? AnscCloneString("true") : AnscCloneString("false");
                AnscTraceWarning(("!!! Mode: true_static, value:%s !!!\n", pVal[0].parameterValue));
            }
            else if ( _ansc_strstr(pStringToken->Name, "website_block_") ) /* Special handle for website_block_xxx */
            {
                if ( iBlockedURLInsNum == 0 || CosaDmlTSIPTestFullPathName(pMyObject) == FALSE )
                {
                    ret = 
                        CcspCcMbi_AddTblRow
                            (
                                0,
                                "Device.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.",
                                &InsNum,
                                NULL
                            );
                    if ( ret != CCSP_SUCCESS )
                    {
                        free_parameterValStruct(g_MessageBusHandle, 1, pVal);
                        AnscFreeMemory(pStringToken);
                        continue;
                    }
                    else
                    {
                        iBlockedURLInsNum = InsNum;
                        _ansc_sprintf(FullName, "Device.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.%d.BlockMethod", iBlockedURLInsNum);
                        g_SetParamValueString(FullName, "URL");
                    }
                }
                
                AnscFreeMemory(pVal[0].parameterName);
                pVal[0].parameterName = AnscAllocateMemory(AnscSizeOfString(pMapping[0].FullPath) + 8);
                if ( !pVal[0].parameterName )
                {
                    free_parameterValStruct(g_MessageBusHandle, 1, pVal);
                    AnscFreeMemory(pStringToken);
                    continue;
                }
                _ansc_sprintf(pVal[0].parameterName, pMapping[0].FullPath, iBlockedURLInsNum);
                printf("!!! pVal[0].parameterName: %s !!!\n", pVal[0].parameterName);
                if ( AnscEqualString(pStringToken->Name, "website_block_schedule_day", FALSE) )
                {
                    if (AnscEqualString(pValue, "all", FALSE))
                    {
                        pVal[0].parameterValue = AnscCloneString("Mon,Tue,Wed,Thu,Fri,Sat,Sun");
                    }
                }
                else
                {
                    pVal[0].parameterValue = AnscCloneString(pValue);
                }
            }
            else if ( _ansc_strstr(pStringToken->Name, "lan_ip_dns_pri") || _ansc_strstr(pStringToken->Name, "lan_ip_dns_sec") )
            {
                char strval[64] = {0};
                char val[64] = {0};
                ULONG size = sizeof(strval);
      
                int i = g_GetParamValueString(g_pDslhDmlAgent, "Device.DHCPv4.Server.Pool.1.DNSServers", strval, &size);

                if ( strval[0] != NULL && ! _ansc_strstr(strval, pValue) && ! _ansc_strstr(strval, ",") )
                {
                    _ansc_sprintf(val, "%s,%s", strval, pValue);
                    g_SetParamValueString("Device.DHCPv4.Server.Pool.1.DNSServers", val);
                }
                else
                {
                    g_SetParamValueString("Device.DHCPv4.Server.Pool.1.DNSServers", pValue);
                }

                free_parameterValStruct(g_MessageBusHandle, 1, pVal);
                AnscFreeMemory(pStringToken);
                continue;
            }
            else if ( _ansc_strstr(pStringToken->Name, "wan_rip2") && AnscEqualString(pValue, "true", FALSE) )
            {
                g_SetParamValueString("Device.Routing.RIP.InterfaceSetting.1.X_CISCO_COM_SendVersion", "RIP2");
                g_SetParamValueBool  ("Device.Routing.RIP.InterfaceSetting.1.SendRA", TRUE);
                g_SetParamValueBool  ("Device.Routing.RIP.InterfaceSetting.1.Enable", TRUE);
                g_SetParamValueBool  ("Device.Routing.RIP.Enable", TRUE);

                free_parameterValStruct(g_MessageBusHandle, 1, pVal);
                AnscFreeMemory(pStringToken);
                continue;
            }
            else 
            {
                pVal[0].parameterValue = AnscCloneString(pValue);
            }
            pVal[0].type = pMapping[0].Type;       
        }
        else
        {
            /* Special cases for remote_management_acl and website_block_schedule_time */
            char* pTmp = NULL;
            
            if ( AnscEqualString(pStringToken->Name, "remote_management_acl", FALSE) )
            {
                ret = 
                    CcspCcMbi_AddTblRow
                        (
                            0,
                            "Device.UserInterface.X_CISCO_COM_RemoteAccess.iprange.",
                            &InsNum,
                            NULL
                        );
                if ( ret != CCSP_SUCCESS )
                {
                    free_parameterValStruct(g_MessageBusHandle, 1, pVal);
                    AnscFreeMemory(pStringToken);
                    continue;
                }
                
                for ( i = 0; i < ulParamCount; i++ )
                {
                    pVal[i].parameterName = (char*)AnscAllocateMemory(256);
                    if ( !pVal[i].parameterName )
                    {
                        free_parameterValStruct(g_MessageBusHandle, 1, pVal);
                        AnscFreeMemory(pStringToken);
                        continue;
                    }
                    
                    _ansc_sprintf(pVal[i].parameterName, pMapping[i].FullPath, InsNum);
                    if ( pValue )
                    {
                        pTmp = _ansc_strstr(pValue, " ");
                        if ( pTmp )
                        {
                            *pTmp++ = '\0';
                        }
                    }
                    
                    if ( i == 2 )
                    {
                        pVal[i].parameterValue = AnscCloneString("TRUE_STATIC_IP");
                    }
                    else if ( i == 3 )
                    {
                        pVal[i].parameterValue = AnscCloneString("false");
                    }
                    else
                    {
                        pVal[i].parameterValue = AnscCloneString(pValue);
                    }

                    pValue = pTmp;
                    pVal[i].type = pMapping[i].Type;
                }
            }
            else if( AnscEqualString(pStringToken->Name, "website_block_schedule_time", FALSE) )
            {
                char AlwaysBlock[8] = {0};
                ULONG StartHour, StartMin, EndHour, EndMin = 0;
                char Start[3] = {0};
                char End[3] = {0};
                char StartTime[8] = {0};
                char EndTime[8] = {0};
                
                if ( iBlockedURLInsNum == 0 || CosaDmlTSIPTestFullPathName(pMyObject) == FALSE )
                {
                    ret = 
                        CcspCcMbi_AddTblRow
                            (
                                0,
                                "Device.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.",
                                &InsNum,
                                NULL
                            );
                    if ( ret != CCSP_SUCCESS )
                    {
                        free_parameterValStruct(g_MessageBusHandle, 1, pVal);
                        AnscFreeMemory(pStringToken);
                        continue;
                    }
                    else
                    {
                        iBlockedURLInsNum = InsNum;
                        _ansc_sprintf(FullName, "Device.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.%d.BlockMethod", iBlockedURLInsNum);
                        g_SetParamValueString(FullName, "URL");
                    }
                }
                
                if ( pValue )
                {
                    _ansc_sscanf
                        (
                            pValue,
                            "%s %d %d %s %d %d %s",
                            AlwaysBlock,
                            &StartHour,
                            &StartMin,
                            Start,
                            &EndHour,
                            &EndMin,
                            End
                        );
                        
                    if (AnscEqualString(Start, "pm", FALSE))
                    {
                        StartHour += 12;
                    }
                    
                    if (AnscEqualString(End,   "pm", FALSE))
                    {
                        EndHour += 12;
                    }
                    
                    printf("%02d\n", StartMin);
                    
                    _ansc_sprintf(StartTime, "%02d:%02d", StartHour, StartMin);
                    _ansc_sprintf(EndTime,   "%02d:%02d", EndHour,   EndMin);
                }
                
                printf("%s %02d %02d %s %02d %02d %s\n", AlwaysBlock, StartHour, StartMin, Start, EndHour, EndMin, End);
                for ( i = 0; i < ulParamCount; i++ )
                {
                    pVal[i].parameterName = (char*)AnscAllocateMemory(256);
                    if ( !pVal[i].parameterName )
                    {
                        free_parameterValStruct(g_MessageBusHandle, 1, pVal);
                        AnscFreeMemory(pStringToken);
                        continue;
                    }
                    _ansc_sprintf(pVal[i].parameterName, pMapping[i].FullPath, iBlockedURLInsNum);
                    printf("!!! pVal[%d].parameterName: %s !!!\n", i, pVal[i].parameterName);
                    pVal[i].type = pMapping[i].Type;
                }
                
                pVal[0].parameterValue = AnscCloneString(AlwaysBlock);
                pVal[1].parameterValue = AnscCloneString(StartTime);
                pVal[2].parameterValue = AnscCloneString(EndTime);
            }
        }
    
        /* We assume all the parameters are in PAM for now */
        ret = CcspCcMbi_SetParameterValues
                (
                    0,   /* SessionID */
                    0x0, /* WriteID */
                    pVal,
                    ulParamCount,
                    0,  /* bCommit = false */
                    &pFaultParamName,
                    NULL
                );

        free_parameterValStruct(g_MessageBusHandle, ulParamCount, pVal);
        AnscFreeMemory(pStringToken);

        if ( ret != CCSP_SUCCESS )
        {
            AnscTraceWarning(("CosaDmlTSIPApplyConfigFile: SetParameterValue Error, fault param name: %s\n", pFaultParamName));
            if ( pFaultParamName )
            {
                AnscFreeMemory(pFaultParamName);
                pFaultParamName = NULL;
            }
      
            continue;
        }
        else
        {
            bCommit = TRUE;
        }
    }

    if ( bCommit )
    {
        /* Call SetCommit to push the config to backend */
        ret = CcspCcMbi_SetCommit
                (
                    0,
                    0x0,
                    1,
                    NULL
                );

        if ( ret != CCSP_SUCCESS )
        {
            AnscTraceWarning(("CosaDmlTSIPApplyConfigFile: SetCommit Error!\n"));
            returnStatus = ANSC_STATUS_FAILURE;
        }
    }
    

EXIT:   
    if ( pConfigTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pConfigTokenChain);
    }
    
    if ( pBuffer )
    {
        AnscFreeMemory(pBuffer);
    }

    if ( pBuffer2 )
    {
        AnscFreeMemory(pBuffer2);
    }

    if (fpConfig)
    {
        fclose(fpConfig);
    }

    return returnStatus;
}

ANSC_STATUS
CosaDmlTSIPApplyConfigFile
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus           = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP      )hContext;

    AnscTraceWarning(("!!!!!!!!!! Creating thread to apply config !!!!!!!!!!\n"));

    returnStatus = 
        AnscSpawnTask
            (
                CosaDmlTSIPApplyConfigFileTask,
                (ANSC_HANDLE)pMyObject,
                "ConfigApplyTask"
            );

    return returnStatus;
}


ANSC_STATUS
CosaDmlTSIPSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_CFG          pCfg
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject       = (PCOSA_DATAMODEL_TSIP)hContext;
    int                             iReturnValue    = CCSP_SUCCESS;
    char                            pParamPath[64]  = {0};
    unsigned int                    RecordType      = ccsp_string;
    char                            RecordValue[64] = {0};

    if ( pCfg->bIPInfoChanged && (!AnscIsValidIpString(pCfg->IPAddress) || !AnscIsValidIpString(pCfg->SubnetMask) /*|| !AnscIsValidIpString(pCfg->GatewayIPAddress)*/) )
    {
        AnscTraceWarning(("Invalid argument for True Static IP!\n"));
        return ANSC_STATUS_FAILURE;
    }

    /* Send sysevent... */
    if ( pCfg->bIPInfoChanged )
    {
        AnscTraceWarning(("True Static IP info changed, apply...\n"));

#if  defined(_COSA_INTEL_USG_ARM_)

        if ( TRUE )
        {
            char                        scriptCmd[128];

            /* IP Address */
            _ansc_sprintf
                (
                    scriptCmd,
                    "sysevent set ipv4-tsip_IPAddress %s",
                    pCfg->IPAddress
                );

            AnscTraceWarning(("!!!!!!!!!! scriptCmd: %s !!!!!!!!!!\n", scriptCmd));
            system(scriptCmd);

            /* Subnet */
            _ansc_sprintf
                (
                    scriptCmd,
                    "sysevent set ipv4-tsip_Subnet %s",
                    pCfg->SubnetMask
                );

            AnscTraceWarning(("!!!!!!!!!! scriptCmd: %s !!!!!!!!!!\n", scriptCmd));
            system(scriptCmd);

            /* Gateway */
            _ansc_sprintf
                (
                    scriptCmd,
                    "sysevent set ipv4-tsip_Gateway %s",
                    pCfg->GatewayIPAddress
                );

            AnscTraceWarning(("!!!!!!!!!! scriptCmd: %s !!!!!!!!!!\n", scriptCmd));
            system(scriptCmd);

            _ansc_sprintf
                (
                    scriptCmd,
                    "sysevent set ipv4-resync_tsip %d",
                    pCfg->Enabled
                );

            AnscTraceWarning(("!!!!!!!!!! scriptCmd: %s !!!!!!!!!!\n", scriptCmd));
            system(scriptCmd);
        }
#endif

         /* Will change to Storing to PSM in Utopia script */
         sleep(3);

         if ( TRUE )     /* Enable */
         {
            _ansc_sprintf
                (
                    pParamPath,
                    DMSB_TR181_PSM_Tsip_Root DMSB_TR181_PSM_Tsip_Enable
                );

            RecordType = ccsp_boolean;
            _ansc_sprintf(RecordValue, "%d", pCfg->Enabled);

            iReturnValue =
                PSM_Set_Record_Value2
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        RecordType,
                        RecordValue
                    );

            if ( iReturnValue != CCSP_SUCCESS )
            {
                AnscTraceWarning
                    ((
                        "%s -- failed to set the parameter %s, error code %d, type %d, value %s.\n",
                        __FUNCTION__,
                        pParamPath,
                        iReturnValue,
                        RecordType,
                        RecordValue
                    ));
            }
        }

        if ( TRUE )     /* IPAddress */
        {
            _ansc_sprintf
                (
                    pParamPath,
                    DMSB_TR181_PSM_Tsip_Root DMSB_TR181_PSM_Tsip_IpAddress
                );

            RecordType = ccsp_string;
            AnscCopyString(RecordValue, pCfg->IPAddress);

            iReturnValue =
                PSM_Set_Record_Value2
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        RecordType,
                        RecordValue
                    );

            if ( iReturnValue != CCSP_SUCCESS )
            {
                AnscTraceWarning
                    ((
                        "%s -- failed to set the parameter %s, error code %d, type %d, value %s.\n",
                        __FUNCTION__,
                        pParamPath,
                        iReturnValue,
                        RecordType,
                        RecordValue
                    ));
            }
        }

        if ( TRUE )     /* Subnet Mask */
        {
            _ansc_sprintf
                (
                    pParamPath,
                    DMSB_TR181_PSM_Tsip_Root DMSB_TR181_PSM_Tsip_SubnetMask
                );

            RecordType = ccsp_string;
            AnscCopyString(RecordValue, pCfg->SubnetMask);

            iReturnValue =
                PSM_Set_Record_Value2
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        RecordType,
                        RecordValue
                    );

            if ( iReturnValue != CCSP_SUCCESS )
            {
                AnscTraceWarning
                    ((
                        "%s -- failed to set the parameter %s, error code %d, type %d, value %s.\n",
                        __FUNCTION__,
                        pParamPath,
                        iReturnValue,
                        RecordType,
                        RecordValue
                    ));
            }
        }

        if ( TRUE )     /* Gateway */
        {
            _ansc_sprintf
                (
                    pParamPath,
                    DMSB_TR181_PSM_Tsip_Root DMSB_TR181_PSM_Tsip_Gateway
                );

            RecordType = ccsp_string;
            AnscCopyString(RecordValue, pCfg->GatewayIPAddress);

            iReturnValue =
                PSM_Set_Record_Value2
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        RecordType,
                        RecordValue
                    );

            if ( iReturnValue != CCSP_SUCCESS )
            {
                AnscTraceWarning
                    ((
                        "%s -- failed to set the parameter %s, error code %d, type %d, value %s.\n",
                        __FUNCTION__,
                        pParamPath,
                        iReturnValue,
                        RecordType,
                        RecordValue
                    ));
            }
        }

        /* Need do sth about additional subnet if true static mode is turned off */
        if ( pCfg->Enabled == FALSE )
        {
            PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnetEntry           = (PCOSA_DML_TSIP_SUBNET_ENTRY)NULL;
            PSINGLE_LINK_ENTRY              pSLinkEntry            = (PSINGLE_LINK_ENTRY         )NULL;

            pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SubnetList);

            while ( pSLinkEntry )
            {
                pSubnetEntry    = ACCESS_DMSB_TR181_TSIP_ASN(pSLinkEntry);
                pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

                pSubnetEntry->Enabled = FALSE;

                CosaDmlTSIPSubnetSetEntry((ANSC_HANDLE)pMyObject, pSubnetEntry);
            }
        }

        TriggerOtherModule(hContext);
        pCfg->bIPInfoChanged = FALSE;
    }

    if ( TRUE )     /* Encrypt Key */
    {
        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Root DMSB_TR181_PSM_Tsip_EncryptKey
            );

        RecordType = ccsp_string;
        AnscCopyString(RecordValue, pCfg->ConfigEncryptKey);

        iReturnValue =
            PSM_Set_Record_Value2
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    RecordType,
                    RecordValue
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning
                ((
                    "%s -- failed to set the parameter %s, error code %d, type %d, value %s.\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType,
                    RecordValue
                ));
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_CFG          pCfg
    )
{
    int                             iReturnValue    = CCSP_SUCCESS;
    char                            pParamPath[64]  = {0};
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};

    /* Fetch Cfg */

    if ( TRUE )     /* Enable */
    {
        SlapInitVariable(&SlapValue);

        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Root DMSB_TR181_PSM_Tsip_Enable
            );

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_boolean))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            pCfg->Enabled = !!(SlapValue.Variant.varBool);
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( TRUE )     /* IPAddress */
    {
        SlapInitVariable(&SlapValue);

        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Root DMSB_TR181_PSM_Tsip_IpAddress
            );

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            AnscCopyString(pCfg->IPAddress, SlapValue.Variant.varString);
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( TRUE )     /* Subnet Mask */
    {
        SlapInitVariable(&SlapValue);

        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Root DMSB_TR181_PSM_Tsip_SubnetMask
            );

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            AnscCopyString(pCfg->SubnetMask, SlapValue.Variant.varString);
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( TRUE )     /* Gateway */
    {
        SlapInitVariable(&SlapValue);

        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Root DMSB_TR181_PSM_Tsip_Gateway
            );

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            AnscCopyString(pCfg->GatewayIPAddress, SlapValue.Variant.varString);
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( TRUE )     /* Encrypt Key */
    {
        SlapInitVariable(&SlapValue);

        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Root DMSB_TR181_PSM_Tsip_EncryptKey
            );

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            AnscCopyString(pCfg->ConfigEncryptKey, SlapValue.Variant.varString);
        }

        SlapCleanVariable(&SlapValue);
    }

    return ANSC_STATUS_SUCCESS;
}

/* Additional subnet */

#if 0
PCOSA_DML_TSIP_SUBNET_ENTRY   
CosaDmlAdditionalSubnetFindByInstNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstNum
    )
{
    ANSC_STATUS                     returnStatus           = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP       )hContext;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnetEntry           = (PCOSA_DML_TSIP_SUBNET_ENTRY)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry            = (PSINGLE_LINK_ENTRY         )NULL;

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SubnetList);

    while ( pSLinkEntry )
    {
        pSubnetEntry    = ACCESS_DMSB_TR181_TSIP_ASN(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pSubnetEntry->InstanceNumber == ulInstNum )
        {
            return  pSubnetEntry;
        }
    }

    AnscTraceWarning(("%s -- failed to find %d!\n", __FUNCTION__, ulInstNum));

    return  NULL;
}
#endif

ANSC_STATUS
CosaDmlAdditionalSubnetLoadPsm
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus           = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP       )hContext;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnetEntry           = (PCOSA_DML_TSIP_SUBNET_ENTRY)NULL;
    char                            pParamPath[64]         = {0};
    unsigned int                    RecordType             = 0;
    SLAP_VARIABLE                   SlapValue              = {0};
    int                             iReturnValue           = CCSP_SUCCESS;
    int                             iNumInst               = 0;
    int*                            pInstArray             = NULL;
    ULONG                           ulIndex                = 0;

    iReturnValue = 
        PsmGetNextLevelInstances
            (
                g_MessageBusHandle,
                g_SubsystemPrefix,
                DMSB_TR181_PSM_Tsip_Asn_Root,
                &iNumInst,
                &pInstArray
            );

    if ( iReturnValue != CCSP_SUCCESS )
    {
        AnscTraceWarning(("%s -- PsmGetNextLevelInstances failed, error code = %d!\n", __FUNCTION__, iReturnValue));
        goto  EXIT;
    }

    for ( ulIndex = 0; ulIndex < iNumInst; ulIndex++ )
    {
        pSubnetEntry = (PCOSA_DML_TSIP_SUBNET_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_TSIP_SUBNET_ENTRY));

        if ( !pSubnetEntry )
        {
            AnscTraceWarning(("%s -- insufficient resources, Subnet instance %d\n", __FUNCTION__, ulIndex));
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pSubnetEntry->InstanceNumber = pInstArray[ulIndex];
            if ( pSubnetEntry->InstanceNumber >= pMyObject->ulSubnetNextInsNum )
            {
                pMyObject->ulSubnetNextInsNum = pSubnetEntry->InstanceNumber + 1;
            }
        }

        /* Fetch Cfg */

        if ( TRUE )     /* Enable */
        {
            SlapInitVariable(&SlapValue);

            _ansc_sprintf
                (
                    pParamPath,
                    DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_Enable,
                    pInstArray[ulIndex]
                );

            iReturnValue =
                PSM_Get_Record_Value
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        &RecordType,
                        &SlapValue
                    );

            if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_boolean))
            {
                AnscTraceWarning
                    ((
                        "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                        __FUNCTION__,
                        pParamPath,
                        iReturnValue,
                        RecordType
                    ));
            }
            else
            {
                pSubnetEntry->Enabled = !!(SlapValue.Variant.varBool);
            }

            SlapCleanVariable(&SlapValue);
        }

        if ( TRUE )     /* Alias */
        {
            SlapInitVariable(&SlapValue);

            _ansc_sprintf
                (
                    pParamPath,
                    DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_Alias,
                    pInstArray[ulIndex]
                );

            iReturnValue =
                PSM_Get_Record_Value
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        &RecordType,
                        &SlapValue
                    );

            if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
            {
                AnscTraceWarning
                    ((
                        "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                        __FUNCTION__,
                        pParamPath,
                        iReturnValue,
                        RecordType
                    ));
            }
            else
            {
                _ansc_strcpy(pSubnetEntry->Alias, SlapValue.Variant.varString);
            }

            SlapCleanVariable(&SlapValue);
        }

        if ( TRUE )     /* Ipaddress */
        {
            SlapInitVariable(&SlapValue);

            _ansc_sprintf
                (
                    pParamPath,
                    DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_IpAddress,
                    pInstArray[ulIndex]
                );

            iReturnValue =
                PSM_Get_Record_Value
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        &RecordType,
                        &SlapValue
                    );

            if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
            {
                AnscTraceWarning
                    ((
                        "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                        __FUNCTION__,
                        pParamPath,
                        iReturnValue,
                        RecordType
                    ));
            }
            else
            {
                _ansc_strcpy(pSubnetEntry->IPAddress, SlapValue.Variant.varString);
            }

            SlapCleanVariable(&SlapValue);
        }

        if ( TRUE )     /* Subnet mask */
        {
            SlapInitVariable(&SlapValue);

            _ansc_sprintf
                (
                    pParamPath,
                    DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_SubnetMask,
                    pInstArray[ulIndex]
                );

            iReturnValue =
                PSM_Get_Record_Value
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        &RecordType,
                        &SlapValue
                    );

            if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
            {
                AnscTraceWarning
                    ((
                        "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                        __FUNCTION__,
                        pParamPath,
                        iReturnValue,
                        RecordType
                    ));
            }
            else
            {
                _ansc_strcpy(pSubnetEntry->SubnetMask, SlapValue.Variant.varString);
            }

            SlapCleanVariable(&SlapValue);
        }

        AnscSListPushEntryAtBack(&pMyObject->SubnetList, &pSubnetEntry->Linkage);
    }

EXIT:

    if ( pInstArray )
    {
        AnscFreeMemory(pInstArray);
        pInstArray = NULL;
    }

    return  iReturnValue;
}

ANSC_STATUS
CosaDmlAdditionalSubnetSavePsm
    (
        ANSC_HANDLE                 hContext,
        ANSC_HANDLE                 hSubnetEntry
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP       )hContext;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnetEntry           = (PCOSA_DML_TSIP_SUBNET_ENTRY)hSubnetEntry;
    int                             iReturnValue           = CCSP_SUCCESS;
    char                            pParamPath[64]         = {0};
    unsigned int                    RecordType             = ccsp_string;
    char                            RecordValue[64]        = {0};

    if ( !pSubnetEntry )
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    if ( TRUE )     /* Enable */
    {
        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_Enable,
                pSubnetEntry->InstanceNumber
            );

        RecordType = ccsp_boolean;
        _ansc_strcpy(RecordValue, pSubnetEntry->Enabled ? PSM_TRUE : PSM_FALSE);

        iReturnValue =
            PSM_Set_Record_Value2
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    RecordType,
                    RecordValue
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning
                ((
                    "%s -- failed to set the parameter %s, error code %d, type %d, value %s.\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType,
                    RecordValue
                ));
        }
    }

    if ( TRUE )     /* Alias */
    {
        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_Alias,
                pSubnetEntry->InstanceNumber
            );

        RecordType = ccsp_string;
        _ansc_strcpy(RecordValue, pSubnetEntry->Alias);

        iReturnValue =
            PSM_Set_Record_Value2
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    RecordType,
                    RecordValue
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning
                ((
                    "%s -- failed to set the parameter %s, error code %d, type %d, value %s.\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType,
                    RecordValue
                ));
        }
    }

    if ( TRUE )     /* Ipaddress */
    {
        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_IpAddress,
                pSubnetEntry->InstanceNumber
            );

        RecordType = ccsp_string;
        _ansc_strcpy(RecordValue, pSubnetEntry->IPAddress);

        iReturnValue =
            PSM_Set_Record_Value2
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    RecordType,
                    RecordValue
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning
                ((
                    "%s -- failed to set the parameter %s, error code %d, type %d, value %s.\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType,
                    RecordValue
                ));
        }
    }

    if ( TRUE )     /* Subnet mask */
    {
        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_SubnetMask,
                pSubnetEntry->InstanceNumber
            );

        RecordType = ccsp_string;
        _ansc_strcpy(RecordValue, pSubnetEntry->SubnetMask);

        iReturnValue =
            PSM_Set_Record_Value2
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    RecordType,
                    RecordValue
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning
                ((
                    "%s -- failed to set the parameter %s, error code %d, type %d, value %s.\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType,
                    RecordValue
                ));
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlAdditionalSubnetDelPsm
    (
        ANSC_HANDLE                 hContext,
        ANSC_HANDLE                 hSubnetEntry
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP       )hContext;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnetEntry           = (PCOSA_DML_TSIP_SUBNET_ENTRY)hSubnetEntry;
    int                             iReturnValue           = CCSP_SUCCESS;
    char                            pParamPath[64]         = {0};

    if ( !pSubnetEntry )
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    if ( TRUE )
    {
        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i,
                pSubnetEntry->InstanceNumber
            );

        iReturnValue =
            PSM_Del_Record
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning(("%s -- failed to delete PSM records, error code %d", __FUNCTION__, iReturnValue));
        }
    }

    return ANSC_STATUS_SUCCESS;
}

#if 0
ULONG
CosaDmlTSIPSubnetGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    return 0;
}

ANSC_STATUS
CosaDmlTSIPSubnetGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_TSIP_SUBNET_ENTRY pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPSubnetSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPSubnetAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_SUBNET_ENTRY pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}
#endif

ANSC_STATUS
CosaDmlTSIPSubnetDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_SUBNET_ENTRY pEntry
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP       )hContext;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnetEntry           = (PCOSA_DML_TSIP_SUBNET_ENTRY)pEntry;

    if ( !pSubnetEntry )
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    /* Send sysevent */

#if  defined(_COSA_INTEL_USG_ARM_)

    if ( TRUE )
    {
        char                        scriptCmd[128];

        system("sysevent set ipv4-tsip_asn_enable 0");

        _ansc_sprintf
            (
                scriptCmd,
                "sysevent set ipv4-resync_tsip_asn %d",
                pSubnetEntry->InstanceNumber
            );

        AnscTraceWarning(("!!!!!!!!!! scriptCmd: %s !!!!!!!!!!\n", scriptCmd));
        system(scriptCmd);
    }
            
#endif

    /* Will change to Storing to PSM in Utopia script */
    sleep(3);

    CosaDmlAdditionalSubnetDelPsm((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSubnetEntry);

    TriggerOtherModule(hContext);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPSubnetSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_SUBNET_ENTRY pEntry
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP       )hContext;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pSubnetEntry           = (PCOSA_DML_TSIP_SUBNET_ENTRY)pEntry;

    if ( !pSubnetEntry || !AnscIsValidIpString(pSubnetEntry->IPAddress) || !AnscIsValidIpString(pSubnetEntry->SubnetMask) )
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    /* Send sysevent */

#if  defined(_COSA_INTEL_USG_ARM_)

    if ( TRUE )
    {
        char                        scriptCmd[128];

        /* Enable */
        _ansc_sprintf
            (
                scriptCmd,
                "sysevent set ipv4-tsip_asn_enable %d",
                pSubnetEntry->Enabled
            );

        AnscTraceWarning(("!!!!!!!!!! scriptCmd: %s !!!!!!!!!!\n", scriptCmd));
        system(scriptCmd);

        /* IP Address */
        _ansc_sprintf
            (
                scriptCmd,
                "sysevent set ipv4-tsip_asn_ipaddress %s",
                pSubnetEntry->IPAddress
            );

        AnscTraceWarning(("!!!!!!!!!! scriptCmd: %s !!!!!!!!!!\n", scriptCmd));
        system(scriptCmd);

        /* Subnet */
        _ansc_sprintf
            (
                scriptCmd,
                "sysevent set ipv4-tsip_asn_subnet %s",
                pSubnetEntry->SubnetMask
            );

        AnscTraceWarning(("!!!!!!!!!! scriptCmd: %s !!!!!!!!!!\n", scriptCmd));
        system(scriptCmd);

        _ansc_sprintf
            (
                scriptCmd,
                "sysevent set ipv4-resync_tsip_asn %d",
                pSubnetEntry->InstanceNumber
            );

        AnscTraceWarning(("!!!!!!!!!! scriptCmd: %s !!!!!!!!!!\n", scriptCmd));
        system(scriptCmd);
    }
            
#endif

    /* Will change to Storing to PSM in Utopia script */
    sleep(3);

    CosaDmlAdditionalSubnetSavePsm((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSubnetEntry);

    TriggerOtherModule(hContext);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPSubnetGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_SUBNET_ENTRY pEntry
    )
{
    ANSC_STATUS                     returnStatus           = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP       )hContext;
    SLAP_VARIABLE                   SlapValue              = {0};
    int                             iReturnValue           = CCSP_SUCCESS;
    char                            pParamPath[64]         = {0};
    unsigned int                    RecordType             = ccsp_string;
    char                            RecordValue[64]        = {0};

    if ( !pEntry )
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    /* Fetch Cfg */

    if ( TRUE )     /* Enable */
    {
        SlapInitVariable(&SlapValue);

        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_Enable,
                pEntry->InstanceNumber
            );

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_boolean))
        {
            AnscTraceWarning
               ((
                    "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            pEntry->Enabled = !!(SlapValue.Variant.varBool);
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( TRUE )     /* Alias */
    {
        SlapInitVariable(&SlapValue);

        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_Alias,
                pEntry->InstanceNumber
            );

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            _ansc_strcpy(pEntry->Alias, SlapValue.Variant.varString);
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( TRUE )     /* Ipaddress */
    {
        SlapInitVariable(&SlapValue);

        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_IpAddress,
                pEntry->InstanceNumber
            );

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            _ansc_strcpy(pEntry->IPAddress, SlapValue.Variant.varString);
        }

        SlapCleanVariable(&SlapValue);
    }

    if ( TRUE )     /* Subnet mask */
    {
        SlapInitVariable(&SlapValue);

        _ansc_sprintf
            (
                pParamPath,
                DMSB_TR181_PSM_Tsip_Asn_Root DMSB_TR181_PSM_Tsip_Asn_i DMSB_TR181_PSM_Tsip_Asn_SubnetMask,
                pEntry->InstanceNumber
            );

        iReturnValue =
            PSM_Get_Record_Value
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    &RecordType,
                    &SlapValue
                );

        if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
        {
            AnscTraceWarning
                ((
                    "%s -- failed to retrieve the parameter %s, error code %d, type %d\n",
                    __FUNCTION__,
                    pParamPath,
                    iReturnValue,
                    RecordType
                ));
        }
        else
        {
            _ansc_strcpy(pEntry->SubnetMask, SlapValue.Variant.varString);
        }

        SlapCleanVariable(&SlapValue);
    }

    return ANSC_STATUS_SUCCESS;
}

#define PT_MGMT_PREFIX "tsip_pm_"
unsigned long pmCount = 0;

ANSC_STATUS
CosaDmlTSIPPortManagementSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg
    )
{
    UtopiaContext ctx;
    unsigned long ins;
    char utVal[64];

    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    pCfg->Enabled ? strcpy(utVal, "1") : strcpy(utVal, "0");
    Utopia_RawSet(&ctx, NULL, PT_MGMT_PREFIX"enabled", utVal);

    pCfg->RuleType == COSA_DML_TSIP_RULE_White ? strcpy(utVal, "white") : strcpy(utVal, "black");
    Utopia_RawSet(&ctx, NULL, PT_MGMT_PREFIX"type", utVal);

    Utopia_Free(&ctx, 1);

    commonSyseventSet("firewall-restart", "");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPPortManagementGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg
    )
{
    UtopiaContext ctx;
    unsigned long ins;
    char utVal[64] = {0};
    int rc;

    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    Utopia_RawGet(&ctx, NULL, PT_MGMT_PREFIX"enabled", utVal, sizeof(utVal));
    pCfg->Enabled = (utVal[0] == 0 ? true : strcmp("1", utVal) == 0);

    Utopia_RawGet(&ctx, NULL, PT_MGMT_PREFIX"type", utVal, sizeof(utVal));
    pCfg->RuleType = (utVal[0] == 0 || strcmp("white", utVal) == 0) ? COSA_DML_TSIP_RULE_White : COSA_DML_TSIP_RULE_Black;

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlTSIPRuleGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    UtopiaContext ctx;
    char countStr[16] = {0};

    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_RawGet(&ctx, NULL, PT_MGMT_PREFIX"count", countStr, sizeof(countStr));
    pmCount = strtoul(countStr, NULL, 10);
    Utopia_Free(&ctx, 0);

    return pmCount;
}

ANSC_STATUS
CosaDmlTSIPRuleGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_TSIP_RULE_ENTRY   pEntry
    )
{
    UtopiaContext ctx;
    unsigned long ins;
    char utVal[64];
    char utKey[64];

    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_ins", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    pEntry->InstanceNumber = strtoul(utVal, NULL, 10);
    
    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_alias", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    strncpy(pEntry->Alias, utVal, sizeof(pEntry->Alias));

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_enabled", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    pEntry->Enabled = (strcmp("1", utVal) == 0);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_name", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    strncpy(pEntry->Name, utVal, sizeof(pEntry->Name));

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_protocol", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    
    if(strcmp("tcp", utVal) == 0)
        pEntry->Protocol = COSA_DML_TSIP_RULE_PROTOCOL_TCP;
    if(strcmp("udp", utVal) == 0)
        pEntry->Protocol = COSA_DML_TSIP_RULE_PROTOCOL_UDP;
    if(strcmp("both", utVal) == 0)
        pEntry->Protocol = COSA_DML_TSIP_RULE_PROTOCOL_BOTH;

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_startIP", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    strncpy(pEntry->IPRangeMin, utVal, sizeof(pEntry->IPRangeMin));
    
    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_endIP", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    strncpy(pEntry->IPRangeMax, utVal, sizeof(pEntry->IPRangeMax));
    
    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_startPort", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    pEntry->PortRangeMin = strtoul(utVal, NULL, 10);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_endPort", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    pEntry->PortRangeMax = strtoul(utVal, NULL, 10);

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPRuleSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UtopiaContext ctx;
    char utVal[64];
    char utKey[64];

    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_ins", ulIndex);
    snprintf(utVal, sizeof(utVal), "%u", ulInstanceNumber);
    Utopia_RawSet(&ctx, NULL, utKey, utVal);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_alias", ulIndex);
    Utopia_RawSet(&ctx, NULL, utKey, pAlias);

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPRuleAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_RULE_ENTRY   pEntry
    )
{
    UtopiaContext ctx;
    char utVal[64];
    char utKey[64];

    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    pmCount++;

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"count");
    snprintf(utVal, sizeof(utVal), "%u", pmCount);
    Utopia_RawSet(&ctx, NULL, utKey, utVal);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_ins", pmCount-1);
    snprintf(utVal, sizeof(utVal), "%u", pEntry->InstanceNumber);
    Utopia_RawSet(&ctx, NULL, utKey, utVal);
    Utopia_Free(&ctx, 1);

    CosaDmlTSIPRuleSetEntry(hContext, pEntry);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPRuleDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_RULE_ENTRY   pEntry
    )
{
    UtopiaContext ctx;
    char utVal[64];
    char utKey[64];
    unsigned long i;

    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    for(i = 0; i < pmCount; i++) {
        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_ins", i);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        if(pEntry->InstanceNumber == strtoul(utVal, NULL, 10))
            break;
    }

    for(; i+1 < pmCount; i++) {

        //get elements
        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_ins", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        pEntry->InstanceNumber = strtoul(utVal, NULL, 10);

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_alias", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        strncpy(pEntry->Alias, utVal, sizeof(pEntry->Alias));

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_enabled", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        pEntry->Enabled = (strcmp("1", utVal) == 0);

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_name", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        strncpy(pEntry->Name, utVal, sizeof(pEntry->Name));

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_protocol", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));

        if(strcmp("tcp", utVal) == 0)
            pEntry->Protocol = COSA_DML_TSIP_RULE_PROTOCOL_TCP;
        if(strcmp("udp", utVal) == 0)
            pEntry->Protocol = COSA_DML_TSIP_RULE_PROTOCOL_UDP;
        if(strcmp("both", utVal) == 0)
            pEntry->Protocol = COSA_DML_TSIP_RULE_PROTOCOL_BOTH;

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_startIP", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        strncpy(pEntry->IPRangeMin, utVal, sizeof(pEntry->IPRangeMin));

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_endIP", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        strncpy(pEntry->IPRangeMax, utVal, sizeof(pEntry->IPRangeMax));

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_startPort", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        pEntry->PortRangeMin = strtoul(utVal, NULL, 10);

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_endPort", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        pEntry->PortRangeMax = strtoul(utVal, NULL, 10);

        //move elements
        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_ins", i);
        snprintf(utVal, sizeof(utVal), "%u", pEntry->InstanceNumber);
        Utopia_RawSet(&ctx, NULL, utKey, utVal);

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_alias", i);
        Utopia_RawSet(&ctx, NULL, utKey, pEntry->Alias);

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_enabled", i);
        Utopia_RawSet(&ctx, NULL, utKey, pEntry->Enabled ? "1" : "0");

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_name", i);
        Utopia_RawSet(&ctx, NULL, utKey, pEntry->Name);

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_protocol", i);
        switch(pEntry->Protocol) {
            case COSA_DML_TSIP_RULE_PROTOCOL_TCP:
                strcpy(utVal, "tcp");
                break;
            case COSA_DML_TSIP_RULE_PROTOCOL_UDP:
                strcpy(utVal, "udp");
                break;
            case COSA_DML_TSIP_RULE_PROTOCOL_BOTH:
                strcpy(utVal, "both");
                break;
        }
        Utopia_RawSet(&ctx, NULL, utKey, utVal);

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_startIP", i);
        Utopia_RawSet(&ctx, NULL, utKey, pEntry->IPRangeMin);

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_endIP", i);
        Utopia_RawSet(&ctx, NULL, utKey, pEntry->IPRangeMax);

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_startPort", i);
        snprintf(utVal, sizeof(utVal), "%u", pEntry->PortRangeMin);
        Utopia_RawSet(&ctx, NULL, utKey, utVal);

        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_endPort", i);
        snprintf(utVal, sizeof(utVal), "%u", pEntry->PortRangeMax);
        Utopia_RawSet(&ctx, NULL, utKey, utVal);
    }

    //delete last element
    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_ins", pmCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_alias", pmCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_enabled", pmCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_name", pmCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_protocol", pmCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_startIP", pmCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);
    
    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_endIP", pmCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);
    
    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_startPort", pmCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);
    
    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_endPort", pmCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);

    //update counter
    pmCount--;
    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"count");
    snprintf(utVal, sizeof(utVal), "%u", pmCount);
    Utopia_RawSet(&ctx, NULL, utKey, utVal);

    Utopia_Free(&ctx, 1);

    commonSyseventSet("firewall-restart", "");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPRuleSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_RULE_ENTRY   pEntry
    )
{
    UtopiaContext ctx;
    unsigned long i;
    char utVal[64];
    char utKey[64];

    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    for(i = 0; i < pmCount; i++) {
        snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_ins", i);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        if(pEntry->InstanceNumber == strtoul(utVal, NULL, 10))
            break;
    }
    
    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_alias", i);
    Utopia_RawSet(&ctx, NULL, utKey, pEntry->Alias);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_enabled", i);
    Utopia_RawSet(&ctx, NULL, utKey, pEntry->Enabled ? "1" : "0");

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_name", i);
    Utopia_RawSet(&ctx, NULL, utKey, pEntry->Name);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_protocol", i);
    switch(pEntry->Protocol) {
        case COSA_DML_TSIP_RULE_PROTOCOL_TCP:
            strcpy(utVal, "tcp");
            break;
        case COSA_DML_TSIP_RULE_PROTOCOL_UDP:
            strcpy(utVal, "udp");
            break;
        case COSA_DML_TSIP_RULE_PROTOCOL_BOTH:
            strcpy(utVal, "both");
            break;
    }
    Utopia_RawSet(&ctx, NULL, utKey, utVal);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_startIP", i);
    Utopia_RawSet(&ctx, NULL, utKey, pEntry->IPRangeMin);
    
    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_endIP", i);
    Utopia_RawSet(&ctx, NULL, utKey, pEntry->IPRangeMax);
    
    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_startPort", i);
    snprintf(utVal, sizeof(utVal), "%u", pEntry->PortRangeMin);
    Utopia_RawSet(&ctx, NULL, utKey, utVal);

    snprintf(utKey, sizeof(utKey), PT_MGMT_PREFIX"%u_endPort", i);
    snprintf(utVal, sizeof(utVal), "%u", pEntry->PortRangeMax);
    Utopia_RawSet(&ctx, NULL, utKey, utVal);

    Utopia_Free(&ctx, 1);

    commonSyseventSet("firewall-restart", "");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTSIPRuleGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_RULE_ENTRY   pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}
