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

    module: cosa_nat_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

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
#include "cosa_nat_dml.h"
#include "cosa_nat_dml_custom.h"
#include "plugin_main_apis.h"
#include "cosa_nat_internal.h"
#include "msgpack.h"
#include "webcfgparam.h"
#include "helpers.h"
#include "portmappingdoc.h"
#include "ccsp_trace.h"
#include "dml_tr181_custom_cfg.h"
#include "safec_lib_common.h"
#include "cosa_nat_apis.h"
#include "base64.h"
#include "cosa_x_comcast_com_parentalcontrol_apis.h"

#if     CFG_USE_CCSP_SYSLOG
    #include <ccsp_syslog.h>
#endif

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

    NAT.

    *  NAT_GetParamBoolValue
    *  NAT_GetParamIntValue
    *  NAT_GetParamUlongValue
    *  NAT_GetParamStringValue

***********************************************************************/
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        NAT_GetParamBoolValue
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
NAT_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    BOOL                            bReturnValue;

    bReturnValue =
        NAT_GetParamBoolValue_Custom
            (
                hInsContext,
                ParamName,
                pBool
            );

    if ( bReturnValue )
    {
        return bReturnValue;
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        NAT_GetParamIntValue
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
NAT_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        NAT_GetParamUlongValue
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
NAT_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT             pMyObject    = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT                   pNat         = &pMyObject->Nat;

    CosaDmlNatGet(NULL, pNat);
#if defined(FEATURE_MAPT) || defined(FEATURE_SUPPORT_MAPT_NAT46)
//#if defined (_XB6_PRODUCT_REQ_) || defined (_XB7_PRODUCT_REQ_)
    int nports=0;
#endif

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "X_CISCO_COM_TCPTimeout") == 0)
    {
        /* collect value */
        *puLong  = pNat->X_CISCO_COM_TCPTimeout;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_UDPTimeout") == 0)
    {
        /* collect value */
        *puLong  = pNat->X_CISCO_COM_UDPTimeout;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_ICMPTimeout") == 0)
    {
        /* collect value */
        *puLong  = pNat->X_CISCO_COM_ICMPTimeout;

        return TRUE;
    }

#if defined(FEATURE_MAPT) || defined(FEATURE_SUPPORT_MAPT_NAT46)
//if defined (_XB6_PRODUCT_REQ_) || defined (_XB7_PRODUCT_REQ_)
    if (strcmp(ParamName, "X_RDK_NumberActiveIPv4TcpInternalPorts") == 0)
    {
       nports=0;
       CosaDmlNatGetActiveIPv4TcpInternalPorts(&nports);
       *puLong = nports;

       return TRUE; 
    }
       
    if (strcmp(ParamName, "X_RDK_NumberActiveIPv4UdpInternalPorts") == 0)
    {
       nports=0;
       CosaDmlNatGetActiveIPv4UdpInternalPorts(&nports);
       *puLong = nports;

       return TRUE;        
    }
#endif
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        NAT_GetParamStringValue
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
NAT_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

BOOL
NAT_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    BOOL                            bReturnValue;

    bReturnValue =
        NAT_SetParamBoolValue_Custom
            (
                hInsContext,
                ParamName,
                bValue
            );

    if ( bReturnValue )
    {
        return bReturnValue;
    }
    else
    {
        /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
        return FALSE;
    }
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        NAT_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
NAT_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{    
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT             pMyObject    = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT                   pNat         = &pMyObject->Nat;

    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return FALSE;
    #endif

    if (strcmp(ParamName, "X_CISCO_COM_TCPTimeout") == 0)
    {
        /* save update to backup */
        pNat->X_CISCO_COM_TCPTimeout = uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_UDPTimeout") == 0)
    {
        /* save update to backup */
        pNat->X_CISCO_COM_UDPTimeout = uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_ICMPTimeout") == 0)
    {
        /* save update to backup */
        pNat->X_CISCO_COM_ICMPTimeout = uValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        NAT_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation.

                ULONG*                      puLength
                The output length of the param name.

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
NAT_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        NAT_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
NAT_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pMyObject     = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT                   pNat          = &pMyObject->Nat;

    returnStatus = CosaDmlNatSet(NULL, pNat);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        NAT_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
NAT_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pMyObject     = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT                   pNat          = &pMyObject->Nat;

    returnStatus = CosaDmlNatGetDmz(NULL, (PCOSA_DML_NAT_DMZ)pNat);

    return returnStatus;

}

/***********************************************************************

 APIs for Object:

    NAT.X_CISCO_COM_DMZ.

    *  X_CISCO_COM_DMZ_GetParamBoolValue
    *  X_CISCO_COM_DMZ_GetParamStringValue
    *  X_CISCO_COM_DMZ_SetParamBoolValue
    *  X_CISCO_COM_DMZ_SetParamStringValue
    *  X_CISCO_COM_DMZ_Validate
    *  X_CISCO_COM_DMZ_Commit
    *  X_CISCO_COM_DMZ_Rollback

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_CISCO_COM_DMZ_GetParamBoolValue
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
X_CISCO_COM_DMZ_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT             pNat         = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT_DMZ               pDmz         = &pNat->Dmz;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pDmz->bEnabled;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        X_CISCO_COM_DMZ_GetParamStringValue
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
X_CISCO_COM_DMZ_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT             pNat         = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT_DMZ               pDmz         = &pNat->Dmz;
    errno_t                         rc           = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Data") == 0)
    {
        CcspTraceWarning(("Data Get Not supported\n"));
        return 0;
    }

    if (strcmp(ParamName, "RemoteIPStart") == 0)
    {
        /* collect value */
        if ( AnscSizeOfString(pDmz->RemoteIPStart) < *pUlSize)
        {
            rc = strcpy_s(pValue, *pUlSize, pDmz->RemoteIPStart);
            if (rc != EOK)
            {
              ERR_CHK(rc);
              return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDmz->RemoteIPStart)+1;
            return 1;
        }
    }

    if (strcmp(ParamName, "RemoteIPEnd") == 0)
    {
        /* collect value */
        if ( AnscSizeOfString(pDmz->RemoteIPEnd) < *pUlSize)
        {
            rc = strcpy_s(pValue, *pUlSize, pDmz->RemoteIPEnd);
            if (rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDmz->RemoteIPEnd)+1;
            return 1;
        }
    }

    if (strcmp(ParamName, "InternalIP") == 0)
    {
        /* collect value */
        if ( AnscSizeOfString(pDmz->InternalIP) < *pUlSize)
        {
            rc = strcpy_s(pValue, *pUlSize, pDmz->InternalIP);
            if (rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDmz->InternalIP)+1;
            return 1;
        }
    }

    if (strcmp(ParamName, "InternalMAC") == 0)
    {
        /* collect value */
        if ( AnscSizeOfString(pDmz->InternalMAC) < *pUlSize)
        {
            rc = strcpy_s(pValue, *pUlSize, pDmz->InternalMAC);
            if (rc != EOK)
            {  
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDmz->InternalMAC)+1;
            return 1;
        }
    }

    if (strcmp(ParamName, "IPv6Host") == 0)
    {
        /* collect value */
        if ( AnscSizeOfString(pDmz->IPv6Host) < *pUlSize)
        {
            rc = strcpy_s(pValue, *pUlSize, pDmz->IPv6Host);
            if (rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDmz->IPv6Host)+1;
            return 1;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_CISCO_COM_DMZ_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_CISCO_COM_DMZ_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT             pNat         = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT_DMZ               pDmz         = &pNat->Dmz;
    errno_t                         rc           = -1;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pDmz->bEnabled     = bValue;
        if (bValue == FALSE) {
            rc = STRCPY_S_NOCLOBBER(pDmz->InternalIP, sizeof(pDmz->InternalIP), "0.0.0.0");  /* keep sync between webui and snmp */
            if (rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }
            //CISCOXB3-5927 : ip6 table is not getting restored
            memset(pDmz->IPv6Host ,0 ,sizeof(pDmz->IPv6Host));
        }
    #if CFG_USE_CCSP_SYSLOG
        /* Bad practice to use platform dependent and will be rectified -- CCSP_TRACE should be used */
        syslog_systemlog("DMZ", LOG_NOTICE, "%s", (bValue==TRUE)?"Enabled":"Disabled");
    #endif

        CcspTraceNotice(("DMZ_split:Feature Switch DMZ %d\n", bValue));
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_CISCO_COM_DMZ_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_CISCO_COM_DMZ_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT             pNat         = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT_DMZ               pDmz         = &pNat->Dmz;
    ULONG                           dmzHost;
    errno_t                         rc           = -1;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Data") == 0)
     {
	char * decodeMsg =NULL;
	int decodeMsgSize =0;
	int size =0;

	struct timespec start,end,*startPtr,*endPtr;
        startPtr = &start;
        endPtr = &end;

	msgpack_zone mempool;
	msgpack_object deserialized;
	msgpack_unpack_return unpack_ret;

	getCurrentTime(startPtr);
	decodeMsgSize = b64_get_decoded_buffer_size(strlen(pString));
	decodeMsg = (char *) malloc(sizeof(char) * decodeMsgSize);
	if( NULL == decodeMsg)
	{
		CcspTraceWarning(("decodeMsg allocation failed\n"));
		return FALSE;
	}

	size = b64_decode( (const uint8_t*)pString, strlen(pString), (uint8_t *)decodeMsg );
	CcspTraceWarning(("base64 decoded data contains %d bytes\n",size));

	getCurrentTime(endPtr);
        CcspTraceWarning(("Base64 decode Elapsed time : %ld ms\n", timeValDiff(startPtr, endPtr)));

	msgpack_zone_init(&mempool, 2048);
	unpack_ret = msgpack_unpack(decodeMsg, size, NULL, &mempool, &deserialized);

	switch(unpack_ret)
	{
		case MSGPACK_UNPACK_SUCCESS:
			CcspTraceWarning(("MSGPACK_UNPACK_SUCCESS :%d\n",unpack_ret));
		break;
		case MSGPACK_UNPACK_EXTRA_BYTES:
			CcspTraceWarning(("MSGPACK_UNPACK_EXTRA_BYTES :%d\n",unpack_ret));
		break;
		case MSGPACK_UNPACK_CONTINUE:
			CcspTraceWarning(("MSGPACK_UNPACK_CONTINUE :%d\n",unpack_ret));
		break;
		case MSGPACK_UNPACK_PARSE_ERROR:
			CcspTraceWarning(("MSGPACK_UNPACK_PARSE_ERROR :%d\n",unpack_ret));
		break;
		case MSGPACK_UNPACK_NOMEM_ERROR:
			CcspTraceWarning(("MSGPACK_UNPACK_NOMEM_ERROR :%d\n",unpack_ret));
		break;
		default:
			CcspTraceWarning(("Message Pack decode failed with error: %d\n", unpack_ret));
	}

	msgpack_zone_destroy(&mempool);
	//End of msgpack decoding

	if(unpack_ret == MSGPACK_UNPACK_SUCCESS)
	{
		dmz_wandoc_t *rpm;
		rpm = dmz_wandoc_convert( decodeMsg, size+1 );
		if(decodeMsg != NULL)
		{
			free(decodeMsg);
			decodeMsg = NULL;
		}

		if (NULL !=rpm)
		{
			CcspTraceWarning(("rpm->subdoc_name is %s\n", rpm->subdoc_name));
			CcspTraceWarning(("rpm->version is %lu\n", (unsigned long)rpm->version));
			CcspTraceWarning(("rpm->transaction_id is %d\n", rpm->transaction_id));
			CcspTraceWarning(("DMZ/wan  configuration received\n"));

			execData *execDataDmz = NULL ;

			execDataDmz = (execData*) malloc (sizeof(execData));

			if ( execDataDmz != NULL )
			{

				memset(execDataDmz, 0, sizeof(execData));

				execDataDmz->txid = rpm->transaction_id;
				execDataDmz->version = rpm->version;

				strncpy(execDataDmz->subdoc_name,"wan",sizeof(execDataDmz->subdoc_name)-1);

				execDataDmz->user_data = (void*) rpm ;
				execDataDmz->calcTimeout = NULL ;
				execDataDmz->executeBlobRequest = Process_DMZ_WebConfigRequest;
				execDataDmz->rollbackFunc = rollback_dmz;
				execDataDmz->freeResources = freeResources_dmz;
				PushBlobRequest(execDataDmz);

				CcspTraceWarning(("PushBlobRequest complete\n"));

				return TRUE;
			}
			else
			{
				CcspTraceWarning(("execData memory allocation failed\n"));
				dmz_wandoc_destroy( rpm );
				return FALSE;
			}

		  }
		return TRUE;
	}
	else
	{
		if(decodeMsg != NULL)
		{
			free(decodeMsg);
			decodeMsg = NULL;
		}
		CcspTraceWarning(("Corrupted DMZ/wan  value\n"));
		return FALSE;
	}
        return TRUE;
    }


    if (strcmp(ParamName, "RemoteIPStart") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER( pDmz->RemoteIPStart, sizeof(pDmz->RemoteIPStart), pString);
        if (rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "RemoteIPEnd") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER( pDmz->RemoteIPEnd, sizeof(pDmz->RemoteIPEnd), pString );
        if (rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "InternalIP") == 0)
    {
        /* save update to backup */
        if (strcmp(pString, "0.0.0.0") == 0) { /* keep sync between gui and snmp */
            // pDmz->bEnabled = FALSE;
            rc = STRCPY_S_NOCLOBBER(pDmz->InternalIP, sizeof(pDmz->InternalIP), pString);
            if (rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }
        }
        else if (strcmp(pString, "") == 0) { /* snmp comes with 0.0.0.0 */
            // pDmz->bEnabled = FALSE;
            rc = STRCPY_S_NOCLOBBER(pDmz->InternalIP, sizeof(pDmz->InternalIP), "0.0.0.0");
            if (rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }
        }
        else{
            // return FALSE if address is not of "a.b.c.d" form
            ANSC_STATUS ret;
            char wrapstring[256] = {0};
            ret = isValidIP(4, pString, wrapstring, sizeof(wrapstring));
            if(ANSC_STATUS_SUCCESS != ret)
                return FALSE;

            dmzHost = (ULONG)_ansc_inet_addr(pString);
            if(FALSE == CosaDmlNatChkPortMappingClient(dmzHost)){
                return FALSE;   /* dmz host not in local lan network */
            }else{
                rc = STRCPY_S_NOCLOBBER( pDmz->InternalIP, sizeof(pDmz->InternalIP), pString );
                if (rc != EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                pDmz->bEnabled = TRUE;
            }
        }

        return TRUE;
    }

    if (strcmp(ParamName, "InternalMAC") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER( pDmz->InternalMAC, sizeof(pDmz->InternalMAC), pString );
        if (rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "IPv6Host") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER( pDmz->IPv6Host, sizeof(pDmz->IPv6Host), pString );
        if (rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_CISCO_COM_DMZ_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation.

                ULONG*                      puLength
                The output length of the param name.

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
X_CISCO_COM_DMZ_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    PCOSA_DATAMODEL_NAT             pNat         = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT_DMZ               pDmz         = &pNat->Dmz;
    ULONG                           dmzHost      = (ULONG)_ansc_inet_addr(pDmz->InternalIP);

    if (IPv4Addr_IsLoopback(dmzHost)    ||
        IPv4Addr_IsMulticast(dmzHost)   ||
        (dmzHost == 0xffffffff)){       /* only check whole network broadcast address */
        goto rollback;
    }

    return TRUE;

rollback:
    CosaDmlNatGetDmz(NULL, pDmz);
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        X_CISCO_COM_DMZ_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
X_CISCO_COM_DMZ_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pNat          = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT_DMZ               pDmz          = &pNat->Dmz;

    CcspTraceWarning(("!!!!!! in X_CISCO_COM_DMZ_Commit pDmz->InternalIP: %s !!!!!!\n", pDmz->InternalIP));

    returnStatus = CosaDmlNatSetDmz(NULL, pDmz);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        X_CISCO_COM_DMZ_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
X_CISCO_COM_DMZ_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pNat          = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT_DMZ               pDmz          = &pNat->Dmz;

    returnStatus = CosaDmlNatGetDmz(NULL, pDmz);

    return returnStatus;

}

/***********************************************************************

 APIs for Object:

    NAT.PortMapping.{i}.

    *  PortMapping_GetEntryCount
    *  PortMapping_GetEntry
    *  PortMapping_IsUpdated
    *  PortMapping_Synchronize
    *  PortMapping_AddEntry
    *  PortMapping_DelEntry
    *  PortMapping_GetParamBoolValue
    *  PortMapping_GetParamIntValue
    *  PortMapping_GetParamUlongValue
    *  PortMapping_GetParamStringValue
    *  PortMapping_SetParamBoolValue
    *  PortMapping_SetParamIntValue
    *  PortMapping_SetParamUlongValue
    *  PortMapping_SetParamStringValue    
    *  PortMapping_Validate
    *  PortMapping_Commit
    *  PortMapping_Rollback

***********************************************************************/
/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PortMapping_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
PortMapping_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT             pNat         = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;

    return AnscSListQueryDepth( &pNat->NatPMappingList );

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        PortMapping_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
PortMapping_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT                   pMyObject         = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PSINGLE_LINK_ENTRY                    pSListEntry       = NULL;
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT    pCxtLink          = NULL;

    pSListEntry       = AnscSListGetEntryByIndex(&pMyObject->NatPMappingList, nIndex);
    if ( pSListEntry )
    {
        pCxtLink      = ACCESS_COSA_CONTEXT_PMAPPING_LINK_OBJECT(pSListEntry);
        *pInsNumber   = pCxtLink->InstanceNumber;
    }

    return (ANSC_HANDLE)pSListEntry;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortMapping_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
PortMapping_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT             pMyObject    = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    BOOL                            bIsUpdated   = TRUE;


    /*
        We can use one rough granularity interval to get whole table in case
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pMyObject->PreviousVisitTime ) < COSA_DML_NAT_PORTMAPPING_ACCESS_INTERVAL )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pMyObject->PreviousVisitTime =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

    return bIsUpdated;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PortMapping_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
PortMapping_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                           returnStatus      = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_NAT                   pNat              = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT    pCxtLink          = NULL;
    PSINGLE_LINK_ENTRY                    pSListEntry       = NULL;
    PSINGLE_LINK_ENTRY                    pSListEntry2      = NULL;
    PCOSA_DML_NAT_PMAPPING                pPortMapping      = NULL;
    PCOSA_DML_NAT_PMAPPING                pPortMapping2     = NULL;
    ULONG                                 entryCount        = 0;
    ULONG                                 i                 = 0;
    PULONG                                pulTmp            = 0;

    pPortMapping         = CosaDmlNatGetPortMappings(NULL,&entryCount);
    if ( pPortMapping && entryCount )
    {
        pulTmp    =   AnscAllocateMemory( entryCount * sizeof(ULONG) );
        if ( !pulTmp )
        {
            goto EXIT;
        }
    }

    /* Search the whole link and mark bFound of exist entry to FALSE */
    pSListEntry =   AnscSListGetFirstEntry(&pNat->NatPMappingList);
    while( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_PMAPPING_LINK_OBJECT(pSListEntry);
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);

        pCxtLink->bFound  = FALSE;
    }
	
    /* go over all new entries, find them in current link table and mark them */
    for ( i = 0; i < entryCount; i++)
    {
        pSListEntry =   AnscSListGetFirstEntry(&pNat->NatPMappingList);
        while( pSListEntry )
        {
            pCxtLink          = ACCESS_COSA_CONTEXT_PMAPPING_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);

            pPortMapping2     = (PCOSA_DML_NAT_PMAPPING)pCxtLink->hContext;
            if ( pPortMapping2->InstanceNumber == pPortMapping[i].InstanceNumber )
            {
                pulTmp[i]         =  1;
                pCxtLink->bFound  = TRUE;

                /* If found, update the content also */
                *pPortMapping2    = pPortMapping[i];
                pCxtLink->bNew    = FALSE;

                break;
            }
        }
    }

    /* We need del unreferred entry if it's not delay_added entry */
    pSListEntry =   AnscSListGetFirstEntry(&pNat->NatPMappingList);
    while( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_PMAPPING_LINK_OBJECT(pSListEntry);
        pSListEntry2      = pSListEntry;
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);

        if ( pCxtLink->bFound  == FALSE && pCxtLink->bNew  == FALSE )
        {
            AnscSListPopEntryByLink(&pNat->NatPMappingList, pSListEntry2);

            AnscFreeMemory( pCxtLink->hContext );
            AnscFreeMemory( pCxtLink );
        }
    }

    /* We add new entry into our link table */
    for ( i = 0; i < entryCount; i++ )
    {
        if ( pulTmp[i] == 0 )
        {
            /* Add new one */
            pCxtLink             = AnscAllocateMemory(sizeof(COSA_CONTEXT_PMAPPING_LINK_OBJECT));
            if ( !pCxtLink )
            {
                goto EXIT;
            }

            pPortMapping2        = AnscAllocateMemory(sizeof(COSA_DML_NAT_PMAPPING));
            if ( !pPortMapping2 )
            {
                AnscFreeMemory(pCxtLink);
                goto EXIT;
            }
            /* copy new content which should include InstanceNumber and Alias */
            *pPortMapping2       = pPortMapping[i];

            pCxtLink->hContext   = (ANSC_HANDLE)pPortMapping2;
            pCxtLink->InstanceNumber = pPortMapping2->InstanceNumber;
            pCxtLink->bNew       = FALSE;

            CosaSListPushEntryByInsNum(&pNat->NatPMappingList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
        }
    }

    returnStatus =  ANSC_STATUS_SUCCESS;

EXIT:
    if (pulTmp)
        AnscFreeMemory(pulTmp);

    if (pPortMapping)
        AnscFreeMemory(pPortMapping);
		
    return returnStatus;


}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        PortMapping_AddEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to add a new entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle of new added entry.

**********************************************************************/
ANSC_HANDLE
PortMapping_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT                  pNat              = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT_PMAPPING               pNatPMapping      = NULL;
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT   pPMappingCxtLink  = NULL;

    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return NULL;
    #endif
    

    pNatPMapping = (PCOSA_DML_NAT_PMAPPING)AnscAllocateMemory(sizeof(COSA_DML_NAT_PMAPPING));
    if ( !pNatPMapping )
    {
        return NULL;
    }

    pPMappingCxtLink = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_PMAPPING_LINK_OBJECT));
    if ( !pPMappingCxtLink )
    {
        goto EXIT;
    }

    NAT_PORTMAPPING_SET_DEFAULTVALUE(pNatPMapping);

    /* now we have this link content */
    pPMappingCxtLink->hContext = (ANSC_HANDLE)pNatPMapping;
    pPMappingCxtLink->bNew     = TRUE;

    /* Get InstanceNumber and Alias */
    CosaNatGen(NULL, pNatPMapping);

    pNatPMapping->InstanceNumber     = pNat->MaxInstanceNumber;
    pPMappingCxtLink->InstanceNumber = pNatPMapping->InstanceNumber ;
    *pInsNumber                      = pNatPMapping->InstanceNumber ;
    
    /*set the origin of this rule to "Static" when add the new entry*/
    pNatPMapping->X_CISCO_COM_Origin = COSA_DML_NAT_PMAPPING_Origin_Static;

    CosaSListPushEntryByInsNum(&pNat->NatPMappingList, (PCOSA_CONTEXT_LINK_OBJECT)pPMappingCxtLink);

    CosaNatRegSetNatInfo(pNat);

    return (ANSC_HANDLE)pPMappingCxtLink;

EXIT:
    AnscFreeMemory(pNatPMapping);

    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PortMapping_DelEntry
            (
                ANSC_HANDLE                 hInsContext,
                ANSC_HANDLE                 hInstance
            );

    description:

        This function is called to delete an exist entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ANSC_HANDLE                 hInstance
                The exist entry handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
PortMapping_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                              returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT                      pNat              = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT       pPMappingCxtLink  = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)hInstance;
    PCOSA_DML_NAT_PMAPPING                   pNatPMapping      = (PCOSA_DML_NAT_PMAPPING)pPMappingCxtLink->hContext;

    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return ANSC_STATUS_FAILURE;
    #endif

    if ( pPMappingCxtLink->bNew )
    {
        /* Set bNew to FALSE to indicate this node is not going to save to SysRegistry */
        pPMappingCxtLink->bNew = FALSE;

        returnStatus = CosaNatRegSetNatInfo(pNat);
    }
    else
    {
        returnStatus = CosaDmlNatDelPortMapping(NULL, pNatPMapping->InstanceNumber);
    }

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        AnscSListPopEntryByLink(&pNat->NatPMappingList, &pPMappingCxtLink->Linkage);

        AnscFreeMemory(pPMappingCxtLink->hContext);
        AnscFreeMemory(pPMappingCxtLink);
    }
#ifdef _BWG_PRODUCT_REQ_
    //CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: START
    UpdateList(NULL);
    //CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: END
#endif
    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortMapping_GetParamBoolValue
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
PortMapping_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PMAPPING                   pNatPMapping  = (PCOSA_DML_NAT_PMAPPING)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pNatPMapping->bEnabled;

        return TRUE;
    }

    if (strcmp(ParamName, "AllInterfaces") == 0)
    {
        /* collect value */
        *pBool = pNatPMapping->AllInterfaces;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortMapping_GetParamIntValue
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
PortMapping_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortMapping_GetParamUlongValue
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
PortMapping_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT        pCxtLink      = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PMAPPING                    pNatPMapping  = (PCOSA_DML_NAT_PMAPPING)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        *puLong  = pNatPMapping->Status;

        return TRUE;
    }

    if (strcmp(ParamName, "LeaseDuration") == 0)
    {
        /* collect value */
        *puLong  = pNatPMapping->LeaseDuration;

        return TRUE;
    }

    if (strcmp(ParamName, "ExternalPort") == 0)
    {
        /* collect value */
        *puLong  = pNatPMapping->ExternalPort;

        return TRUE;
    }

    if (strcmp(ParamName, "ExternalPortEndRange") == 0)
    {
        /* collect value */
        *puLong  = pNatPMapping->ExternalPortEndRange;

        return TRUE;
    }

    if (strcmp(ParamName, "InternalPort") == 0)
    {
        /* collect value */
        *puLong  = pNatPMapping->InternalPort;

        return TRUE;
    }

    if (strcmp(ParamName, "Protocol") == 0)
    {
        /* collect value */
        *puLong  = pNatPMapping->Protocol;

        return TRUE;
    }

    if (strcmp(ParamName, "InternalClient") == 0)
    {
        /* collect value */
        *puLong = pNatPMapping->InternalClient.Value;

        return TRUE;
    }

    if (strcmp(ParamName, "RemoteHost") == 0)
    {
        /* collect value */
        *puLong = pNatPMapping->RemoteHost.Value;

        return TRUE;
    }

    if ( PortMapping_GetParamUlongValue_Custom(hInsContext, ParamName, puLong) )
    {
        return  TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_Origin") == 0)
    {
        /* collect value */
        *puLong = pNatPMapping->X_CISCO_COM_Origin;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PortMapping_GetParamStringValue
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
PortMapping_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT        pCxtLink      = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PMAPPING                    pNatPMapping  = (PCOSA_DML_NAT_PMAPPING)pCxtLink->hContext;
    PUCHAR                                    pString       = NULL;
    errno_t                                   rc            = -1;

    /* check the parameter name and return the corresponding value */
    

    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        if ( AnscSizeOfString(pNatPMapping->Alias) < *pUlSize)
        {
            rc = strcpy_s(pValue, *pUlSize, pNatPMapping->Alias);
            if (rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pNatPMapping->Alias)+1;
            return 1;
        }
    }

    if (strcmp(ParamName, "Description") == 0)
    {
        /* collect value */
        if ( AnscSizeOfString(pNatPMapping->Description) < *pUlSize)
        {
            rc = strcpy_s(pValue, *pUlSize, pNatPMapping->Description);
            if (rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pNatPMapping->Description)+1;
            return 1;
        }

    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        /* collect value */
        pString = CosaUtilGetFullPathNameByKeyword
                    (
                        (PUCHAR)"Device.IP.Interface.",
                        (PUCHAR)"Name",
                        (PUCHAR)pNatPMapping->Interface
                    );
        if ( pString )
        {
            if ( AnscSizeOfString((const char*)pString) < *pUlSize)
            {
                rc = strcpy_s(pValue, *pUlSize, (char*)pString);
                AnscFreeMemory(pString);
                if (rc != EOK)
                {
                    ERR_CHK(rc);
                    return -1;
                }
                return 0;
            }
            else
            {
                *pUlSize = AnscSizeOfString((const char*)pString)+1;

                AnscFreeMemory(pString);

                return 1;
            }
        }
        else
        {
            return 0;
        }

    }

    if (strcmp(ParamName, "X_CISCO_COM_InternalClientV6") == 0)
    {
        /* collect value */
        if ( AnscSizeOfString(pNatPMapping->X_CISCO_COM_InternalClientV6) < *pUlSize)
        {
            rc = strcpy_s(pValue, *pUlSize, pNatPMapping->X_CISCO_COM_InternalClientV6);
            if (rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pNatPMapping->X_CISCO_COM_InternalClientV6)+1;
            return 1;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortMapping_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
PortMapping_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT        pCxtLink      = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PMAPPING                    pNatPMapping  = (PCOSA_DML_NAT_PMAPPING)pCxtLink->hContext;

    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return FALSE;
    #endif
    
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pNatPMapping->bEnabled     = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "AllInterfaces") == 0)
    {
        /* save update to backup */
        pNatPMapping->AllInterfaces = bValue;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

#if 0 //TO-DO to handle migration or reboot case
void prepare_portmapCacheFromDB(t_cache tmp_pf_cache)
{

    syscfg_get( NULL, "SinglePortForwardCount", tmp_pf_cache[i].cmd, BLOCK_SIZE);


}
#endif

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortMapping_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
PortMapping_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortMapping_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
PortMapping_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT        pCxtLink      = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PMAPPING                    pNatPMapping  = (PCOSA_DML_NAT_PMAPPING)pCxtLink->hContext;
    ANSC_STATUS                               returnStatus  = ANSC_STATUS_SUCCESS;

    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return FALSE;
    #endif

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "LeaseDuration") == 0)
    {
        /* save update to backup */
        /* pNatPMapping->LeaseDuration = uValue; */
        /* Shiywan: LeaseDuration support for dynamic (non-static) port mappings is OPTIONAL */
		/* Return true for SET if the value to be set is 0. As static port mapping 0 is valid.
			This is to allow SET during rollback of cached rows that were deleted during WEBPA PUT */
		if(uValue == 0 && pNatPMapping->LeaseDuration == 0)
		{
			return TRUE;
		}
        return FALSE;
    }

    if (strcmp(ParamName, "ExternalPort") == 0)
    {
        /* save update to backup */
        pNatPMapping->ExternalPort = (USHORT)uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "ExternalPortEndRange") == 0)
    {
        /* save update to backup */

        pNatPMapping->ExternalPortEndRange = (USHORT)uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "InternalPort") == 0)
    {
        /* save update to backup */
        pNatPMapping->InternalPort = (USHORT)uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "Protocol") == 0)
    {
        /* save update to backup */
        pNatPMapping->Protocol = (UCHAR)uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "InternalClient") == 0)
    {
        /* save update to backup */
        pNatPMapping->InternalClient.Value = uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "RemoteHost") == 0)
    {
        /* save update to backup */
        pNatPMapping->RemoteHost.Value = uValue;

        return TRUE;
    }

    if ( PortMapping_SetParamUlongValue_Custom(hInsContext, ParamName, uValue) )
    {
        return  TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return (returnStatus==ANSC_STATUS_SUCCESS) ? TRUE : FALSE;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortMapping_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
PortMapping_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_NAT                       pNat          = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT        pCxtLink      = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PMAPPING                    pNatPMapping  = (PCOSA_DML_NAT_PMAPPING)pCxtLink->hContext;
    errno_t                                   rc            = -1;

    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return FALSE;
    #endif

    /* check the parameter name and set the corresponding value */

    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER( (char*)pNat->AliasOfPortMapping, sizeof(pNat->AliasOfPortMapping),pNatPMapping->Alias);
        if (rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        rc = STRCPY_S_NOCLOBBER( pNatPMapping->Alias, sizeof(pNatPMapping->Alias), pString );
        if (rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "Description") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER( pNatPMapping->Description, sizeof(pNatPMapping->Description), pString );
        if (rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER( pNatPMapping->Interface, sizeof(pNatPMapping->Interface), pString );
        if (rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_InternalClientV6") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER( pNatPMapping->X_CISCO_COM_InternalClientV6, sizeof(pNatPMapping->X_CISCO_COM_InternalClientV6), pString );
        if (rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortMapping_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation.

                ULONG*                      puLength
                The output length of the param name.

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
PortMapping_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(puLength);
    PCOSA_DATAMODEL_NAT                      pNat              = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT       pCxtLink2         = NULL;
    PSINGLE_LINK_ENTRY                       pSListEntry       = NULL;
    PCOSA_DML_NAT_PMAPPING                   pPortMapping      = (PCOSA_DML_NAT_PMAPPING)pCxtLink->hContext;
    PCOSA_DML_NAT_PMAPPING                   pPortMapping2     = NULL;
    BOOL                                     bFound            = FALSE;
    errno_t                                  rc                = -1;
#if defined (MULTILAN_FEATURE)
    if( pPortMapping->bEnabled && (
        !_Check_PF_parameter(pPortMapping) ||
        !CosaDmlChkDesp(pPortMapping->Description) ||
        !CosaDmlNatChkPortMappingMaxRuleNum(pPortMapping) ||
        !CosaDmlNatChkEnableFlg(pPortMapping)))
#else
    if( !_Check_PF_parameter(pPortMapping) ||
        !CosaDmlChkDesp(pPortMapping->Description) ||
        !CosaDmlNatChkPortMappingMaxRuleNum(pPortMapping) ||
        !CosaDmlNatChkEnableFlg(pPortMapping))
#endif
    {
        CcspTraceWarning(("Parameter Error in %s \n", __FUNCTION__));

        if ( pCxtLink->bNew )
        {
            NAT_PORTMAPPING_SET_DEFAULTVALUE(pPortMapping);
        }else{
            if(ANSC_STATUS_SUCCESS != CosaDmlNatGetPortMapping(NULL, pPortMapping->InstanceNumber, pPortMapping)){
                NAT_PORTMAPPING_SET_DEFAULTVALUE(pPortMapping);
           }
        }
        return FALSE;
    }

    /* promise the alias is exclusive */
    if ( pNat->AliasOfPortMapping[0] )
    {
        pSListEntry =   AnscSListGetFirstEntry( &pNat->NatPMappingList );

        while( pSListEntry )
        {
            pCxtLink2         = ACCESS_COSA_CONTEXT_PMAPPING_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);

            pPortMapping2     = (PCOSA_DML_NAT_PMAPPING)pCxtLink2->hContext;
            if (strcmp(pPortMapping2->Alias, pPortMapping->Alias) == 0)
            {
                if ( pCxtLink2 == pCxtLink )
                {
                    continue;
                }

                rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Alias");
                if(rc != EOK)
                {
                  ERR_CHK(rc);
                  return FALSE;
                }

                bFound = TRUE;
                break;
            }
        }

        if ( bFound )
        {
#if COSA_NAT_ROLLBACK_TEST
            PortMapping_Rollback(hInsContext);
#endif

            return FALSE;
        }

    }
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PortMapping_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
PortMapping_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                         returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT  pCxtLink      = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PMAPPING              pNatPMapping  = (PCOSA_DML_NAT_PMAPPING)pCxtLink->hContext;
    PCOSA_DATAMODEL_NAT                 pNat          = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    errno_t                             rc            = -1;

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlNatAddPortMapping(NULL, pNatPMapping );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaNatRegSetNatInfo(pNat);
        }
        else
        {
            NAT_PORTMAPPING_SET_DEFAULTVALUE(pNatPMapping);

            if ( pNat->AliasOfPortMapping[0] ) {
                rc = STRCPY_S_NOCLOBBER( pNatPMapping->Alias, sizeof(pNatPMapping->Alias), (char*)pNat->AliasOfPortMapping );
                ERR_CHK(rc);
            }
        }
    }
    else
    {
        returnStatus = CosaDmlNatSetPortMapping(NULL, pNatPMapping);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            if(ANSC_STATUS_SUCCESS != CosaDmlNatGetPortMapping(NULL, pNatPMapping->InstanceNumber, pNatPMapping)){
                NAT_PORTMAPPING_SET_DEFAULTVALUE(pNatPMapping);
           }
        }
#ifdef _BWG_PRODUCT_REQ_
        //CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: START
        UpdateList(NULL);
        //CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: END
#endif
    }

    AnscZeroMemory( pNat->AliasOfPortMapping, sizeof(pNat->AliasOfPortMapping) );

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PortMapping_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
PortMapping_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                         returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT                 pNat          = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT  pCxtLink      = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PMAPPING              pNatPMapping  = (PCOSA_DML_NAT_PMAPPING)pCxtLink->hContext;
    errno_t                             rc            = -1;

    if ( pNat->AliasOfPortMapping[0] ) {
        rc = STRCPY_S_NOCLOBBER( pNatPMapping->Alias, sizeof(pNatPMapping->Alias), (char*)pNat->AliasOfPortMapping );
        ERR_CHK(rc);
    }
    if ( !pCxtLink->bNew )
    {
        /* We have nothing to do with this case unless we have one getbyEntry() */
    }
    else
    {
        NAT_PORTMAPPING_SET_DEFAULTVALUE(pNatPMapping);
    }

    AnscZeroMemory( pNat->AliasOfPortMapping, sizeof(pNat->AliasOfPortMapping) );

    return returnStatus;

}

