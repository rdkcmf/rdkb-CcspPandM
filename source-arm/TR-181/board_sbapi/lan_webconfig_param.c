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
#include <errno.h>
#include <string.h>
#include <msgpack.h>
#include <stdarg.h>
#include "ccsp_trace.h"
#include "helpers.h"
#include "lan_webconfig_param.h"
/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
/* none */
/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/
enum {
    OK                       = HELPERS_OK,
    OUT_OF_MEMORY            = HELPERS_OUT_OF_MEMORY,
    INVALID_FIRST_ELEMENT    = HELPERS_INVALID_FIRST_ELEMENT,
    MISSING_ENTRY         = HELPERS_MISSING_WRAPPER,
    INVALID_INTERNAL_IPV4,
    BOTH_IPV4_AND_IPV6_TARGETS_EXIST,
    INVALID_IPV6,
    MISSING_INTERNAL_PORT,
    INVALID_DATATYPE,
    MISSING_TARGET_IP,
    MISSING_PROTOCOL,
    INVALID_OBJECT,
    INVALID_VERSION,
};
/*----------------------------------------------------------------------------*/
/*                            File Scoped Variables                           */
/*----------------------------------------------------------------------------*/
/* none */
/*----------------------------------------------------------------------------*/
/*                             Function Prototypes                            */
/*----------------------------------------------------------------------------*/
int process_lanparams( lanparam_t *e, msgpack_object_map *map );
int process_landoc( landoc_t *ld, int num, ...); 
/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
/* See landoc.h for details. */
landoc_t* landoc_convert( const void *buf, size_t len )
{
	return helper_convert( buf, len, sizeof(landoc_t), "lan", 
                            MSGPACK_OBJECT_MAP, true,
                           (process_fn_t) process_landoc,
                           (destroy_fn_t) landoc_destroy );
}
/* See landoc.h for details. */
void landoc_destroy( landoc_t *ld )
{
	if( NULL != ld )
	{
		if( NULL != ld->param )
		{
			if( NULL != ld->param->lan_ip_address )
			{
				free( ld->param->lan_ip_address );
			}
			if( NULL != ld->param->lan_subnet_mask )
			{
				free( ld->param->lan_subnet_mask );
			}
			if( NULL != ld->param->dhcp_start_ip_address )
			{
				free( ld->param->dhcp_start_ip_address );
			}
			if( NULL != ld->param->dhcp_end_ip_address )
			{
				free( ld->param->dhcp_end_ip_address );
			}
			free(ld->param);
			
		}
		if( NULL != ld->subdoc_name )
		{
			free( ld->subdoc_name );
		}
		free( ld );
	}
}
/* See landoc.h for details. */
const char* landoc_strerror( int errnum )
{
    struct error_map {
        int v;
        const char *txt;
    } map[] = {
        { .v = OK,                               .txt = "No errors." },
        { .v = OUT_OF_MEMORY,                    .txt = "Out of memory." },
        { .v = INVALID_FIRST_ELEMENT,            .txt = "Invalid first element." },
        { .v = INVALID_VERSION,                 .txt = "Invalid 'version' value." },
        { .v = INVALID_OBJECT,                .txt = "Invalid 'value' array." },
        { .v = 0, .txt = NULL }
    };
    int i = 0;
    while( (map[i].v != errnum) && (NULL != map[i].txt) ) { i++; }
    if( NULL == map[i].txt )
    {
	CcspTraceWarning(("----landoc_strerror----\n"));
        return "Unknown error.";
    }
    return map[i].txt;
}
/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/
/**
 *  Convert the msgpack map into the doc_t structure.
 *
 *  @param e    the entry pointer
 *  @param map  the msgpack map pointer
 *
 *  @return 0 on success, error otherwise
 */
int process_lanparams( lanparam_t *e, msgpack_object_map *map )
{
    int left = map->size;
    uint8_t objects_left = 0x06;
    msgpack_object_kv *p;
    p = map->ptr;
    while( (0 < objects_left) && (0 < left--) )
    {
        if( MSGPACK_OBJECT_STR == p->key.type )
        {
              if( MSGPACK_OBJECT_BOOLEAN == p->val.type )
              {
                 if( 0 == match(p, "DhcpServerEnable") )
                 {
                     e->dhcp_server_enable = p->val.via.boolean;
                     objects_left &= ~(1 << 0);
                 }
              }
              else if(MSGPACK_OBJECT_STR == p->val.type)
              {
                 if( 0 == match(p, "LanIPAddress") )
                 {
                     e->lan_ip_address = strndup( p->val.via.str.ptr, p->val.via.str.size );
                     objects_left &= ~(1 << 1);
                 }
                 if( 0 == match(p, "LanSubnetMask") )
                 {
                     e->lan_subnet_mask = strndup( p->val.via.str.ptr, p->val.via.str.size );
                     objects_left &= ~(1 << 3);
                 }
                 if( 0 == match(p, "DhcpStartIPAddress") )
                 {
                     e->dhcp_start_ip_address = strndup( p->val.via.str.ptr, p->val.via.str.size );
                     objects_left &= ~(1 << 4);
                 }
                 if( 0 == match(p, "DhcpEndIPAddress") )
                 {
                     e->dhcp_end_ip_address = strndup( p->val.via.str.ptr, p->val.via.str.size );
                     objects_left &= ~(1 << 5);
                 }
              }
              else if( MSGPACK_OBJECT_POSITIVE_INTEGER == p->val.type )
              {
                 if( 0 == match(p, "LeaseTime") )
                 {
                      e->leasetime = (uint32_t) p->val.via.u64;
                      objects_left &= ~(1 << 2);
                }
             }
        }
           p++;
    }
        
    
    if( 1 & objects_left ) {
    } else {
        errno = OK;
    }
   
    return (0 == objects_left) ? 0 : -1;
}
int process_landoc( landoc_t *ld,int num, ... )
{
//To access the variable arguments use va_list 
	va_list valist;
	va_start(valist, num);//start of variable argument loop

	msgpack_object *obj = va_arg(valist, msgpack_object *);//each usage of va_arg fn argument iterates by one time
	msgpack_object_map *mapobj = &obj->via.map;

	msgpack_object *obj1 = va_arg(valist, msgpack_object *);
	ld->subdoc_name = strndup( obj1->via.str.ptr, obj1->via.str.size );

	msgpack_object *obj2 = va_arg(valist, msgpack_object *);
	ld->version = (uint32_t) obj2->via.u64;

	msgpack_object *obj3 = va_arg(valist, msgpack_object *);
	ld->transaction_id = (uint16_t) obj3->via.u64;
	va_end(valist);//End of variable argument loop


	ld->param = (lanparam_t *) malloc( sizeof(lanparam_t) );
        if( NULL == ld->param )
        {
	    CcspTraceWarning(("entries count malloc failed\n"));
            return -1;
        }
        memset( ld->param, 0, sizeof(lanparam_t));


	if( 0 != process_lanparams(ld->param, mapobj) )
	{
		CcspTraceWarning(("process_lanparams failed\n"));
		return -1;
	}

    return 0;
}


