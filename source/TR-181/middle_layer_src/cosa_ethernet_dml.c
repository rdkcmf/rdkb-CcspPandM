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

    module: cosa_ethernet_dml.c

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

        01/17/2011    initial revision.

**************************************************************************/

#include "dml_tr181_custom_cfg.h"
#include "cosa_ethernet_dml.h"
#ifdef _HUB4_PRODUCT_REQ_
#include <sys/sysinfo.h>
#define WAN_INTERFACE_LEN 8
#define BUFLEN_16 16
#endif
#if defined(MULTILAN_FEATURE) && defined (INTEL_PUMA7)
#if defined (ETH_4_PORTS) && defined (ENABLE_ETH_WAN)
#include "syscfg/syscfg.h"
#define BUFLEN_6 6
#endif
#endif
#include "safec_lib_common.h"

ANSC_STATUS COSAGetParamValueByPathName(void* bus_handle, parameterValStruct_t *val, ULONG *parameterValueLength);

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

    Ethernet.

    *  Ethernet_GetParamBoolValue
    *  Ethernet_GetParamIntValue
    *  Ethernet_GetParamUlongValue
    *  Ethernet_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Ethernet_GetParamBoolValue
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
Ethernet_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Ethernet_GetParamIntValue
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
Ethernet_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Ethernet_GetParamUlongValue
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
Ethernet_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Ethernet_GetParamStringValue
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
Ethernet_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);

    return -1;
}
#ifndef FEATURE_RDKB_WAN_MANAGER
/***********************************************************************

 APIs for Object:

    Ethernet.Interface.{i}.

    *  Interface_GetEntryCount
    *  Interface_GetEntry
    *  Interface_GetParamBoolValue
    *  Interface_GetParamIntValue
    *  Interface_GetParamUlongValue
    *  Interface_GetParamStringValue
    *  Interface_SetParamBoolValue
    *  Interface_SetParamIntValue
    *  Interface_SetParamUlongValue
    *  Interface_SetParamStringValue
    *  Interface_Validate
    *  Interface_Commit
    *  Interface_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface_GetEntryCount
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
Interface_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{   
    UNREFERENCED_PARAMETER(hInsContext);
    return CosaDmlEthPortGetNumberOfEntries(NULL);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Interface_GetEntry
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
Interface_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_ETHERNET        pMyObject     = (PCOSA_DATAMODEL_ETHERNET)g_pCosaBEManager->hEthernet;

    *pInsNumber = pMyObject->EthernetPortFullTable[nIndex].Cfg.InstanceNumber;

    return &pMyObject->EthernetPortFullTable[nIndex]; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface_GetParamBoolValue
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
Interface_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
         CosaDmlEthPortGetCfg(NULL, &pEthernetPortFull->Cfg);
        *pBool = pEthernetPortFull->Cfg.bEnabled;
        return TRUE;
    }

    if (strcmp(ParamName, "Upstream") == 0)
    {
        /* collect value */
        *pBool = pEthernetPortFull->StaticInfo.bUpstream;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface_GetParamIntValue
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
Interface_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "MaxBitRate") == 0)
    {
        /* collect value */
	CosaDmlEthPortGetCfg(NULL, &pEthernetPortFull->Cfg);
	*pInt = pEthernetPortFull->Cfg.MaxBitRate;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface_GetParamUlongValue
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
Interface_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        CosaDmlEthPortGetDinfo(NULL, pEthernetPortFull->Cfg.InstanceNumber, &pEthernetPortFull->DynamicInfo);
        
        *puLong = pEthernetPortFull->DynamicInfo.Status;
        return TRUE;
    }

    if (strcmp(ParamName, "LastChange") == 0)
    {
        /* collect value */
        CosaDmlEthPortGetDinfo(NULL, pEthernetPortFull->Cfg.InstanceNumber, &pEthernetPortFull->DynamicInfo);
        
        *puLong = AnscGetTimeIntervalInSeconds(pEthernetPortFull->DynamicInfo.LastChange, AnscGetTickInSeconds());
        return TRUE;
    }

    if (strcmp(ParamName, "DuplexMode") == 0)
    {
        /* collect value */
        *puLong = pEthernetPortFull->Cfg.DuplexMode;
        return TRUE;
    }

    if (strcmp(ParamName, "CurrentBitRate") == 0)
    {
        /* collect value */

        CosaDmlEthPortGetDinfo(NULL, pEthernetPortFull->Cfg.InstanceNumber, &pEthernetPortFull->DynamicInfo);
        
        *puLong = pEthernetPortFull->DynamicInfo.CurrentBitRate;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface_GetParamStringValue
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
Interface_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;
    char assocDeviceMacList[(17 * ETH_INTERFACE_MAX_ASSOC_DEVICES) + 1];
    errno_t rc = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pEthernetPortFull->Cfg.Alias);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "Name") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue,*pUlSize, pEthernetPortFull->StaticInfo.Name);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        /* collect value */
        
        return 0;
    }

    if (strcmp(ParamName, "MACAddress") == 0)
    {
	//when dut reboot, brlan0 start so late then sometimes g_EthIntSInfo still can't get brlan0's mac
#if CFG_TR181_ETH_BORROW_MAC
	if( (0x00 == pEthernetPortFull->StaticInfo.MacAddress[0]) && (0x00 == pEthernetPortFull->StaticInfo.MacAddress[1]) && (0x00 == pEthernetPortFull->StaticInfo.MacAddress[2]) && (0x00 == pEthernetPortFull->StaticInfo.MacAddress[3]) && (0x00 == pEthernetPortFull->StaticInfo.MacAddress[4]) && (0x00 == pEthernetPortFull->StaticInfo.MacAddress[5]))
	{
		CHAR strMac[128] = {0};
		if ( -1 != _getMac("brlan0", strMac) )
		{
			AnscCopyMemory(pEthernetPortFull->StaticInfo.MacAddress,strMac,6);
		}
	}
#endif 	 
#if defined(MULTILAN_FEATURE) && defined (INTEL_PUMA7)

        CHAR strMac[128] = {0};
#ifdef FEATURE_RDKB_XDSL_PPP_MANAGER
        char wanPhyName[32] = {0};
        char out_value[32] = {0};	
        if (syscfg_get(NULL, "wan_physical_ifname", out_value, sizeof(out_value)) == 0)
        {
           strncpy(wanPhyName, out_value, sizeof(wanPhyName));
           CcspTraceInfo(("%s %d - WanPhyName=%s \n", __FUNCTION__,__LINE__, wanPhyName));
        }
        else
        {
           strncpy(wanPhyName, "erouter0", sizeof(wanPhyName));
           CcspTraceInfo(("%s %d - WanPhyName=%s \n", __FUNCTION__,__LINE__, wanPhyName));
        }
        if(strcmp(pEthernetPortFull->StaticInfo.Name, wanPhyName) == 0 ) {
                if ( -1 != _getMac(wanPhyName, strMac) )
#else	
        if(strcmp(pEthernetPortFull->StaticInfo.Name, "erouter0") == 0 ) {
                if ( -1 != _getMac("erouter0", strMac) )
#endif			
                {
                        AnscCopyMemory(pEthernetPortFull->StaticInfo.MacAddress,strMac,6);
                }
        }
#if defined (ETH_4_PORTS) && defined (ENABLE_ETH_WAN)
        if(strcmp(pEthernetPortFull->StaticInfo.Name, "sw_4") == 0) {
                char buf[BUFLEN_6]= {0};
                syscfg_get(NULL, "eth_wan_enabled", buf, sizeof(buf));
                if(strcmp(buf, "true") == 0) {
                        if ( -1 != _getMac("erouter0", strMac) )
                        {
                               AnscCopyMemory(pEthernetPortFull->StaticInfo.MacAddress,strMac,6);
                        }
                }
        }
#endif
#endif
        rc = sprintf_s
            (
                pValue,
                *pUlSize,
                "%02x:%02x:%02x:%02x:%02x:%02x",
                pEthernetPortFull->StaticInfo.MacAddress[0],
                pEthernetPortFull->StaticInfo.MacAddress[1],
                pEthernetPortFull->StaticInfo.MacAddress[2],
                pEthernetPortFull->StaticInfo.MacAddress[3],
                pEthernetPortFull->StaticInfo.MacAddress[4],
                pEthernetPortFull->StaticInfo.MacAddress[5]
            );
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "X_CISCO_COM_AssociatedDevice") == 0)
	{
		CosaDmlEthPortGetDinfo(NULL, pEthernetPortFull->Cfg.InstanceNumber, &pEthernetPortFull->DynamicInfo);
		CosaEthPortGetAssocDevices(pEthernetPortFull->DynamicInfo.AssocDevices,assocDeviceMacList,pEthernetPortFull->DynamicInfo.AssocDevicesCount);

		if(AnscSizeOfString(assocDeviceMacList) < *pUlSize) 
		{
			rc = strcpy_s(pValue,*pUlSize,assocDeviceMacList);
			if(rc != EOK)
			{
				ERR_CHK(rc);
				return -1;
			}
			return 0;
		}
		else
		{
			*pUlSize = AnscSizeOfString(assocDeviceMacList);
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
        Interface_SetParamBoolValue
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
Interface_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;
    
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pEthernetPortFull->Cfg.bEnabled = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface_SetParamIntValue
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
Interface_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;
    
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "MaxBitRate") == 0)
    {
        /* save update to backup */
        pEthernetPortFull->Cfg.MaxBitRate = iValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface_SetParamUlongValue
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
Interface_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;
    
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "DuplexMode") == 0)
    {
        /* save update to backup */
        pEthernetPortFull->Cfg.DuplexMode = uValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface_SetParamStringValue
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
Interface_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;
    errno_t                         rc                = -1;
    
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pEthernetPortFull->Cfg.Alias, sizeof(pEthernetPortFull->Cfg.Alias),pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        /* save update to backup */
        /* Interface is a layer 1 interface. LowerLayers will not be used. */
        return FALSE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface_Validate
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
Interface_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET)g_pCosaBEManager->hEthernet;
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull       = (PCOSA_DML_ETH_PORT_FULL )hInsContext;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    errno_t                         rc                      = -1;
    
    ulEntryCount = CosaDmlEthPortGetNumberOfEntries(NULL);
    
    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        if (
                ((ULONG)pEthernetPortFull != (ULONG)&pMyObject->EthernetPortFullTable[ulIndex]) &&
                (strcmp(pEthernetPortFull->Cfg.Alias, pMyObject->EthernetPortFullTable[ulIndex].Cfg.Alias) == 0)
            )
        {
            rc = strcpy_s(pReturnParamName, *puLength, "Alias");
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }

            *puLength = AnscSizeOfString("Alias");

            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface_Commit
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
Interface_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;

    CosaDmlEthPortSetCfg(NULL, &pEthernetPortFull->Cfg);
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface_Rollback
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
Interface_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;

    CosaDmlEthPortGetCfg(NULL, &pEthernetPortFull->Cfg);

    return 0;
}