BOOL
NatPortTrigger_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        if (CosaDmlNatGetPortTriggerEnable(pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
 
        return TRUE;
    }

    return FALSE;
}

BOOL
NatPortTrigger_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return FALSE;
    #endif

    if (strcmp(ParamName, "Enable") == 0)
    {
        if (CosaDmlNatSetPortTriggerEnable(bValue) != ANSC_STATUS_SUCCESS)
            return FALSE;
        CcspTraceNotice(("PT_split:Feature Switch Port Triggers %d\n", bValue));
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}


ULONG
X_RDK_PortMapping_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
	if (strcmp(ParamName, "Data") == 0)
	{
		CcspTraceWarning(("Data Get Not supported\n"));
		return 0;
	}
	
	CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
	return -1;
}

/**********************************************************************
caller:     owner of this object

    prototype:

        BOOL
        X_RDK_PortMapping_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_RDK_PortMapping_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Data") == 0)
     {
	char * decodeMsg =NULL;
	int decodeMsgSize =0;
	int size =0;

	struct timespec start,end,*startPtr,*endPtr;
        startPtr = &start;
        endPtr = &end;

	msgpack_zone mempool;
	msgpack_object deserialized;
	msgpack_unpack_return unpack_ret;

       	getCurrentTime(startPtr);
	decodeMsgSize = b64_get_decoded_buffer_size(strlen(pString));

	decodeMsg = (char *) malloc(sizeof(char) * decodeMsgSize);

	size = b64_decode( (const uint8_t*)pString, strlen(pString), (uint8_t *)decodeMsg );
	CcspTraceWarning(("base64 decoded data contains %d bytes\n",size));

	getCurrentTime(endPtr);
        CcspTraceWarning(("Base64 decode Elapsed time : %ld ms\n", timeValDiff(startPtr, endPtr)));
	
	msgpack_zone_init(&mempool, 2048);
	unpack_ret = msgpack_unpack(decodeMsg, size, NULL, &mempool, &deserialized);

	switch(unpack_ret)
	{
		case MSGPACK_UNPACK_SUCCESS:
			CcspTraceWarning(("MSGPACK_UNPACK_SUCCESS :%d\n",unpack_ret));
		break;
		case MSGPACK_UNPACK_EXTRA_BYTES:
			CcspTraceWarning(("MSGPACK_UNPACK_EXTRA_BYTES :%d\n",unpack_ret));
		break;
		case MSGPACK_UNPACK_CONTINUE:
			CcspTraceWarning(("MSGPACK_UNPACK_CONTINUE :%d\n",unpack_ret));
		break;
		case MSGPACK_UNPACK_PARSE_ERROR:
			CcspTraceWarning(("MSGPACK_UNPACK_PARSE_ERROR :%d\n",unpack_ret));
		break;
		case MSGPACK_UNPACK_NOMEM_ERROR:
			CcspTraceWarning(("MSGPACK_UNPACK_NOMEM_ERROR :%d\n",unpack_ret));
		break;
		default:
			CcspTraceWarning(("Message Pack decode failed with error: %d\n", unpack_ret));
	}

	msgpack_zone_destroy(&mempool);
	//End of msgpack decoding
	
	if(unpack_ret == MSGPACK_UNPACK_SUCCESS)
	{	
		portmappingdoc_t *rpm;
		rpm = portmappingdoc_convert( decodeMsg, size+1 );

        if ( decodeMsg )
        {
                free(decodeMsg);
                decodeMsg = NULL;
        }

		if (NULL !=rpm)
		{


			CcspTraceWarning(("rpm->entries_count is %u\n", rpm->entries_count));
			CcspTraceWarning(("rpm->subdoc_name is %s\n", rpm->subdoc_name));
			CcspTraceWarning(("rpm->version is %lu\n", (unsigned long)rpm->version));
			CcspTraceWarning(("rpm->transaction_id is %d\n", rpm->transaction_id));
			CcspTraceWarning(("Portmap configuration received\n"));
                
			execData *execDataPf = NULL ;

                	execDataPf = (execData*) malloc (sizeof(execData));

                	if ( execDataPf != NULL )
                	{

                    		memset(execDataPf, 0, sizeof(execData));

                    		execDataPf->txid = rpm->transaction_id; 
                    		execDataPf->version = rpm->version; 
                    		execDataPf->numOfEntries = rpm->entries_count; 

                    		strncpy(execDataPf->subdoc_name,"portforwarding",sizeof(execDataPf->subdoc_name)-1);

                    		execDataPf->user_data = (void*) rpm ;
			         	execDataPf->calcTimeout = NULL ;
                     		execDataPf->executeBlobRequest = Process_PF_WebConfigRequest;
                    		execDataPf->rollbackFunc = rollback_PortForwarding ;
                            	execDataPf->freeResources = freeResources_PortForwarding ;

                    		PushBlobRequest(execDataPf);

                    		CcspTraceWarning(("PushBlobRequest complete\n"));

                        	return TRUE;
                            
                	}

                	else 
                	{
                        	CcspTraceWarning(("execData memory allocation failed\n"));
                            portmappingdoc_destroy( rpm );  

                        	return FALSE;

                	}

		  }
        	return TRUE;

   	 } 
	else
	{
        if ( decodeMsg )
        {
                free(decodeMsg);
                decodeMsg = NULL;
        }
		CcspTraceWarning(("Corrupted portMapping value\n"));
		return FALSE;
	}
        return TRUE;
}    
    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}
/***********************************************************************

 APIs for Object:

    NAT.X_CISCO_COM_PortTriggers.Trigger.{i}.

    *  PortTrigger_GetEntryCount
    *  PortTrigger_GetEntry
    *  PortTrigger_AddEntry
    *  PortTrigger_DelEntry
    *  PortTrigger_GetParamBoolValue
    *  PortTrigger_GetParamUlongValue
    *  PortTrigger_GetParamStringValue
    *  PortTrigger_SetParamBoolValue
    *  PortTrigger_SetParamUlongValue
    *  PortTrigger_SetParamStringValue
    *  PortTrigger_Validate
    *  PortTrigger_Commit
    *  PortTrigger_Rollback

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PortTrigger_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/

ULONG
PortTrigger_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT             pNat         = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;

    return AnscSListQueryDepth( &pNat->NatPTriggerList );

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        PortTrigger_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/

ANSC_HANDLE
PortTrigger_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT             pMyObject         = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = NULL;

    pSListEntry       = AnscSListGetEntryByIndex(&pMyObject->NatPTriggerList, nIndex);

    if ( pSListEntry )
    {
        pCxtLink      = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
        *pInsNumber   = pCxtLink->InstanceNumber;
    }

    return (ANSC_HANDLE)pSListEntry;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        PortTrigger_AddEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to add a new entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle of new added entry.

**********************************************************************/

