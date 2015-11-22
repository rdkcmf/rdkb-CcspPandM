/**********************************************************************
   Copyright [2015] [Comcast Corp.]
 
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

    module: cosa_x_comcast-com_gre_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Comcast Corp.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        zhicheng_qiu@cable.comcast.com

    -------------------------------------------------------------------

    revision:

        05/18/2015    initial revision.

**************************************************************************/

#include "dml_tr181_custom_cfg.h"

#ifdef CONFIG_CISCO_HOTSPOT
#include "plugin_main_apis.h"
#include "cosa_x_comcast_com_gre_internal.h"

extern void* g_pDslhDmlAgent;

ANSC_HANDLE
CosaGreTunnelCreate
    (
        VOID
    )
{
    //ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    COSA_DATAMODEL_GRE2          *pMyObject   = NULL;

    pMyObject = AnscAllocateMemory(sizeof(COSA_DATAMODEL_GRE2));
    if (!pMyObject)
    {
        return NULL;
    }

    pMyObject->Oid               = COSA_DATAMODEL_GRE_OID; // TODO: COSA_DATAMODEL_GRE2_OID;
    pMyObject->Create            = CosaGreTunnelCreate;
    pMyObject->Remove            = CosaGreTunnelRemove;
    pMyObject->Initialize        = CosaGreTunnelInitialize;
	printf("-- %s CosaGreTunnelInitialize >>\n", __func__);
    pMyObject->Initialize((ANSC_HANDLE)pMyObject);
	printf("-- %s CosaGreTunnelInitialize <<\n", __func__);
    return (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaGreTunnelInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
	COSA_DATAMODEL_GRE2            *gre = (COSA_DATAMODEL_GRE2 *)hThisObject;
    ULONG                           tuCnt, i, tuIns;//,  nextIns;
	
			
    if (CosaDml_GreTunnelInit() != ANSC_STATUS_SUCCESS)		//only init tunnel
        return ANSC_STATUS_FAILURE;
	
    memset(gre->GreTu, 0, sizeof(COSA_DML_GRE_TUNNEL) * MAX_GRE_TU);
    tuCnt = CosaDml_GreTunnelGetNumberOfEntries();
    //nextIns = 1;

    for (i = 0, tuIns=1; i < tuCnt; i++, tuIns++)
    {
        if (CosaDml_GreTunnelGetEntryByIndex(tuIns, &gre->GreTu[i]) != ANSC_STATUS_SUCCESS)
            return ANSC_STATUS_FAILURE;			
		CosaGreTunnelIfInitialize(tuIns, (ANSC_HANDLE)(&gre->GreTu[i]));
    }

EXIT:	
    return returnStatus;
}

ANSC_STATUS
CosaGreTunnelIfInitialize
    (
		ULONG						tuIns,
        ANSC_HANDLE                 hThisObject
    )
{
    COSA_DML_GRE_TUNNEL              *tu = (COSA_DML_GRE_TUNNEL *)hThisObject;
    ULONG                           ifCnt, i, ins;//, nextIns;

    memset(tu->GreTunnelIf, 0, sizeof(COSA_DML_GRE_TUNNEL_IF) * MAX_GRE_TUIF);

	//TODO: not hardcode tunnel id
    ifCnt = CosaDml_GreTunnelIfGetNumberOfEntries(1);
    //nextIns = 1;

    for (i = 0, ins=1; i < ifCnt; i++, ins++)
    {
        if (CosaDml_GreTunnelIfGetEntryByIndex(tuIns, ins, &tu->GreTunnelIf[i]) != 0)
            return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaGreTunnelRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    COSA_DATAMODEL_GRE2              *gre = (COSA_DATAMODEL_GRE2 *)hThisObject;

    if (CosaDml_GreTunnelFinalize() != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    if (gre)
        AnscFreeMemory(gre);
    return ANSC_STATUS_SUCCESS;
}

#endif
