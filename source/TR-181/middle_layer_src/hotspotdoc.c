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
#include <errno.h>
#include <string.h>
#include <msgpack.h>
#include <stdarg.h>
#include "ccsp_trace.h"
#include "hotspot_helpers.h"
#include "hotspotdoc.h"

/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
/* none */
/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/
enum {
    OK                       = T_HELPERS_OK,
    OUT_OF_MEMORY            = T_HELPERS_OUT_OF_MEMORY,
    INVALID_FIRST_ELEMENT    = T_HELPERS_INVALID_FIRST_ELEMENT,
    MISSING_ENTRY         = T_HELPERS_MISSING_WRAPPER,
    INVALID_DATATYPE,
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
int process_hotspotparams( hparam_t *e, msgpack_object_map *map );
int process_tunnel_tableparams( tunnel_t *e, msgpack_object_map *map );
int process_tunnelparams( tdoc_t *e, msgpack_object_map *map );
int process_hotspotdoc( hotspotparam_t *hd, int num, ...); 
int process_tunneldoc( tunneldoc_t *td,int num, ... );
int process_wifi_doc( wifi_doc_t *wd,int num, ... );

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
/* See hotspotdoc.h for details. */
hotspotparam_t* hotspotdoc_convert( const void *buf, size_t len )
{
 
	return comp_helper_convert( buf, len, sizeof(hotspotparam_t), "PublicHotspotData", 
                            MSGPACK_OBJECT_ARRAY, true,
                           (process1_fn_t) process_hotspotdoc,
                           (destroy1_fn_t) hotspotdoc_destroy );
}

tunneldoc_t* tunneldoc_convert(const void *buf, size_t len)
{
	return comp_helper_convert( buf, len, sizeof(tunneldoc_t), "Tunnels", 
                            MSGPACK_OBJECT_ARRAY, true,
                           (process1_fn_t) process_tunneldoc,
                           (destroy1_fn_t) tunneldoc_destroy );
}

wifi_doc_t* wifi_doc_convert(const void *buf, size_t len)
{
	return comp_helper_convert( buf, len, sizeof(tunneldoc_t), "WifiVapConfig", 
                            MSGPACK_OBJECT_ARRAY, true,
                           (process1_fn_t) process_wifi_doc,
                           (destroy1_fn_t) wifi_doc_destroy );
}
/* See hotspotdoc.h for details. */
void tunneldoc_destroy( tunneldoc_t *td )
{
	size_t i, j = 0;

	if( NULL != td )
	{
		if(NULL != td->entries)
		{
			for(i = 0; i< td->entries_count; i++)
			{
				if( NULL != td->entries[i].gre_primary_endpoint )
				{
					free(td->entries[i].gre_primary_endpoint);
				}
				if( NULL != td->entries[i].gre_sec_endpoint)
				{
					free(td->entries[i].gre_sec_endpoint);
				}
				if( NULL != td->entries[i].table_param )
				{
					if( NULL != td->entries[i].table_param->entries )
					{
						for( j = 0; j < td->entries[i].table_param->entries_count; j++ )
						{
							if( NULL != td->entries[i].table_param->entries[j].vap_name )
							{
								free(td->entries[i].table_param->entries[j].vap_name);
							}
						
						}
						free(td->entries[i].table_param->entries);
					}
					free(td->entries[i].table_param);
				}
			}
		free(td->entries);
		}
		free( td );
	}
}

void wifi_doc_destroy( wifi_doc_t *wd )
{
	if(wd != NULL)
	{
		free(wd);
	}
	
}

void hotspotdoc_destroy( hotspotparam_t *hd )
{

	if( NULL != hd )
	{
		size_t i;
		for( i = 0; i < hd->entries_count; i++ )
		{
			if( NULL != hd->entries[i].name )
			{
				free( hd->entries[i].name );
			}
			if( NULL != hd->entries[i].value )
			{
				free( hd->entries[i].value );
			}
		}
		if( NULL != hd->entries )
		{
			free( hd->entries );
		}
		if(NULL != hd->subdoc_name)
		{
			free(hd->subdoc_name);
		}
		free( hd );
    	}
	
}

/* See hotspotdoc.h for details. */
const char* hotspotdoc_strerror( int errnum )
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
	CcspTraceWarning(("----hotspotdoc_strerror----\n"));
        return "Unknown error.";
    }
    return map[i].txt;
}
/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/
/**
 *  Convert the msgpack map into the dnsMapping_t structure.
 *
 *  @param e    the entry pointer
 *  @param map  the msgpack map pointer
 *
 *  @return 0 on success, error otherwise
 */
