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
/****************************************************************************
  Copyright 2016-2018 Intel Corporation

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
******************************************************************************/

#ifdef INTEL_GRE_HOTSPOT

#include "dml_tr181_custom_cfg.h"
#include "plugin_main_apis.h"
#include "cosa_gre_internal.h"

#define  MAX_REC_LEN  256

ANSC_STATUS CosaDmlGREInit(ANSC_HANDLE hDml,PANSC_HANDLE phContext);
ANSC_STATUS CosaDmlGREInit_HotspotParams(ANSC_HANDLE hContext);

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaGRECreate
            (
            );

    description:

        This function constructs cosa gre object and return handle.

    argument:

    return:     newly created gre object.

**********************************************************************/

ANSC_HANDLE
CosaGRECreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_GRE         pMyObject    = (PCOSA_DATAMODEL_GRE)NULL;

    pMyObject = (PCOSA_DATAMODEL_GRE)AnscAllocateMemory(sizeof(COSA_DATAMODEL_GRE));
    if (!pMyObject)
    {
        return (ANSC_HANDLE)NULL;
    }

    pMyObject->Oid               = COSA_DATAMODEL_IGRE_OID;
    pMyObject->Create            = CosaGRECreate;
    pMyObject->Remove            = CosaGRERemove;
    pMyObject->Initialize        = CosaGREInitialize;

    pMyObject->Initialize((ANSC_HANDLE)pMyObject);
    return (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaGREInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa gre object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaGREInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_GRE            pMyObject               = (PCOSA_DATAMODEL_GRE)hThisObject;
    ULONG                          ulGRETunnelCount        = 0;
    ULONG                          ulGRETunnelIfCount      = 0;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL  pDmlGRETunnel           = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)NULL;
    PCOSA_DML_GRE_TUNNEL_IF_FULL   pTunnelIf               = (PCOSA_DML_GRE_TUNNEL_IF_FULL)NULL;
    PCOSA_CONTEXT_LINK_OBJECT      pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_CONTEXT_LINK_OBJECT      pCosaContext2           = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    ULONG                          ulIndex                 = 0;
    ULONG                          ulSubIndex              = 0;
    UCHAR                          param_name[MAX_REC_LEN] = {0};
    ANSC_STATUS                    returnStatus            = ANSC_STATUS_SUCCESS;

    CosaDmlGREInit(NULL, &pMyObject->hSbContext);

    ulGRETunnelCount = CosaDmlGRETunnelGetNumberOfEntries(pMyObject->hSbContext);

    AnscSListInitializeHeader(&pMyObject->GRETunnelList);

    pMyObject->ulNextGRETunnelInstance = 1;

    CosaDmlGREInit_HotspotParams(pMyObject->hSbContext);

    for (ulIndex = 0; ulIndex < ulGRETunnelCount; ulIndex++)
    {
        pDmlGRETunnel = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)AnscAllocateMemory(sizeof(COSA_DML_GRE_TUNNEL_FULL_ALL));

        if ( !pDmlGRETunnel )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            goto EXIT;
        }
        AnscSListInitializeHeader(&pDmlGRETunnel->InterfaceList);
        pDmlGRETunnel->ulNextIfInsNum  = 1;

        CosaDmlGRETunnelGetEntry(pMyObject->hSbContext, ulIndex, (PCOSA_DML_GRE_TUNNEL_FULL)pDmlGRETunnel);
        if (TRUE) 
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            _ansc_memset(pCosaContext, 0, sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCosaContext )
            {
                AnscFreeMemory(pDmlGRETunnel);

                returnStatus = ANSC_STATUS_RESOURCES;

                AnscTraceFlow(("<HL>%s cannot allocate pCosaContext\n", __FUNCTION__));
                goto  EXIT;
            }

            if ( pDmlGRETunnel->Cfg.TunnelInstanceNumber !=0 )
            {
                pCosaContext->InstanceNumber = pDmlGRETunnel->Cfg.TunnelInstanceNumber;
                if ( pMyObject->ulNextGRETunnelInstance <= pDmlGRETunnel->Cfg.TunnelInstanceNumber )
                {
                    pMyObject->ulNextGRETunnelInstance = pDmlGRETunnel->Cfg.TunnelInstanceNumber + 1;
                    if ( pMyObject->ulNextGRETunnelInstance == 0 )
                    {
                        pMyObject->ulNextGRETunnelInstance = 1;
                    }
                }
            }
            else
            {
                pDmlGRETunnel->Cfg.TunnelInstanceNumber = pCosaContext->InstanceNumber = pMyObject->ulNextGRETunnelInstance;
                pMyObject->ulNextGRETunnelInstance++;
                if ( pMyObject->ulNextGRETunnelInstance == 0)
                {
                    pMyObject->ulNextGRETunnelInstance = 1;
                }
                _ansc_sprintf(pDmlGRETunnel->Cfg.Alias, "cpe-GRETunnel%lu", pCosaContext->InstanceNumber);

                CosaDmlGRETunnelSetValues(pMyObject->hSbContext, ulIndex, pDmlGRETunnel->Cfg.Alias);
            }

            pCosaContext->hContext     = (ANSC_HANDLE)pDmlGRETunnel;
            pCosaContext->hParentTable = NULL;
            pCosaContext->bNew         = FALSE;

            CosaSListPushEntryByInsNum(&pMyObject->GRETunnelList, pCosaContext);
        }

        // Tunnel.{i}.Interface
        ulGRETunnelIfCount = CosaDmlGRETunnelIfGetNumberOfEntries(pMyObject->hSbContext, ulIndex);
        for ( ulSubIndex = 0; ulSubIndex < ulGRETunnelIfCount; ulSubIndex++ )
        {
            pTunnelIf = (PCOSA_DML_GRE_TUNNEL_IF_FULL)AnscAllocateMemory(sizeof(COSA_DML_GRE_TUNNEL_IF_FULL));
            if ( !pTunnelIf )
            {
                returnStatus = ANSC_STATUS_RESOURCES;
                goto EXIT;
            }
            _ansc_memset(pTunnelIf, 0, sizeof(COSA_DML_GRE_TUNNEL_IF_FULL));

            returnStatus=CosaDmlGRETunnelIfGetEntry(pMyObject->hSbContext, ulIndex, ulSubIndex, pTunnelIf);
            if(returnStatus == ANSC_STATUS_SUCCESS)
            {
                if (pTunnelIf->Cfg.Enable == true)
                {
                    _ansc_sprintf((char *)param_name, "sysevent set igre-start %lu", ulIndex + 1);
                    system((const char *)param_name);
                }
            }
            if (TRUE)
            {
                pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
                if ( !pCosaContext2 )
                {
                    AnscFreeMemory(pTunnelIf);
                    returnStatus = ANSC_STATUS_RESOURCES;
                    goto EXIT;
                }
                _ansc_memset(pCosaContext2, 0, sizeof(COSA_CONTEXT_LINK_OBJECT));
                if ( pTunnelIf->Cfg.IfInstanceNumber != 0 )
                {
                    pCosaContext2->InstanceNumber = pTunnelIf->Cfg.IfInstanceNumber;
                    if ( pDmlGRETunnel->ulNextIfInsNum <= pTunnelIf->Cfg.IfInstanceNumber )
                    {
                        pDmlGRETunnel->ulNextIfInsNum = pTunnelIf->Cfg.IfInstanceNumber + 1;
                        if ( pDmlGRETunnel->ulNextIfInsNum == 0 )
                        {
                            pDmlGRETunnel->ulNextIfInsNum = 1;
                        }
                    }
                }
                else
                {
                    pCosaContext2->InstanceNumber = pTunnelIf->Cfg.IfInstanceNumber  = pDmlGRETunnel->ulNextIfInsNum;
                    pDmlGRETunnel->ulNextIfInsNum++;
                    if ( pDmlGRETunnel->ulNextIfInsNum == 0 )
                    {
                        pDmlGRETunnel->ulNextIfInsNum = 1;
                    }
                    _ansc_sprintf(pTunnelIf->Cfg.Alias, "cpe-GRETunnel%luInterface%lu", ulIndex, pCosaContext2->InstanceNumber);

                    CosaDmlGRETunnelIfSetValues
                        (
                            pMyObject->hSbContext,
                            ulIndex,
                            ulSubIndex,
                            pTunnelIf->Cfg.Alias
                        );
                }
                pCosaContext2->hContext     = (ANSC_HANDLE)pTunnelIf;
                pCosaContext2->hParentTable = (ANSC_HANDLE)pDmlGRETunnel;
                pCosaContext2->bNew         = FALSE;

                CosaSListPushEntryByInsNum(&pDmlGRETunnel->InterfaceList, pCosaContext2);
            }
        }
    }
