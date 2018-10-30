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

    module: cosa_com_cisco_spvtg_ccsp_pam_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes middle layer for the COSA Data Model Library

        *  Pam_GetFirstIpInterfaceObjectName

        *  Pam_GetParamBoolValue
        *  Pam_GetParamIntValue
        *  Pam_GetParamUlongValue
        *  Pam_GetParamStringValue
        
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        Ding Hua

    -------------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "plugin_main_apis.h"
#include "cosa_com_cisco_spvtg_ccsp_pam_dml.h"

#define PAM_FIRST_IP_INTERFACE      "PAM_FIRST_IP_INTERFACE"
#define PAM_MAX_IP_INTERFACE_NUM    (256)
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
                            UTILITY ROUTINES
***********************************************************************/

ANSC_STATUS
Pam_GetFirstIpInterfaceObjectName
    (
        BOOL                        bUpstream,
        char*                       pIfObjName,
        PULONG                      pulObjNameSize
    );

ANSC_STATUS
Pam_GetFirstIpInterfaceObjectName
    (
        BOOL                        bUpstream,
        char*                       pIfObjName,
        PULONG                      pulObjNameSize
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_CANT_FIND;
    int                             iReturnValue        = 0;
    ULONG                           ulTotal             = 0;
    ULONG                           ulIndex             = 0;
    ULONG                           ulInstNum           = 0;
    ULONG                           ulLen               = 0;
    char                            pObjName[256]       = {0};
    char                            Buffer[128]         = {0};
    ULONG                           BufferSize          = 0;
    char                            LowerLayers[256]    = {0};
    ULONG                           LowerLayersSize     = 0;
    BOOL                            bLowerLayerUpstream = FALSE;
    char*                           EnvIndex            = NULL;
    ULONG                           ulEnvIndex          = PAM_MAX_IP_INTERFACE_NUM;

#ifdef _COSA_SIM_
    // Hard coded, RTian 09/20/2013
    {
        if(bUpstream) {
            pulObjNameSize = sprintf(pIfObjName, "Device.IP.Interface.1.") + 1;
            //        else pulObjNameSize = sprintf(pIfObjName, "Device.IP.Interface.4.") + 1;
            return ANSC_STATUS_SUCCESS;
        }
    }
#endif

    ulTotal = CosaGetParamValueUlong("Device.IP.InterfaceNumberOfEntries");

    EnvIndex = getenv(PAM_FIRST_IP_INTERFACE);
    if(EnvIndex != NULL)
    {
        ulEnvIndex = atol(EnvIndex);
    }
    
    for ( ulIndex = 0; ulIndex < ulTotal; ulIndex++ )
    {
        if(EnvIndex != NULL && ulEnvIndex < PAM_MAX_IP_INTERFACE_NUM)
        {
            ulInstNum = CosaGetInstanceNumberByIndex("Device.IP.Interface.", ulEnvIndex);
            if( 0 == ulInstNum)
            {
                EnvIndex    = NULL;
                ulEnvIndex  = PAM_MAX_IP_INTERFACE_NUM;
            }
            else
            {
                ulIndex     = EnvIndex;
            }
        }
        ulInstNum = CosaGetInstanceNumberByIndex("Device.IP.Interface.", ulIndex);

        if ( 0 == ulInstNum )
        {
            CcspTraceWarning(("Pam_GetFirstIpInterfaceObjectName -- invalid Device.IP.Interface instance number!\n"));
            returnStatus = ANSC_STATUS_FAILURE;
            break;
        }
        else
        {
            _ansc_sprintf(pObjName, "Device.IP.Interface.%d.LowerLayers", ulInstNum);
            /*CcspTraceInfo(("Checking %s...\n", pObjName));*/

            LowerLayersSize = sizeof(LowerLayers);
            iReturnValue    = CosaGetParamValueString(pObjName, LowerLayers, &LowerLayersSize);
            
            if ( iReturnValue != 0 )
            {
                returnStatus = ANSC_STATUS_FAILURE;
                break;
            }
            else if ( LowerLayersSize == 0 )
            {
                continue;
            }

            do
            {
                /*
                 *  We have to check whether LowerLayers is a partial object name (without '.' at the end) or not 
                 *  Assuming only one lower layer value for now
                 *  Look for the LowerLayer with a empty LowerLayers parameter
                 */
                ulLen = _ansc_strlen(LowerLayers);

                if ( LowerLayers[ulLen - 1] == '.' )
                {
                    LowerLayers[ulLen - 1] = '\0';
                }

                _ansc_strcpy (pObjName, LowerLayers);
                _ansc_sprintf(Buffer, "%s.LowerLayers", LowerLayers);
            
                CcspTraceDebug(("Checking %s...\n", Buffer));
                
                LowerLayersSize = sizeof(LowerLayers);
                iReturnValue    = CosaGetParamValueString(Buffer, LowerLayers, &LowerLayersSize);

                if ( iReturnValue != 0 )
                {
                    break;
                }
                else if ( LowerLayersSize == 0 )
                {
                    break;
                }
            }
            while (TRUE);
            
            /*
             *  Check the Upstream parameter.
             *  Note, not all error cases are covered well here, which happened to help to pass the test
             *  on DRG -- LowerLayers parameter of IP.Interface on top of Bridge.1 is not actually correct.  
             */
            _ansc_sprintf(Buffer, "%s.Upstream", pObjName);
            bLowerLayerUpstream = CosaGetParamValueBool(Buffer);
            
            if ( bUpstream == bLowerLayerUpstream )
            {
                CcspTraceDebug
                    ((
                         "Found the lowest %s layer %s for IP interface Device.IP.Interface.%d.\n",
                         bUpstream ? "upstream" : "downstream",
                         LowerLayers,
                         ulInstNum
                    ));

                _ansc_sprintf(Buffer, "Device.IP.Interface.%d.", ulInstNum);
                BufferSize = _ansc_strlen(Buffer);
                
                if ( BufferSize < *pulObjNameSize )
                {
                    AnscCopyString(pIfObjName, Buffer);
                    *pulObjNameSize = BufferSize;
                    returnStatus    = ANSC_STATUS_SUCCESS;
                }
                else
                {
                    *pulObjNameSize = BufferSize;
                    returnStatus = ANSC_STATUS_MORE_DATA;
                }

                break;                
            }
        }
    }

    return  returnStatus;
}