ANSC_HANDLE
PortTrigger_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NAT                  pNat              = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_DML_NAT_PTRIGGER               pNatPTrigger      = NULL;
    PCOSA_CONTEXT_LINK_OBJECT            pPTriggerCxtLink  = NULL;

    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return NULL;
    #endif

    pNatPTrigger = (PCOSA_DML_NAT_PTRIGGER)AnscAllocateMemory(sizeof(COSA_DML_NAT_PTRIGGER));

    if ( !pNatPTrigger )
    {
        return NULL;
    }

    pPTriggerCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if ( !pPTriggerCxtLink )
    {
        goto EXIT;
    }

    /* now we have this link content */
    pPTriggerCxtLink->hContext = (ANSC_HANDLE)pNatPTrigger;
    pPTriggerCxtLink->bNew     = TRUE;

    /* Get InstanceNumber and Alias */
    CosaNatGenForTriggerEntry(NULL, pNatPTrigger);

    pPTriggerCxtLink->InstanceNumber = pNatPTrigger->InstanceNumber ;
    *pInsNumber                      = pNatPTrigger->InstanceNumber ;

    CosaSListPushEntryByInsNum(&pNat->NatPTriggerList, (PCOSA_CONTEXT_LINK_OBJECT)pPTriggerCxtLink);

    CosaNatRegSetNatInfo(pNat);

    return (ANSC_HANDLE)pPTriggerCxtLink;