EXIT:

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaGRERemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa gre object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaGRERemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_GRE             pMyObject         = (PCOSA_DATAMODEL_GRE )hThisObject;
    PSLIST_HEADER                   pGRETunnelHead    = (PSLIST_HEADER )&pMyObject->GRETunnelList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2     = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY         )NULL;
    PSLIST_HEADER                   pListHead         = (PSLIST_HEADER              )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry2      = (PSINGLE_LINK_ENTRY         )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext3     = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pTunnelIf         = (PCOSA_DML_GRE_TUNNEL_IF_FULL    )NULL;

    pSLinkEntry = AnscSListGetFirstEntry(pGRETunnelHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);
        pDmlGRETunnel = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext2->hContext;

        if ( pDmlGRETunnel )
        {
            AnscSListPopEntryByLink(pGRETunnelHead, &pCosaContext2->Linkage);
            pListHead = &pDmlGRETunnel->InterfaceList;
            pSLinkEntry2 = AnscSListGetFirstEntry(pListHead);

            while ( pSLinkEntry2 )
            {
                pCosaContext3 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry2);
                pSLinkEntry2 = AnscSListGetNextEntry(pSLinkEntry2);
                pTunnelIf = (PCOSA_DML_GRE_TUNNEL_IF_FULL)pCosaContext3->hContext;

                AnscSListPopEntryByLink(pListHead, &pCosaContext3->Linkage);
                if ( pTunnelIf )
                {
                    AnscFreeMemory(pTunnelIf);
                }
                AnscFreeMemory(pCosaContext3);
            }
            AnscFreeMemory(pDmlGRETunnel);
            AnscFreeMemory(pCosaContext2);
        }
    }
    /* For GRE Hotspot */
    if(sysevent_fd)
    {
        sysevent_close(sysevent_fd, sysevent_token);
    }

    AnscFreeMemory((ANSC_HANDLE)pMyObject);
    return returnStatus;
}

#endif
