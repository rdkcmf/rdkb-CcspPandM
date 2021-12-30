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

    module: cosa_x_cisco_com_security_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the internal apis for objects to support Data Model Library.

        *   CosaSecurityCreate
        *   CosaSecurityInitialize
        *   CosaSecurityRemove
        *   CosaSecurityIARegGetInfo
        *   CosaSecurityIARegSetInfo

    -------------------------------------------------------------------


    author:

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "cosa_x_cisco_com_security_internal.h"
#include "safec_lib_common.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaSecurityCreate
            (
                VOID
            );

    description:

        This function constructs cosa Security object and return handle.

    argument:  

    return:     newly created Security object.

**********************************************************************/

ANSC_HANDLE
CosaSecurityCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_SECURITY        pMyObject    = (PCOSA_DATAMODEL_SECURITY)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_SECURITY)AnscAllocateMemory(sizeof(COSA_DATAMODEL_SECURITY));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_SECURITY_OID;
    pMyObject->Create            = CosaSecurityCreate;
    pMyObject->Remove            = CosaSecurityRemove;
    pMyObject->Initialize        = CosaSecurityInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        CosaSecurityInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Security object and return handle.

    argument:	ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaSecurityInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_SECURITY        pMyObject           = (PCOSA_DATAMODEL_SECURITY   )hThisObject;    
    PCOSA_DML_FIREWALL_CFG2         pFirewallCfg        = (PCOSA_DML_FIREWALL_CFG2    )NULL;
    PSLAP_VARIABLE                  pSlapVariable       = (PSLAP_VARIABLE             )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA     = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIA       = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIAHA     = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNextIns  = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext        = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2       = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_DML_IA_POLICY2            pDmlIAPolicy        = (PCOSA_DML_IA_POLICY2       )NULL;
    PCOSA_DML_IA_POLICY_URL         pURL                = (PCOSA_DML_IA_POLICY_URL    )NULL;
    PCOSA_DML_IA_POLICY_KEYWORD     pKeyword            = (PCOSA_DML_IA_POLICY_KEYWORD)NULL;
    PCOSA_DML_IA_POLICY_APP         pApp                = (PCOSA_DML_IA_POLICY_APP    )NULL;
    ULONG                           ulPolicyCount       = 0;
    ULONG                           ulSubCount          = 0;
    ULONG                           ulIndex             = 0;
    ULONG                           ulSubIndex          = 0;
    char                            FolderName[32]      = {0};
    errno_t                         rc                  = -1;

    returnStatus = CosaDmlIaInit(NULL, NULL);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return returnStatus;
    }

    /* Initiation all functions */
    pMyObject->ulNextAPInstanceNum  = 1;
    pMyObject->ulNextLogInstance = 1;

    AnscSListInitializeHeader(&pMyObject->AccessPolicyList);
    AnscSListInitializeHeader(&pMyObject->LogList);

    pMyObject->pLog = NULL;
    pMyObject->ulLogEntryCount = 0;

    /*Get SecuritConfig */
    CosaDmlSecurityGetConfig(NULL, &pMyObject->SecurityConfig);

    pFirewallCfg = AnscAllocateMemory(sizeof(COSA_DML_FIREWALL_CFG2));

    if ( !pFirewallCfg )
    {
        CcspTraceWarning(("X_CISCO_COM_SECURITY Resource allocation error\n"));

        return ANSC_STATUS_RESOURCES;
    }

    returnStatus = CosaDmlFirewallGetConfig2(NULL, pFirewallCfg);

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        pMyObject->FirewallConfig.FirewallLevel = pFirewallCfg->FirewallLevel;
        pMyObject->FirewallConfig.FirewallLevelV6 = pFirewallCfg->FirewallLevelV6;
        pMyObject->FirewallConfig.FilterAnonymousInternetRequests = pFirewallCfg->FilterAnonymousInternetRequests;
        pMyObject->FirewallConfig.FilterAnonymousInternetRequestsV6 = pFirewallCfg->FilterAnonymousInternetRequestsV6;
        pMyObject->FirewallConfig.FilterIdent = pFirewallCfg->FilterIdent;
        pMyObject->FirewallConfig.FilterIdentV6 = pFirewallCfg->FilterIdentV6;
        pMyObject->FirewallConfig.FilterMulticast = pFirewallCfg->FilterMulticast;
        pMyObject->FirewallConfig.FilterMulticastV6 = pFirewallCfg->FilterMulticastV6;
        pMyObject->FirewallConfig.FilterP2P = pFirewallCfg->FilterP2P;
        pMyObject->FirewallConfig.FilterP2PV6 = pFirewallCfg->FilterP2PV6;
        pMyObject->FirewallConfig.FilterHTTP = pFirewallCfg->FilterHTTP;
        pMyObject->FirewallConfig.FilterHTTPV6 = pFirewallCfg->FilterHTTPV6;
        pMyObject->FirewallConfig.FilterHTTPs = pFirewallCfg->FilterHTTPs;
        pMyObject->FirewallConfig.FilterHTTPsV6 = pFirewallCfg->FilterHTTPsV6;
        pMyObject->FirewallConfig.FilterMulticast = pFirewallCfg->FilterMulticast;
        pMyObject->FirewallConfig.FilterNATRedirection = pFirewallCfg->FilterNATRedirection;
        pMyObject->FirewallConfig.IPSecPassthrough = pFirewallCfg->IPSecPassthrough;
        pMyObject->FirewallConfig.L2TPPassthrough = pFirewallCfg->L2TPPassthrough;
        pMyObject->FirewallConfig.PPTPPassthrough = pFirewallCfg->PPTPPassthrough;
        pMyObject->FirewallConfig.SSLPassthrough = pFirewallCfg->SSLPassthrough;
        pMyObject->FirewallConfig.WebBlockActiveX = pFirewallCfg->WebBlockActiveX;
        pMyObject->FirewallConfig.WebBlockCookies = pFirewallCfg->WebBlockCookies;
        pMyObject->FirewallConfig.WebBlockJava = pFirewallCfg->WebBlockJava;
        pMyObject->FirewallConfig.WebBlockProxy = pFirewallCfg->WebBlockProxy;
        pMyObject->FirewallConfig.TrueStaticIpEnable = pFirewallCfg->TrueStaticIpEnable;
        pMyObject->FirewallConfig.TrueStaticIpEnableV6 = pFirewallCfg->TrueStaticIpEnableV6;
        pMyObject->FirewallConfig.SmartPktDectionEnable = pFirewallCfg->SmartPktDectionEnable;
        pMyObject->FirewallConfig.SmartPktDectionEnableV6 = pFirewallCfg->SmartPktDectionEnableV6;
        pMyObject->FirewallConfig.WanPingEnable = pFirewallCfg->WanPingEnable;
        pMyObject->FirewallConfig.WanPingEnableV6 = pFirewallCfg->WanPingEnableV6;
    }
    else
    {
        goto EXIT;
    }

    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;

    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }

    pPoamIrepFoIA = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_IA
            );

    if ( !pPoamIrepFoIA )
    {
        pPoamIrepFoIA =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_IA,
                    0
                );
    }

    if ( !pPoamIrepFoIA )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderIA = (ANSC_HANDLE)pPoamIrepFoIA;
    }

    pPoamIrepFoIAHA = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_IAHA
            );

    if ( !pPoamIrepFoIAHA )
    {
        pPoamIrepFoIAHA =
            pPoamIrepFoIA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoIA,
                    COSA_IREP_FOLDER_NAME_IAHA,
                    0
                );
    }

    if ( !pPoamIrepFoIAHA )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderIAHA = (ANSC_HANDLE)pPoamIrepFoIAHA;
    }

    /* Retrieve the next instance number for Internet Access Policy */

    if ( TRUE )
    {
        rc = sprintf_s
                (
                    FolderName,
                    sizeof(FolderName),
                    "%s%d", 
                    COSA_DML_RR_NAME_IA_NextInsNunmber, 
                    0
                );
        if(rc < EOK)
        {
          ERR_CHK(rc);
          returnStatus = ANSC_STATUS_FAILURE;
          goto  EXIT;
        }

        pPoamIrepFoNextIns = 
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIA->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoIA,
                    FolderName
                );

        if ( pPoamIrepFoNextIns )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoNextIns,
                        COSA_DML_RR_NAME_IA_NextInsNunmber,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pMyObject->ulNextAPInstanceNum = pSlapVariable->Variant.varUint32;

                SlapFreeVariable(pSlapVariable);
            }

            pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
            pPoamIrepFoNextIns = NULL;
        }
    }

    /* Initialize middle layer for Internet Access Policy */

    ulPolicyCount = CosaDmlIaGetNumberOfPolicies(NULL);

    for ( ulIndex = 0; ulIndex < ulPolicyCount; ulIndex++ )
    {
        pDmlIAPolicy = (PCOSA_DML_IA_POLICY2)AnscAllocateMemory(sizeof(COSA_DML_IA_POLICY2));

        if ( !pDmlIAPolicy )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT;
        }

        AnscSListInitializeHeader(&pDmlIAPolicy->URLList    );
        AnscSListInitializeHeader(&pDmlIAPolicy->KeywordList);
        AnscSListInitializeHeader(&pDmlIAPolicy->AppList    );

        pDmlIAPolicy->ulNextURLInsNum = 1;
        pDmlIAPolicy->ulNextKeyInsNum = 1;
        pDmlIAPolicy->ulNextAppInsNum = 1;
      
        returnStatus = CosaDmlIaGetPolicy(NULL, ulIndex, (PCOSA_DML_IA_POLICY)pDmlIAPolicy);

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                AnscFreeMemory(pDmlIAPolicy);

                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT;
            }

            if ( pDmlIAPolicy->InstanceNumber != 0 )
            {
                pCosaContext->InstanceNumber = pDmlIAPolicy->InstanceNumber;

                if ( pMyObject->ulNextAPInstanceNum <= pDmlIAPolicy->InstanceNumber )
                {
                    pMyObject->ulNextAPInstanceNum = pDmlIAPolicy->InstanceNumber + 1;

                    if ( pMyObject->ulNextAPInstanceNum == 0 )
                    {
                        pMyObject->ulNextAPInstanceNum = 1;
                    }
                }
            }
            else
            {
                pDmlIAPolicy->InstanceNumber = pCosaContext->InstanceNumber = pMyObject->ulNextAPInstanceNum;

                pMyObject->ulNextAPInstanceNum++;

                if ( pMyObject->ulNextAPInstanceNum == 0 )
                {
                    pMyObject->ulNextAPInstanceNum = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pDmlIAPolicy->Alias, sizeof(pDmlIAPolicy->Alias),"DmlIAPolicy%lu", pMyObject->ulNextAPInstanceNum);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  AnscFreeMemory(pDmlIAPolicy);
                  AnscFreeMemory(pCosaContext);
                  continue;
                }

                returnStatus = 
                    CosaDmlIaSetPolicyValues
                        (
                            NULL,
                            ulIndex,
                            pDmlIAPolicy->InstanceNumber,
                            pDmlIAPolicy->Alias
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    CcspTraceWarning(("!!! CosaDmlIaSetPolicyValues index: %lu-- fail !!!\n", ulIndex));

                    AnscFreeMemory(pDmlIAPolicy);
                    AnscFreeMemory(pCosaContext);

                    continue;
                }
            }

            returnStatus = 
                CosaDmlIaGetPolicySchedule
                    (
                        NULL, 
                        pDmlIAPolicy->InstanceNumber, 
                        &pDmlIAPolicy->Schedule
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                CcspTraceWarning(("!!! CosaDmlIaGetPolicySchedule fail !!!\n"));
            }

            pCosaContext->hContext     = (ANSC_HANDLE)pDmlIAPolicy;
            pCosaContext->hParentTable = NULL;
            pCosaContext->bNew         = FALSE;

            CosaSListPushEntryByInsNum(&pMyObject->AccessPolicyList, pCosaContext);
        }

        /* Blocked URL */

        if ( TRUE )
        {
            rc = sprintf_s
                (
                    FolderName,
                    sizeof(FolderName),
                    "%s%lu", 
                    COSA_DML_RR_NAME_IA_URL_NextInsNunmber, 
                    pDmlIAPolicy->InstanceNumber
                );
            if(rc < EOK)
            {
              ERR_CHK(rc);
              AnscFreeMemory(pDmlIAPolicy);
              AnscFreeMemory(pCosaContext);
              continue;
            }

            pPoamIrepFoNextIns = 
                (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIA->GetFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoIA,
                        FolderName
                    );

            if ( pPoamIrepFoNextIns )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoNextIns,
                            COSA_DML_RR_NAME_IA_URL_NextInsNunmber,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    pDmlIAPolicy->ulNextURLInsNum = pSlapVariable->Variant.varUint32;

                    SlapFreeVariable(pSlapVariable);
                }

                pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
                pPoamIrepFoNextIns = NULL;
            }            
        }

        ulSubCount = CosaDmlIaPolicyGetNumberOfUrls(NULL, pDmlIAPolicy->InstanceNumber);

        for ( ulSubIndex = 0; ulSubIndex < ulSubCount; ulSubIndex++ )
        {
            pURL = (PCOSA_DML_IA_POLICY_URL)AnscAllocateMemory(sizeof(COSA_DML_IA_POLICY_URL));

            if ( !pURL )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT;
            }

            returnStatus = CosaDmlIaPolicyGetUrl(NULL, pDmlIAPolicy->InstanceNumber, ulSubIndex, pURL);

            if ( returnStatus == ANSC_STATUS_SUCCESS )
            {
                pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pCosaContext2 )
                {
                    AnscFreeMemory(pURL);
            
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto  EXIT;
                }

                if ( pURL->InstanceNumber != 0 )
                {
                    pCosaContext2->InstanceNumber = pURL->InstanceNumber;

                    if ( pDmlIAPolicy->ulNextURLInsNum <= pURL->InstanceNumber )
                    {
                        pDmlIAPolicy->ulNextURLInsNum = pURL->InstanceNumber + 1;

                        if ( pDmlIAPolicy->ulNextURLInsNum == 0 )
                        {
                            pDmlIAPolicy->ulNextURLInsNum = 1;
                        }
                    }  
                }
                else
                {
                    pCosaContext2->InstanceNumber = pURL->InstanceNumber = pDmlIAPolicy->ulNextURLInsNum;

                    pDmlIAPolicy->ulNextURLInsNum++;

                    if ( pDmlIAPolicy->ulNextURLInsNum == 0 )
                    {
                        pDmlIAPolicy->ulNextURLInsNum = 1;
                    }

                    /* Generate Alias */
                    rc = sprintf_s(pURL->Alias, sizeof(pURL->Alias),"BlockedURL%lu", pCosaContext2->InstanceNumber);
                    if(rc < EOK)
                    {
                      ERR_CHK(rc);
                      AnscFreeMemory(pURL);
                      AnscFreeMemory(pCosaContext2);
                      continue;
                    }

                    /* TODO: Set InstanceNumber Alias back */
                    returnStatus =
                        CosaDmlIaPolicySetUrlValues
                            (
                                NULL, 
                                pDmlIAPolicy->InstanceNumber, 
                                ulSubIndex, 
                                pURL->InstanceNumber,
                                pURL->Alias
                            );

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        CcspTraceWarning(("!!! CosaDmlIaPolicySetUrlValues index: %lu-- fail !!!\n", ulSubIndex));

                        AnscFreeMemory(pURL);
                        AnscFreeMemory(pCosaContext2);

                        continue;
                    }
                }                

                pCosaContext2->hContext     = (ANSC_HANDLE)pURL;
                pCosaContext2->hParentTable = (ANSC_HANDLE)pDmlIAPolicy;
                pCosaContext2->bNew         = FALSE;

                CosaSListPushEntryByInsNum(&pDmlIAPolicy->URLList, pCosaContext2);
            }
            else /*RDKB-6746, CID-33059, free unused resource before exit*/
            {
                AnscFreeMemory(pURL);
            }
            pURL = NULL; /*Make null for next iteration*/
        }

        /* Blocked Keyword */

        if ( TRUE )
        {
            rc = sprintf_s
                (
                    FolderName,
                    sizeof(FolderName),
                    "%s%lu", 
                    COSA_DML_RR_NAME_IA_KEYWORD_NextInsNunmber, 
                    pDmlIAPolicy->InstanceNumber
                );
            if(rc < EOK)
            {
              ERR_CHK(rc);
              continue;
            }

            pPoamIrepFoNextIns = 
                (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIA->GetFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoIA,
                        FolderName
                    );

            if ( pPoamIrepFoNextIns )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoNextIns,
                            COSA_DML_RR_NAME_IA_KEYWORD_NextInsNunmber,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    pDmlIAPolicy->ulNextKeyInsNum = pSlapVariable->Variant.varUint32;

                    SlapFreeVariable(pSlapVariable);
                }

                pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
                pPoamIrepFoNextIns = NULL;
            }            
        }

        ulSubCount = CosaDmlIaPolicyGetNumberOfKeywords(NULL, pDmlIAPolicy->InstanceNumber);

        for ( ulSubIndex = 0; ulSubIndex < ulSubCount; ulSubIndex++ )
        {
            pKeyword = (PCOSA_DML_IA_POLICY_KEYWORD)AnscAllocateMemory(sizeof(COSA_DML_IA_POLICY_KEYWORD));

            if ( !pKeyword )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT;
            }

            returnStatus = CosaDmlIaPolicyGetKeyword(NULL, pDmlIAPolicy->InstanceNumber, ulSubIndex, pKeyword);

            if ( returnStatus == ANSC_STATUS_SUCCESS )
            {
                pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pCosaContext2 )
                {
                    AnscFreeMemory(pKeyword);
            
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto  EXIT;
                }

                if ( pKeyword->InstanceNumber != 0 )
                {
                    pCosaContext2->InstanceNumber = pKeyword->InstanceNumber;

                    if ( pDmlIAPolicy->ulNextKeyInsNum <= pKeyword->InstanceNumber )
                    {
                        pDmlIAPolicy->ulNextKeyInsNum = pKeyword->InstanceNumber + 1;

                        if ( pDmlIAPolicy->ulNextKeyInsNum == 0 )
                        {
                            pDmlIAPolicy->ulNextKeyInsNum = 1;
                        }
                    }  
                }
                else
                {
                    pCosaContext2->InstanceNumber = pKeyword->InstanceNumber = pDmlIAPolicy->ulNextKeyInsNum;

                    pDmlIAPolicy->ulNextKeyInsNum++;

                    if ( pDmlIAPolicy->ulNextKeyInsNum == 0 )
                    {
                        pDmlIAPolicy->ulNextKeyInsNum = 1;
                    }

                    /* Generate Alias */
                    rc = sprintf_s(pKeyword->Alias, sizeof(pKeyword->Alias),"BlockedKeyword%lu", pCosaContext2->InstanceNumber);
                    if(rc < EOK)
                    {
                       ERR_CHK(rc);
                       AnscFreeMemory(pKeyword);
                       AnscFreeMemory(pCosaContext2);
                       continue;
                    }

                    /* TODO: Set InstanceNumber Alias back */
                    returnStatus = 
                        CosaDmlIaPolicySetKeywordValues
                            (
                                NULL, 
                                pDmlIAPolicy->InstanceNumber, 
                                ulSubIndex, 
                                pKeyword->InstanceNumber,
                                pKeyword->Alias
                            );

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        CcspTraceWarning(("!!! CosaDmlIaPolicySetKeywordValues index: %lu-- fail !!!\n", ulSubIndex));

                        AnscFreeMemory(pKeyword);
                        AnscFreeMemory(pCosaContext2);

                        continue;
                    }
                }                

                pCosaContext2->hContext     = (ANSC_HANDLE)pKeyword;
                pCosaContext2->hParentTable = (ANSC_HANDLE)pDmlIAPolicy;
                pCosaContext2->bNew         = FALSE;

                CosaSListPushEntryByInsNum(&pDmlIAPolicy->KeywordList, pCosaContext2);
            }
            else /*RDKB-6746, CID-33437, free unused resource before exit*/
            {
                AnscFreeMemory(pKeyword);
            }
            pKeyword = NULL; /*Make null for next iteration*/
        }

        /* Blocked App */

        if ( TRUE )
        {
            rc = sprintf_s
                (
                    FolderName,
                    sizeof(FolderName),
                    "%s%lu", 
                    COSA_DML_RR_NAME_IA_APP_NextInsNunmber, 
                    pDmlIAPolicy->InstanceNumber
                );
            if(rc < EOK)
            {
              ERR_CHK(rc);
              continue;
            }

            pPoamIrepFoNextIns = 
                (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIA->GetFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoIA,
                        FolderName
                    );

            if ( pPoamIrepFoNextIns )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoNextIns,
                            COSA_DML_RR_NAME_IA_APP_NextInsNunmber,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    pDmlIAPolicy->ulNextAppInsNum = pSlapVariable->Variant.varUint32;

                    SlapFreeVariable(pSlapVariable);
                }

                pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
                pPoamIrepFoNextIns = NULL;
            }            
        }

        ulSubCount = CosaDmlIaPolicyGetNumberOfApps(NULL, pDmlIAPolicy->InstanceNumber);

        for ( ulSubIndex = 0; ulSubIndex < ulSubCount; ulSubIndex++ )
        {
            pApp = (PCOSA_DML_IA_POLICY_APP)AnscAllocateMemory(sizeof(COSA_DML_IA_POLICY_APP));

            if ( !pApp )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT;
            }

            returnStatus = CosaDmlIaPolicyGetApp(NULL, pDmlIAPolicy->InstanceNumber, ulSubIndex, pApp);

            if ( returnStatus == ANSC_STATUS_SUCCESS )
            {
                pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pCosaContext2 )
                {
                    AnscFreeMemory(pApp);
            
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto  EXIT;
                }

                if ( pApp->InstanceNumber != 0 )
                {
                    pCosaContext2->InstanceNumber = pApp->InstanceNumber;

                    if ( pDmlIAPolicy->ulNextAppInsNum <= pApp->InstanceNumber )
                    {
                        pDmlIAPolicy->ulNextAppInsNum = pApp->InstanceNumber + 1;

                        if ( pDmlIAPolicy->ulNextAppInsNum == 0 )
                        {
                            pDmlIAPolicy->ulNextAppInsNum = 1;
                        }
                    }  
                }
                else
                {
                    pCosaContext2->InstanceNumber = pApp->InstanceNumber = pDmlIAPolicy->ulNextAppInsNum;

                    pDmlIAPolicy->ulNextAppInsNum++;

                    if ( pDmlIAPolicy->ulNextAppInsNum == 0 )
                    {
                        pDmlIAPolicy->ulNextAppInsNum = 1;
                    }

                    /* Generate Alias */
                    rc = sprintf_s(pApp->Alias, sizeof(pApp->Alias),"BlockedApp%lu", pCosaContext2->InstanceNumber);
                    if(rc < EOK)
                    {
                      ERR_CHK(rc);
                      AnscFreeMemory(pApp);
                      AnscFreeMemory(pCosaContext2);
                      continue;
                    }

                    /* TODO: Set InstanceNumber Alias back */
                    returnStatus = 
                        CosaDmlIaPolicySetAppValues
                            (
                                NULL, 
                                pDmlIAPolicy->InstanceNumber, 
                                ulSubIndex, 
                                pApp->InstanceNumber,
                                pApp->Alias
                            );

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        CcspTraceWarning(("!!! CosaDmlIaPolicySetAppValues index: %lu-- fail !!!\n", ulSubIndex));

                        AnscFreeMemory(pApp);
                        AnscFreeMemory(pCosaContext2);

                        continue;
                    }
                }                

                pCosaContext2->hContext     = (ANSC_HANDLE)pApp;
                pCosaContext2->hParentTable = (ANSC_HANDLE)pDmlIAPolicy;
                pCosaContext2->bNew         = FALSE;

                CosaSListPushEntryByInsNum(&pDmlIAPolicy->AppList, pCosaContext2);
            }
            else  /*RDKB-6746, CID-33351, free unused resource before exit*/
            {
                AnscFreeMemory(pApp);
            }
            pApp = NULL; /*Make null for next iteration*/
        }
    }

    /* Load the newly added but not yet commited entries, if any */

    CosaSecurityIARegGetInfo((ANSC_HANDLE)pMyObject);


