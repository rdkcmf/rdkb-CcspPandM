/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2021 RDK Management
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


#include "cosa_GRE_dml.h"

#include "cosa_GRE_webconfig_apis.h"
#include "ccsp_psm_helper.h"
#include "safec_lib_common.h"
#include <rbus.h>

#define RDKB_WIFI_COMPONENT_NAME                  "com.cisco.spvtg.ccsp.wifi"
#define RDKB_WIFI_DBUS_PATH                       "/com/cisco/spvtg/ccsp/wifi"
#define WEBCONFIG_ENABLE                          "eRT.com.cisco.spvtg.ccsp.webpa.WebConfigRfcEnable"

extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
extern int gBroadcastSubscribed;

BOOL unpackAndProcessHotspotData(char* pString)
{

    CcspTraceWarning(("Entering %s\n",__FUNCTION__));
    char * decodeMsg =NULL;
    char * wifi_encoded_data = NULL;
    int size =0;
    int retval = 0;
    errno_t rc = -1;
    msgpack_unpack_return unpack_ret = MSGPACK_UNPACK_SUCCESS;

    retval = get_base64_decodedbuffer(pString, &decodeMsg, &size);
    if (retval == 0)
    {
        unpack_ret = get_msgpack_unpack_status(decodeMsg,size);
    }
    else
    {
        if (decodeMsg)
        {
	    free(decodeMsg);
	    decodeMsg = NULL;
        }
        return FALSE;
    }
    if(unpack_ret == MSGPACK_UNPACK_SUCCESS)
    {   
        hotspotparam_t *hd = NULL;
        tunneldoc_t * td = NULL;
        wifi_doc_t* wd = NULL;

        int td_count = 0;
        int i, j=0;
        hd = hotspotdoc_convert( decodeMsg, size+1 );

        if ( decodeMsg )
        {
                free(decodeMsg);
                decodeMsg = NULL;
        }

        if (NULL !=hd)
        {
            char filename[64]={0};
            FILE    *fp = NULL;
            CcspTraceWarning(("The transaction id is %d\n", hd->transaction_id));
            CcspTraceWarning(("The version is %lu\n", (long)hd->version));
            CcspTraceWarning(("The subdoc_name is %s\n", hd->subdoc_name));

            //Tunnel Data
            td = tunneldoc_convert( hd->entries[0].value, hd->entries[0].value_size );

            //Wifi Vap Data
            wd = wifi_doc_convert( hd->entries[1].value, hd->entries[1].value_size);

            CcspTraceWarning(("hd->entries[0].name is %s\n", hd->entries[0].name));
            if(td != NULL)
            {
                for(i = 0; i<(int) td->entries_count ; i++)
                {
                    CcspTraceWarning(("td->entries[%d].gre_primary_endpoint is %s\n", i, td->entries[i].gre_primary_endpoint));
                    CcspTraceWarning(("td->entries[%d].gre_sec_endpoint is %s\n", i, td->entries[i].gre_sec_endpoint));
                    CcspTraceWarning(("td->entries[%d].gre_dscp is %d\n", i, td->entries[i].gre_dscp));
                    CcspTraceWarning(("td->entries[%d].gre_enable %s\n", i, (1 == td->entries[i].gre_enable)?"true":"false"));
                    if(td->entries[i].table_param != NULL)
                    {
                        td_count = (int)td->entries[i].table_param->entries_count;

                        for(j = 0; j<(int) td->entries[i].table_param->entries_count; j++)
                        {
                            CcspTraceWarning(("td->entries[%d].table_param->entries[%d].vap_name %s\n",i, j, td->entries[i].table_param->entries[j].vap_name));
                            CcspTraceWarning(("td->entries[%d].table_param->entries[%d].wan_vlan %d\n",i, j,td->entries[i].table_param->entries[j].wan_vlan));
                            CcspTraceWarning(("td->entries[%d].table_param->entries[%d].enable %s\n",i, j,(1 == td->entries[i].table_param->entries[j].enable)?"true":"false"));
                        }
                    }
                    CcspTraceWarning(("td_count is %d\n", td_count));
                }
            }
            CcspTraceWarning(("hd->entries[1].name is %s\n", hd->entries[1].name));

            //wifi_encoded_data is the b64 encoded data obtained after appending subdoc_name, version and transaction_id
            wifi_encoded_data = append_wifi_doc(hd->subdoc_name,hd->version,hd->transaction_id,hd->entries[1].value, hd->entries[1].value_size);
            CcspTraceWarning(("The wifi_encoded_data is %s\n", wifi_encoded_data));
            if(wd != NULL)
            {
                CcspTraceWarning(("wd->entries_count is %d\n", (int)wd->entries_count));
                if(td_count == (int)wd->entries_count)
                {
                    CcspTraceWarning(("tunnel and wifi vap count matching\n"));
                }
                else
                {
                    CcspTraceWarning(("tunnel and wifi vap count NOT matching!!\n"));
                    send_NACK(hd->subdoc_name,hd->transaction_id,(uint32_t)hd->version,TUNNEL_WIFI_VAP_COUNT_MISMATCH,"Tunnel and Wifi vap count NOT matching");
                    if(td != NULL)
                    {
                        tunneldoc_destroy(td);
                    }
                    if(wd != NULL)
                    {
                        wifi_doc_destroy(wd);
                    }

                    if(hd != NULL)
                    {
                        hotspotdoc_destroy(hd);
                    }
                    return FALSE;
                }
            }
            execData *execDataHotspot = NULL;
            policySequence *sequenceDetails = NULL;
            execDataHotspot = (execData*) malloc (sizeof(execData));

            memset(execDataHotspot, 0, sizeof(execData));

            sequenceDetails = (policySequence*) malloc (sizeof (policySequence) );
            memset(sequenceDetails, 0, sizeof(policySequence));
            sequenceDetails->isExecInSequenceNeeded = 1 ;
            sequenceDetails->numOfComponents = 2 ;

            // Post exec settings
            sequenceDetails->postExecSettings = confirmVap ;

            sequenceDetails->multiCompExecData = (MultiComp_ExecInfo*) malloc (sequenceDetails->numOfComponents * sizeof(MultiComp_ExecInfo));
            memset(sequenceDetails->multiCompExecData, 0, sequenceDetails->numOfComponents * sizeof(MultiComp_ExecInfo));

            sequenceDetails->multiCompExecData->isMaster = 1 ;
            sequenceDetails->multiCompExecData->comp_exec_data = (void*) td ;
            CcspTraceWarning(("DEBUG : master data %p\n",sequenceDetails->multiCompExecData->comp_exec_data));


            sequenceDetails->multiCompExecData++;
            sequenceDetails->multiCompExecData->isMaster =0 ;

#if defined (RDK_ONEWIFI)
            strncpy(sequenceDetails->multiCompExecData->CompName,"OneWifi",sizeof(sequenceDetails->multiCompExecData->CompName)-1);
#else
            strncpy(sequenceDetails->multiCompExecData->CompName,"CcspWifiSsp",sizeof(sequenceDetails->multiCompExecData->CompName)-1);
#endif
	    CcspTraceWarning(("DEBUG : wifi_encoded_data %p\n",wifi_encoded_data));

            sequenceDetails->multiCompExecData->comp_exec_data = (void*) wifi_encoded_data ;

            CcspTraceWarning(("DEBUG : sequenceDetails->multiCompExecData->comp_exec_data pointer is %p\n",sequenceDetails->multiCompExecData->comp_exec_data));

            sequenceDetails->multiCompExecData--;


            execDataHotspot->txid = hd->transaction_id; 
            execDataHotspot->version = (uint32_t)hd->version; 
            execDataHotspot->numOfEntries = td->entries_count; 

            strncpy(execDataHotspot->subdoc_name,"hotspot",sizeof(execDataHotspot->subdoc_name)-1);
            execDataHotspot->executeBlobRequest = setHotspot;
            execDataHotspot->rollbackFunc = deleteHotspot ;
            execDataHotspot->freeResources = freeMem_hotspot ;
            execDataHotspot->calcTimeout = calculateTimeout ;
            execDataHotspot->user_data = (void*) sequenceDetails;
            execDataHotspot->multiCompRequest =1 ;

            memset(filename,0,sizeof(filename));
            rc = sprintf_s(filename,sizeof(filename),"/tmp/.%s%u",execDataHotspot->subdoc_name,execDataHotspot->version);
            if(rc < EOK) ERR_CHK(rc);
            fp = fopen(filename,"w") ;
            if (fp != NULL )
            {
                fprintf(fp, "%s",pString);
                fclose(fp);
            }
            PushMultiCompBlobRequest(execDataHotspot);


        }
        // freeResources api will free this memory
        #if 0
        if(td != NULL)
        {
            tunneldoc_destroy(td);
        }
        #endif
        if(wd != NULL)
        {
            wifi_doc_destroy(wd);
        }

        if(hd != NULL)
        {
            hotspotdoc_destroy(hd);
        }
    }
    else
    {
        if ( decodeMsg )
        {
            free(decodeMsg);
            decodeMsg = NULL;
        }

        CcspTraceWarning(("Corrupted hotspotdoc value\n"));
        return FALSE;
    }
        return TRUE;

}


