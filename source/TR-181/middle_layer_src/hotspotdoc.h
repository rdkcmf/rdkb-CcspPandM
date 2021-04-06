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
#ifndef __HOTSPOTDOC_H__
#define __HOTSPOTDOC_H__

#include <stdint.h>
#include <stdlib.h>
#include <msgpack.h>

#include "libHotspotApi.h"

/**
 *  This function converts a msgpack buffer into an hotspotparam_t structure
 *  if possible.
 *
 *  @param buf the buffer to convert
 *  @param len the length of the buffer in bytes
 *
 *  @return NULL on error, success otherwise
 */
hotspotparam_t* hotspotdoc_convert( const void *buf, size_t len );
/**
 *  This function returns a general reason why the conversion failed.
 *
 *  @param errnum the errno value to inspect
 *
 *  @return the constant string (do not alter or free) describing the error
 */
const char* hotspotdoc_strerror( int errnum );

/**
 *  This function destroys an hotspotparam_t object.
 *
 *  @param e the pamdoc to destroy
 */
void hotspotdoc_destroy( hotspotparam_t *hd );

/**
 *  This function converts a msgpack buffer into an tunneldoc_t structure
 *  if possible.
 *
 *  @param buf the buffer to convert
 *  @param len the length of the buffer in bytes
 *
 *  @return NULL on error, success otherwise
 */
tunneldoc_t* tunneldoc_convert(const void *buf, size_t len);
/**
 *  This function converts a msgpack buffer into an wifi_doc_t structure
 *  if possible.
 *
 *  @param buf the buffer to convert
 *  @param len the length of the buffer in bytes
 *
 *  @return NULL on error, success otherwise
 */

/**
 *  This function destroys an tunneldoc_t object.
 *
 *  @param e the pamdoc to destroy
 */
void tunneldoc_destroy( tunneldoc_t *td );

wifi_doc_t* wifi_doc_convert(const void *buf, size_t len);
/**
 *  This function destroys an wifi_doc_t object.
 *
 *  @param e the pamdoc to destroy
 */
void wifi_doc_destroy( wifi_doc_t *wd );

#endif
