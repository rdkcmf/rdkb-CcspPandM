/*
 * Copyright 2020 Comcast Cable Communications Management, LLC
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
#include "ccsp_trace.h"
#include "hotspot_helpers.h"
#include "hotspotdoc.h"
/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
#define WIFI_METADATA_MAP_SIZE                3
/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/
struct wifi_token {
    const char *name;
    size_t length;
};
/*----------------------------------------------------------------------------*/
/*                            File Scoped Variables                           */
/*----------------------------------------------------------------------------*/
/* none */
/*----------------------------------------------------------------------------*/
/*                             Function Prototypes                            */
/*----------------------------------------------------------------------------*/
msgpack_object* __finder_comp( const char *name, 
                          msgpack_object_type expect_type,
                          msgpack_object_map *map );
/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
void* comp_helper_convert( const void *buf, size_t len,
                      size_t struct_size, const char *wrapper,
                      msgpack_object_type expect_type, bool optional,
                      process1_fn_t process,
                      destroy1_fn_t destroy )
{
    void *p = malloc( struct_size );
    if( NULL == p )
    {
        errno = T_HELPERS_OUT_OF_MEMORY;
    }
    else
    {
        memset( p, 0, struct_size );
        if( NULL != buf && 0 < len )
        {
            size_t offset = 0;
            msgpack_unpacked msg;
            msgpack_unpack_return mp_rv;
            msgpack_unpacked_init( &msg );
            /* The outermost wrapper MUST be a map. */
            mp_rv = msgpack_unpack_next( &msg, (const char*) buf, len, &offset );
	/*For Printing the msgpack object use the following 3 lines*/
	    msgpack_object obj = msg.data;
            msgpack_object_print(stdout, obj);
            printf("\n");

            if( (MSGPACK_UNPACK_SUCCESS == mp_rv) && (0 != offset) &&
                (MSGPACK_OBJECT_MAP == msg.data.type) )
            {
                msgpack_object *inner;
                msgpack_object *subdoc_name;
                msgpack_object *version;
                msgpack_object *transaction_id;
                
                if( NULL != wrapper && 0 != strncmp(wrapper,"PublicHotspotData",strlen("PublicHotspotData"))) 
                {
                    inner = __finder_comp( wrapper, expect_type, &msg.data.via.map );
                    
                    if( ((NULL != inner) && (0 == (process)(p, 1, inner))) || 
                              ((true == optional) && (NULL == inner)) )
                    {
                         msgpack_unpacked_destroy( &msg );
                         errno = T_HELPERS_OK;
                         return p;
                    }
                    else 
                    {
                         errno = T_HELPERS_INVALID_FIRST_ELEMENT;
                    }
                }
                else if( NULL != wrapper && 0 == strncmp(wrapper,"PublicHotspotData",strlen("PublicHotspotData"))) 
                {
                    inner = __finder_comp( wrapper, expect_type, &msg.data.via.map );
                    subdoc_name =  __finder_comp( "subdoc_name", expect_type, &msg.data.via.map );
                    version =  __finder_comp( "version", expect_type, &msg.data.via.map );
                    transaction_id =  __finder_comp( "transaction_id", expect_type, &msg.data.via.map );
                    
                    if( ((NULL != inner && NULL != subdoc_name && NULL != version && NULL != transaction_id) && (0 == (process)(p,4, inner, subdoc_name, version, transaction_id))) || ((true == optional) && (NULL == inner)) )
                    {
                         msgpack_unpacked_destroy( &msg );
                         errno = T_HELPERS_OK;
                         return p;
                    }
                    else 
                    {     
                         CcspTraceWarning(("Invalid first element\n"));
                         errno = T_HELPERS_INVALID_FIRST_ELEMENT;
                    }
                } 
              }
            CcspTraceWarning(("Msgpack unpack failed\n"));
            msgpack_unpacked_destroy( &msg );
            if(NULL!=p)
            {
               (destroy)( p );
                p = NULL;
            }
            
        }
    }
    return p;
}
/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/
static void __msgpack_pack_string( msgpack_packer *pk, const void *string, size_t n );
static void __msgpack_pack_string_nvp( msgpack_packer *pk,
                                       const struct wifi_token *token,
                                       const char *val );

static int alterWifiMapData( char * buf );

//Used to find the map object using the key 

msgpack_object* __finder_comp( const char *name, 
                          msgpack_object_type expect_type,
                          msgpack_object_map *map )
{
    uint32_t i;
    
    for( i = 0; i < map->size; i++ ) 
    {

        if( MSGPACK_OBJECT_STR == map->ptr[i].key.type ) 
        {
            if( expect_type == map->ptr[i].val.type )
            {
                if( 0 == match(&(map->ptr[i]), name) )
                {
                    return &map->ptr[i].val;
                }
            }
            else if(MSGPACK_OBJECT_STR == map->ptr[i].val.type)
            {   
                if(0 == strncmp(map->ptr[i].key.via.str.ptr, name, strlen(name)))
                {   
                    return &map->ptr[i].val;
                }
                
             }
             else 
            {   
                if(0 == strncmp(map->ptr[i].key.via.str.ptr, name, strlen(name)))
                {   
                    return &map->ptr[i].val;
                }
                
             }
            }
        }
	CcspTraceWarning(("The wrapper %s is missing\n", name));
     errno = T_HELPERS_MISSING_WRAPPER;
    return NULL;
}