/***********************************************************************

 APIs for Object:

    Ethernet.Interface.{i}.X_RDKCENTRAL-COM_AssociatedDevice.{i}.

    *  AssociatedDevice1_GetEntryCount
    *  AssociatedDevice1_GetEntry
    *  AssociatedDevice1_IsUpdated
    *  AssociatedDevice1_Synchronize
    *  AssociatedDevice1_GetParamStringValue
***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        AssociatedDevice_GetEntryCount
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
AssociatedDevice1_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
	PCOSA_DATAMODEL_ETHERNET        pMyObject     = (PCOSA_DATAMODEL_ETHERNET)g_pCosaBEManager->hEthernet;
	PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;
	ULONG                           InterfaceIndex  = pEthernetPortFull->Cfg.InstanceNumber;

	return pMyObject->EthernetPortFullTable[InterfaceIndex - 1].DynamicInfo.AssocDevicesCount;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        AssociatedDevice1_GetEntry
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
AssociatedDevice1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
	PCOSA_DATAMODEL_ETHERNET        pMyObject     = (PCOSA_DATAMODEL_ETHERNET)g_pCosaBEManager->hEthernet;
	PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;
	ULONG                           InterfaceIndex  = pEthernetPortFull->Cfg.InstanceNumber;

	*pInsNumber = nIndex + 1;
	
	return &pMyObject->EthernetPortFullTable[InterfaceIndex - 1].AssocClient[nIndex]; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        AssociatedDevice1_IsUpdated
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
AssociatedDevice1_IsUpdated
    (
     ANSC_HANDLE hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
	AssociatedDevice1_Synchronize
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
AssociatedDevice1_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
	PCOSA_DATAMODEL_ETHERNET        pMyObject     = (PCOSA_DATAMODEL_ETHERNET)g_pCosaBEManager->hEthernet;
	PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;
	ULONG                           InterfaceIndex  = pEthernetPortFull->Cfg.InstanceNumber;
	ANSC_STATUS                     ret                  = ANSC_STATUS_SUCCESS;

	ret = CosaDmlEthPortGetClientMac(&pMyObject->EthernetPortFullTable[InterfaceIndex - 1], InterfaceIndex);
	
	return ret;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        AssociatedDevice1_GetParamStringValue
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
ULONG
AssociatedDevice1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
	UNREFERENCED_PARAMETER(pUlSize);
        PCOSA_DML_ASSOCDEV_INFO	     pAssocClient	= (PCOSA_DML_ASSOCDEV_INFO)hInsContext;

	if (strcmp(ParamName, "MACAddress") == 0)
	{
		AnscCopyMemory(pValue, pAssocClient->MacAddress, 18);
		return 0;
	}

	/* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
	return -1;
}

/***********************************************************************

 APIs for Object:

    Ethernet.Interface.{i}.Stats.

    *  Stats_GetParamBoolValue
    *  Stats_GetParamIntValue
    *  Stats_GetParamUlongValue
    *  Stats_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats_GetParamBoolValue
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
Stats_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats_GetParamIntValue
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
Stats_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats_GetParamUlongValue
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
Stats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_ETH_PORT_FULL         pEthernetPortFull = (PCOSA_DML_ETH_PORT_FULL)hInsContext;
    COSA_DML_ETH_STATS              stats;

    CosaDmlEthPortGetStats(NULL, pEthernetPortFull->Cfg.InstanceNumber, &stats);
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "BytesSent") == 0)
    {
        /* collect value */
        *puLong = stats.BytesSent;
        return TRUE;
    }

    if (strcmp(ParamName, "BytesReceived") == 0)
    {
        /* collect value */
        *puLong = stats.BytesReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "PacketsSent") == 0)
    {
        /* collect value */
        *puLong = stats.PacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "PacketsReceived") == 0)
    {
        /* collect value */
        *puLong = stats.PacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "ErrorsSent") == 0)
    {
        /* collect value */
        *puLong = stats.ErrorsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "ErrorsReceived") == 0)
    {
        /* collect value */
        *puLong = stats.ErrorsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "UnicastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = stats.UnicastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "UnicastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = stats.UnicastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "DiscardPacketsSent") == 0)
    {
        /* collect value */
        *puLong = stats.DiscardPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "DiscardPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = stats.DiscardPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "MulticastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = stats.MulticastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "MulticastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = stats.MulticastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "BroadcastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = stats.BroadcastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "BroadcastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = stats.BroadcastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "UnknownProtoPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = stats.UnknownProtoPacketsReceived;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Stats_GetParamStringValue
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
Stats_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);

    return -1;
}
#endif // #ifndef FEATURE_RDKB_WAN_MANAGER 
/***********************************************************************

 APIs for Object:

    Ethernet.Link.{i}.

    *  Link_GetEntryCount
    *  Link_GetEntry
    *  Link_AddEntry
    *  Link_DelEntry
    *  Link_GetParamBoolValue
    *  Link_GetParamIntValue
    *  Link_GetParamUlongValue
    *  Link_GetParamStringValue
    *  Link_SetParamBoolValue
    *  Link_SetParamIntValue
    *  Link_SetParamUlongValue
    *  Link_SetParamStringValue
    *  Link_Validate
    *  Link_Commit
    *  Link_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Link_GetEntryCount
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
Link_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_ETHERNET        pMyObject = (PCOSA_DATAMODEL_ETHERNET)g_pCosaBEManager->hEthernet;
    
    return AnscSListQueryDepth(&pMyObject->EthernetLinkList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Link_GetEntry
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
Link_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY       )NULL;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL  )NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(&pMyObject->EthernetLinkList, nIndex);

    if ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);

        *pInsNumber = pCosaContext->InstanceNumber;

        pEntry = pCosaContext->hContext;

        CosaDmlEthLinkGetCfg(pMyObject->hSbContext, &pEntry->Cfg);
    }

    return pCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Link_AddEntry
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
Link_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->EthernetLinkList;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL  )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    errno_t                         rc                      = -1;


    pEntry = (PCOSA_DML_ETH_LINK_FULL)AnscAllocateMemory(sizeof(COSA_DML_ETH_LINK_FULL));
    if (!pEntry)
    {
        return NULL;
    }

    rc = sprintf_s(pEntry->Cfg.Alias, sizeof(pEntry->Cfg.Alias),"Link%lu", pMyObject->ulEthernetLinkNextInstance);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pEntry);
      return NULL;
    }

    /* Update the cache */
    pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pCosaContext)
    {
        AnscFreeMemory(pEntry);

        return NULL;
    }

    pCosaContext->InstanceNumber = pEntry->Cfg.InstanceNumber = pMyObject->ulEthernetLinkNextInstance;

    pMyObject->ulEthernetLinkNextInstance++;

    if ( pMyObject->ulEthernetLinkNextInstance == 0 )
    {
        pMyObject->ulEthernetLinkNextInstance = 1;
    }

    pCosaContext->hContext        = (ANSC_HANDLE)pEntry;
    pCosaContext->hParentTable    = NULL;
    pCosaContext->bNew            = TRUE;

    CosaSListPushEntryByInsNum(pListHead, pCosaContext);

    CosaEthLinkRegAddInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pCosaContext);

    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Link_DelEntry
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
Link_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2           = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL  )pCosaContext->hContext;
    PCOSA_DML_ETH_LINK_FULL         pNewEntry               = (PCOSA_DML_ETH_LINK_FULL  )NULL;
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->EthernetLinkList;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = NULL;


    CosaDmlEthLinkDelEntry(pMyObject->hSbContext, pEntry->Cfg.InstanceNumber);

    /* Update the cache */
    if ( TRUE )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->EthernetLinkList);

        while ( pSLinkEntry )
        {
            pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

            pNewEntry = (PCOSA_DML_ETH_LINK_FULL)pCosaContext2->hContext;

            if ( pNewEntry && AnscEqualString(pNewEntry->Cfg.Alias, pEntry->Cfg.Alias, FALSE))
            {
                AnscSListPopEntryByLink(pListHead, &pCosaContext2->Linkage);

                CosaEthLinkRegDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pCosaContext2);

                AnscFreeMemory(pNewEntry);
                AnscFreeMemory(pCosaContext2);

                break;
            }
        }
    }
        
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Link_GetParamBoolValue
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
Link_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL)pContextLinkObject->hContext;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pEntry->Cfg.bEnabled;
        return TRUE;
    }

    if (strcmp(ParamName, "PriorityTagging") == 0)
    {
        /* collect value */
        *pBool = pEntry->Cfg.bPriorityTagging;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Link_GetParamIntValue
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
Link_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Link_GetParamUlongValue
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
Link_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
#ifdef _HUB4_PRODUCT_REQ_
    struct sysinfo info            = {0};
    char cWanUpDownTime[BUFLEN_16] = {0};
    ULONG ulWanUpDownTime          = 0;
    ULONG ulCurrentTime            = 0;
    ULONG ulNameBufSize            = 0;
#endif
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL)pContextLinkObject->hContext;

    CosaDmlEthLinkGetDinfo(pMyObject->hSbContext, pEntry->Cfg.InstanceNumber, &pEntry->DynamicInfo);
 
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        *puLong = pEntry->DynamicInfo.Status;
        return TRUE;
    }

    if (strcmp(ParamName, "LastChange") == 0)
    {
#ifdef _HUB4_PRODUCT_REQ_
        if ( _ansc_strlen(pEntry->StaticInfo.Name) == 0 )
        {
            ulNameBufSize = sizeof(pEntry->StaticInfo.Name);
            CosaUtilGetLowerLayerName
                    (
                        pEntry->Cfg.LinkType,
                        pEntry->Cfg.LinkInstNum,
                        pEntry->StaticInfo.Name,
                        &ulNameBufSize
                    );
        }
	/* Check for a wan interface */
        if (!strncmp(pEntry->StaticInfo.Name, "erouter0", WAN_INTERFACE_LEN))
        {
            /* Get the wan Up/Down time in seconds */
            if(0 == CosaDmlEthLinkGetWanUpDownTime(cWanUpDownTime, BUFLEN_16))
            {
                ulWanUpDownTime = atol(cWanUpDownTime);
                sysinfo(&info);
                /*Get the current time in seconds*/
                ulCurrentTime = info.uptime;
                /* collect value */
                *puLong = ulCurrentTime - ulWanUpDownTime;
                return TRUE;	
            }
        }
#endif
        /* collect value */
        *puLong = AnscGetTimeIntervalInSeconds(pEntry->DynamicInfo.LastChange, AnscGetTickInSeconds());
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Link_GetParamStringValue
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
Link_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL)pContextLinkObject->hContext;
    errno_t                         rc                      = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue,*pUlSize, pEntry->Cfg.Alias);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return -1;
        }

        return 0;
    }

    if (strcmp(ParamName, "Name") == 0)
    {
        if ( _ansc_strlen(pEntry->StaticInfo.Name) == 0 )
        {
            ANSC_STATUS         returnStatus;
            ULONG               ulNameBufSize;

            ulNameBufSize = sizeof(pEntry->StaticInfo.Name);
            returnStatus =
                CosaUtilGetLowerLayerName
                    (
                        pEntry->Cfg.LinkType,
                        pEntry->Cfg.LinkInstNum,
                        pEntry->StaticInfo.Name,
                        &ulNameBufSize
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscTraceWarning(("%s -- failed to retrieve LowerLayer name parameter, error code %lu\n", __FUNCTION__, returnStatus));
            }
			else
			{
				CosaDmlEthLinkUpdateStaticLowerLayerName( pMyObject->hSbContext,
	 													  pEntry->Cfg.InstanceNumber,
	 													  pEntry );
			}
        }

        rc = strcpy_s(pValue,*pUlSize, pEntry->StaticInfo.Name);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        if( _ansc_strlen(pEntry->Cfg.LowerLayers) == 0 )
        {            
            if ( (pEntry->Cfg.LinkType == COSA_DML_LINK_TYPE_LAST) || (pEntry->Cfg.LinkInstNum == 0) )
            {
                /* Lower Link is invalid*/
                pEntry->Cfg.LowerLayers[0] = '\0';

            }
            else
            {
                ULONG               ulBufLen    = sizeof(pEntry->Cfg.LowerLayers);

                CosaUtilConstructLowerLayers
                    (
                        pEntry->Cfg.LinkType,
                        pEntry->Cfg.LinkInstNum,
                        pEntry->Cfg.LowerLayers,
                        &ulBufLen
                    );
            }

            rc = strcpy_s(pValue,*pUlSize, pEntry->Cfg.LowerLayers);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
        }
        else //LowerLayer is pre-set
        {
           rc = strcpy_s(pValue,*pUlSize, pEntry->Cfg.LowerLayers);
           if(rc != EOK)
           {
             ERR_CHK(rc);
             return -1;
           }
        }

        return 0;
    }

    if (strcmp(ParamName, "MACAddress") == 0)
    {       
	PCOSA_DATAMODEL_ETHERNET pMyObject = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
	CosaDmlEthLinkUpdateStaticMac(pMyObject->hSbContext, &pEntry->Cfg,pEntry);

	/*
	//dpotter
	printf
        (
            "%02x:%02x:%02x:%02x:%02x:%02x",
            pEntry->StaticInfo.MacAddress[0],
            pEntry->StaticInfo.MacAddress[1],
            pEntry->StaticInfo.MacAddress[2],
            pEntry->StaticInfo.MacAddress[3],
            pEntry->StaticInfo.MacAddress[4],
            pEntry->StaticInfo.MacAddress[5]
        );
	fflush(stdout);
	*/
	
        rc = sprintf_s
        (
            pValue,
            *pUlSize,
            "%02x:%02x:%02x:%02x:%02x:%02x",
            pEntry->StaticInfo.MacAddress[0],
            pEntry->StaticInfo.MacAddress[1],
            pEntry->StaticInfo.MacAddress[2],
            pEntry->StaticInfo.MacAddress[3],
            pEntry->StaticInfo.MacAddress[4],
            pEntry->StaticInfo.MacAddress[5]
        );
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return -1;
        }
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Link_SetParamBoolValue
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
Link_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL)pContextLinkObject->hContext;
    
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pEntry->Cfg.bEnabled = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "PriorityTagging") == 0)
    {
        /* save update to backup */
        pEntry->Cfg.bPriorityTagging = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Link_SetParamIntValue
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
Link_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Link_SetParamUlongValue
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
Link_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(uValue);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Link_SetParamStringValue
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
Link_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL)pContextLinkObject->hContext;
    errno_t                         rc                      = -1;
    
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pEntry->Cfg.Alias, sizeof(pEntry->Cfg.Alias) ,pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        ULONG                           ulIndex;
        CHAR                            ucEntryParamName[256]       = {0};
        CHAR                            ucEntryNameValue[256]       = {0};
