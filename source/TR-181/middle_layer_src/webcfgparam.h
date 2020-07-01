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
#ifndef __WEBCFGPARAM_H__
#define __WEBCFGPARAM_H__

#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    char *name;
    char *value;
    int   value_size;
    uint16_t type;
} wparam_t;

typedef struct
{
    wparam_t *entries;
    size_t      entries_count;
} webcfgparam_t;

/**
 *  This function converts a msgpack buffer into an webcfgparam_t structure
 *  if possible.
 *
 *  @param buf the buffer to convert
 *  @param len the length of the buffer in bytes
 *
 *  @return NULL on error, success otherwise
 */
webcfgparam_t* webcfgparam_convert( const void *buf, size_t len );

/**
 *  This function destroys an webcfgparam_t object.
 *
 *  @param e the webcfgparam to destroy
 */
void webcfgparam_destroy( webcfgparam_t *d );

/**
 *  This function returns a general reason why the conversion failed.
 *
 *  @param errnum the errno value to inspect
 *
 *  @return the constant string (do not alter or free) describing the error
 */
const char* webcfgparam_strerror( int errnum );

#endif
