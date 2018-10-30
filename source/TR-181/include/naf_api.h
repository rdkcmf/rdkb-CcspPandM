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

/// \mainpage Cisco Native Application Framework (NAF) API
/// 
/// This document contains the complete set of platform API for working with
/// the NAF.
/// 


#ifndef _NAF_API_H_

#define _NAF_API_H_

#include <stdint.h>
#include <stdbool.h>
#include "naf.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/* prototypes */

///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Native Application Framework initialization.
///
///         Operation: Initialization function to be called by each process
///         using NAFI.  This is required before any calls to naf_*() APIs.
///         This function should only be called *ONCE* per process. An
///         assigned client ID may be passed in on successive calls.
///
///         Scope: Public
///
/// \param[in,out]  clientId - Assigns this client an ID, unless passed in
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAF
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_Init(tNafClientId *clientId);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Release resources allocated by initialization.
///
///         Operation: Releases all resources allocated by naf_Init().
///         Any active EE instances continue to run after this call.
///
///         Scope: Public
///
/// \param[in]    NONE
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAF
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_Exit();


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Execution Environment creation.
///
///         Operation: Creates a new instance of a native execution environment
///
///         Scope: Public
///
/// \param[out]    eeHandle - Handle for newly created EE
///
/// \return       NAF_SUCCESS, NAF_MEMORY_ERROR, NAF_OTHER_FAILURE,
///               NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAF
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_EEInstanceCreate(tNafEEInstanceHandle *eeHandle);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Free memory allocated by NAF.
///
///         Operation: Frees memory allocated by NAF via the pointer passed
///                    in as a parameter.
///
///         Scope: Public
///
/// \param[in]    ptr - pointer to memory location to be freed
///
/// \return       NAF_SUCCESS, NAF_MEMORY_ERROR, NAF_OTHER_FAILURE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAF
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_Free(void *ptr);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Execution Environment destruction.
///
///         Operation: Destroys an instance of EE, all installed deployment
///                    and execution units are deleted
///
///         Scope: Public
///
/// \param[in]    eeHandle - Handle for EE to be destroyed
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_EEM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_eem_Destroy(const tNafEEInstanceHandle eeHandle);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Execution Environment list.
///
///         Operation: Retrieves a list of all created EE instance handles. In
///         the implementation of this API function, the callee will allocate
///         a buffer, write an array of handles into the buffer, and pass a
///         pointer to the buffer back to the caller through the "eeHandles"
///         parameter. The callee should also pass a pointer to an int
///         containing the size of the array of handles allocated via the
///         "numHandles" parameter. Once the caller has used the output
///         parameters, it will be required to free the pointers.
///
///         Scope: Public
///
/// \param[out]    eeHandles -  List of EE handles
/// \param[out]    numHandles -  Number of elements in list
///
/// \return       NAF_SUCCESS, NAF_MEMORY_ERROR, NAF_OTHER_FAILURE,
///               NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_EEM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_eem_List(tNafEEInstanceHandle *eeHandles, uint32_t *numHandles);

 
///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Execution Environment startup.
///
///         Operation: Starts the EE instance by launching all installed
///         execution units as necessary based on run-level settings.
///
///         Scope: Public
///
/// \param[in]    eeHandle -  Handle for EE instance to be started
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_EEM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_eem_Start(const tNafEEInstanceHandle eeHandle);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Stop an Execution Environment .
///
///         Operation: Stops the EE instance.  Stop all execution units
///         contained within the specified EE instance.
///
///         Scope: Public
///
/// \param[in]    eeHandle -  Handle for EE instance to be stopped
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_EEM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_eem_Stop(const tNafEEInstanceHandle eeHandle);

 
///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Get Execution Environment parameters.
///
///         Operation: Get specified parameter for the specified EE.
///         In the implementation of this API function, for variable length
///         parameters the callee will allocate a buffer, write the value of
///         the specified parameter into the buffer, and pass a pointer to the
///         buffer back to the caller through the "paramValue" parameter. It
///         will also pass back a pointer to an int containing the length of
///         the parameter via the "paramLength" parameter. Once the caller
///         has used the output parameters, it will be required to free the
///         pointers. If the parameter is fixed size, the caller will pass a
///         pointer to the fixed-size type and the callee will write the value
///         into the pointer.
///
///         Scope: Public
///
/// \param[in]    eeHandle - Handle for EE instance to query
/// \param[in]    paramType - Parameter type, specified by #tNafEEParamType
/// \param[out]   paramLength - Length of parameter queried, in bytes
/// \param[out]   paramValue - Returned parameter
///
/// \return       NAF_SUCCESS, NAF_INVALID_PARAM_TYPE, NAF_OTHER_FAILURE,
///               NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_EEM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_eem_ParamGet(const tNafEEInstanceHandle eeHandle,
                               const tNafEEParamType paramType,
                               uint32_t *paramLength, void *paramValue);

 