#if defined (MULTILAN_FEATURE)
        ULONG                           ulEntryNameLen = 256;
#else
        ULONG                             size;
        parameterValStruct_t            varStruct;
#endif
        if ( _ansc_strlen(pString) == 0 )
        {
            pEntry->Cfg.LinkType    = COSA_DML_LINK_TYPE_LAST;
            pEntry->Cfg.LinkInstNum = 0;
            pEntry->Cfg.LinkName[0] = '\0';
        }
        else
        {
            pEntry->Cfg.LinkType = CosaUtilGetLinkTypeFromPath(pString);

            /* Normalize the LowerLayer string -- remove the '.' at the end */
            if ( pString[_ansc_strlen(pString) - 1] == '.' )
            {
                pString[_ansc_strlen(pString) - 1] = '\0';   
            }

            /* Extract Instance Number */
            ulIndex = _ansc_strlen(pString) - 1;
#if defined (MULTILAN_FEATURE)
            while ( (ulIndex != 0) && (pString[ulIndex - 1] != '.') )
#else
	    while ( (ulIndex != 0) && (pString[ulIndex] != '.') )
#endif
            {
                ulIndex--;
            }

            if ( ulIndex == 0 )
            {
                pEntry->Cfg.LinkInstNum = 0;
            }
            else
            {
                pEntry->Cfg.LinkInstNum = (ULONG)AnscString2Int(&pString[ulIndex]);
            }

            /* Retrieve LinkName */
            rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName),"%s.Name", pString);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              return FALSE;
            }