/***********************************************************************

 APIs for Object:

    InterfaceStack.{i}.

    *  Pam_GetParamBoolValue
    *  Pam_GetParamIntValue
    *  Pam_GetParamUlongValue
    *  Pam_GetParamStringValue

***********************************************************************/


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceStack_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
Pam_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pam_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
Pam_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pam_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
Pam_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;

    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Pam_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pulSize
            );

    description:

        This function is called to retrieve string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pulSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pulSize = required size)
                -1 if not supported.

**********************************************************************/

ULONG
Pam_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    int                             iReturnValue        = 0;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext        = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    char                            IpIfObjName[128]    = {0};
    ULONG                           IpIfObjNameSize     = 0;
    ULONG                           ulInstNum           = 0;
    char                            Buffer[128]         = {0};
    ULONG                           BufferSize          = 0;

/*
    CcspTraceInfo(("Pam_GetParamStringValue -- '%s'\n", ParamName));
*/

    IpIfObjNameSize = sizeof(IpIfObjName) - 1;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "FirstUpstreamIpInterface", TRUE))
    {
        /* We hardcode the value here temporarily. In the future, if LowerLayers mechanism works well, we can change back*/
        /*
        AnscCopyString(pValue, "Device.IP.Interface.1.");
        *pulSize = _ansc_strlen("Device.IP.Interface.1.");
        return  0;
        */

        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(TRUE, IpIfObjName, &IpIfObjNameSize) )
        {
            if ( IpIfObjNameSize < *pulSize )
            {
                AnscCopyString(pValue, IpIfObjName);
                *pulSize = IpIfObjNameSize;
                return  0;
            }
            else
            {
                *pulSize = IpIfObjNameSize;
                return  1;
            }
        }
        else
        {
            return  -1;
        }
    }
    else if( AnscEqualString(ParamName, "FirstDownstreamIpInterface", TRUE))
    {
        /* We hardcode the value here temporarily. In the future, if LowerLayers mechanism works well, we can change back*/
        /*
        AnscCopyString(pValue, "Device.IP.Interface.4.");
        *pulSize = _ansc_strlen("Device.IP.Interface.4.");
        return  0;
        */

        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(FALSE, IpIfObjName, &IpIfObjNameSize) )
        {
            if ( IpIfObjNameSize < *pulSize )
            {
                AnscCopyString(pValue, IpIfObjName);
                *pulSize = IpIfObjNameSize;
                return  0;
            }
            else
            {
                *pulSize = IpIfObjNameSize;
                return  1;
            }
        }
        else
        {
            return  -1;
        }
    }
    else if( AnscEqualString(ParamName, "FirstUpstreamIpv4Address", TRUE))
    {
        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(TRUE, IpIfObjName, &IpIfObjNameSize) )
        {
            _ansc_strcat(IpIfObjName, "IPv4Address.");            
            ulInstNum = CosaGetInstanceNumberByIndex(IpIfObjName, 0);
            
            if ( ulInstNum == 0 )
            {
                 return  -1;
            }
            else
            {
                _ansc_sprintf(Buffer, "%d.", ulInstNum);
                _ansc_strcat(IpIfObjName, Buffer);
                _ansc_strcat(IpIfObjName, "IPAddress");
            }

            BufferSize   = sizeof(Buffer);
            iReturnValue = CosaGetParamValueString(IpIfObjName, Buffer, &BufferSize);
            
            if ( iReturnValue != 0 )
            {
                CcspTraceWarning(("Pam_GetParamStringValue -- failed to retrieve IP address, error code %d!\n", iReturnValue));
                
                return  -1;
            }

            if ( BufferSize < *pulSize )
            {
                AnscCopyString(pValue, Buffer);
                *pulSize = IpIfObjNameSize;
                return  0;
            }
            else
            {
                _ansc_strncpy(pValue, Buffer, *pulSize);       
                *pulSize = IpIfObjNameSize;
                return  1;
            }
        }
        else
        {
            return  -1;
        }
    }
    else if( AnscEqualString(ParamName, "FirstUpstreamIpv4SubnetMask", TRUE))
    {
        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(TRUE, IpIfObjName, &IpIfObjNameSize) )
        {
            _ansc_strcat(IpIfObjName, "IPv4Address.");
            ulInstNum = CosaGetInstanceNumberByIndex(IpIfObjName, 0);
            
            if ( ulInstNum == 0 )
            {
                 return  -1;
            }
            else
            {
                _ansc_sprintf(Buffer, "%d.", ulInstNum);
                _ansc_strcat(IpIfObjName, Buffer);
                _ansc_strcat(IpIfObjName, "SubnetMask");
            }

            BufferSize   = sizeof(Buffer);
            iReturnValue = CosaGetParamValueString(IpIfObjName, Buffer, &BufferSize);

            if ( iReturnValue != 0 )
            {
                CcspTraceWarning(("Pam_GetParamStringValue -- failed to retrieve subnet mask, error code %d!\n", iReturnValue));
                
                return  -1;
            }

            if ( BufferSize < *pulSize )
            {
                AnscCopyString(pValue, Buffer);
                *pulSize = IpIfObjNameSize;
                return  0;
            }
            else
            {
                _ansc_strncpy(pValue, Buffer, *pulSize);
                *pulSize = IpIfObjNameSize;
                return  1;
            }
        }
        else
        {
            return  -1;
        }
    }
    else if( AnscEqualString(ParamName, "FirstDownstreamIpv4Address", TRUE))
    {
        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(FALSE, IpIfObjName, &IpIfObjNameSize) )
        {
            _ansc_strcat(IpIfObjName, "IPv4Address.");            
            ulInstNum = CosaGetInstanceNumberByIndex(IpIfObjName, 0);
            
            if ( ulInstNum == 0 )
            {
                 return  -1;
            }
            else
            {
                _ansc_sprintf(Buffer, "%d.", ulInstNum);
                _ansc_strcat(IpIfObjName, Buffer);
                _ansc_strcat(IpIfObjName, "IPAddress");
            }

            BufferSize   = sizeof(Buffer);
            iReturnValue = CosaGetParamValueString(IpIfObjName, Buffer, &BufferSize);
            
            if ( iReturnValue != 0 )
            {
                CcspTraceWarning(("Pam_GetParamStringValue -- failed to retrieve IP address, error code %d!\n", iReturnValue));
                
                return  -1;
            }

            if ( BufferSize < *pulSize )
            {
                AnscCopyString(pValue, Buffer);
                *pulSize = IpIfObjNameSize;
                return  0;
            }
            else
            {
                _ansc_strncpy(pValue, Buffer, *pulSize);       
                *pulSize = IpIfObjNameSize;
                return  1;
            }
        }
        else
        {
            return  -1;
        }
    }
    else if( AnscEqualString(ParamName, "FirstDownstreamIpv4SubnetMask", TRUE))
    {
        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(FALSE, IpIfObjName, &IpIfObjNameSize) )
        {
            _ansc_strcat(IpIfObjName, "IPv4Address.");
            ulInstNum = CosaGetInstanceNumberByIndex(IpIfObjName, 0);
            
            if ( ulInstNum == 0 )
            {
                 return  -1;
            }
            else
            {
                _ansc_sprintf(Buffer, "%d.", ulInstNum);
                _ansc_strcat(IpIfObjName, Buffer);
                _ansc_strcat(IpIfObjName, "SubnetMask");
            }

            BufferSize   = sizeof(Buffer);
            iReturnValue = CosaGetParamValueString(IpIfObjName, Buffer, &BufferSize);

            if ( iReturnValue != 0 )
            {
                CcspTraceWarning(("Pam_GetParamStringValue -- failed to retrieve subnet mask, error code %d!\n", iReturnValue));
                
                return  -1;
            }

            if ( BufferSize < *pulSize )
            {
                AnscCopyString(pValue, Buffer);
                *pulSize = IpIfObjNameSize;
                return  0;
            }
            else
            {
                _ansc_strncpy(pValue, Buffer, *pulSize);
                *pulSize = IpIfObjNameSize;
                return  1;
            }
        }
        else
        {
            return  -1;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

