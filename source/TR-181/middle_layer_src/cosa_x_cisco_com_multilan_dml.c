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


/**************************************************************************

    module: cosa_x_cisco_com_multilan.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes SBAPIs for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_multilan_dml.h"
#include "cosa_x_cisco_com_multilan_apis.h"
#include "cosa_x_cisco_com_multilan_internal.h"

/***********************************************************************
 IMPORTANT NOTE:

 According to TR69 spec:
 On successful receipt of a SetParameterValues RPC, the CPE MUST apply 
 the changes to all of the specified Parameters atomically. That is, either 
 all of the value changes are applied together, or none of the changes are 
 applied at all. In the latter case, the CPE MUST return a fault response 
 indicating the reason for the failure to apply the changes. 
 
 The CPE MUST NOT apply any of the specified changes without applying all 
 of them.

 In order to set parameter values correctly, the back-end is required to
 hold the updated values until "Validate" and "Commit" are called. Only after
 all the "Validate" passed in different objects, the "Commit" will be called.
 Otherwise, "Rollback" will be called instead.

 The sequence in COSA Data Model will be:

 SetParamBoolValue/SetParamIntValue/SetParamUlongValue/SetParamStringValue
 -- Backup the updated values;

 if( Validate_XXX())
 {
     Commit_XXX();    -- Commit the update all together in the same object
 }
 else
 {
     Rollback_XXX();  -- Remove the update at backup;
 }
 
***********************************************************************/

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_MultiLAN.

    *  X_CISCO_COM_MultiLAN_GetParamStringValue

***********************************************************************/

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_MultiLAN_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
X_CISCO_COM_MultiLAN_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus;
    PCOSA_BASE_OBJECT               pMyObject    = (PCOSA_BASE_OBJECT)g_pCosaBEManager->hMultiLan;

//    AnscTraceFlow(("%s -- %s, buffer size %ul...\n", __FUNCTION__, ParamName, *pUlSize));

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "PrimayLANIPInterface") == 0)
    {
        returnStatus = CosaDmlMlanGetPrimaryLanIpIf((ANSC_HANDLE)pMyObject, pValue, pUlSize);
    }
    else if (strcmp(ParamName, "HomeSecurityIPInterface") == 0)
    {
        returnStatus = CosaDmlMlanGetHomeSecurityIpIf((ANSC_HANDLE)pMyObject, pValue, pUlSize);
    }
    else if (strcmp(ParamName, "PrimaryLANBridge") == 0)
    {
        returnStatus = CosaDmlMlanGetPrimaryLanBridge((ANSC_HANDLE)pMyObject, pValue, pUlSize);
    }
    else if (strcmp(ParamName, "PrimaryLANBridgeHSPorts") == 0)
    {
        returnStatus = CosaDmlMlanGetPrimaryLanBridgeHsPorts((ANSC_HANDLE)pMyObject, pValue, pUlSize);
    }
    else if (strcmp(ParamName, "PrimaryLANDHCPv4ServerPool") == 0)
    {
        returnStatus = CosaDmlMlanGetPrimaryLanDhcpv4ServerPool((ANSC_HANDLE)pMyObject, pValue, pUlSize);
    }
    else if (strcmp(ParamName, "HomeSecurityBridge") == 0)
    {
        returnStatus = CosaDmlMlanGetHomeSecurityBridge((ANSC_HANDLE)pMyObject, pValue, pUlSize);
    }
    else if (strcmp(ParamName, "HomeSecurityBridgePorts") == 0)
    {
        returnStatus = CosaDmlMlanGetHomeSecurityBridgePorts((ANSC_HANDLE)pMyObject, pValue, pUlSize);
    }
    else if (strcmp(ParamName, "HomeSecurityDHCPv4ServerPool") == 0)
    {
        returnStatus = CosaDmlMlanGetHomeSecurityDhcpv4ServerPool((ANSC_HANDLE)pMyObject, pValue, pUlSize);
    }
    else if (strcmp(ParamName, "HomeSecurityWiFiRadio") == 0)
    {
        returnStatus = CosaDmlMlanGetHomeSecurityWiFiRadio((ANSC_HANDLE)pMyObject, pValue, pUlSize);
    }
    else if (strcmp(ParamName, "HomeSecurityWiFiSsid") == 0)
    {
        returnStatus = CosaDmlMlanGetHomeSecurityWiFiSsid((ANSC_HANDLE)pMyObject, pValue, pUlSize);
    }
    else if (strcmp(ParamName, "HomeSecurityWiFiAp") == 0)
    {
        returnStatus = CosaDmlMlanGetHomeSecurityWiFiAp((ANSC_HANDLE)pMyObject, pValue, pUlSize);
    }
    else
    {
        CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
        return  -1;
    }

    /* Process the return code */
    if ( returnStatus == ANSC_STATUS_MORE_DATA )
    {
        return  1;
    }
    else if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        return  0;
    }
    else
    {
        /* What else can I return? */
        return  0;
    }
}

