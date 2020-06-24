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

#ifndef  _COSA_NAT_WEBCONFIG_API_H
#define  _COSA_NAT_WEBCONFIG_API_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "ansc_status.h"
#include "dmz_wan_doc.h"
#include "webconfig_framework.h"


void init_dmz_cache(t_cache *tmp_dmz_cache);
void clear_dmz_cache(t_cache *tmp_dmz_cache);
void print_dmz_cache(t_cache *tmp_dmz_cache);
int clear_dmz_cache_DB(t_cache *tmp_dmz_cache);
void apply_dmz_ToDML(t_cache *tmp_dmz_cache);
void backup_dmz_cache(t_cache *tmp_dmz_cache,t_cache *tmp_dmz_cache_bkup,int cache_size);
int apply_dmz_cache_ToDB(t_cache *tmp_dmz_cache, int cache_size);
pErr Process_DMZ_WebConfigRequest(void *Data);
int set_dmz_conf(dmz_wandoc_t *rpm);
int rollback_dmz() ;
void freeResources_dmz(void *arg);
#endif

