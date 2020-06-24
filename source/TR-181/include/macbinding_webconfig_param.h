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
#ifndef __MACBINDING_WEBCONFIG_PARAM_H__
#define __MACBINDING_WEBCONFIG_PARAM_H__

#include <stdint.h>
#include <stdlib.h>
#include <msgpack.h>
typedef struct
{
    char      *yiaddr; 
    char      *chaddr;     
} macdoc_t;

typedef struct
{
    macdoc_t     *entries;       
    size_t       entries_count;
    char *       subdoc_name;
    uint32_t     version;
    uint16_t     transaction_id;
} macbindingdoc_t;

/**
 *  This function converts a msgpack buffer into an macbindingdoc_t structure
 *  if possible.
 *
 *  @param buf the buffer to convert
 *  @param len the length of the buffer in bytes
 *
 *  @return NULL on error, success otherwise
 */
macbindingdoc_t* macbindingdoc_convert( const void *buf, size_t len );

/**
 *  This function destroys an macbindingdoc_t object.
 *
 *  @param e the macbindingdoc to destroy
 */
void macbindingdoc_destroy( macbindingdoc_t *d );

/**
 *  This function returns a general reason why the conversion failed.
 *
 *  @param errnum the errno value to inspect
 *
 *  @return the constant string (do not alter or free) describing the error
 */
const char* macbindingdoc_strerror( int errnum );

#endif

