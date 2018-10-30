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

/* NAF API Version: 0.1 */

#ifndef _NAF_H_

#define _NAF_H_

#ifdef __cplusplus
extern "C" 
{
#endif

/* defines */

#define NAF_UUID_LENGTH 36

#define NAF_CLIENTID_LENGTH 50

/* typedefs */

typedef char tNafClientId[NAF_CLIENTID_LENGTH];

typedef void *tNafEEInstanceHandle;

typedef void *tNafPkgHandle;

typedef char tNafPkgUUID[NAF_UUID_LENGTH + 1];

typedef void *tNafSvcHandle;

typedef char *tNafErrorLogPath;

typedef enum
{
    EE_SYSTEM = 0     ///< EE for system processes
}tNafEEType;

typedef enum
{
    EE_STARTED = 0,   ///< Execution environment is running
    EE_STOPPED = 1,   ///< Execution environment is stopped
    EE_ERROR   = 2    ///< Error while running execution environment
}tNafEEStatus;

typedef enum
{
    EE_TYPE             = 0,    ///< Type of this EE, defined by #tNafEEType (Read-only, Size 4 bytes, int)
    EE_NAME             = 0,    ///< Type of this EE, defined by #tNafEEType (Read-only, Size 4 bytes, int)
    EE_STATUS           = 1,    ///< Run status for this EE, defined by #tNafEEStatus (Read-only, Size 4 bytes, int)
    EE_VERSION          = 2,    ///< Version of this EE (Read-only, Size 4 bytes, int)
    EE_DEFAULT_RUNLEVEL = 3,    ///< Default run level for this EE (Read-only, Size 4 bytes, int)
    EE_CURRENT_RUNLEVEL = 4,    ///< Current run level for this EE (Read-only, Size 4 bytes, int)
    EE_AUTOSTART        = 5     ///< Flag indicating this EE should start at startup (Read/write, Size 1 byte, bool)
}tNafEEParamType;

typedef enum
{
    NAF_SUCCESS             =  0,   ///< No error
    NAF_OTHER_FAILURE       = -1,   ///< Undefined failure
    NAF_PKG_NOT_FOUND       = -2,   ///< Package was not found
    NAF_MULTIPLE_PACKAGES   = -3,   ///< Multiple packages were found, could not return single package
    NAF_ALREADY_INSTALLED   = -4,   ///< Tried to install a package that was already installed
    NAF_MEMORY_ERROR        = -5,   ///< Memory-related error while performing an API operation
    NAF_INVALID_PKG_HANDLE  = -6,   ///< Package handle provided was invalid
    NAF_INVALID_PARAM_TYPE  = -7,   ///< Parameter type provided was invalid
    NAF_INVALID_URL         = -8,   ///< URL provided was invalid or not found
    NAF_INVALID_EE_HANDLE   = -9,   ///< EE handle provided was invalid
    NAF_INVALID_SVC_HANDLE  = -10,  ///< Service handle provided was invalid
    NAF_VERIFICATION_FAILED = -11   ///< Package verification failed, see logfile for details
}tNafErrorCode;

typedef enum
{
    PKG_VENDOR       = 0,    ///< Package Vendor Name (Read-only, Size 128 bytes, string)
    PKG_NAME         = 1,    ///< Package Name (Read-only, Size 128 bytes, string)
    PKG_VERSION      = 2,    ///< Package Version (Read-only, Size 128 bytes, string)
    PKG_URL          = 3,    ///< URL the package was retrieved from (Read-only, Size 128 bytes, string)
    PKG_CFG_LIST     = 4,    ///< List of package configuration files (Read-only, Size 4 bytes, pointer)
    PKG_CFG_LISTLEN  = 5,    ///< Configuration file list length (Read-only, Size 4 bytes, int)
    PKG_LOG_LIST     = 6,    ///< List of package log files (Read-only, Size 4 bytes, pointer)
    PKG_LOG_LISTLEN  = 7,    ///< Log file list length (Read-only, Size 4 bytes, int)
    PKG_SVC_LIST     = 8,    ///< List of package service scripts (Read-only, Size 4 bytes, pointer)
    PKG_SVC_LISTLEN  = 9,    ///< Service script list length (Read-only, Size 4 bytes, int)
    PKG_UUID         = 10,   ///< Package UUID (Read-only, Size 36 bytes, string)
    PKG_DUID         = 11,   ///< Package DUID (Read-only, Size 128 bytes, string)
    PKG_RESOLVED     = 12,   ///< Flag indicating package dependency resolution (Read-only, Size 1 byte, bool)
    PKG_DEPLIST      = 13,   ///< List of dependencies for this package (Read-only, Size 4 bytes, pointer)
    PKG_DEPLISTLEN   = 14,   ///< Dependency list length (Read-only, Size 4 bytes, int)
    PKG_AUTOSTART    = 15    ///< Flag indicating this package should run at startup (Read/Write, Size 1 byte, bool)
}tNafPkgParamType;

typedef enum
{
    DM_PARAM = 0   ///< Placeholder parameter
}tNafDmParamType;

typedef enum
{
    SVC_IDLE     = 0,   ///< Service is currently idle
    SVC_STARTING = 1,   ///< Service is currently starting
    SVC_ACTIVE   = 2,   ///< Service is currently active
    SVC_STOPPING = 3,   ///< Service is currently stopping
    SVC_STOPPED  = 4,   ///< Service is currently stopped
    SVC_FAILED   = 5    ///< Service failed to perform an operation
}tNafSvcState;

typedef enum
{
    SVC_EUID        = 0,    ///< Path to the service script for this service (Read-only, Size 128 bytes, string)
    SVC_SCRIPTPATH  = 0,    ///< Path to the service script for this service (Read-only, Size 128 bytes, string)
    SVC_RUNSTATE    = 1,    ///< Run state for this service, defined by #tNafSvcState (Read-only, Size 4 bytes, int)
    SVC_MEMUSAGE    = 2,    ///< Current memory usage for this service (Read-only, Size 4 bytes, int)
    SVC_CPUUTIL     = 3,    ///< Current CPU utilization for this service (Read-only, Size 4 bytes, int)
    SVC_TX_NETUSAGE = 4,    ///< Current transmit network usage for this service (Read-only, Size 4 bytes, int)
    SVC_RX_NETUSAGE = 5     ///< Current receive network usage for this service (Read-only, Size 4 bytes, int)
}tNafSvcParamType;

#ifdef __cplusplus
}
#endif   // _cplusplus

#endif   // _NAF_API_H_