///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Set Execution Environment parameters.
///
///         Operation: Set parameter for the specified EE.
///
///         Scope: Public
///
/// \param[in]    eeHandle -  Handle for EE instance to modify
/// \param[in]    paramType -  Parameter type, specified by #tNafEEParamType
/// \param[in]    paramLength - Length of parameter being set, in bytes
/// \param[in]    paramValue -  Parameter to set
///
/// \return       NAF_SUCCESS, NAF_INVALID_PARAM_TYPE, NAF_OTHER_FAILURE,
///               NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_EEM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_eem_ParamSet(const tNafEEInstanceHandle eeHandle,
                               const tNafEEParamType paramType,
                               const uint32_t paramLength, const void *paramValue);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Returns a list of package name handles from a given
///         package name
///
///         Operation: Returns zero or more package name handles based on
///         package vendor name, package name, and/or package version.
/// 
///         An empty but valid string ("") should be passed in for any 
///         optional input parameters that are not used (vendor, pkgName,
///         version).
/// 
///         In the implementation of this API function, the callee will
///         allocate a buffer, write an array of handles into the buffer,
///         and pass a pointer to the buffer back to the caller through the
///         "pkgHandles" parameter. The callee is also passed a pointer to
///         an int (the "pkgNum" parameter) into which it will write the
///         size of the array of handles. Once the caller has used the output
///         parameters, it will be required to free the pointers.
/// 
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to modify
/// \param[in]      vendor - String specifying the package vendor (optional)
/// \param[in]      pkgName - String specifying the package name (optional)
/// \param[in]      version - String specifying the package version (optional)
/// \param[out]     pkgHandles - Pointer to array of package handles
/// \param[out]     pkgNum - Number of package handles in array
///
/// \return       NAF_SUCCESS, NAF_PKG_NOT_FOUND, NAF_OTHER_FAILURE,
///               NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_DM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_dm_PackageList(const tNafEEInstanceHandle eeHandle,
                                 const char *vendor,
                                 const char *pkgName, const char *version, 
                                 tNafPkgHandle *pkgHandles, uint32_t *pkgNum);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Returns a package name handle for the given package UUID.
///
///         Operation when only UUID provided: Search internal database for
///         UUID. If no UUID match, return NAF_PKG_NOT_FOUND error. If
///         multiple entries with the same UUID are matched, return a 
///         NAF_MULTIPLE_PACKAGES error. If a single entry matches the UUID,
///         return the package handle.
/// 
///         Operation when both UUID and Package Version provided: Search
///         internal database for UUID and Pkg Version.  This case implies
///         that there are multiple entries with the same UUID such that
///         version must used to find the unique entry. If found, return
///         matched entry's package handle. If not found, return a
///         NAF_PKG_NOT_FOUND error.
/// 
///         An empty but valid string ("") should be passed in for any 
///         optional input parameters that are not used (version).
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to modify
/// \param[in]      uuid - String specifying the package UUID
/// \param[in]      version - String specifying version number of package
/// \param[out]     pkgHandle - Package name handle
///
/// \return       NAF_SUCCESS, NAF_PKG_NOT_FOUND, NAF_MULTIPLE_PACKAGES,
///               NAF_OTHER_FAILURE, NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_DM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_dm_HandleForPackageUUIDGet(const tNafEEInstanceHandle eeHandle,
                                             const tNafPkgUUID uuid,
                                             const char *version,
                                             tNafPkgHandle *pkgHandle);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Returns a package name handle for the given package URL.