static void checkComponentHealthStatus(char *compName, char * dbusPath, char *status, int *retStatus)
{
        int ret = 0, val_size = 0;
        parameterValStruct_t **parameterval = NULL;
        char *parameterNames[1] = {};
        char param_name[256] = {0};
        char wifi_comp_id[256] = {0};

        memset(param_name,0,sizeof(param_name));
        memset(wifi_comp_id,0,sizeof(wifi_comp_id));

        sprintf(param_name,"%s.%s",compName, "Health");
        parameterNames[0] = param_name;

        snprintf(wifi_comp_id, sizeof(wifi_comp_id), "eRT.%s", compName);
        CcspTraceInfo(("param_name is %p,dbusPath is %s,wifi_comp_id is %s\n", parameterNames,dbusPath,wifi_comp_id));

        ret = CcspBaseIf_getParameterValues(bus_handle, wifi_comp_id, dbusPath,  parameterNames, 1, &val_size, &parameterval);
        CcspTraceInfo(("ret = %d val_size = %d\n",ret,val_size));
        if(ret == CCSP_SUCCESS)
        {
                CcspTraceInfo(("parameterval[0]->parameterName : %s parameterval[0]->parameterValue : %s\n",parameterval[0]->parameterName,parameterval[0]->parameterValue));
                strcpy(status, parameterval[0]->parameterValue);
                CcspTraceInfo(("status of component:%s\n", status));
        }
        free_parameterValStruct_t (bus_handle, val_size, parameterval);

        *retStatus = ret;
} 