EXIT:

    AnscFreeMemory(pFirewallCfg);

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaSecurityRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Security object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaSecurityRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_SECURITY        pMyObject         = (PCOSA_DATAMODEL_SECURITY   )hThisObject;
    PSLIST_HEADER                   pPolicyHead       = (PSLIST_HEADER              )&pMyObject->AccessPolicyList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIA     = (PPOAM_IREP_FOLDER_OBJECT   )pMyObject->hIrepFolderIA;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIAHA   = (PPOAM_IREP_FOLDER_OBJECT   )pMyObject->hIrepFolderIAHA;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2     = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_DML_IA_POLICY2            pSecurityIAPolicy = (PCOSA_DML_IA_POLICY2       )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY         )NULL;
    PSLIST_HEADER                   pListHead         = (PSLIST_HEADER              )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry2      = (PSINGLE_LINK_ENTRY         )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext3     = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_DML_IA_POLICY_URL         pURL              = (PCOSA_DML_IA_POLICY_URL    )NULL;
    PCOSA_DML_IA_POLICY_KEYWORD     pKeyword          = (PCOSA_DML_IA_POLICY_KEYWORD)NULL;
    PCOSA_DML_IA_POLICY_APP         pApp              = (PCOSA_DML_IA_POLICY_APP    )NULL;

	/* Remove necessary resounce */

    /* AccessPolicyList */
    pSLinkEntry = AnscSListGetFirstEntry(pPolicyHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        pSecurityIAPolicy = (PCOSA_DML_IA_POLICY2)pCosaContext2->hContext;

        if ( pSecurityIAPolicy )
        {
            AnscSListPopEntryByLink(pPolicyHead, &pCosaContext2->Linkage);

            /* Free the down-layer list one by one */
                
            /* BlockedURL List */
            pListHead = &pSecurityIAPolicy->URLList;

            pSLinkEntry2 = AnscSListGetFirstEntry(pListHead);

            while ( pSLinkEntry2 )
            {
                pCosaContext3 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry2);
                pSLinkEntry2  = AnscSListGetNextEntry(pSLinkEntry2);

                pURL = (PCOSA_DML_IA_POLICY_URL)pCosaContext3->hContext;

                AnscSListPopEntryByLink(pListHead, &pCosaContext3->Linkage);

                if ( pURL )
                {
                    AnscFreeMemory(pURL);
                }

                pPoamIrepFo = pCosaContext3->hPoamIrepFo;

                if ( pPoamIrepFo )
                {
                    pPoamIrepFo->Remove((ANSC_HANDLE)pPoamIrepFo);
                }

                AnscFreeMemory(pCosaContext3);
            }

            /* BlockedKeyword List */
            pListHead = &pSecurityIAPolicy->KeywordList;

            pSLinkEntry2 = AnscSListGetFirstEntry(pListHead);

            while ( pSLinkEntry2 )
            {
                pCosaContext3 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry2);
                pSLinkEntry2  = AnscSListGetNextEntry(pSLinkEntry2);

                pKeyword = (PCOSA_DML_IA_POLICY_KEYWORD)pCosaContext3->hContext;

                AnscSListPopEntryByLink(pListHead, &pCosaContext3->Linkage);

                if ( pKeyword )
                {
                    AnscFreeMemory(pKeyword);
                }

                pPoamIrepFo = pCosaContext3->hPoamIrepFo;

                if ( pPoamIrepFo )
                {
                    pPoamIrepFo->Remove((ANSC_HANDLE)pPoamIrepFo);
                }
                
                AnscFreeMemory(pCosaContext3);
            }

            /* BlockedApp List */
            pListHead = &pSecurityIAPolicy->AppList;

            pSLinkEntry2 = AnscSListGetFirstEntry(pListHead);

            while ( pSLinkEntry2 )
            {
                pCosaContext3 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry2);
                pSLinkEntry2  = AnscSListGetNextEntry(pSLinkEntry2);

                pApp = (PCOSA_DML_IA_POLICY_APP)pCosaContext3->hContext;

                AnscSListPopEntryByLink(pListHead, &pCosaContext3->Linkage);

                if ( pApp )
                {
                    AnscFreeMemory(pApp);
                }

                pPoamIrepFo = pCosaContext3->hPoamIrepFo;

                if ( pPoamIrepFo )
                {
                    pPoamIrepFo->Remove((ANSC_HANDLE)pPoamIrepFo);
                }

                AnscFreeMemory(pCosaContext3);
            }
                
            AnscFreeMemory(pSecurityIAPolicy);

            pPoamIrepFo = pCosaContext2->hPoamIrepFo;

            if ( pPoamIrepFo )
            {
                pPoamIrepFo->Remove((ANSC_HANDLE)pPoamIrepFo);
            }

            AnscFreeMemory(pCosaContext2);
        }
    }

    if ( pPoamIrepFoIAHA )
    {
        pPoamIrepFoIAHA->Remove((ANSC_HANDLE)pPoamIrepFoIAHA);
    }

    if ( pPoamIrepFoIA )
    {
        pPoamIrepFoIA->Remove((ANSC_HANDLE)pPoamIrepFoIA);
    }

    if ( pMyObject->pLog && pMyObject->ulLogEntryCount > 0 )
    {
        AnscFreeMemory(pMyObject->pLog);
    }

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    /*backend related unInitialization*/
    CosaDmlIaRemove();

	return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaSecurityIARegGetInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaSecurityIARegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_SECURITY        pMyObject               = (PCOSA_DATAMODEL_SECURITY    )hThisObject;
    PSLIST_HEADER                   pPolicyHead             = (PSLIST_HEADER               )&pMyObject->AccessPolicyList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIAHA         = (PPOAM_IREP_FOLDER_OBJECT    )pMyObject->hIrepFolderIAHA;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIASp         = (PPOAM_IREP_FOLDER_OBJECT    )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT   )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2           = (PCOSA_CONTEXT_LINK_OBJECT   )NULL;
    PCOSA_DML_IA_POLICY2            pDmlIAPolicy            = (PCOSA_DML_IA_POLICY2        )NULL;
    PCOSA_DML_IA_POLICY2            pDmlIAEntry             = (PCOSA_DML_IA_POLICY2        )NULL;
    PCOSA_DML_IA_POLICY_URL         pURL                    = (PCOSA_DML_IA_POLICY_URL     )NULL;
    PCOSA_DML_IA_POLICY_KEYWORD     pKeyword                = (PCOSA_DML_IA_POLICY_KEYWORD )NULL;
    PCOSA_DML_IA_POLICY_APP         pApp                    = (PCOSA_DML_IA_POLICY_APP     )NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE              )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY          )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    ULONG                           ulUpperInstanceNumber   = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoIAHA )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */

    ulEntryCount = pPoamIrepFoIAHA->GetFolderCount((ANSC_HANDLE)pPoamIrepFoIAHA);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoIAHA->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoIAHA,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoIASp = pPoamIrepFoIAHA->GetFolder((ANSC_HANDLE)pPoamIrepFoIAHA, pFolderName);

        if ( !pPoamIrepFoIASp )
        {
            continue;
        }

        if ( _ansc_strstr(pFolderName, "InternetAccessPolicy") )
        {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoIASp->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoIASp,
                            COSA_DML_RR_NAME_IA_InsNum,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    ulInstanceNumber = pSlapVariable->Variant.varUint32;

                    SlapFreeVariable(pSlapVariable);
                }

                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoIASp->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoIASp,
                            COSA_DML_RR_NAME_IA_Alias,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    pAlias = AnscCloneString(pSlapVariable->Variant.varString);

                    SlapFreeVariable(pSlapVariable);
                }

            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto EXIT;
            }

            pDmlIAPolicy = (PCOSA_DML_IA_POLICY2)AnscAllocateMemory(sizeof(COSA_DML_IA_POLICY2));

            if ( !pDmlIAPolicy )
            {
                AnscFreeMemory(pCosaContext);

                returnStatus = ANSC_STATUS_RESOURCES;

                goto EXIT;
            }

            AnscSListInitializeHeader(&pDmlIAPolicy->URLList    );
            AnscSListInitializeHeader(&pDmlIAPolicy->KeywordList);
            AnscSListInitializeHeader(&pDmlIAPolicy->AppList    );

            pDmlIAPolicy->ulNextURLInsNum = 1;
            pDmlIAPolicy->ulNextKeyInsNum = 1;
            pDmlIAPolicy->ulNextAppInsNum = 1;

            pDmlIAPolicy->InstanceNumber = ulInstanceNumber;
            rc = STRCPY_S_NOCLOBBER(pDmlIAPolicy->Alias, sizeof(pDmlIAPolicy->Alias), pAlias ? pAlias : "");
            ERR_CHK(rc);

            pCosaContext->InstanceNumber   = ulInstanceNumber;
            pCosaContext->bNew             = TRUE;
            pCosaContext->hContext         = (ANSC_HANDLE)pDmlIAPolicy;
            pCosaContext->hParentTable     = NULL;
            pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoIAHA;
            pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoIASp;
            
            CosaSListPushEntryByInsNum(pPolicyHead, pCosaContext);
        }
        else
        {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoIASp->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoIASp,
                            COSA_DML_RR_NAME_IA_Upper_InsNum,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    ulUpperInstanceNumber = pSlapVariable->Variant.varUint32;

                    SlapFreeVariable(pSlapVariable);
                }

                if ( ulUpperInstanceNumber != 0 )
                {
                    pSLinkEntry = AnscSListGetFirstEntry(pPolicyHead);

                    while ( pSLinkEntry )
                    {
                        pCosaContext  = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
                        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

                        pDmlIAEntry = (PCOSA_DML_IA_POLICY2)pCosaContext->hContext;

                        if ( pDmlIAEntry && (pDmlIAEntry->InstanceNumber == ulUpperInstanceNumber) )
                        {
                            break;
                        }
                    }

                    if ( !pDmlIAEntry )
                    {
                        CcspTraceWarning(("!!! CosaSecurityIARegGetInfo pDmlIAEntry is NULL, skip this entry...!!!\n"));

                        continue;
                    }
                }
                else
                {
                    CcspTraceWarning(("!!! CosaSecurityIARegGetInfo ulUpperInstanceNumber is ZERO, skip this entry... !!!\n"));

                    continue;
                }

                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoIASp->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoIASp,
                            COSA_DML_RR_NAME_IA_InsNum,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    ulInstanceNumber = pSlapVariable->Variant.varUint32;

                    SlapFreeVariable(pSlapVariable);
                }

                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoIASp->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoIASp,
                            COSA_DML_RR_NAME_IA_Alias,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    pAlias = AnscCloneString(pSlapVariable->Variant.varString);

                    SlapFreeVariable(pSlapVariable);
                }

            if ( _ansc_strstr(pFolderName, "URL") )
            {
                pURL = (PCOSA_DML_IA_POLICY_URL)AnscAllocateMemory(sizeof(COSA_DML_IA_POLICY_URL));

                if ( !pURL )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pCosaContext2 )
                {
                    AnscFreeMemory(pURL);
            
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pURL->InstanceNumber = ulInstanceNumber;
                rc = STRCPY_S_NOCLOBBER(pURL->Alias, sizeof(pURL->Alias), pAlias ? pAlias : "");
                ERR_CHK(rc);

                pCosaContext2->InstanceNumber   = ulInstanceNumber;
                pCosaContext2->hContext         = (ANSC_HANDLE)pURL;
                pCosaContext2->hParentTable     = (ANSC_HANDLE)pDmlIAEntry;
                pCosaContext2->bNew             = TRUE;
                pCosaContext2->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoIAHA;
                pCosaContext2->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoIASp;

                CosaSListPushEntryByInsNum(&pDmlIAEntry->URLList, pCosaContext2);
            }
            else if( _ansc_strstr(pFolderName, "Keyword") )
            {
                pKeyword = (PCOSA_DML_IA_POLICY_KEYWORD)AnscAllocateMemory(sizeof(COSA_DML_IA_POLICY_KEYWORD));

                if ( !pKeyword )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pCosaContext2 )
                {
                    AnscFreeMemory(pKeyword);
            
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pKeyword->InstanceNumber = ulInstanceNumber;
                rc = STRCPY_S_NOCLOBBER(pKeyword->Alias, sizeof(pKeyword->Alias), pAlias ? pAlias : "");
                ERR_CHK(rc);

                pCosaContext2->InstanceNumber   = ulInstanceNumber;
                pCosaContext2->hContext         = (ANSC_HANDLE)pKeyword;
                pCosaContext2->hParentTable     = (ANSC_HANDLE)pDmlIAEntry;
                pCosaContext2->bNew             = TRUE;
                pCosaContext2->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoIAHA;
                pCosaContext2->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoIASp;

                CosaSListPushEntryByInsNum(&pDmlIAEntry->KeywordList, pCosaContext2);
            }
            else if ( _ansc_strstr(pFolderName, "App") )
            {
                pApp = (PCOSA_DML_IA_POLICY_APP)AnscAllocateMemory(sizeof(COSA_DML_IA_POLICY_APP));

                if ( !pApp )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pCosaContext2 )
                {
                    AnscFreeMemory(pApp);
            
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pApp->InstanceNumber = ulInstanceNumber;
                rc = STRCPY_S_NOCLOBBER(pApp->Alias, sizeof(pApp->Alias), pAlias ? pAlias : "");
                ERR_CHK(rc);

                pCosaContext2->InstanceNumber   = ulInstanceNumber;
                pCosaContext2->hContext         = (ANSC_HANDLE)pApp;
                pCosaContext2->hParentTable     = (ANSC_HANDLE)pDmlIAEntry;
                pCosaContext2->bNew             = TRUE;
                pCosaContext2->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoIAHA;
                pCosaContext2->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoIASp;

                CosaSListPushEntryByInsNum(&pDmlIAEntry->AppList, pCosaContext2);
            }
        }

        if ( pAlias )
        {
            AnscFreeMemory(pAlias);

            pAlias = NULL;
        }

        if ( pFolderName )
        {
            AnscFreeMemory(pFolderName);
        }
    }