#if defined (MULTILAN_FEATURE)
            ulEntryNameLen = sizeof(ucEntryNameValue);
            if ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen))
#else
	    varStruct.parameterName  = ucEntryParamName;
            varStruct.parameterValue = ucEntryNameValue;

            if ( ANSC_STATUS_SUCCESS == 
                    COSAGetParamValueByPathName(g_MessageBusHandle, &varStruct, &size) )
#endif
            {
               rc = STRCPY_S_NOCLOBBER(pEntry->Cfg.LinkName, sizeof(pEntry->Cfg.LinkName),ucEntryNameValue);
               if(rc != EOK)
               {
                 ERR_CHK(rc);
                 return FALSE;
               }
            }
            else
            {
                pEntry->Cfg.LinkName[0] = '\0';
            }
        }

        return  TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Link_Validate
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
Link_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_ETHERNET        pMyObject         = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PSLIST_HEADER                   pListHead         = (PSLIST_HEADER            )&pMyObject->EthernetLinkList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_LINK_FULL         pEntry            = (PCOSA_DML_ETH_LINK_FULL  )pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2     = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_ETH_LINK_FULL         pEntry2           = (PCOSA_DML_ETH_LINK_FULL  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    errno_t                         rc                = -1;

    pSLinkEntry = AnscSListGetFirstEntry(pListHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        pEntry2       = (PCOSA_DML_ETH_LINK_FULL)pCosaContext2->hContext;

        if ( 
                 pEntry2 && 
                 ((ULONG)pEntry2 != (ULONG)pEntry) && 
                  (strcmp(pEntry->Cfg.Alias, pEntry2->Cfg.Alias) == 0)
           )
        {
            rc = strcpy_s(pReturnParamName, *puLength, "Alias");
            if(rc != EOK)
            {
              ERR_CHK(rc);
              return FALSE;
            }

            *puLength = AnscSizeOfString("Alias");
            
            return FALSE;
        }
    }
    
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Link_Commit
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
Link_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL  )pContextLinkObject->hContext;

    if ( pContextLinkObject->bNew )
    {
        pContextLinkObject->bNew = FALSE;

        CosaDmlEthLinkAddEntry(pMyObject->hSbContext, pEntry);
        CosaEthLinkRegDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pContextLinkObject);
    }
    else
    {
        CosaDmlEthLinkSetCfg(pMyObject->hSbContext, &pEntry->Cfg);
    }
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Link_Rollback
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
Link_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL)pContextLinkObject->hContext;

    CosaDmlEthLinkGetCfg(pMyObject->hSbContext, &pEntry->Cfg);
    
    return 0;
}

