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
#include <stdarg.h>
#include <msgpack.h>
#include "ccsp_trace.h"
#include "helpers.h"
#include "macbinding_webconfig_param.h"

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
int process_macdocparams( macdoc_t *e, msgpack_object_map *map );
int process_macbindingdoc( macbindingdoc_t *md, int num, ... );

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/

/* See macbindingdoc.h for details. */
macbindingdoc_t* macbindingdoc_convert( const void *buf, size_t len )
{
    return helper_convert( buf, len, sizeof(macbindingdoc_t), "macbinding",
                           MSGPACK_OBJECT_ARRAY, true,
                           (process_fn_t) process_macbindingdoc,
                           (destroy_fn_t) macbindingdoc_destroy );
}

/* See macbindingdoc.h for details. */
void macbindingdoc_destroy( macbindingdoc_t *md )
{
    if( NULL != md )
    {
        size_t i;
        for( i = 0; i < md->entries_count; i++ )
        {
            if( NULL != md->entries[i].chaddr )
            {
                free( md->entries[i].chaddr );
            }
        
        if( NULL != md->entries[i].yiaddr )
            {
                free( md->entries[i].yiaddr );
            }
        }
        if( NULL != md->entries )
        {
            free( md->entries );
        }
    if( NULL != md->subdoc_name )
    {
        free( md->subdoc_name );
    }
        free( md );
    }
}

/* See macbindingdoc.h for details. */
const char* macbindingdoc_strerror( int errnum )
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
int process_macdocparams( macdoc_t *e, msgpack_object_map *map )
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
                if( 0 == match(p, "Yiaddr") )
                {
                    e->yiaddr = strndup( p->val.via.str.ptr, p->val.via.str.size );
                    objects_left &= ~(1 << 0);
                }
                else if( 0 == match(p, "Chaddr") )
                {
                    e->chaddr = strndup( p->val.via.str.ptr, p->val.via.str.size );
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

int process_macbindingdoc( macbindingdoc_t *md, int num, ... )
{
    va_list valist;
    va_start(valist, num);//start of variable argument loop

    msgpack_object *obj = va_arg(valist, msgpack_object *);//each usage of va_arg fn argument iterates by one time
    msgpack_object_array *array = &obj->via.array;

    msgpack_object *obj1 = va_arg(valist, msgpack_object *);
    md->subdoc_name = strndup( obj1->via.str.ptr, obj1->via.str.size );

    msgpack_object *obj2 = va_arg(valist, msgpack_object *);
    md->version = (uint32_t) obj2->via.u64;

    msgpack_object *obj3 = va_arg(valist, msgpack_object *);
    md->transaction_id = (uint16_t) obj3->via.u64;

    va_end(valist);//End of variable argument loop

    if( 0 < array->size )
    {
        size_t i;
        md->entries_count = array->size;

        md->entries = (macdoc_t *) malloc( sizeof(macdoc_t) * md->entries_count );

        if( NULL == md->entries )
        {
            md->entries_count = 0;
            return -1;
        }

        memset( md->entries, 0, sizeof(macdoc_t) * md->entries_count );

        for( i = 0; i < md->entries_count; i++ )
        {
            if( MSGPACK_OBJECT_MAP != array->ptr[i].type )
            {
                errno = INVALID_OBJECT;
                return -1;
            }

            if( 0 != process_macdocparams(&md->entries[i], &array->ptr[i].via.map) )
            {
                CcspTraceWarning(("process_macdocparams failed\n"));
                return -1;
            }
        }
    }

    return 0;
}