static void __msgpack_pack_string( msgpack_packer *pk, const void *string, size_t n )
{
    msgpack_pack_str( pk, n );
    msgpack_pack_str_body( pk, string, n );
}

static void __msgpack_pack_string_nvp( msgpack_packer *pk,
                                       const struct wifi_token *token,
                                       const char *val )
{
    if( ( NULL != token ) && ( NULL != val ) )
    {
        __msgpack_pack_string( pk, token->name, token->length );
        __msgpack_pack_string( pk, val, strlen( val ) );
    }
}

ssize_t wifi_pack_appenddoc(const wifi_appenddoc_t *appenddocData,void **data)
{
    size_t rv = -1;

    msgpack_sbuffer sbuf;
    msgpack_packer pk;
    msgpack_sbuffer_init( &sbuf );
    msgpack_packer_init( &pk, &sbuf, msgpack_sbuffer_write );
    msgpack_zone mempool;
    msgpack_object deserialized;
    if( appenddocData != NULL )
    {
        struct wifi_token APPENDDOC_MAP_SUBDOC_NAME;

        APPENDDOC_MAP_SUBDOC_NAME.name = "subdoc_name";
        APPENDDOC_MAP_SUBDOC_NAME.length = strlen( "subdoc_name" );
        __msgpack_pack_string_nvp( &pk, &APPENDDOC_MAP_SUBDOC_NAME, appenddocData->subdoc_name );

        struct wifi_token APPENDDOC_MAP_VERSION;
             
        APPENDDOC_MAP_VERSION.name = "version";
        APPENDDOC_MAP_VERSION.length = strlen( "version" );
        __msgpack_pack_string( &pk, APPENDDOC_MAP_VERSION.name, APPENDDOC_MAP_VERSION.length );
        msgpack_pack_uint32(&pk,appenddocData->version);

        struct wifi_token APPENDDOC_MAP_TRANSACTION_ID;
             
        APPENDDOC_MAP_TRANSACTION_ID.name = "transaction_id";
        APPENDDOC_MAP_TRANSACTION_ID.length = strlen( "transaction_id" );
        __msgpack_pack_string( &pk, APPENDDOC_MAP_TRANSACTION_ID.name, APPENDDOC_MAP_TRANSACTION_ID.length );
        msgpack_pack_uint16(&pk, appenddocData->transaction_id);
    }
    else 
    {    
        CcspTraceWarning(("Doc append data is NULL\n" ));
        return rv;
    } 

    if( sbuf.data ) 
    {
        *data = ( char * ) malloc( sizeof( char ) * sbuf.size );

        if( NULL != *data ) 
        {
            memcpy( *data, sbuf.data, sbuf.size );
	    CcspTraceWarning(("sbuf.data of appenddoc is %s sbuf.size %zu\n", sbuf.data, sbuf.size));
            rv = sbuf.size;
        }
    }

    msgpack_zone_init(&mempool, 2048);

    msgpack_unpack(sbuf.data, sbuf.size, NULL, &mempool, &deserialized);
    msgpack_object_print(stdout, deserialized);

    msgpack_zone_destroy(&mempool);

    msgpack_sbuffer_destroy( &sbuf );
    return rv;   
}

/**
 * @brief alterMapData function to change MAP size of encoded msgpack object.
 *
 * @param[in] encodedBuffer msgpack object
 * @param[out] return 0 in success or less than 1 in failure case
 */

static int alterWifiMapData( char * buf )
{
    //Extract 1st byte from binary stream which holds type and map size
    unsigned char *byte = ( unsigned char * )( &( buf[0] ) ) ;
    int mapSize;
    CcspTraceWarning(("First byte in hex : %x\n", 0xff & *byte ));
    //Calculate map size
    mapSize = ( 0xff & *byte ) % 0x10;

    if( mapSize == 15 )
    {
        CcspTraceWarning(("Msgpack Map (fixmap) is already at its MAX size i.e. 15\n" ));
        return -1;
    }

    *byte = *byte + WIFI_METADATA_MAP_SIZE;
    mapSize = ( 0xff & *byte ) % 0x10;
    CcspTraceWarning(("New Map size : %d\n", mapSize ));
    CcspTraceWarning(("First byte in hex : %x\n", 0xff & *byte ));
    //Update 1st byte with new MAP size
    buf[0] = *byte;
    return 0;
}

