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

#include "array_helpers.h"
/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                            File Scoped Variables                           */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                             Function Prototypes                            */
/*----------------------------------------------------------------------------*/
msgpack_object* __finderarray( const char *name, 
                          msgpack_object_type expect_type,
                          msgpack_object_map *map );

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
void* helper_convert_array( const void *buf, size_t len,
                      size_t struct_size, bool optional,
                      process_fn_t process,
                      destroy_fn_t destroy )
{
    void *p = malloc( struct_size );
    if( NULL == p ) {
        errno = HELPERS_OUT_OF_MEMORY;
    } else {
        memset( p, 0, struct_size );

        if( NULL != buf && 0 < len ) {
            size_t offset = 0;
            msgpack_unpacked msg;
            msgpack_unpack_return mp_rv;

            msgpack_unpacked_init( &msg );

            /* The outermost wrapper is not present */
            mp_rv = msgpack_unpack_next( &msg, (const char*) buf, len, &offset );
			printf("mp_rv is %d MSGPACK_UNPACK_SUCCESS %d offset %lu\n", mp_rv, MSGPACK_UNPACK_SUCCESS, (long unsigned int)offset);
			msgpack_object obj = msg.data;
			msgpack_object_print(stdout, obj);

            if( (MSGPACK_UNPACK_SUCCESS == mp_rv) && (0 != offset) &&
                (MSGPACK_OBJECT_ARRAY == msg.data.type) )
            {
                msgpack_object *inner;

                inner = &msg.data;
                /*if( NULL != wrapper ) {
                    inner = __finderarray( wrapper, expect_type, &msg.data.via.map );
                }*/

                if( ((true == optional) && (NULL == inner)) ||
                    ((NULL != inner) && (0 == (process)(p, inner))) )
                {
                    msgpack_unpacked_destroy( &msg );
                    errno = HELPERS_OK;
                    return p;
                }
            } else {
		printf("Invalid first element\n");
                errno = HELPERS_INVALID_FIRST_ELEMENT;
            }

            msgpack_unpacked_destroy( &msg );

            (destroy)( p );
            p = NULL;
        }
    }
	
    return p;
}

/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/
//To find wrapper map object. 
//As blob starts with array and map is not present, this check is not required.
msgpack_object* __finderarray( const char *name, 
                          msgpack_object_type expect_type,
                          msgpack_object_map *map )
{
    uint32_t i;

    for( i = 0; i < map->size; i++ ) {
        if( MSGPACK_OBJECT_STR == map->ptr[i].key.type ) {
            if( expect_type == map->ptr[i].val.type ) {
                if( 0 == match(&(map->ptr[i]), name) ) {
                    return &map->ptr[i].val;
                }
            }
        }
    }
    errno = HELPERS_MISSING_WRAPPER;
    return NULL;
}