EXIT:
    AnscFreeMemory(pNatPTrigger);

    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PortTrigger_DelEntry
            (
                ANSC_HANDLE                 hInsContext,
                ANSC_HANDLE                 hInstance
            );

    description:

        This function is called to delete an exist entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ANSC_HANDLE                 hInstance
                The exist entry handle;

    return:     The status of the operation.

**********************************************************************/

ULONG
PortTrigger_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pNat              = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;
    PCOSA_CONTEXT_LINK_OBJECT       pPTriggerCxtLink  = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_NAT_PTRIGGER          pNatPTrigger      = (PCOSA_DML_NAT_PTRIGGER)pPTriggerCxtLink->hContext;

    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return ANSC_STATUS_FAILURE;
    #endif
 
    if ( pPTriggerCxtLink->bNew )
    {
        /* Set bNew to FALSE to indicate this node is not going to save to SysRegistry */
        pPTriggerCxtLink->bNew = FALSE;

        returnStatus = CosaNatRegSetNatInfo(pNat);
    }
    else
    {
        returnStatus = CosaDmlNatDelPortTrigger(NULL, pNatPTrigger);

        /* Need re-sync here? */
    }

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        AnscSListPopEntryByLink(&pNat->NatPTriggerList, &pPTriggerCxtLink->Linkage);

        AnscFreeMemory(pPTriggerCxtLink->hContext);
        AnscFreeMemory(pPTriggerCxtLink);
    }

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortTrigger_GetParamBoolValue
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
PortTrigger_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PTRIGGER          pNatPTrigger  = (PCOSA_DML_NAT_PTRIGGER   )pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* Considering there is a global variable for port trigger. It may disable all entries.
           Here Enable will be get from backend */
        PCOSA_DML_NAT_PTRIGGER          pPortTrigger  = NULL;
        ULONG                           entryCount    = 0;
        ULONG                           i             = 0;
        
        pPortTrigger = CosaDmlNatGetPortTriggers(NULL,&entryCount,FALSE);
        
        if ( pPortTrigger )
        {
            for(i=0; i<entryCount; i++)
            {
                if ( pPortTrigger[i].InstanceNumber == pNatPTrigger->InstanceNumber )
                {
                    pNatPTrigger->bEnabled = pPortTrigger[i].bEnabled;
                    break;
                }
            }
            
            AnscFreeMemory(pPortTrigger);
        }

        /* collect value */
        *pBool = pNatPTrigger->bEnabled;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortTrigger_GetParamUlongValue
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
PortTrigger_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT        pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PTRIGGER           pNatPTrigger  = (PCOSA_DML_NAT_PTRIGGER   )pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "TriggerProtocol") == 0)
    {
        /* collect value */
        *puLong  = pNatPTrigger->TriggerProtocol;

        return TRUE;
    }

    if (strcmp(ParamName, "TriggerPortStart") == 0)
    {
        /* collect value */
        *puLong  = pNatPTrigger->TriggerPortStart;

        return TRUE;
    }

    if (strcmp(ParamName, "TriggerPortEnd") == 0)
    {
        /* collect value */
        *puLong  = pNatPTrigger->TriggerPortEnd;

        return TRUE;
    }

    if (strcmp(ParamName, "ForwardProtocol") == 0)
    {
        /* collect value */
        *puLong  = pNatPTrigger->ForwardProtocol;

        return TRUE;
    }

    if (strcmp(ParamName, "ForwardPortStart") == 0)
    {
        /* collect value */
        *puLong  = pNatPTrigger->ForwardPortStart;

        return TRUE;
    }

    if (strcmp(ParamName, "ForwardPortEnd") == 0)
    {
        /* collect value */
        *puLong = pNatPTrigger->ForwardPortEnd;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PortTrigger_GetParamStringValue
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
PortTrigger_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PTRIGGER          pNatPTrigger  = (PCOSA_DML_NAT_PTRIGGER   )pCxtLink->hContext;
    errno_t                         rc            = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        if ( AnscSizeOfString(pNatPTrigger->Alias) < *pUlSize)
        {
            rc = STRCPY_S_NOCLOBBER(pValue, *pUlSize, pNatPTrigger->Alias);
            if (rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pNatPTrigger->Alias)+1;
            return 1;
        }
    }

    if (strcmp(ParamName, "Description") == 0)
    {
        /* collect value */
        if ( AnscSizeOfString(pNatPTrigger->Description) < *pUlSize)
        {
            rc = STRCPY_S_NOCLOBBER(pValue, *pUlSize, pNatPTrigger->Description);
            if (rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pNatPTrigger->Description)+1;
            return 1;
        }
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortTrigger_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
PortTrigger_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PTRIGGER          pNatPTrigger  = (PCOSA_DML_NAT_PTRIGGER   )pCxtLink->hContext;

    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return FALSE;
    #endif

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pNatPTrigger->bEnabled     = bValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortTrigger_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
PortTrigger_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PTRIGGER          pNatPTrigger  = (PCOSA_DML_NAT_PTRIGGER   )pCxtLink->hContext;

    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return FALSE;
    #endif

    /* check the parameter name and set the corresponding value */
#if 1
    /* XB3/BWG don't need filter output/input packet protocol. Those two DM need to be set as same value. 
     * For some reason SNMP cannot set those DM both, so set it in here */  
    if ((strcmp(ParamName, "TriggerProtocol") == 0) || (strcmp(ParamName, "ForwardProtocol") == 0))
    {
        pNatPTrigger->TriggerProtocol = uValue;
        pNatPTrigger->ForwardProtocol = uValue;

        return TRUE;
    }

#else
    if (strcmp(ParamName, "TriggerProtocol") == 0)
    {
        pNatPTrigger->TriggerProtocol = uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "ForwardProtocol") == 0)
    {
        pNatPTrigger->ForwardProtocol = uValue;

        return TRUE;
    }
