/************************************************************************************
  If not stated otherwise in this file or this component's Licenses.txt file the
  following copyright and licenses apply:

  Copyright 2018 RDK Management

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
**************************************************************************/

/**************************************************************************

    module: cosa_x_comcast-com_gre_internal.c

        For COSA Data Model Library Development

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
#include "cosa_x_comcast-com_gre_internal.h"
#include "cosa_x_comcast-com_gre_apis.h"
#include "libHotspotApi.h"
#include "cosa_deviceinfo_internal.h"
#define SIZE_OF_IP 16
#define POLL_CIRCUIT_ID_SLEEP 3

int hotspot_update_circuit_ids(int,int);
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

void* update_circuitID_thread(void* arg)
{
    UNREFERENCED_PARAMETER(arg);
    static BOOL running=0;
    int ret=-1;
    if(!running)
    {
        running=1;
        CcspTraceInfo(("%s:hotspot_update_circuit_ids starting\n", __FUNCTION__));
        ret = hotspot_update_circuit_ids(1, 1);
        while (ret < 0)
        {
            sleep(POLL_CIRCUIT_ID_SLEEP);
            CcspTraceWarning(("%s:Retrying hotspot_update_circuit_ids\n", __FUNCTION__));
            ret = hotspot_update_circuit_ids(1, 1);
        }
        running=0;
    }
    else
    {
        CcspTraceWarning(("%s: already hotspot_update_circuit_ids is running\n", __FUNCTION__));
        return NULL;
    }
    return NULL;
}

void callbackWCConfirmVap(tunnelSet_t *tunnelSet){

   pthread_t circuitid_thread;

   CcspTraceWarning(("HOTSPOT_LIB: Entering '%s'\n", __FUNCTION__));

   COSA_DATAMODEL_GRE2           *pGreMyObject   = (COSA_DATAMODEL_GRE2 *)g_pCosaBEManager->hTGRE;
   PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

   if((NULL == pGreMyObject) || (NULL == pMyObject) || (NULL == tunnelSet)){
      CcspTraceWarning(("HOTSPOT_LIB : Datamodel null \n"));
      return;
   }

   CcspTraceWarning(("primary ip in '%s'\n",tunnelSet->set_primary_endpoint ));
   strncpy(pGreMyObject->GreTu[0].PrimaryRemoteEndpoint, tunnelSet->set_primary_endpoint, SIZE_OF_IP);
   pGreMyObject->GreTu[0].ChangeFlag |= GRETU_CF_PRIEP;
   strncpy(pGreMyObject->GreTu[0].SecondaryRemoteEndpoint, tunnelSet->set_sec_endpoint, SIZE_OF_IP);
   pGreMyObject->GreTu[0].ChangeFlag |= GRETU_CF_SECEP;
   pGreMyObject->GreTu[0].Enable = tunnelSet->set_gre_enable;
   pGreMyObject->GreTu[0].ChangeFlag |= GRETU_CF_ENABLE;
   pGreMyObject->GreTu[0].GreTunnelIf[0].VLANID = tunnelSet->vlan_id_list[0];
   pGreMyObject->GreTu[0].GreTunnelIf[0].ChangeFlag |= GRETUIF_CF_VLANID;
   pGreMyObject->GreTu[0].GreTunnelIf[1].VLANID = tunnelSet->vlan_id_list[1];
   pGreMyObject->GreTu[0].GreTunnelIf[1].ChangeFlag |= GRETUIF_CF_VLANID;
   pMyObject->bxfinitywifiEnable = tunnelSet->set_gre_enable;
   
   if(NULL != tunnelSet){
       free(tunnelSet);
       tunnelSet = NULL;
   }

   if(0 != pthread_create(&circuitid_thread, NULL, update_circuitID_thread, NULL))
   {
       CcspTraceError(("%s: Error in creating update_circuitID_thread \n", __FUNCTION__));
   }
   else
   {
       pthread_detach(circuitid_thread);
   }
}
#endif
