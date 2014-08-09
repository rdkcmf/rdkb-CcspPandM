/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

#if (defined _COSA_DRG_TPG_) || (defined _COSA_INTEL_USG_ARM_)
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include "sysevent/sysevent.h"
#include "utapi/utapi.h"
#include "utapi/utapi_util.h"
#include "cosa_drg_common.h"
#ifdef _COSA_DRG_TPG_
int gLanSwDev = -1;
int gWanSwDev = -1;
#endif

int commonSyseventFd = -1;
token_t commonSyseventToken;

static int openCommonSyseventConnection() {
    if (commonSyseventFd == -1) {
        commonSyseventFd = s_sysevent_connect(&commonSyseventToken);
    }
    return 0;
}

int commonSyseventSet(char* key, char* value){
    if(commonSyseventFd == -1) {
        openCommonSyseventConnection();
    }
    return sysevent_set(commonSyseventFd, commonSyseventToken, key, value, 0);
}

int commonSyseventGet(char* key, char* value, int valLen){
    if(commonSyseventFd == -1) {
        openCommonSyseventConnection();
    }
    return sysevent_get(commonSyseventFd, commonSyseventToken, key, value, valLen);
}

int commonSyseventClose() {
    int retval;

    if(commonSyseventFd == -1) {
        return 0;
    }

    retval = sysevent_close(commonSyseventFd, commonSyseventToken);
    commonSyseventFd = -1;
    return retval;
}


int getOSStats(char* iface, POSStats stats) {
    struct stat fileAttrs;			/* /proc/net/dev file attributes. */
    struct stat linkAttrs;			/* /proc/net/dev symlink attributes. */
    char *filePath = NULL;			/* Path to /proc/net/dev file. */
    FILE *file = NULL;				/* File pointer for fopen(). */
    int status = 0;				/* /proc/net/dev file close() status. */
    char buf[1024] = "";			/* Character buffer. */
    int conv = 0;				/* sscanf() string match count. */
    char *delim = NULL;				/* Ptr to matched character in string. */
    mode_t mode = 0;				/* /proc/net/dev file permissions. */

    if( iface == NULL || iface[0] == '\0')
    {
        fprintf( stderr, "ERROR: interface list is empty\n" );
        return (-1);
     }

    filePath = "/proc/net/dev";

    /*
     * Get /proc/net/dev symlink attributes.
     */
    if( ( lstat( filePath, &linkAttrs ) ) != 0 )
    {
        perror( "lstat" );
        return (-1);
     }

    /*
     * Is /proc/net/dev a symlink?
     */
    if( S_ISLNK( linkAttrs.st_mode ) )
    {
        fprintf( stderr, "ERROR: /proc/net/dev is a symbolic link\n" );
        return (-1);
     }

    /*
     * Open /proc/net/dev for reading.
     */
    if( ( file = fopen( "/proc/net/dev", "r" ) ) == NULL )
    {
        perror( "fopen" );
        return (-1);
     }

    /*
     * Get /proc/net/dev file attributes.
     */
    if( ( fstat( fileno( file ), &fileAttrs ) ) != 0 )
    {
        perror( "fstat" );
        return (-1);
     }

    /*
     * Does lstat(2) and fstat(2) agree on /proc/net/dev?
     */
    if( ( ( linkAttrs.st_ino ) != ( fileAttrs.st_ino ) ) || 
        ( ( linkAttrs.st_dev ) != ( fileAttrs.st_dev ) ) )
    {
        fprintf( stderr, "ERROR: /proc/net/dev file attribute inconsistency\n" );
        return (-1);
     }

    /*
     * Is /proc/net/dev a regular file?
     */
    if( ! ( S_ISREG( fileAttrs.st_mode ) ) )
    {
        fprintf( stderr, "ERROR: /proc/net/dev is not a regular file\n" );
        return (-1);
     }

    /*
     * Is /proc/net/dev zero bytes in length?
     */
    if( ( ( fileAttrs.st_size ) || ( fileAttrs.st_blocks ) ) != 0 )
    {
        fprintf( stderr, "ERROR: /proc/net/dev file size is greater than 0\n" );
        return (-1);
     }

    /*
     * Is /proc/net/dev chowned UID/GID 0?
     */
    if( ( ( fileAttrs.st_uid ) || ( fileAttrs.st_gid ) ) != 0 )
    {
        fprintf( stderr, "ERROR: /proc/net/dev is not owned by UID 0, GID 0\n" );
        return (-1);
     }

    /*
     * Is /proc/net/dev mode 0444?
     */
    if( ( mode = fileAttrs.st_mode & ALLPERMS ) != ( S_IRUSR | S_IRGRP | S_IROTH ) )
    {
        fprintf( stderr, "ERROR: /proc/net/dev permissions are not mode 0444\n" );
        return (-1);
     }

    /*
     * Read past 1st and 2nd line of /proc/net/dev header.
     */
    if( ! fgets(buf, sizeof( buf ), file) )
    {
        perror( "fgets" );
        return (-1);
     }

    if( ! fgets(buf, sizeof( buf ), file) )
    {
        perror( "fgets" );
        return (-1);
     }

    /*
     * Check /proc/net/dev format.
     */
    if( ( strstr( buf, "compressed" ) ) == NULL )
    {
        fprintf( stderr, "ERROR: /proc/net/dev header format is not supported\n" );
        return (-1);
     } 

    printf("-----we are just here...\n");
    /*
     * Loop through /proc/net/dev metrics & output
     * one line of metrics data for each interface.
     */
    while( fgets( buf, sizeof( buf ), file ) )
    {
        /*
         * Is this the current interface?
         */
        if( ( strstr( buf, iface ) ) != NULL )
        {
            delim = strchr( buf, ':' );

            *delim = '\0';
           
            conv = sscanf( delim+1,
                           "%Lu %Lu %lu %lu %*lu %*lu %*lu %lu %Lu %Lu %lu %lu %*lu %*lu %*lu %lu",
                           &stats->rxBytes, &stats->rxPackets, &stats->rxErrors, &stats->rxDrops, &stats->rxMulticast,
                           &stats->txBytes, &stats->txPackets, &stats->txErrors, &stats->txDrops, &stats->txMulticast );
            break;
         }

     }

    printf("-----Interface:%s, rxBytes:%d\n", iface, stats->rxBytes);
    if( conv != 10 )
    {
        //fprintf( stderr, "ERROR: /proc/net/dev parse error\n" );
        return (-1);
     }

    /*
     * Clean-up via close().
     */
    if( ( status = fclose( file ) != 0 ) )
    {
        perror( "fclose" );
        return (-1);
     }

    return (0);
}
#endif