int checkIfSubscribedToBroadcast()
{
    if (gBroadcastSubscribed)
        return 0;
    else
        return -1;
}
void* initialize_hotspot_webconfig(void *arg)
{
    (void)(arg);
    pthread_detach(pthread_self());
    CcspTraceInfo(("Entering thread %s\n", __FUNCTION__));

    char comp_status[32] = {0};
    int ret = 0;
    while(1)
    {

        checkComponentHealthStatus(RDKB_WIFI_COMPONENT_NAME, RDKB_WIFI_DBUS_PATH, comp_status,&ret);
        if(ret == CCSP_SUCCESS && (strcmp(comp_status, "Green") == 0))
        {
            CcspTraceInfo(("WiFi component health is %s, continue\n", comp_status));
            break;
        }
        else
        {
            sleep(15);
        }   

    }

    // adding 5 sec delay so that in boot up wifi is ready to receive the requests
    sleep(5);

    if(RBUS_ENABLED == rbus_checkStatus()) 
    {   
        while ( 0 != checkIfSubscribedToBroadcast())
        {
            CcspTraceWarning(("Waiting for slave component to subscribe broadcast event \n"));
            sleep(30);
        }
        
        CcspTraceWarning(("Slave component subscribed to broadcast event,sending hotspot blob\n"));
    }
    char *blob = NULL ;
    FILE *fp;

    fp = fopen (HOTSPOT_BLOB_FILE, "r");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        int sz = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        CcspTraceInfo(("%s file size is %d\n",HOTSPOT_BLOB_FILE,sz));

        blob = (char*) malloc(sizeof(char) * (sz + 1));
        if (blob != NULL)
        {
            fscanf(fp,"%s",blob);
            unpackAndProcessHotspotData(blob);    
            free(blob);
            blob = NULL ;  
        }

        fclose(fp);
    }
   return NULL;
}