#endif
    if (strcmp(ParamName, "TriggerPortStart") == 0)
    {
        pNatPTrigger->TriggerPortStart = (USHORT)uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "TriggerPortEnd") == 0)
    {
        pNatPTrigger->TriggerPortEnd = (USHORT)uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "ForwardPortStart") == 0)
    {
        pNatPTrigger->ForwardPortStart = (USHORT)uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "ForwardPortEnd") == 0)
    {
        pNatPTrigger->ForwardPortEnd = (USHORT)uValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortTrigger_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
PortTrigger_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PTRIGGER          pNatPTrigger  = (PCOSA_DML_NAT_PTRIGGER   )pCxtLink->hContext;
    errno_t                         rc            = -1;

    #ifndef MULTILAN_FEATURE
    BOOL bridgeMode;
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return FALSE;
    #endif

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */

        if ( pNatPTrigger->pOriAlias )
        {
            AnscFreeMemory(pNatPTrigger->pOriAlias);
        }

        pNatPTrigger->pOriAlias = AnscCloneString(pNatPTrigger->Alias);

        rc = STRCPY_S_NOCLOBBER( pNatPTrigger->Alias, sizeof(pNatPTrigger->Alias), pString );
        if (rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }

        return TRUE;
    }
    if (strcmp(ParamName, "Description") == 0)
    {
        /* Needs to copy required size of the parameter */
        rc = strcpy_s(pNatPTrigger->Description, sizeof(pNatPTrigger->Description), pString);
        if(rc != EOK)
        {
            CcspTraceWarning(("Error in copying to pNatPTrigger->Description rc - %d\n", rc));
            return FALSE;
        }

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PortTrigger_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation.

                ULONG*                      puLength
                The output length of the param name.

    return:     TRUE if there's no validation.

**********************************************************************/

BOOL
PortTrigger_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(puLength);
    PCOSA_DATAMODEL_NAT             pNat              = (PCOSA_DATAMODEL_NAT      )g_pCosaBEManager->hNat;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink2         = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DML_NAT_PTRIGGER          pPortTrigger      = (PCOSA_DML_NAT_PTRIGGER   )pCxtLink->hContext;
    PCOSA_DML_NAT_PTRIGGER          pPortTrigger2     = NULL;
    BOOL                            bFound            = FALSE;
    errno_t                         rc                = -1;

#if defined (MULTILAN_FEATURE)
    /* Don't validate all fields here if entry is not enabled */
    if( !pPortTrigger->bEnabled )
        return TRUE;
#endif

    if( ! _Check_PT_parameter(pPortTrigger) || 
        (FALSE == CosaDmlNatChkPortRange(pPortTrigger->InstanceNumber, pPortTrigger->bEnabled, pPortTrigger->ForwardPortStart, pPortTrigger->ForwardPortEnd, pPortTrigger->ForwardProtocol, 1 )) ||
        (!CosaDmlChkDesp(pPortTrigger->Description)) ||
        (FALSE == CosaDmlNatChkTriggerPortRange(pPortTrigger->InstanceNumber, pPortTrigger->bEnabled, pPortTrigger->TriggerPortStart, pPortTrigger->TriggerPortEnd, pPortTrigger->TriggerProtocol, 1 ))){

        CcspTraceWarning(("Parameter Error in %s \n", __FUNCTION__));
    
        if ( pCxtLink->bNew )
        {
            COSA_DML_NAT_PTRIGGER_INIT(pPortTrigger);
        }else{
            if( ANSC_STATUS_SUCCESS != CosaDmlNatGetPortTrigger(NULL, pPortTrigger->InstanceNumber, pPortTrigger)){
                COSA_DML_NAT_PTRIGGER_INIT(pPortTrigger);
            } 
        }
        return FALSE;
    }

    /* promise the alias is exclusive */
    if ( pPortTrigger->pOriAlias )
    {
        pSListEntry =   AnscSListGetFirstEntry( &pNat->NatPTriggerList );

        while( pSListEntry )
        {
            pCxtLink2         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);

            pPortTrigger2     = (PCOSA_DML_NAT_PTRIGGER)pCxtLink2->hContext;

            if (strcmp(pPortTrigger2->Alias, pPortTrigger->Alias) == 0)
            {
                if ( pCxtLink2 == pCxtLink )
                {
                    continue;
                }

                rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Alias");
                if(rc != EOK)
                {
                  ERR_CHK(rc);
                  return FALSE;
                }

                bFound = TRUE;
                break;
            }
        }

        if ( bFound )
        {
#if COSA_NAT_ROLLBACK_TEST
            PortTrigger_Rollback(hInsContext);
#endif

            return FALSE;
        }

    }
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PortTrigger_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG
PortTrigger_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PTRIGGER          pNatPTrigger  = (PCOSA_DML_NAT_PTRIGGER   )pCxtLink->hContext;
    PCOSA_DATAMODEL_NAT             pNat          = (PCOSA_DATAMODEL_NAT      )g_pCosaBEManager->hNat;
    errno_t                         rc            = -1;

    /*returnStatus = CosaDmlNatSetPortTrigger(NULL, pNatPTrigger);*/

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlNatAddPortTrigger(NULL, pNatPTrigger );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaNatRegSetNatInfo(pNat);
        }
        else
        {
            COSA_DML_NAT_PTRIGGER_INIT(pNatPTrigger);
            if ( pNatPTrigger->pOriAlias ) {
                rc = STRCPY_S_NOCLOBBER( pNatPTrigger->Alias, sizeof(pNatPTrigger->Alias), pNatPTrigger->pOriAlias );
                ERR_CHK(rc);
            }
        }
    }
    else
    {
        returnStatus = CosaDmlNatSetPortTrigger(NULL, pNatPTrigger);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            if( ANSC_STATUS_SUCCESS != CosaDmlNatGetPortTrigger(NULL, pNatPTrigger->InstanceNumber, pNatPTrigger)){
                COSA_DML_NAT_PTRIGGER_INIT(pNatPTrigger);
            } 
        }
    }

    if ( pNatPTrigger->pOriAlias )
    {
        AnscFreeMemory(pNatPTrigger->pOriAlias);

        pNatPTrigger->pOriAlias = NULL;
    }

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PortTrigger_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG
PortTrigger_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NAT_PTRIGGER          pNatPTrigger  = (PCOSA_DML_NAT_PTRIGGER   )pCxtLink->hContext;
    errno_t                         rc            = -1;

    if ( pNatPTrigger->pOriAlias ) {
        rc = STRCPY_S_NOCLOBBER(pNatPTrigger->Alias, sizeof(pNatPTrigger->Alias), pNatPTrigger->pOriAlias);
        ERR_CHK(rc);
    }

    if ( !pCxtLink->bNew )
    {
        /* We have nothing to do with this case unless we have one getbyEntry() */
    }
    else
    {
        COSA_DML_NAT_PTRIGGER_INIT(pNatPTrigger);
    }

    if ( pNatPTrigger->pOriAlias )
    {
        AnscFreeMemory(pNatPTrigger->pOriAlias);

        pNatPTrigger->pOriAlias = NULL;
    }

    return returnStatus;

}
