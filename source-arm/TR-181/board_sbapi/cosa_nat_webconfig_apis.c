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
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cosa_webconfig_api.h"
#include "webconfig_framework.h"
#include "cosa_nat_webconfig_apis.h"
#include "cosa_nat_apis.h"
#include "cosa_nat_internal.h"
#include "plugin_main_apis.h"
#include <syscfg/syscfg.h>
#include "safec_lib_common.h"

/* Initialize dmz cache , this API will be called once in boot up */
void init_dmz_cache(t_cache *tmp_dmz_cache)
{
	int i = 0;

	memset(tmp_dmz_cache[i].cmd,0,BLOCK_SIZE);
	memset(tmp_dmz_cache[i].val,0,VAL_BLOCK_SIZE);
	snprintf(tmp_dmz_cache[i].cmd,BLOCK_SIZE,"dmz_enabled");

	i++;
	memset(tmp_dmz_cache[i].cmd,0,BLOCK_SIZE);
	memset(tmp_dmz_cache[i].val,0,VAL_BLOCK_SIZE);
	snprintf(tmp_dmz_cache[i].cmd,BLOCK_SIZE,"dmz_dst_ip_addr");

}

/* API to clear the dmz buffer */
void clear_dmz_cache(t_cache *tmp_dmz_cache)
{
	int i = 0;
    	for(i = 0; i < DMZ_CACHE_SIZE; i++)
    	{
        	memset(tmp_dmz_cache[i].cmd,0,BLOCK_SIZE);
        	memset(tmp_dmz_cache[i].val,0,VAL_BLOCK_SIZE);
    	}
}

/* API to print dmz cache */
void print_dmz_cache(t_cache *tmp_dmz_cache)
{
	int i = 0;
    	for(i = 0;i <DMZ_CACHE_SIZE ; i++)
    	{
        	CcspTraceWarning(("dmz_cache[%d].cmd - %s\n",i,tmp_dmz_cache[i].cmd));
        	CcspTraceWarning(("dmz_cache[%d].val - %s\n",i,tmp_dmz_cache[i].val));
    	}

}

/* API to unset dmz entries from DB */
int clear_dmz_cache_DB(t_cache *tmp_dmz_cache)
{
	int i = 0;
    	for(i = 0; i < DMZ_CACHE_SIZE; i++)
    	{

        	if (syscfg_unset(NULL, tmp_dmz_cache[i].cmd) != 0)
        	{
                    CcspTraceError(("syscfg_set failed for %s %s\n",tmp_dmz_cache[i].cmd,tmp_dmz_cache[i].val));
                    return SYSCFG_FAILURE;
        	}
        	memset(tmp_dmz_cache[i].cmd,0,BLOCK_SIZE);
        	memset(tmp_dmz_cache[i].val,0,VAL_BLOCK_SIZE);
        	if (syscfg_commit() != 0)
        	{
            		CcspTraceError(("syscfg_commit failed apply_cache_ToDB\n"));
			return SYSCFG_FAILURE;
        	}
    	}
		
return 0;
}


/* API to back up the dmz cache */
void backup_dmz_cache(t_cache *tmp_dmz_cache,t_cache *tmp_dmz_cache_bkup,int cache_size)
{
	int i = 0;
    errno_t safec_rc = -1;
    	for(i = 0; i < cache_size; i++)
    	{
			safec_rc = strcpy_s(tmp_dmz_cache_bkup[i].cmd, BLOCK_SIZE, tmp_dmz_cache[i].cmd);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
			safec_rc = strcpy_s(tmp_dmz_cache_bkup[i].val, VAL_BLOCK_SIZE, tmp_dmz_cache[i].val);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}

		 CcspTraceWarning(("dmz_cache backup [%d].cmd - %s\n",i,tmp_dmz_cache_bkup[i].cmd));
		 CcspTraceWarning(("dmz_cache backup [%d].val - %s\n",i,tmp_dmz_cache_bkup[i].val));

   	 } 
}