void wbInitializeHotspot()
{

    char *param_value= NULL;
    char webconf_enable[16] = {0};
    char cmd[64] = {0};
    if ( CCSP_SUCCESS == PSM_Get_Record_Value2(bus_handle, g_Subsystem,WEBCONFIG_ENABLE, NULL, &param_value) )
    {
        memset(webconf_enable,0,sizeof(webconf_enable));
        if( param_value )
        {
            CcspTraceWarning(("%s : WebConfig RFC value is %s\n",__FUNCTION__,param_value));
            strncpy(webconf_enable,param_value,sizeof(webconf_enable)-1);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
            param_value = NULL;
        }
    }
    
    if (strcmp(webconf_enable,"false") == 0)
    {
            CcspTraceWarning(("%s : WebConfig RFC is not enabled, returning\n",__FUNCTION__));
            return;
    }
    else if( access("/tmp/.hotspot_blob_executed", F_OK ) == 0 ) {
            CcspTraceWarning(("%s :hotspot_blob already executed in bootup, not executing again\n",__FUNCTION__));
            return;
    }
    else if (access(HOTSPOT_BLOB_FILE, F_OK ) != 0 )
    {
        CcspTraceWarning(("%s : %s not available , no need to init hotspot using webconfig\n",__FUNCTION__,HOTSPOT_BLOB_FILE));
        return;
    }
    else {

            /*
            if(syscfg_set_commit(NULL,"hotspot_version","0000") != 0)
            {
                CcspTraceError(("syscfg_set for hotspot_version failed\n"));
            }
            */
            memset(cmd,0,sizeof(cmd));
            snprintf(cmd,sizeof(cmd),"touch %s",HOTSPOT_VERSION_IGNORE);
            system(cmd);

        pthread_t hotspot_init_thread;
        pthread_create(&hotspot_init_thread,NULL,initialize_hotspot_webconfig,NULL);

    }
    return;
}


void freeMem_hotspot(void* arg){
    CcspTraceInfo((": Entering %s \n",__FUNCTION__));

    execData *exec_data_hotspot  = (execData*) arg;
    
    policySequence *sequenceDetails = (policySequence*)exec_data_hotspot->user_data;
    tunneldoc_t *td  =  (tunneldoc_t*) sequenceDetails->multiCompExecData->comp_exec_data;
    if ( td != NULL )
    {
        tunneldoc_destroy(td);  
    }
    sequenceDetails->multiCompExecData++;
    char * wifi_encoded_data = (char*) sequenceDetails->multiCompExecData->comp_exec_data ;
    if ( wifi_encoded_data != NULL )
    {
        free(wifi_encoded_data);  
        wifi_encoded_data = NULL;
    }
    sequenceDetails->multiCompExecData--;
    if (sequenceDetails->multiCompExecData)
    {
        free(sequenceDetails->multiCompExecData);
        sequenceDetails->multiCompExecData = NULL;
    }
    if (sequenceDetails)
    {
        free(sequenceDetails);
        sequenceDetails = NULL;
    }
    if ( exec_data_hotspot != NULL )
    {
        free(exec_data_hotspot);
        exec_data_hotspot = NULL ;
    }

}