int process_tunnel_tableparams( tunnel_t *e, msgpack_object_map *map )
{
    int left = map->size;
    uint8_t objects_left = 0x03;
    msgpack_object_kv *p;
    p = map->ptr;

    while( (0 < objects_left) && (0 < left--) )
    {
        if( MSGPACK_OBJECT_STR == p->key.type )
        {
              if(MSGPACK_OBJECT_STR == p->val.type)
              {
                 if( 0 == match(p, "vap_name") )
                 {
                     e->vap_name = strndup( p->val.via.str.ptr, p->val.via.str.size );
                     objects_left &= ~(1 << 0);
                 }
              }
              else if( MSGPACK_OBJECT_POSITIVE_INTEGER == p->val.type )
              {
                 if( 0 == match(p, "wan_vlan") )
                 {
                     if( UINT16_MAX < p->val.via.u64 )
                     {
                         errno = INVALID_DATATYPE;
                         return -1;
                     }
                     else
                     {
                         e->wan_vlan = (unsigned int) p->val.via.u64;
                     }
                     objects_left &= ~(1 << 2);
                 }
              }
              else if( MSGPACK_OBJECT_BOOLEAN == p->val.type )
              {
                 if( 0 == match(p, "enable") )
                 {
                     e->enable = p->val.via.boolean;
                     objects_left &= ~(1 << 1);
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

/**
 *  Convert the msgpack map into the tdoc_t structure.
 *
 *  @param e    the entry pointer
 *  @param map  the msgpack map pointer
 *
 *  @return 0 on success, error otherwise
 */
int process_tunnelparams( tdoc_t *e, msgpack_object_map *map )
{
    int left = map->size;
    size_t i =0;
    /*4th bit position is enabled for 4 required parameters 0000 1000
    and for each parameter one bit right shift is done, example- 0000 1000 >>1 = 0000 0100 */
    uint8_t objects_left = 0x08;
    msgpack_object_kv *p;
    p = map->ptr;
    while( (0 < objects_left) && (0 < left--) )
    {
        if( MSGPACK_OBJECT_STR == p->key.type )
        {
              if( MSGPACK_OBJECT_BOOLEAN == p->val.type )
              {
                 if( 0 == match(p, "gre_enable") )
                 {
                     e->gre_enable = p->val.via.boolean;
                     objects_left = objects_left >> 1;
                 }
              }
              else if(MSGPACK_OBJECT_STR == p->val.type)
              {
                 if( 0 == match(p, "gre_primary_endpoint") )
                 {
                     e->gre_primary_endpoint = strndup( p->val.via.str.ptr, p->val.via.str.size );
                     objects_left = objects_left >> 1;
                 }
                 if( 0 == match(p, "gre_sec_endpoint") )
                 {
                     e->gre_sec_endpoint = strndup( p->val.via.str.ptr, p->val.via.str.size );
                     objects_left = objects_left >> 1;
                 }
              }
              else if( MSGPACK_OBJECT_POSITIVE_INTEGER == p->val.type )
              {
                 if( 0 == match(p, "gre_dscp") )
                 {
                     if( UINT16_MAX < p->val.via.u64 )
                     {
                         errno = INVALID_DATATYPE;
                         return -1;
                     }
                     else
                     {
                         e->gre_dscp = (int) p->val.via.u64;// no shifting since it is optional
                     }
                 }
              }
              else if( MSGPACK_OBJECT_ARRAY == p->val.type )
              {
                 if( 0 == match(p, "tunnel_network") )
                 {
                      e->table_param = (tunnelTable_t *) malloc( sizeof(tunnelTable_t) );
                      if( NULL == e->table_param )
                      {
	                  CcspTraceWarning(("table_param malloc failed\n"));
                          return -1;
                      }
                      memset( e->table_param, 0, sizeof(tunnelTable_t));

                      e->table_param->entries_count = p->val.via.array.size;

                      e->table_param->entries = (tunnel_t *) malloc( sizeof(tunnel_t) * e->table_param->entries_count);

                      if( NULL == e->table_param->entries )
                      {
	                  CcspTraceWarning(("table_param malloc failed\n"));
                          e->table_param->entries_count = 0;
                          return -1;
                      }
                      memset( e->table_param->entries, 0, sizeof(tunnel_t) * e->table_param->entries_count);

                      for( i = 0; i < e->table_param->entries_count; i++ )
                      {
                          if( MSGPACK_OBJECT_MAP != p->val.via.array.ptr[i].type )
                          {
                              CcspTraceWarning(("invalid OBJECT \n"));
                              errno = INVALID_OBJECT;
                              return -1;
                          }

                          if( 0 != process_tunnel_tableparams(&e->table_param->entries[i], &p->val.via.array.ptr[i].via.map) )
                          {
		              CcspTraceWarning(("process_dnsparams failed\n"));
                              return -1;
                          }
           
                      }
                      objects_left = objects_left >> 1;
                }
             }
        }
           p++;
    }
     
    if(e->gre_enable == 0) //since shifting has to happen even when gre_enable is false
    {
		objects_left = objects_left >> 1;
    }
    if( 1 & objects_left ) {
    } else {
        errno = OK;
    }
   
    return (0 == objects_left) ? 0 : -1;
}

int process_hotspotparams( hparam_t *e, msgpack_object_map *map )
{
    int left = map->size;
    uint8_t objects_left = 0x02;
    msgpack_object_kv *p;

    p = map->ptr;
    while( (0 < objects_left) && (0 < left--) )
	{
        if( MSGPACK_OBJECT_STR == p->key.type )
	{
        	if( MSGPACK_OBJECT_STR == p->val.type )
		{
		        if( 0 == match(p, "Name") )
			{
		            e->name = strndup( p->val.via.str.ptr, p->val.via.str.size );
		            objects_left &= ~(1 << 0);
		        }
			if( 0 == match(p, "Value"))
			{
				printf("blob size update\n");
				e->value = malloc(sizeof(char) * p->val.via.str.size+1 );
				memset( e->value, 0, sizeof(char) * p->val.via.str.size+1);
				e->value = memcpy(e->value, p->val.via.str.ptr, p->val.via.str.size+1 );
				e->value[p->val.via.str.size] = '\0';
				e->value_size =(uint32_t) p->val.via.str.size;
				CcspTraceWarning(("e->value_size is %lu\n", (long)e->value_size));
				objects_left &= ~(1 << 1);
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

int process_hotspotdoc( hotspotparam_t *hd,int num, ... )
{
//To access the variable arguments use va_list 
	va_list valist;
	va_start(valist, num);//start of variable argument loop

	msgpack_object *obj = va_arg(valist, msgpack_object *);//each usage of va_arg fn argument iterates by one time
	msgpack_object_array *array = &obj->via.array;

	msgpack_object *obj1 = va_arg(valist, msgpack_object *);
	hd->subdoc_name = strndup( obj1->via.str.ptr, obj1->via.str.size );

	msgpack_object *obj2 = va_arg(valist, msgpack_object *);
	hd->version = (uint32_t) obj2->via.u64;

	msgpack_object *obj3 = va_arg(valist, msgpack_object *);
	hd->transaction_id = (uint16_t) obj3->via.u64;

	va_end(valist);//End of variable argument loop

	if( 0 < array->size )
	{
		size_t i;

		hd->entries_count = array->size;
		hd->entries = (hparam_t *) malloc( sizeof(hparam_t) * hd->entries_count );
		if( NULL == hd->entries )
		{
		    hd->entries_count = 0;
		    return -1;
		}

		memset( hd->entries, 0, sizeof(hparam_t) * hd->entries_count );
		for( i = 0; i < hd->entries_count; i++ )
		{
			if( MSGPACK_OBJECT_MAP != array->ptr[i].type )
			{
				errno = INVALID_OBJECT;
				CcspTraceWarning(("Invalid object\n"));
				return -1;
			}
			if( 0 != process_hotspotparams(&hd->entries[i], &array->ptr[i].via.map))
			{
				CcspTraceWarning(("process_hotspotparams failed\n"));
				return -1;
			}
		}
	}
	

    return 0;
}

int process_tunneldoc( tunneldoc_t *td,int num, ... )
{
//To access the variable arguments use va_list 
	va_list valist;
	va_start(valist, num);//start of variable argument loop

	msgpack_object *obj = va_arg(valist, msgpack_object *);//each usage of va_arg fn argument iterates by one time
	msgpack_object_array *array = &obj->via.array;

	va_end(valist);//End of variable argument loop

	if( 0 < array->size )
	{
		size_t i;

		td->entries_count = array->size;
		td->entries = (tdoc_t *) malloc( sizeof(tdoc_t) * td->entries_count );
		if( NULL == td->entries )
		{
		    td->entries_count = 0;
		    return -1;
		}

		memset( td->entries, 0, sizeof(tdoc_t) * td->entries_count );
		for( i = 0; i < td->entries_count; i++ )
		{
			if( MSGPACK_OBJECT_MAP != array->ptr[i].type )
			{
				errno = INVALID_OBJECT;
				CcspTraceWarning(("INVALID_OBJECT\n"));
				return -1;
			}
			if( 0 != process_tunnelparams(&td->entries[i], &array->ptr[i].via.map))
			{
				CcspTraceWarning(("process_tunnelparams failed\n"));
				return -1;
			}
		}
	}
	

    return 0;
}

int process_wifi_doc( wifi_doc_t *wd,int num, ... )
{
//To access the variable arguments use va_list 
	va_list valist;
	va_start(valist, num);//start of variable argument loop

	msgpack_object *obj = va_arg(valist, msgpack_object *);//each usage of va_arg fn argument iterates by one time
	msgpack_object_array *array = &obj->via.array;

	va_end(valist);//End of variable argument loop

	if( 0 < array->size )
	{
		wd->entries_count = array->size;
		CcspTraceWarning(("The wifi doc array size is %d\n", (int)wd->entries_count));
		
	}

    return 0;
}