/***********************************************************************

 APIs for Object:

    Ethernet.Link.{i}.Stats.

    *  Stats1_GetParamBoolValue
    *  Stats1_GetParamIntValue
    *  Stats1_GetParamUlongValue
    *  Stats1_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats1_GetParamBoolValue
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
Stats1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats1_GetParamIntValue
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
Stats1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats1_GetParamUlongValue
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
Stats1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL)pContextLinkObject->hContext;
    COSA_DML_ETH_STATS              Stats;

    CosaDmlEthLinkGetStats(pMyObject->hSbContext, pEntry->Cfg.InstanceNumber, &Stats);

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "BytesSent") == 0)
    {
        /* collect value */
        *puLong = Stats.BytesSent;
        return TRUE;
    }

    if (strcmp(ParamName, "BytesReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.BytesReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "PacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.PacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "PacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.PacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "ErrorsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.ErrorsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "ErrorsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.ErrorsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "UnicastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "UnicastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "DiscardPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "DiscardPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "MulticastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "MulticastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "BroadcastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "BroadcastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "UnknownProtoPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.UnknownProtoPacketsReceived;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Stats1_GetParamStringValue
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
Stats1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);

    return -1;
}


/***********************************************************************

 APIs for Object:

    Ethernet.VLANTermination.{i}.

    *  VLANTermination_GetEntryCount
    *  VLANTermination_GetEntry
    *  VLANTermination_AddEntry
    *  VLANTermination_DelEntry
    *  VLANTermination_GetParamBoolValue
    *  VLANTermination_GetParamIntValue
    *  VLANTermination_GetParamUlongValue
    *  VLANTermination_GetParamStringValue
    *  VLANTermination_SetParamBoolValue
    *  VLANTermination_SetParamIntValue
    *  VLANTermination_SetParamUlongValue
    *  VLANTermination_SetParamStringValue
    *  VLANTermination_Validate
    *  VLANTermination_Commit
    *  VLANTermination_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLANTermination_GetEntryCount
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
VLANTermination_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_ETHERNET        pMyObject = (PCOSA_DATAMODEL_ETHERNET)g_pCosaBEManager->hEthernet;
    
    return AnscSListQueryDepth(&pMyObject->EthernetVlanTerminationList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        VLANTermination_GetEntry
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
VLANTermination_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
   PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY       )NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(&pMyObject->EthernetVlanTerminationList, nIndex);

    if ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);

        *pInsNumber = pCosaContext->InstanceNumber;
    }

    return pCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        VLANTermination_AddEntry
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
VLANTermination_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->EthernetVlanTerminationList;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL  )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    errno_t                         rc                      = -1;

    pEntry = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)AnscAllocateMemory(sizeof(COSA_DML_ETH_VLAN_TERMINATION_FULL));
    if (!pEntry)
    {
        return NULL;
    }

    rc = sprintf_s(pEntry->StaticInfo.Name, sizeof(pEntry->StaticInfo.Name),"cpe-vlan-termination%lu", pMyObject->ulEthernetVlanTerminationNextInstance);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pEntry);
      return NULL;
    }

    rc = sprintf_s(pEntry->Cfg.Alias, sizeof(pEntry->Cfg.Alias),"cpe-vlan-termination%lu", pMyObject->ulEthernetVlanTerminationNextInstance);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pEntry);
      return NULL;
    }

    pEntry->DynamicInfo.Status = COSA_DML_IF_STATUS_NotPresent;

    /* Update the cache */
    pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pCosaContext)
    {
        AnscFreeMemory(pEntry);

        return NULL;
    }

    pCosaContext->InstanceNumber = pEntry->Cfg.InstanceNumber = pMyObject->ulEthernetVlanTerminationNextInstance;

    pMyObject->ulEthernetVlanTerminationNextInstance++;

    if ( pMyObject->ulEthernetVlanTerminationNextInstance == 0 )
    {
        pMyObject->ulEthernetVlanTerminationNextInstance = 1;
    }

    pCosaContext->hContext        = (ANSC_HANDLE)pEntry;
    pCosaContext->hParentTable    = NULL;
    pCosaContext->bNew            = TRUE;

    CosaSListPushEntryByInsNum(pListHead, pCosaContext);

    CosaEthVlanTerminationRegAddInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pCosaContext);

    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLANTermination_DelEntry
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
VLANTermination_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2           = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pCosaContext->hContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pNewEntry           = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)NULL;
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->EthernetVlanTerminationList;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = NULL;

    CosaDmlEthVlanTerminationDelEntry(NULL, pEntry->Cfg.InstanceNumber);

    /* Update the cache */
    if ( TRUE )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->EthernetVlanTerminationList);

        while ( pSLinkEntry )
        {
            pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

            pNewEntry = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pCosaContext2->hContext;

            if ( pNewEntry && AnscEqualString(pNewEntry->Cfg.Alias, pEntry->Cfg.Alias, FALSE))
            {
                AnscSListPopEntryByLink(pListHead, &pCosaContext2->Linkage);

                CosaEthVlanTerminationRegDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pCosaContext2);

                AnscFreeMemory(pNewEntry);
                AnscFreeMemory(pCosaContext2);

                break;
            }
        }
    }
        
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANTermination_GetParamBoolValue
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
VLANTermination_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pContextLinkObject->hContext;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pEntry->Cfg.bEnabled;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANTermination_GetParamIntValue
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
VLANTermination_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANTermination_GetParamUlongValue
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
VLANTermination_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pContextLinkObject->hContext;

    CosaDmlEthVlanTerminationGetDinfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->DynamicInfo);
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        *puLong = pEntry->DynamicInfo.Status;
        return TRUE;
    }

    if (strcmp(ParamName, "LastChange") == 0)
    {
        /* collect value */
        *puLong = AnscGetTimeIntervalInSeconds(pEntry->DynamicInfo.LastChange, AnscGetTickInSeconds());
        return TRUE;
    }

    if (strcmp(ParamName, "VLANID") == 0)
    {
        /* collect value */
        *puLong = pEntry->Cfg.VLANID;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLANTermination_GetParamStringValue
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
VLANTermination_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pContextLinkObject->hContext;
    errno_t                             rc                  = -1;
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue,*pUlSize, pEntry->Cfg.Alias);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "Name") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue,*pUlSize, pEntry->StaticInfo.Name);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue,*pUlSize, pEntry->Cfg.LowerLayers);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return -1;
        }
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANTermination_SetParamBoolValue
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
VLANTermination_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pContextLinkObject->hContext;
    
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pEntry->Cfg.bEnabled = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANTermination_SetParamIntValue
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
VLANTermination_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANTermination_SetParamUlongValue
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
VLANTermination_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pContextLinkObject->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "VLANID") == 0)
    {
        /* save update to backup */
        pEntry->Cfg.VLANID = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANTermination_SetParamStringValue
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
VLANTermination_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pContextLinkObject->hContext;
    errno_t                             rc                  = -1;
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pEntry->Cfg.Alias,sizeof(pEntry->Cfg.Alias), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        /* save update to backup */
        rc = strcpy_s(pEntry->Cfg.LowerLayers,sizeof(pEntry->Cfg.LowerLayers), pString);
        if(rc != EOK)
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
        VLANTermination_Validate
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
VLANTermination_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_ETHERNET        pMyObject         = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PSLIST_HEADER                   pListHead         = (PSLIST_HEADER            )&pMyObject->EthernetVlanTerminationList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry        = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2     = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry2       = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    errno_t                         rc                = -1;

    if (!CosaDmlEthVlanTerminationValidateCfg(NULL, &pEntry->Cfg, pReturnParamName, puLength))
    {
        return FALSE;
    }

    pSLinkEntry = AnscSListGetFirstEntry(pListHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        pEntry2       = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pCosaContext2->hContext;

        if ( 
                 pEntry2 && 
                 ((ULONG)pEntry2 != (ULONG)pEntry) && 
                 (strcmp(pEntry->Cfg.Alias, pEntry2->Cfg.Alias) == 0)
           )
        {
            rc = strcpy_s(pReturnParamName, *puLength, "Alias");
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }
            *puLength = AnscSizeOfString("Alias");
            
            return FALSE;
        }

        if (     pEntry->Cfg.bEnabled &&
                 pEntry2 && 
                 ((ULONG)pEntry2 != (ULONG)pEntry) && 
                 pEntry2->Cfg.bEnabled &&
                 (strcmp(pEntry->Cfg.EthLinkName, pEntry2->Cfg.EthLinkName) == 0) &&
                 pEntry->Cfg.VLANID == pEntry2->Cfg.VLANID
           )
        {
            // We can not enable
            rc = strcpy_s(pReturnParamName, *puLength, "VLANID");
            if(rc != EOK)
            {
              ERR_CHK(rc);
              return FALSE;
            }
            *puLength = AnscSizeOfString("VLANID");
            
            return FALSE;
        }
    }
    
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLANTermination_Commit
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
VLANTermination_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )g_pCosaBEManager->hEthernet;
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pContextLinkObject->hContext;

    if ( pContextLinkObject->bNew )
    {
        pContextLinkObject->bNew = FALSE;

        CosaDmlEthVlanTerminationAddEntry(NULL, pEntry);
        CosaEthVlanTerminationRegDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pContextLinkObject);
    }
    else
    {
        CosaDmlEthVlanTerminationSetCfg(NULL, &pEntry->Cfg);
    }
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLANTermination_Rollback
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
VLANTermination_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pContextLinkObject->hContext;

    CosaDmlEthVlanTerminationGetCfg(NULL, &pEntry->Cfg);
    
    return 0;
}