///
///         Operation: Returns a package handle for an installed package for the
///         given URL.  This assumes that there is a single installed package
///         with the given URL.
/// 
///         Detailed behavior: Search internal database for URL. If a single
///         entry matches the URL, return the matched entry's package handle.
///         If multiple entries match the URL, return a NAF_MULTIPLE_PACKAGES
///         error. If no entries match the URL, return a NAF_PKG_NOT_FOUND
///         error.
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to modify
/// \param[in]      url - String specifying the package URL
/// \param[out]     pkgHandle - Package name handle
///
/// \return       NAF_SUCCESS, NAF_PKG_NOT_FOUND, NAF_MULTIPLE_PACKAGES,
///               NAF_OTHER_FAILURE, NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_DM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_dm_HandleForPackageURLGet(const tNafEEInstanceHandle eeHandle,
                                            const char *url,
                                            tNafPkgHandle *pkgHandle);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Installs a package.
/// 
///         Operation when both URL and UUID provided: Search internal database
///         for UUID. If an entry or entries matches the UUID, a version of
///         the package is already installed.  Fetch the package from the
///         supplied URL and if the same version is installed, return a
///         NAF_ALREADY_INSTALLED error.  Otherwise, install the version
///         supplied along side the existing version, save the URL and UUID
///         for subsequent upgrades and return a package handle. If no UUID
///         matches, install the package, save the URL and UUID, and return
///         a package handle.
/// 
///         Operation when only URL provided: Fetch the package pointed to by
///         the URL. Query package for the vendor name, pkg name and version
///         and attempt to find a match in the internal database.  If there is
///         a match, reject the install and return a NAF_ALREADY_INSTALLED
///         error, along with the package handle for the installed package.
///         Otherwise, if there is no match, install the package, save
///         the URL and generate a UUID, then return a package handle.
/// 
///         An empty but valid string ("") should be passed in for any 
///         optional input parameters that are not used (username, password,
///         uuid).
/// 
///         If the installation fails, the returned package handle will be
///         set to "-1".
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to modify
/// \param[in]      url - URL with location of package file
/// \param[in]      username - String with username for repository
/// \param[in]      password - String with password for repository
/// \param[in,out]  uuid - String specifying the package UUID
/// \param[out]     pkgHandle - Package handle created by install
///
/// \return       NAF_SUCCESS, NAF_ALREADY_INSTALLED, NAF_OTHER_FAILURE,
///               NAF_INVALID_URL, NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_DM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_dm_PackageInstall(const tNafEEInstanceHandle eeHandle,
                                    const char *url, const char *username,
                                    const char *password, tNafPkgUUID uuid,
                                    tNafPkgHandle *pkgHandle);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Uninstalls a package.
///
///         Operation: Uninstalls a package referenced by the package handle
///         from the specified EE instance. Upon completion, the package
///         handle is no longer valid.
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle to execution environment to modify
/// \param[in]      pkgHandle - Handle to package name to uninstall
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_EE_HANDLE, 
///               NAF_INVALID_PKG_HANDLE
///
/// \pre          NONE
///
/// \post         Packages that listed this package as a dependency are now
///               marked as "unresolved"
///
/// \sa
///
/// \ingroup      NAFI_DM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_dm_PackageUninstall(const tNafEEInstanceHandle eeHandle,
                                      const tNafPkgHandle pkgHandle);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Upgrade a package in all EE instances.
///
///         Operation: Upgrades a package referenced by the packageHandle and
///         an optional URL in all EE instances. If a URL is not provided,
///         the upgrade process uses the saved URL from the previous
///         install or upgrade. If URL is provided, the upgraded process uses
///         the URL and saves it for use in subsequent upgrades.
/// 
///         An empty but valid string ("") should be passed in for any 
///         optional input parameters that are not used (url).
///
///         Scope: Public
///
/// \param[in]    eeHandle - Handle to execution environment to modify
/// \param[in]    pkgHandle - Package name handle for package to be upgraded
/// \param[in]    username - String with updated username for repository
/// \param[in]    password - String with updated password for repository
/// \param[in]    url - URL with location of package file (optional)
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_PKG_HANDLE,
///               NAF_INVALID_URL
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_DM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_dm_PackageUpgrade(const tNafEEInstanceHandle eeHandle,
                                    const tNafPkgHandle pkgHandle,
                                    const char *username, const char *password,
                                    const char *url);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Verify integrity of a package.
