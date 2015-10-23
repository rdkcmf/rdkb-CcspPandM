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

    module: cosa_moca_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaMoCACreate
        *  CosaMoCAInitialize
        *  CosaMoCARemove
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

#include "cosa_moca_internal.h"
#include <sysevent/sysevent.h>


extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaMoCACreate
            (
            );

    description:

        This function constructs cosa MoCA object and return handle.

    argument:  

    return:     newly created MoCA object.

**********************************************************************/

ANSC_HANDLE
CosaMoCACreate
    (
        VOID
    )
{
    ANSC_STATUS           returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_MOCA  pMyObject    = (PCOSA_DATAMODEL_MOCA)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_MOCA)AnscAllocateMemory(sizeof(COSA_DATAMODEL_MOCA));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_MOCA_OID;
    pMyObject->Create            = CosaMoCACreate;
    pMyObject->Remove            = CosaMoCARemove;
    pMyObject->Initialize        = CosaMoCAInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

static int sysevent_fd;
static token_t sysevent_token;
static pthread_t sysevent_tid;
static void *Moca_sysevent_handler (void *data)
{
	async_id_t moca_update;
	sysevent_setnotification(sysevent_fd, sysevent_token, "moca_updated", &moca_update);
	PCOSA_DATAMODEL_MOCA            pMyObject     = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;

	pthread_detach(pthread_self());

	for (;;)
    {
        char name[25]={0};
		char val[42]={0};
		char buf[10]={0};
        int namelen = sizeof(name);
        int vallen  = sizeof(val);
        int err;
        async_id_t getnotification_asyncid;
        err = sysevent_getnotification(sysevent_fd, sysevent_token, name, &namelen,  val, &vallen, &getnotification_asyncid);

        if (err)
        {
           printf("%s-**********ERR: %d\n", __func__, err);
        }
		else 
		{
			if (strcmp(name, "moca_updated")==0)
		    {
			  int isUpdated = atoi(val);
			  if(isUpdated) {
				CosaDmlMocaIfGetCfg(NULL, 0, &pMyObject->MoCAIfFullTable[0].MoCAIfFull.Cfg);
			  } 		   
			}
	   }

	}
	return 0;
}

void CosaMoCAUpdate()
{
	sysevent_fd = sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, SE_VERSION, "moca-update", &sysevent_token);

	if (sysevent_fd >= 0)
	{
		pthread_create(&sysevent_tid, NULL, Moca_sysevent_handler, NULL);
	}
	return;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaMoCAInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa MoCA object and return handle.

    argument: ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaMoCAInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS               returnStatus   = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_MOCA      pMyObject      = (PCOSA_DATAMODEL_MOCA    )hThisObject;
    COSAGetHandleProc         pProc          = (COSAGetHandleProc       )NULL;
    PCOSA_PLUGIN_INFO         pPlugInfo      = (PCOSA_PLUGIN_INFO       )g_pCosaBEManager->hCosaPluginInfo;
    PSLAP_OBJECT_DESCRIPTOR   pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR )NULL;
    ANSC_HANDLE               pPoamMoCADm    = (ANSC_HANDLE )NULL;
    ANSC_HANDLE               pSlapMoCADm    = (ANSC_HANDLE )NULL;
    ULONG                     ulCount        = 0;
    ULONG                     ulIndex        = 0;
    ULONG                     ulRole         = 0;
    ULONG                     ulNextInsNum   = 0;

    pProc = (COSAGetHandleProc)pPlugInfo->AcquireFunction("COSAGetLPCRole");

    if ( pProc )
    {
        ulRole = (ULONG)(*pProc)(g_pDslhDmlAgent);
        CcspTraceWarning(("CosaMoCAInitialize - LPC role is %lu...\n", ulRole));
    }