/***********************************************************************

 APIs for Object:

    Ethernet.VLANTermination.{i}.Stats.

    *  VLANTermination_Stats1_GetParamBoolValue
    *  VLANTermination_Stats1_GetParamIntValue
    *  VLANTermination_Stats1_GetParamUlongValue
    *  VLANTermination_Stats1_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANTermination_Stats_GetParamBoolValue
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
VLANTermination_Stats_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANTermination_Stats_GetParamIntValue
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
VLANTermination_Stats_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANTermination_Stats_GetParamUlongValue
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
VLANTermination_Stats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pContextLinkObject->hContext;
    COSA_DML_ETH_STATS              Stats;

    CosaDmlEthVlanTerminationGetStats(NULL, pEntry->Cfg.InstanceNumber, &Stats);

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "BytesSent") == 0)
    {
        /* collect value */
        *puLong = Stats.BytesSent;
        return TRUE;
    }

    if (strcmp(ParamName, "BytesReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.BytesReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "PacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.PacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "PacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.PacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "ErrorsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.ErrorsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "ErrorsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.ErrorsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "UnicastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "UnicastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "DiscardPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "DiscardPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "MulticastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "MulticastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "BroadcastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "BroadcastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "UnknownProtoPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.UnknownProtoPacketsReceived;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLANTermination_Stats_GetParamStringValue
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
VLANTermination_Stats_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);

    return -1;
}