EXIT:

    if ( pAlias )
    {
        AnscFreeMemory(pAlias);
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaSecurityIARegAddInfo
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pNextInsNumName,
                ULONG                       ulNextInsNum,
                char*                       pPreffix,
                ULONG                       ulUpperInsNum,
                char*                       pAlias,
                ANSC_HANDLE                 hCosaContext
            )

    description:

        This function is called to configure Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaSecurityIARegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pNextInsNumName,
        ULONG                       ulNextInsNum,
        char*                       pPreffix,
        ULONG                       ulUpperInsNum,
        char*                       pAlias,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_SECURITY        pMyObject         = (PCOSA_DATAMODEL_SECURITY )hThisObject;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIA     = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderIA;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoUpper  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;  
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PSLAP_VARIABLE                  pSlapVariable     = (PSLAP_VARIABLE           )NULL;
    char                            FolderName[32]    = {0};
    errno_t                         rc                = -1;

    if ( !pPoamIrepFoIA || !pPoamIrepFoUpper )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoIA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoIA, FALSE);
    }

        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }

    rc = sprintf_s(FolderName, sizeof(FolderName),"%s%lu", pNextInsNumName, ulUpperInsNum);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      returnStatus = ANSC_STATUS_FAILURE;
      goto  EXIT1;
    }

        pPoamIrepFo = 
            pPoamIrepFoIA->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoIA,
                    FolderName
                );

        if ( !pPoamIrepFo )
        {
            pPoamIrepFo = 
                pPoamIrepFoIA->AddFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoIA,
                        FolderName,
                        0
                    );
        }

    if ( pPoamIrepFo != NULL )
    {
        returnStatus = 
            pPoamIrepFo->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFo,
                    pNextInsNumName
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = ulNextInsNum;

        returnStatus =
            pPoamIrepFo->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFo,
                    pNextInsNumName,
                    SYS_REP_RECORD_TYPE_UINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );

        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    

        pPoamIrepFo->Remove((ANSC_HANDLE)pPoamIrepFo);
        pPoamIrepFo = NULL;
    }

        rc = sprintf_s(FolderName, sizeof(FolderName),"%s%lu%lu", pPreffix, ulUpperInsNum, pCosaContext->InstanceNumber);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          returnStatus = ANSC_STATUS_FAILURE;
          goto  EXIT1;
        }

        pPoamIrepFo =
            pPoamIrepFoUpper->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoUpper,
                    FolderName,
                    0
                );

        if ( !pPoamIrepFo )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( ulUpperInsNum != 0 )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = ulUpperInsNum;

            returnStatus =
                pPoamIrepFo->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFo,
                        COSA_DML_RR_NAME_IA_Upper_InsNum,
                        SYS_REP_RECORD_TYPE_UINT,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );
      
            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pCosaContext->InstanceNumber;

            returnStatus =
                pPoamIrepFo->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFo,
                        COSA_DML_RR_NAME_IA_InsNum,
                        SYS_REP_RECORD_TYPE_UINT,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);

            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pAlias);

            returnStatus =
                pPoamIrepFo->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFo,
                        COSA_DML_RR_NAME_IA_Alias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);

        pCosaContext->hPoamIrepFo = (ANSC_HANDLE)pPoamIrepFo;

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoIA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoIA, TRUE);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaSecurityIARegDelInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to configure Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaSecurityIARegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepUpperFo  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepFo;

    if ( !pPoamIrepUpperFo || !pPoamIrepFo )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, FALSE);
    }

        pPoamIrepFo->Close((ANSC_HANDLE)pPoamIrepFo);
        
        pPoamIrepUpperFo->DelFolder
            (
                (ANSC_HANDLE)pPoamIrepUpperFo, 
                pPoamIrepFo->GetFolderName((ANSC_HANDLE)pPoamIrepFo)
            );

        pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, TRUE);

        AnscFreeMemory(pPoamIrepFo);

    return ANSC_STATUS_SUCCESS;
}