/* API to apply port forwarding requests to DB */
int apply_dmz_cache_ToDB(t_cache *tmp_dmz_cache, int cache_size)
{
	int i = 0;
    	for(i = 0;i <cache_size ; i++)
    	{
        	CcspTraceWarning(("dmz_cache apply to DB [%d].cmd - %s\n",i,tmp_dmz_cache[i].cmd));
        	CcspTraceWarning(("dmz_cache apply to DB [%d].val - %s\n",i,tmp_dmz_cache[i].val));

        	if (syscfg_set(NULL, tmp_dmz_cache[i].cmd, tmp_dmz_cache[i].val) != 0)
        	{
               		CcspTraceError(("DMZ syscfg_set failed for %s %s\n",tmp_dmz_cache[i].cmd,tmp_dmz_cache[i].val));
               		return SYSCFG_FAILURE;
        	}
    	}

    	if (syscfg_commit() != 0)
    	{
        	CcspTraceError(("DMZ syscfg_commit failed apply_cache_ToDB\n"));
        	return SYSCFG_FAILURE;
    	}

#ifdef WEBCFG_TEST_SIM
            int fd = -1;
            // to simulate NACK
            fd = access(NACK_SIMULATE_FILE, F_OK); 
            if(fd == 0)
            { 
                    unlink(NACK_SIMULATE_FILE);
                    return SYSCFG_FAILURE;

            }
            fd = -1;

            // to simulate timeout and maxtimeout cases
            FILE *fp;
            int interval = 15 ;
            fp = fopen (TIMEOUT_SIMULATE_FILE, "r");
            if( fp )
            {
                fscanf(fp, "%d", &interval);
                if ( interval <= 0 )
                    interval = 15;

                sleep(interval);
                fclose(fp);
                fp=NULL; 
                unlink(TIMEOUT_SIMULATE_FILE);

            }
#endif 

return 0;
}



/* CallBack API to execute Portforwarding Blob request */
pErr Process_DMZ_WebConfigRequest(void *Data)
{
    	pErr execRetVal = NULL;
	int index = 0;

    	execRetVal = (pErr) malloc (sizeof(Err));
    	if (execRetVal == NULL )
    	{
        	CcspTraceError(("%s : malloc failed\n",__FUNCTION__));
        	return execRetVal;
    	}

    	memset(execRetVal,0,sizeof(Err));

    	execRetVal->ErrorCode = BLOB_EXEC_SUCCESS;

	dmz_wandoc_t *rpm = (dmz_wandoc_t *) Data ;

    	CcspTraceWarning(("DMZ configurartion recieved!!!!!!\n"));

    	backup_dmz_cache(dmz_cache,dmz_cache_bkup,DMZ_CACHE_SIZE);

        if ( SYSCFG_FAILURE == clear_dmz_cache_DB(dmz_cache) )
        {
                CcspTraceError(("%s: Failed to clear_dmz_cache_DB\n",__FUNCTION__));
//                dmz_wandoc_destroy( rpm ); 

                execRetVal->ErrorCode = SYSCFG_FAILURE;

                strncpy(execRetVal->ErrorMsg,"sysconfig failure while clearing DMZ old entries",sizeof(execRetVal->ErrorMsg)-1);

                return execRetVal;
        }

       int ret  = set_dmz_conf(rpm) ;
    	if ( 0 != ret )
        {
            if ( INVALID_DMZ_WAN_IP == ret )
            {
                CcspTraceError(("%s : Invalid dmz wan ip...\n",__FUNCTION__));
                execRetVal->ErrorCode = INVALID_DMZ_WAN_IP;

                strncpy(execRetVal->ErrorMsg,"Invalid IP",sizeof(execRetVal->ErrorMsg)-1);

            }
//            dmz_wandoc_destroy( rpm );

            return execRetVal;
        }

    	if ( SYSCFG_FAILURE == apply_dmz_cache_ToDB(dmz_cache,DMZ_CACHE_SIZE) )
    	{
            	CcspTraceError(("Failed to apply DMZ configurartion!!!!!!!\n"));
//            	dmz_wandoc_destroy( rpm );

            	execRetVal->ErrorCode = SYSCFG_FAILURE;

            	strncpy(execRetVal->ErrorMsg,"sysconfig failure while applying",sizeof(execRetVal->ErrorMsg)-1);

            	return execRetVal;

    	}

     	CcspTraceWarning(("DMZ configurartion applied!!!!!!!!\n"));

	apply_dmz_ToDML(dmz_cache);

    system("sysevent set firewall-restart");
	CcspTraceWarning(("DMZ configurartion applied to DML layer!!!!!!!!\n"));

	CcspTraceWarning(("rpm->param[%d].Enable %s\n", index, (rpm->param[index].enable == TRUE)?"true":"false"));
	CcspTraceWarning(("rpm->entries[%d].internal_ip %s\n" , index, rpm->param[index].internal_ip));

//        dmz_wandoc_destroy( rpm );

return execRetVal;
}