///
///         Operation: Verifies integrity of an installed package referenced
///         by the package handle.
/// 
///         If verification succeeds, the function returns NAF_SUCCESS. If
///         it fails, it returns NAF_VERIFICATION_FAILED and an absolute
///         path to a logfile containing details of the verification failure.
///
///         Scope: Public
///
/// \param[in]    eeHandle - Handle to execution environment to modify
/// \param[in]    pkgHandle - Package name handle for package to be verified
/// \param[out]   path - Absolute path to location of error log file (or null
///               string if no error found)
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_PKG_HANDLE,
///               NAF_VERIFICATION_FAILED
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_DM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_dm_PackageVerify(const tNafEEInstanceHandle eeHandle,
                                   const tNafPkgHandle pkgHandle, 
                                   tNafErrorLogPath path);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Get a package level parameter.
///
///         Operation: Gets a DM package level parameter for the given package
///         handle. In the implementation of this API function, for variable
///         length parameters the callee will allocate a buffer, write the
///         value of the specified parameter into the buffer, and pass a
///         pointer to the buffer back to the caller through the "paramValue"
///         parameter. It will also pass back a pointer to an int containing
///         the length of the parameter via the "paramLength" parameter. Once
///         the caller has used the output parameters, it will be required to
///         free the pointers. If the parameter is fixed size, the caller will
///         pass a pointer to the fixed-size type and the callee will write
///         the value into the pointer.
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to query
/// \param[in]      pkgHandle - Package name handle for package to be queried
/// \param[in]      paramType - Parameter type, specified by #tNafPkgParamType
/// \param[out]     paramLength - Length of parameter to be returned, in bytes
/// \param[out]     paramValue - Value of parameter returned
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_PKG_HANDLE,
///               NAF_INVALID_PARAM_TYPE, NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_DM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_dm_PackageParamGet(const tNafEEInstanceHandle eeHandle,
                                     const tNafPkgHandle pkgHandle,
                                     const tNafPkgParamType paramType,
                                     uint32_t *paramLength, void *paramValue);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Set a package level parameter.
///
///         Operation: Sets a DM package level parameter for the given package
///         handle.
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to modify
/// \param[in]      pkgHandle - Package name handle for package to be set
/// \param[in]      paramType - Parameter type, specified by #tNafPkgParamType
/// \param[in]      paramLength - Length of parameter to be set, in bytes
/// \param[in]      paramValue - Value of parameter to be set
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_PKG_HANDLE,
///               NAF_INVALID_PARAM_TYPE, NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_DM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_dm_PackageParamSet(const tNafEEInstanceHandle eeHandle,
                                     const tNafPkgHandle pkgHandle,
                                     const tNafPkgParamType paramType,
                                     const uint32_t paramLength,
                                     const void *paramValue);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Get a DM level parameter.
///
///         Operation: Gets a DM level parameter for the given EE instance.
///         In the implementation of this API function, for variable length
///         parameters the callee will allocate a buffer, write the value of
///         the specified parameter into the buffer, and pass a pointer to the
///         buffer back to the caller through the "paramValue" parameter. It
///         will also pass back a pointer to an int containing the length of
///         the parameter via the "paramLength" parameter. Once the caller
///         has used the output parameters, it will be required to free the
///         pointers. If the parameter is fixed size, the caller will
///         pass a pointer to the fixed-size type and the callee will write
///         the value into the pointer.
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to query
/// \param[in]      paramType - Parameter type, specified by #tNafDmParamType
/// \param[out]     paramLength - Length of parameter to be returned, in bytes
/// \param[out]     paramValue - Value of parameter returned
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_PARAM_TYPE,
///               NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_DM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_dm_ParamGet(const tNafEEInstanceHandle eeHandle,
                              const tNafDmParamType paramType,
                              uint32_t *paramLength, void *paramValue);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Set a DM level parameter.
///
///         Operation: Sets a DM level parameter for the given EE instance.
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to set
/// \param[in]      paramType - Parameter type, specified by #tNafDmParamType
/// \param[in]      paramLength - Length of parameter to be set, in bytes
/// \param[in]      paramValue - Value of parameter set
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_PARAM_TYPE,
///               NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_DM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_dm_ParamSet(const tNafEEInstanceHandle eeHandle,
                              const tNafDmParamType paramType,
                              const uint32_t paramLength,
                              const void *paramValue);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: List package services.