#if 0
#ifdef _COSA_SIM_ 
    if ( ulRole == LPC_ROLE_MANAGER )
    {
        CcspTraceWarning(("CosaMoCAInitialize - AcquireFunction COSACreateSlapObject..."));
        pProc = (COSAGetHandleProc)pPlugInfo->AcquireFunction("COSACreateSlapObject");
    }
    else if ( ulRole == LPC_ROLE_PARTY )
    {
        CcspTraceWarning(("CosaMoCAInitialize - AcquireFunction COSAAcquirePoamObject..."));
        pProc = (COSAGetHandleProc)pPlugInfo->AcquireFunction("COSAAcquirePoamObject");
    }
    
    if ((ulRole == LPC_ROLE_MANAGER)||(ulRole == LPC_ROLE_PARTY))
    {
        if (NULL != pProc)
        {
            CcspTraceWarning(("succeeded!\n"));
        }
        else
        {
            CcspTraceWarning(("failed!\n"));
        }
    }

    if ( ulRole == LPC_ROLE_MANAGER )
    {
        CcspTraceWarning(("CosaMoCAInitialize - create slap object..."));
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapCOSAMoCADmGetSlapObjDescriptor((ANSC_HANDLE)NULL);
        pSlapMoCADm    = (*pProc)(pObjDescriptor);
        if (NULL != pSlapMoCADm)
        {
            g_pCosaBEManager->has_moca_slap = 1;

            CcspTraceWarning(("succeeded!\n"));
        }
        else
        {
            CcspTraceWarning(("failed!\n"));
        }
    }
    else if ( ulRole == LPC_ROLE_PARTY )
    {
        CcspTraceWarning(("CosaMoCAInitialize - create poam object..."));
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)PoamCOSAMoCADmGetPoamObjDescriptor((ANSC_HANDLE)NULL);
        pPoamMoCADm    = (*pProc)(pObjDescriptor);
        if (NULL != pPoamMoCADm)
        {
            CcspTraceWarning(("succeeded!\n"));
        }
        else
        {
            CcspTraceWarning(("failed!\n"));
        }
    }
    else if ( ulRole == LPC_ROLE_NONE )
    {
            g_pCosaBEManager->has_moca_slap = 1;
    }    
#elif (_COSA_DRG_TPG_)
    if ( ulRole != LPC_ROLE_NONE && ulRole != LPC_ROLE_INVALID )
    {
        CcspTraceWarning(("CosaMoCAInitialize - AcquireFunction COSACreateSlapObject..."));

        pProc = (COSAGetHandleProc)pPlugInfo->AcquireFunction("COSACreateSlapObject");
    
        if (NULL != pProc)
        {
            CcspTraceWarning(("succeeded!\n"));
        }
        else
        {
            CcspTraceWarning(("failed!\n"));
        }

        CcspTraceWarning(("CosaMoCAInitialize - create slap object..."));
    
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapCOSAMoCADmGetSlapObjDescriptor((ANSC_HANDLE)NULL);
    
        pSlapMoCADm    = (*pProc)(pObjDescriptor);
    
        if (NULL != pSlapMoCADm)
        {
            CcspTraceWarning(("succeeded!\n"));
        }
        else
        {
            CcspTraceWarning(("failed!\n"));
        }
    }
#elif (_COSA_DRG_CNS_)
    if ( ulRole != LPC_ROLE_NONE && ulRole != LPC_ROLE_INVALID )
    {
        CcspTraceWarning(("CosaMoCAInitialize - AcquireFunction COSAAcquirePoamObject..."));

        pProc = (COSAGetHandleProc)pPlugInfo->AcquireFunction("COSAAcquirePoamObject");

        if (NULL != pProc)
        {
            CcspTraceWarning(("succeeded!\n"));
        }
        else
        {
            CcspTraceWarning(("failed!\n"));
        }

        CcspTraceWarning(("CosaMoCAInitialize - create poam object..."));
    
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)PoamCOSAMoCADmGetPoamObjDescriptor((ANSC_HANDLE)NULL);
    
        pPoamMoCADm    = (*pProc)(pObjDescriptor);
    
        if (NULL != pPoamMoCADm)
        {
            CcspTraceWarning(("succeeded!\n"));
        }
        else
        {
            CcspTraceWarning(("failed!\n"));
        }
    }