char * base64wifiblobencoder(char * blob_data, size_t blob_size )
{
	char* b64buffer =  NULL;
	size_t encodeSize = -1;
   	CcspTraceWarning(("Data is %s\n", blob_data));
     	CcspTraceWarning(("-----------Start of Base64 Encode ------------\n"));
        encodeSize = b64_get_encoded_buffer_size(blob_size);
        CcspTraceWarning(("encodeSize is %zu\n", encodeSize));
        b64buffer = malloc(encodeSize + 1);
        if(b64buffer != NULL)
        {
            memset( b64buffer, 0, sizeof( encodeSize )+1 );

            b64_encode((uint8_t *)blob_data, blob_size, (uint8_t *)b64buffer);
            b64buffer[encodeSize] = '\0' ;
        }
	return b64buffer;
}

/**
 * @brief appendWifiEncodedData function to append two encoded buffer and change MAP size accordingly.
 * 
 * @note appendWifiEncodedData function allocates memory for buffer, caller needs to free the buffer(appendData)in
 * both success or failure case. use wrp_free_struct() for free
 *
 * @param[in] encodedBuffer msgpack object (first buffer)
 * @param[in] encodedSize is size of first buffer
 * @param[in] metadataPack msgpack object (second buffer)
 * @param[in] metadataSize is size of second buffer
 * @param[out] appendData final encoded buffer after append
 * @return  appended total buffer size or less than 1 in failure case
 */

size_t appendWifiEncodedData( void **appendData, void *encodedBuffer, size_t encodedSize, void *metadataPack, size_t metadataSize )
{
    //Allocate size for final buffer
    *appendData = ( void * )malloc( sizeof( char * ) * ( encodedSize + metadataSize ) );
	if(*appendData != NULL)
	{
		memcpy( *appendData, encodedBuffer, encodedSize );
		//Append 2nd encoded buf with 1st encoded buf
		memcpy( *appendData + ( encodedSize ), metadataPack, metadataSize );
		//Alter MAP
		int ret = alterWifiMapData( ( char * ) * appendData );
		CcspTraceWarning(("The value of ret in alterMapData  %d\n",ret));
		if( ret ) {
		    return -1;
		}
		return ( encodedSize + metadataSize );
	}
	else
	{
		CcspTraceWarning(("Memory allocation failed\n" ));
	}
    return -1;
}

char * append_wifi_doc(char * subdoc_name, uint32_t version, uint16_t trans_id, char * blob_data, size_t blob_size)
{
    wifi_appenddoc_t *wifi_appenddata = NULL;
    size_t wifi_appenddocPackSize = -1;
    size_t wifi_embeddeddocPackSize = -1;
    void *wifi_appenddocdata = NULL;
    void *wifi_embeddeddocdata = NULL;
    char *wifi_finaldocdata = NULL;

    wifi_appenddata = (wifi_appenddoc_t *) malloc(sizeof(wifi_appenddoc_t ));
    if(wifi_appenddata != NULL)
    {   
        memset(wifi_appenddata, 0, sizeof(wifi_appenddoc_t));

        wifi_appenddata->subdoc_name = strdup(subdoc_name);
        wifi_appenddata->version = version;
        wifi_appenddata->transaction_id = trans_id;
	CcspTraceWarning(("subdoc_name: %s, version: %lu, transaction_id: %hu\n", subdoc_name, (unsigned long)version, trans_id));

    	wifi_appenddocPackSize = wifi_pack_appenddoc(wifi_appenddata, &wifi_appenddocdata);
    	CcspTraceWarning(("data packed is %s\n", (char*)wifi_appenddocdata));
 
    	free(wifi_appenddata->subdoc_name);
    	free(wifi_appenddata);

    	wifi_embeddeddocPackSize = appendWifiEncodedData(&wifi_embeddeddocdata, (void *)blob_data, blob_size, wifi_appenddocdata, wifi_appenddocPackSize);
    	CcspTraceWarning(("wifi_appenddocPackSize: %zu, blobSize: %zu, wifi_embeddeddocPackSize: %zu\n", wifi_appenddocPackSize, blob_size, wifi_embeddeddocPackSize));
    	CcspTraceWarning(("The wifi_embedded doc data is %s\n",(char*)wifi_embeddeddocdata));
	free(wifi_appenddocdata);

	msgpack_print(wifi_embeddeddocdata, wifi_embeddeddocPackSize);//enable this if msgpack object needed to be printed in a file
   	wifi_finaldocdata = base64wifiblobencoder((char *)wifi_embeddeddocdata, wifi_embeddeddocPackSize);
	free(wifi_embeddeddocdata);
    }
    
    return wifi_finaldocdata;
}

//To print and store the msgpack output to a file
void msgpack_print(const void *data, size_t len)
{
      if( NULL != data && 0 < len )
      {
          size_t offset = 0;
          FILE *fd = fopen("/tmp/wifi_appenddoc_output.bin", "w+");
          msgpack_unpacked msg;
          msgpack_unpack_return msgpk_rv;
          msgpack_unpacked_init( &msg );

          msgpk_rv = msgpack_unpack_next( &msg, (const char*) data, len, &offset );
          CcspTraceWarning(("msgpk_rv value is %d\n",msgpk_rv));
          msgpack_object obj = msg.data;
          msgpack_object_print(fd, obj);
          msgpack_unpacked_destroy( &msg );
          fclose(fd);
      }
}