/* Read blob entries into a cache  for DMZ*/ 
int set_dmz_conf(dmz_wandoc_t *rpm)
{
	int count = 0;
	int index = 0;
	ULONG	dmzHost;

	snprintf(dmz_cache[count].cmd,BLOCK_SIZE,"dmz_enabled");
	snprintf(dmz_cache[count++].val,VAL_BLOCK_SIZE,"%d",rpm->param[index].enable);

	if (rpm->param[index].enable == FALSE)
	{
		snprintf(dmz_cache[count].cmd,BLOCK_SIZE,"dmz_dst_ip_addr");
		snprintf(dmz_cache[count++].val,VAL_BLOCK_SIZE,"%s","0.0.0.0");
	}
	else if(rpm->param[index].enable == TRUE)
	{
		if(rpm->param[index].internal_ip)
		{
			CcspTraceInfo(("CheckIfIpIsValid:IpAddressReceivedIs:%s\n",rpm->param[index].internal_ip));
			dmzHost = (ULONG)_ansc_inet_addr(rpm->param[index].internal_ip);
			if(FALSE == CosaDmlNatChkPortMappingClient(dmzHost))
			{
				CcspTraceWarning((" ' %s 'dmz host not in local lan network  \n",__FUNCTION__));
				return INVALID_DMZ_WAN_IP;
			}
			else
			{
				if (IPv4Addr_IsLoopback(dmzHost) || IPv4Addr_IsMulticast(dmzHost) || (dmzHost == 0xffffffff))
				{
					CcspTraceWarning((" ' %s 'dmz host not in local lan network  \n",__FUNCTION__));
					return INVALID_DMZ_WAN_IP;
				}
				snprintf(dmz_cache[count].cmd,BLOCK_SIZE,"dmz_dst_ip_addr");
				snprintf(dmz_cache[count++].val,VAL_BLOCK_SIZE,"%s",rpm->param[index].internal_ip);
			}
		}
		else
		{
			CcspTraceWarning((" ' %s ' Ip is not present inside blob\n",__FUNCTION__));
			return INVALID_DMZ_WAN_IP;
		}
	}

return 0;
}

/* save data in dml layer*/
void apply_dmz_ToDML(t_cache *tmp_dmz_cache)
{
	PCOSA_DATAMODEL_NAT             pNat         = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
	PCOSA_DML_NAT_DMZ               pDmlDmz         = &pNat->Dmz;
	int index = 0;
	errno_t                         rc     = -1;
	pDmlDmz->bEnabled = atoi(tmp_dmz_cache[index++].val);
	if (pDmlDmz->bEnabled == FALSE)
	{
		/* keep sync between webui and snmp */
		rc = strcpy_s(pDmlDmz->InternalIP,sizeof(pDmlDmz->InternalIP), "0.0.0.0");
		ERR_CHK(rc);
		//CISCOXB3-5927 : ip6 table is not getting restored
		memset(pDmlDmz->IPv6Host ,0 ,sizeof(pDmlDmz->IPv6Host));
	}
	else if (pDmlDmz->bEnabled == TRUE)
	{
                /* CID: 158833 Array compared against 0*/
	 	rc = strcpy_s( pDmlDmz->InternalIP,sizeof(pDmlDmz->InternalIP), tmp_dmz_cache[index].val);
	 	ERR_CHK(rc);
	}

#if CFG_USE_CCSP_SYSLOG
	/* Bad practice to use platform dependent and will be rectified -- CCSP_TRACE should be used */
	syslog_systemlog("DMZ", LOG_NOTICE, "%s", (pDmlDmz->bEnabled==TRUE)?"Enabled":"Disabled");
#endif
	CcspTraceNotice(("DMZ_split:Feature Switch DMZ %d\n", pDmlDmz->bEnabled));


}


/* Callback function to rollback when DMZ blob execution fails */
int rollback_dmz()
{
    // return 0 to notify framework when rollback is success
    CcspTraceInfo((" Entering %s \n",__FUNCTION__));

    int ret = 0;
    ret = apply_dmz_cache_ToDB(dmz_cache_bkup,DMZ_CACHE_SIZE);

    backup_dmz_cache(dmz_cache_bkup,dmz_cache,DMZ_CACHE_SIZE);

    return ret ;
}

void freeResources_dmz(void *arg)
{
    CcspTraceInfo((" Entering %s \n",__FUNCTION__));
    execData *blob_exec_data  = (execData*) arg;
    /*CID: 158837 Dereference before null check*/
    if(!blob_exec_data)
       return;
    dmz_wandoc_t *rpm = (dmz_wandoc_t *) blob_exec_data->user_data;

    if ( rpm != NULL )
    {
        dmz_wandoc_destroy( rpm );
        rpm = NULL;
    }

    if ( blob_exec_data != NULL )
    {
        free(blob_exec_data);
        blob_exec_data = NULL ;
    }
}