#endif
#endif

    pMyObject->pSlapMoCADm = (ANSC_HANDLE)pSlapMoCADm;
    pMyObject->pPoamMoCADm = (ANSC_HANDLE)pPoamMoCADm;

    CosaDmlMocaInit(NULL, (ANSC_HANDLE)&pMyObject);

    CosaDmlMocaGetCfg(NULL, &pMyObject->MoCACfg);

    _ansc_memset(pMyObject->MoCAIfFullTable, 0, sizeof(COSA_DML_MOCA_IF_FULL_TABLE) * MOCA_INTEFACE_NUMBER);

    ulCount = CosaDmlMocaGetNumberOfIfs((ANSC_HANDLE)pPoamMoCADm);
    if ( ulCount > MOCA_INTEFACE_NUMBER )
    {
        AnscTraceWarning(("CosaMoCAInitialize -- ERROR!!!!! the real MoCA interface number(%lu) is bigger than predefined number(%d).\n", ulCount, MOCA_INTEFACE_NUMBER));
        assert(ulCount <= MOCA_INTEFACE_NUMBER );
    }
    
    ulNextInsNum = 1;

    for ( ulIndex = 0; ulIndex < ulCount; ulIndex++ )
    {
        CosaDmlMocaIfGetEntry((ANSC_HANDLE)pPoamMoCADm, ulIndex, &pMyObject->MoCAIfFullTable[ulIndex].MoCAIfFull);
        
        AnscSListInitializeHeader( &pMyObject->MoCAIfFullTable[ulIndex].pMoCAExtCounterTable );
        AnscSListInitializeHeader( &pMyObject->MoCAIfFullTable[ulIndex].pMoCAExtAggrCounterTable );
        AnscSListInitializeHeader( &pMyObject->MoCAIfFullTable[ulIndex].MoCAMeshTxNodeTable );

        CosaDmlMocaIfGetQos(NULL, ulIndex, &pMyObject->MoCAIfFullTable[ulIndex].MoCAIfQos);
        
        pMyObject->MoCAIfFullTable[ulIndex].MoCAIfFull.Cfg.InstanceNumber = ulNextInsNum++;    
    }
    CosaMoCAUpdate();
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaMoCARemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa MoCA object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaMoCARemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS               returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_MOCA      pMyObject         = (PCOSA_DATAMODEL_MOCA)hThisObject;
    /*PPOAM_COSAMOCADM_OBJECT*/ANSC_HANDLE   pSlapMoCADm       = (/*PPOAM_COSAMOCADM_OBJECT*/ANSC_HANDLE )pMyObject->pSlapMoCADm;
    /*PSLAP_COSAMOCADM_OBJECT*/ANSC_HANDLE   pPoamMoCADm       = (/*PSLAP_COSAMOCADM_OBJECT*/ANSC_HANDLE )pMyObject->pPoamMoCADm;
    COSAGetHandleProc         pProc             = (COSAGetHandleProc       )NULL;
    PCOSA_PLUGIN_INFO         pPlugInfo         = (PCOSA_PLUGIN_INFO       )g_pCosaBEManager->hCosaPluginInfo;
    PSLAP_OBJECT_DESCRIPTOR   pObjDescriptor    = (PSLAP_OBJECT_DESCRIPTOR )NULL;
    ULONG                     i                 = 0;    
#if 0
    /* Remove Poam or Slap resounce */
    if ( pSlapMoCADm )
    {    
        pProc = (COSAGetHandleProc)pPlugInfo->AcquireFunction("COSAReleaseSlapObject");

        if ( pProc )
        {
            pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapCOSAMoCADmGetSlapObjDescriptor((ANSC_HANDLE)NULL);
            (*pProc)(pSlapMoCADm, pObjDescriptor);
        }
    }

    if ( pPoamMoCADm )
    {
        /*pPoamMoCADm->Remove((ANSC_HANDLE)pPoamMoCADm);*/
    }
    
    /* Remove necessary resounce */
    for (i = 0; i<MOCA_INTEFACE_NUMBER; i++)
    {
        if (pMyObject->MoCAIfFullTable[i].pMoCAAssocDevice)
        {
            AnscFreeMemory(pMyObject->MoCAIfFullTable[i].pMoCAAssocDevice);
            pMyObject->MoCAIfFullTable[i].pMoCAAssocDevice = NULL;
        }
    }
#endif
    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}