///
///         Operation: Lists all services associated with the specific package
///         referenced by the package handle.
/// 
///         In the implementation of this API function, the callee will
///         allocate a buffer, write the data into the buffer, and pass back
///         a pointer to the buffer through the "svcHandles" parameter. The
///         callee will also pass a pointer to an int containing the size of
///         the array of service handlesvia the "svcNum" parameter. The
///         caller will be required to free these pointers when it has used
///         the data.
/// 
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to modify
/// \param[in]      pkgHandle - Handle for package to modify
/// \param[out]     svcHandles - Pointer to array of service handles
/// \param[out]     svcNum - Number of service handles in array
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_PKG_HANDLE,
///               NAF_INVALID_SVC_HANDLE, NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_EM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_em_ServiceList(const tNafEEInstanceHandle eeHandle,
                                 const tNafPkgHandle pkgHandle,
                                 tNafSvcHandle *svcHandles, uint32_t *svcNum);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Starts the specified service.
///
///         Operation: Starts the servicee referenced by the service handle
///         from the list returned by naf_em_ServiceList().
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to modify
/// \param[in]      pkgHandle - Handle for package to modify
/// \param[in]      svcHandle - Service handle to start service script for
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_SVC_HANDLE,
///               NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_EM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_em_ServiceStart(const tNafEEInstanceHandle eeHandle,
                                  const tNafPkgHandle pkgHandle,
                                  const tNafSvcHandle svcHandle);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Stops the specified service.
///
///         Operation: Stops the service script referenced by the given
///         service handle.
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to modify
/// \param[in]      pkgHandle - Handle for package to modify
/// \param[in]      svcHandle - Service handle to start service script for
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_SVC_HANDLE,
///               NAF_INVALID_EE_HANDLE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_EM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_em_ServiceStop(const tNafEEInstanceHandle eeHandle,
                                 const tNafPkgHandle pkgHandle,
                                 const tNafSvcHandle svcHandle);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Retrieve a parameter for the specified service.
///
///         Operation: Retrieves a parameter for a given service script
///         referenced by the service handle.
/// 
///         Operation: Gets a DM level parameter for the given EE instance.
///         In the implementation of this API function, for variable length
///         parameters the callee will allocate a buffer, write the value of
///         the specified parameter into the buffer, and pass a pointer to the
///         buffer back to the caller through the "paramValue" parameter. It
///         will also pass back a pointer to an int containing the length of
///         the parameter via the "paramLength" parameter. Once the caller
///         has used the output parameters, it will be required to free the
///         pointers. If the parameter is fixed size, the caller will
///         pass a pointer to the fixed-size type and the callee will write
///         the value into the pointer.
/// 
///         
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to modify
/// \param[in]      pkgHandle - Handle for package to modify
/// \param[in]      svcHandle - Service handle
/// \param[in]      paramType - Parameter type, specified by #tNafSvcParamType
/// \param[out]     paramLength - Length of the parameter returned, in bytes
/// \param[out]     paramValue - Value of parameter returned
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_SVC_HANDLE,
///               NAF_INVALID_EE_HANDLE, NAF_INVALID_PARAM_TYPE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_EM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_em_ServiceParamGet(const tNafEEInstanceHandle eeHandle,
                                     const tNafPkgHandle pkgHandle,
                                     const tNafSvcHandle svcHandle,
                                     const tNafSvcParamType paramType, 
                                     uint32_t *paramLength, void *paramValue);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief  Purpose: Set a parameter for the specified service.
///
///         Operation: Sets a parameter for a given service script
///         referenced by the service handle.
///
///         Scope: Public
///
/// \param[in]      eeHandle - Handle specifying which EE to modify
/// \param[in]      pkgHandle - Handle for package to modify
/// \param[in]      svcHandle - Service handle
/// \param[in]      paramType - Parameter type, specified by #tNafSvcParamType
/// \param[in]     paramLength - Length of the parameter to be set, in bytes
/// \param[in]      paramValue - Value of parameter to be set
///
/// \return       NAF_SUCCESS, NAF_OTHER_FAILURE, NAF_INVALID_SVC_HANDLE,
///               NAF_INVALID_EE_HANDLE, NAF_INVALID_PARAM_TYPE
///
/// \pre          NONE
///
/// \post         NONE
///
/// \sa
///
/// \ingroup      NAFI_EM
///
///////////////////////////////////////////////////////////////////////////////

tNafErrorCode naf_em_ServiceParamSet(const tNafEEInstanceHandle eeHandle,
                                     const tNafPkgHandle pkgHandle,
                                     const tNafSvcHandle svcHandle,
                                     const tNafSvcParamType paramType,
                                     const uint32_t paramLength,
                                     const void *paramValue);


#ifdef __cplusplus
}
#endif   // _cplusplus

#endif   // _NAF_API_H_



