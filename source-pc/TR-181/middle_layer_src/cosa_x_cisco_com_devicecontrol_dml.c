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

    module: cosa_x_cisco_com_devicecontrol_dml.c

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

        07/15/2011    initial revision.

**************************************************************************/

#include "cosa_x_cisco_com_devicecontrol_dml.h"
#include "cosa_drg_common.h"

static int ifWanRestart = 0;
BOOL g_httpPort               = FALSE;
BOOL g_httpsPort              = FALSE;
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

    DeviceControl.

    *  X_CISCO_COM_DeviceControl_GetParamBoolValue
    *  X_CISCO_COM_DeviceControl_GetParamIntValue
    *  X_CISCO_COM_DeviceControl_GetParamUlongValue
    *  X_CISCO_COM_DeviceControl_GetParamStringValue
    *  X_CISCO_COM_DeviceControl_SetParamBoolValue
    *  X_CISCO_COM_DeviceControl_SetParamIntValue
    *  X_CISCO_COM_DeviceControl_SetParamUlongValue
    *  X_CISCO_COM_DeviceControl_SetParamStringValue
    *  X_CISCO_COM_DeviceControl_Validate
    *  X_CISCO_COM_DeviceControl_Commit
    *  X_CISCO_COM_DeviceControl_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DeviceControl_GetParamBoolValue
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
X_CISCO_COM_DeviceControl_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    char                            buf[256] = {0};
    ULONG                           uSize    = 0;
    ANSC_STATUS                     retStatus= ANSC_STATUS_SUCCESS;   
    /* check the parameter name and return the corresponding value */
    CcspTraceWarning(("-----DeviceControl_GetParamBoolValue,Trying to get parameter '%s'\n", ParamName));
    
    if (strcmp(ParamName, "ErouterEnable") == 0) {
#if 0//LNT_EMU
        retStatus = CosaDmlDcGetErouterEnabled(NULL,pBool);
#endif
        if (retStatus != ANSC_STATUS_SUCCESS) {
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "MultiHomedBridgingStatus") == 0)
    {
        /* collect value */
        retStatus = CosaDmlDcGetMultiHomedBridgingStatus(NULL,buf,&uSize);
        if (retStatus != ANSC_STATUS_SUCCESS) {
            return FALSE;
        }
        if( AnscEqualString(buf, "0", FALSE)) {
            *pBool = FALSE;
        }
        else {
            *pBool = TRUE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "WanSecondIPRipAdvertised") == 0)
    {
        if (CosaDmlDcGetWanSecondIPRipAdvertised(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "ResetDefaultEnable") == 0)
    {
        if (CosaDmlDcGetResetDefaultEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "ReinitCmMac") == 0)
    {
		/* Always return false for this parameter while a GET operation.*/
       *pBool = FALSE;
		return TRUE;
    }

    if (strcmp(ParamName, "ReinitCmMac") == 0)
    {
		/* Always return false for this parameter while a GET operation.*/
       *pBool = FALSE;
		return TRUE;
    }

    if (strcmp(ParamName, "IGMPProxyEnable") == 0)
    {
        if (CosaDmlDcGetIGMPProxyEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "DNSProxyEnable") == 0)
    {
        if (CosaDmlDcGetDNSProxyEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "TelnetEnable") == 0)
    {
        if (CosaDmlDcGetTelnetEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "SSHEnable") == 0)
    {
        if (CosaDmlDcGetSSHEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "HNAPEnable") == 0)
    {
        if (CosaDmlDcGetHNAPEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "EnableStaticNameServer") == 0)
    {
        if (CosaDmlDcGetEnableStaticNameServer(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "ReleaseWan") == 0)
    {
        if (CosaDmlDcGetReleaseWan(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "RenewWan") == 0)
    {
        if (CosaDmlDcGetRenewWan(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "EnableZeroConfig") == 0)
    {
        if (CosaDmlDcGetEnableZeroConfig(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "HTTPEnable") == 0)
    {
        if (CosaDmlDcGetHTTPEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "HTTPSEnable") == 0)
    {
        if (CosaDmlDcGetHTTPSEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "IGMPSnoopingEnable") == 0)
    {
        if (CosaDmlDcGetIGMPSnoopingEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "EnableMonitorUserChangedParams") == 0)
    {
        *pBool = pMyObject->EnableMonitorUserChangedParams;
        return TRUE;
    }

    if (strcmp(ParamName, "EnableMsoRemoteMgmt") == 0)
    {
        if (CosaDmlDcGetMsoRemoteMgmtEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
 
    if (strcmp(ParamName, "EnableCusadminRemoteMgmt") == 0)
    {
        if (CosaDmlDcGetCusadminRemoteMgmtEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    
    if (strcmp(ParamName, "XHSEthernetPortEnable") == 0)
    {
#if 0//LNT_EMU
        if (CosaDmlDcGetHSEthernetPortEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
#endif
            return FALSE;
        return TRUE;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DeviceControl_GetParamIntValue
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
X_CISCO_COM_DeviceControl_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */
    CcspTraceWarning(("-----DeviceControl_GetParamIntValue,Trying to get parameter '%s'\n", ParamName));


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DeviceControl_GetParamUlongValue
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
X_CISCO_COM_DeviceControl_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    /* CcspTraceWarning(("-----DeviceControl_GetParamUlongValue,Trying to get parameter '%s'\n", ParamName)); */
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;

    char                            buf[256] = {0};
    ULONG                           uSize    = 0;
    ANSC_STATUS                     retStatus= ANSC_STATUS_SUCCESS;
    COSA_DML_WanAddrMode            wanAddrMode;
    ULONG                           ipAddr;

    if (strcmp(ParamName, "MultiHomedHSDFlag") == 0)
    {
        /* collect value */
        retStatus = CosaDmlDcGetMultiHomedHSDFlag(NULL,buf,&uSize);
        if (retStatus != ANSC_STATUS_SUCCESS) {
            return FALSE;
        }
        CcspTraceWarning(("-----DeviceControl_GetParamUlongValue, buf:%s\n", buf));
        if( AnscEqualString(buf, "primary", FALSE)) {
            *puLong = PRIMARY_MODE;
        }
        else if( AnscEqualString(buf, "byoi", FALSE)) {
            *puLong = BYOI_MODE;
        }
        else {
            *puLong = NONE_MODE;
        }
        CcspTraceWarning(("-----DeviceControl_GetParamUlongValue, puLong:%u\n", *puLong));
        return TRUE;
    }

    if (strcmp(ParamName, "MultiHomedUIPageControl") == 0)
    {
        /* collect value */
        retStatus = CosaDmlDcGetMultiHomedUIPageControl(NULL,buf,&uSize);
        if (retStatus != ANSC_STATUS_SUCCESS) {
            return FALSE;
        }
        if( AnscEqualString(buf, "DOCSIS", FALSE)) {
            *puLong = PRIMARY_MODE;
        }
        else if( AnscEqualString(buf, "Non-DOCSIS", FALSE)) {
            *puLong = BYOI_MODE;
        }
        else {
            *puLong = NONE_MODE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "WanAddressMode") == 0)
    {
        if (CosaDmlDcGetWanAddressMode(NULL, &wanAddrMode) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = (ULONG)wanAddrMode;

        return TRUE;
    }
    if (strcmp(ParamName, "WanStaticIPAddress") == 0)
    {
        if (CosaDmlDcGetWanStaticIPAddress(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }
    if (strcmp(ParamName, "WanStaticSubnetMask") == 0)
    {
        if (CosaDmlDcGetWanStaticSubnetMask(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }
    if (strcmp(ParamName, "WanStaticGatewayIP") == 0)
    {
        if (CosaDmlDcGetWanStaticGatewayIP(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }
    if (strcmp(ParamName, "WanSecondIPAddress") == 0)
    {
        if (CosaDmlDcGetWanSecondIPAddr(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }
    if (strcmp(ParamName, "WanBackupDefaultGateway") == 0)
    {
        if (CosaDmlDcGetWanBackupDefaultGateway(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }

    if (strcmp(ParamName, "NameServer1") == 0)
    {
        if (CosaDmlDcGetWanNameServer(NULL, &ipAddr, 1) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }

    if (strcmp(ParamName, "NameServer2") == 0)
    {
        if (CosaDmlDcGetWanNameServer(NULL, &ipAddr, 2) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }
/*
    if (strcmp(ParamName, "NameServer1") == 0)
    {
       *puLong = pMyObject->NameServer1.Value;
        return TRUE;
    }

    if (strcmp(ParamName, "NameServer2") == 0)
    {

       *puLong = pMyObject->NameServer2.Value;
        return TRUE;
    }
    */

    if (strcmp(ParamName, "DeviceMode") == 0)
    {
        if (CosaDmlDcGetDeviceMode(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }

    if (strcmp(ParamName, "HTTPPort") == 0)
    {
        if (CosaDmlDcGetHTTPPort(NULL, puLong) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "HTTPSPort") == 0)
    {
        if (CosaDmlDcGetHTTPSPort(NULL, puLong) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "WebUITimeout") == 0)
    {
        if (CosaDmlDcGetWebUITimeout(NULL, puLong) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "PowerSavingModeStatus") == 0)
    {
        if (CosaDmlDcGetPowerSavingModeStatus(NULL, puLong) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "NoOfGuests") == 0)
    {
        *puLong = CosaDmlDcGetNoOfGuests();

        return TRUE;
    }

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "MocaHardwareStatus") == 0)
    {
        /* collect value */
        *puLong = CosaDmlGetMocaHardwareStatus();
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_DeviceControl_GetParamStringValue
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
X_CISCO_COM_DeviceControl_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "RebootDevice") == 0)
    {
        /* collect value */
	AnscCopyString(pValue, pMyObject->RebootDevice);//LNT_EMU
        if (CosaDmlDcGetRebootDevice(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if (strcmp(ParamName, "FactoryReset") == 0)
    {
        /* collect value */
	AnscCopyString(pValue, pMyObject->RebootDevice);//LNT_EMU
        if (CosaDmlDcGetFactoryReset(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if (strcmp(ParamName, "UserChangedFlags") == 0)
    {
        /* collect value */
        if (CosaDmlDcGetUserChangedFlags(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if (strcmp(ParamName, "DeviceConfigStatus") == 0)
    {
        /* collect value */
        if (CosaDmlDcGetDeviceConfigStatus(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if (strcmp(ParamName, "DeviceConfigIgnore") == 0)
    {
        /* collect value */
        if (CosaDmlDcGetDeviceConfigIgnore(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if (strcmp(ParamName, "SNMPEnable") == 0)
    {
        /* collect value */
        if (CosaDmlDcGetSNMPEnable(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if (strcmp(ParamName, "UserOpComponents") == 0)
    {
        int n = 0;
        *pValue = '\0';
        if (pMyObject->UserOpComponents & 0x00000001) n += sprintf(pValue+n, "%s%s", n ? "," : "", "WebUI");
        if (pMyObject->UserOpComponents & 0x00000008) n += sprintf(pValue+n, "%s%s", n ? "," : "", "Cli");

        return 0;
    }

    if (strcmp(ParamName, "UserChangedParams") == 0)
    {
        return CosaDevCtrlReg_GetUserChangedParams(pMyObject, pValue, pulSize);
    }

    if (strcmp(ParamName, "HostName") == 0)
    {
        return CosaDmlDcGetWanHostName(NULL, pValue);
    }

    if (strcmp(ParamName, "DomainName") == 0)
    {
        return CosaDmlDcGetWanDomainName(NULL, pValue);
    }
    
    /*get wan static Domain name (readonly)*/    
    if (strcmp(ParamName, "WanStaticDomainName") == 0)
    {
#if 0//LNT_EMU
        return CosaDmlDcGetWanStaticDomainName(NULL, pValue);
#endif
    }

    if (strcmp(ParamName, "GuestPassword") == 0)
    {
        return CosaDmlDcGetGuestPassword(NULL, pValue);
    }

    if (strcmp(ParamName, "ParentalControlPassword") == 0)
    {
        return CosaDmlDcGetParConPassword(NULL, pValue);
    }

    if (strcmp(ParamName, "ParentalControlDefaultPwd") == 0)
    {
        return CosaDmlDcGetDefaultParConPassword(NULL, pValue);
    }

    if (strcmp(ParamName, "ParentalControlQuestion") == 0)
    {
        return CosaDmlDcGetParConQuestion(NULL, pValue);
    }

    if (strcmp(ParamName, "ParentalControlAnswer") == 0)
    {
        return CosaDmlDcGetParConAnswer(NULL, pValue);
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DeviceControl_SetParamBoolValue
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
X_CISCO_COM_DeviceControl_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{

    PCOSA_DATAMODEL_DEVICECONTROL      pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    ANSC_STATUS                        retStatus = ANSC_STATUS_SUCCESS;

    /* check the parameter name and set the corresponding value */
    
    if (strcmp(ParamName, "ErouterEnable") == 0) {
#if 0//LNT_EMU        
        retStatus = CosaDmlDcSetErouterEnabled(NULL, bValue);
#endif
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;
     
        return TRUE;
    }

    if (strcmp(ParamName, "WanSecondIPRipAdvertised") == 0)
    {
        pMyObject->WanSecIPRIPAdv = bValue;

        retStatus = CosaDmlDcSetWanSecondIPRipAdvertised(NULL, pMyObject->WanSecIPRIPAdv);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "ResetDefaultEnable") == 0)
    {
        pMyObject->ResetDefaultEnable = bValue;

        retStatus = CosaDmlDcSetResetDefaultEnable(NULL, pMyObject->ResetDefaultEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "ReinitCmMac") == 0)
    {
        if(bValue == 1)
        {
           	// Call reinit mac function
#if 0
       	   	if(CosaDmlDcSetReInitCmMac() == 0)
	   		{
           		return TRUE;
	   		}
	   		else
				return FALSE;
#endif
        }
        else if(bValue == 0)
        {
           /* Need not have to set and return true from here*/
           return TRUE;
        }
        else
           return FALSE;
    }


    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "ReinitCmMac") == 0)
    {
        if(bValue == 1)
        {
           	// Call reinit mac function
       	   	/*if(CosaDmlDcSetReInitCmMac() == 0)
	   		{
           		return TRUE;
	   		}
	   		else
				return FALSE;*/
        }
        else if(bValue == 0)
        {
           /* Need not have to set and return true from here*/
           return TRUE;
        }
        else
           return FALSE;
    }


    if (strcmp(ParamName, "IGMPProxyEnable") == 0)
    {
        retStatus = CosaDmlDcSetIGMPProxyEnable(NULL, bValue);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "DNSProxyEnable") == 0)
    {
        retStatus = CosaDmlDcSetDNSProxyEnable(NULL, bValue);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "TelnetEnable") == 0)
    {
        pMyObject->TelnetEnable = bValue;

        retStatus = CosaDmlDcSetTelnetEnable(NULL, pMyObject->TelnetEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "SSHEnable") == 0)
    {
        pMyObject->SSHEnable = bValue;

        retStatus = CosaDmlDcSetSSHEnable(NULL, pMyObject->SSHEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "HNAPEnable") == 0)
    {
        pMyObject->HNAPEnable = bValue;

        retStatus = CosaDmlDcSetHNAPEnable(NULL, pMyObject->HNAPEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "EnableStaticNameServer") == 0)
    {
        pMyObject->EnableStaticNameServer = bValue;

        retStatus = CosaDmlDcSetEnableStaticNameServer(NULL, pMyObject->EnableStaticNameServer);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "EnableZeroConfig") == 0)
    {
        pMyObject->EnableZeroConfig = bValue;
        retStatus = CosaDmlDcSetEnableZeroConfig(NULL, pMyObject->EnableZeroConfig);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "ReleaseWan") == 0)
    {
        retStatus = CosaDmlDcSetReleaseWan(NULL, pMyObject->ReleaseWan);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "RenewWan") == 0)
    {
        retStatus = CosaDmlDcSetRenewWan(NULL, pMyObject->RenewWan);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "HTTPEnable") == 0)
    {
        pMyObject->HTTPEnable = bValue;
        pMyObject->WebServerChanged = TRUE;

        return TRUE;
    }
 
    if (strcmp(ParamName, "HTTPSEnable") == 0)
    {
        pMyObject->HTTPSEnable = bValue;
        pMyObject->WebServerChanged = TRUE;

        return TRUE;
    }

    if (strcmp(ParamName, "IGMPSnoopingEnable") == 0)
    {
        pMyObject->IGMPSnoopingEnable = bValue;

        retStatus = CosaDmlDcSetIGMPSnoopingEnable(NULL, pMyObject->IGMPSnoopingEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "EnableMonitorUserChangedParams") == 0)
    {
        pMyObject->EnableMonitorUserChangedParams = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "EnableMsoRemoteMgmt") == 0)
    {
        pMyObject->EnableMsoRemoteMgmt = bValue;

        retStatus = CosaDmlDcSetMsoRemoteMgmtEnable(NULL, pMyObject->EnableMsoRemoteMgmt);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "EnableCusadminRemoteMgmt") == 0)
    {
        pMyObject->EnableCusadminRemoteMgmt = bValue;

        retStatus = CosaDmlDcSetCusadminRemoteMgmtEnable(NULL, pMyObject->EnableCusadminRemoteMgmt);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }
    
    if (strcmp(ParamName, "XHSEthernetPortEnable") == 0)
    {
#if 0//LNT_EMU
         if (CosaDmlDcSetHSEthernetPortEnable(NULL, bValue) != ANSC_STATUS_SUCCESS)
             return FALSE;
#endif
        pMyObject->bXHSPortEnabled = bValue;
        pMyObject->bXHSPortChanged = 1;
        return TRUE;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DeviceControl_SetParamIntValue
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
X_CISCO_COM_DeviceControl_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DeviceControl_SetParamUlongValue
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
X_CISCO_COM_DeviceControl_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL      pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    ANSC_STATUS                        retStatus = ANSC_STATUS_SUCCESS;

    /* check the parameter name and set the corresponding value */

    CcspTraceWarning(("--------X_CISCO_COM_DeviceControl_SetParamUlongValue...\n"));
    if (strcmp(ParamName, "MultiHomedHSDFlag") == 0)
    {
        pMyObject->HsdFlag = uValue;
        pMyObject->Mode = uValue;

        retStatus = CosaDmlDcSetMultiHomedHSDFlag(NULL, pMyObject->HsdFlag);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "MultiHomedUIPageControl") == 0)
    {
        pMyObject->UIPageControl = uValue;

        retStatus = CosaDmlDcSetMultiHomedUIPageControl(NULL, pMyObject->UIPageControl);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "WanAddressMode") == 0)
    {
        pMyObject->WanAddrMode = uValue;

        retStatus = CosaDmlDcSetWanAddressMode(NULL, pMyObject->WanAddrMode);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }
    if (strcmp(ParamName, "WanStaticIPAddress") == 0)
    {
        pMyObject->WanIPAddr.Value = uValue;

        retStatus = CosaDmlDcSetWanStaticIPAddress(NULL, pMyObject->WanIPAddr.Value);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        ifWanRestart = 1;

        return TRUE;
    }

    if (strcmp(ParamName, "WanStaticSubnetMask") == 0)
    {
        pMyObject->WanSubnetMask.Value = uValue;

        retStatus = CosaDmlDcSetWanStaticSubnetMask(NULL, pMyObject->WanSubnetMask.Value);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        ifWanRestart = 1;

        return TRUE;
    }

    if (strcmp(ParamName, "WanStaticGatewayIP") == 0)
    {
        pMyObject->WanGateway.Value = uValue;

        retStatus = CosaDmlDcSetWanStaticGatewayIP(NULL, pMyObject->WanGateway.Value);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        ifWanRestart = 1;

        return TRUE;
    }

    if (strcmp(ParamName, "WanSecondIPAddress") == 0)
    {
        pMyObject->WanSecIPAddr.Value = uValue;

        retStatus = CosaDmlDcSetWanSecondIPAddr(NULL, pMyObject->WanSecIPAddr.Value);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "WanBackupDefaultGateway") == 0)
    {
        pMyObject->WanBackupGateway.Value = uValue;

        retStatus = CosaDmlDcSetWanBackupDefaultGateway(NULL, pMyObject->WanBackupGateway.Value);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "NameServer1") == 0)
    {
        pMyObject->NameServer1.Value = uValue;

        retStatus = CosaDmlDcSetWanNameServer(NULL, pMyObject->NameServer1.Value, 1);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "NameServer2") == 0)
    {
        pMyObject->NameServer2.Value = uValue;

        retStatus = CosaDmlDcSetWanNameServer(NULL, pMyObject->NameServer2.Value, 2);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "DeviceMode") == 0)
    {
        switch(uValue){
            case(COSA_DML_DEVICE_MODE_Bridge):
            case(COSA_DML_DEVICE_MODE_Ipv4):
            case(COSA_DML_DEVICE_MODE_Ipv6):
            case(COSA_DML_DEVICE_MODE_Dualstack):
            break;
            default:
                return(FALSE);
            break;
        }
        if(CosaDmlDcSetDeviceMode(NULL,uValue)!=ANSC_STATUS_SUCCESS)
            return(FALSE);
        pMyObject->DeviceMode = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "HTTPPort") == 0)
    {
	 if (IsPortInUse(uValue))
        {
            CcspTraceWarning(("Port already in use\n"));
            return FALSE;
        }
        pMyObject->HTTPPort = uValue;
        pMyObject->WebServerChanged = TRUE;
	g_httpPort = TRUE;

        return TRUE;
    }

    if (strcmp(ParamName, "HTTPSPort") == 0)
    {
        pMyObject->HTTPSPort = uValue;
        pMyObject->WebServerChanged = TRUE;
	g_httpsPort = TRUE;

        return TRUE;
    }
 
    if (strcmp(ParamName, "WebUITimeout") == 0)
    {
        pMyObject->WebUITimeout = uValue;

        retStatus = CosaDmlDcSetWebUITimeout(NULL, pMyObject->WebUITimeout);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "NoOfGuests") == 0)
    {
        pMyObject->NoOfGuests = uValue;

        if (CosaDmlDcSetNoOfGuests(pMyObject->NoOfGuests) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }
 
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DeviceControl_SetParamStringValue
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
X_CISCO_COM_DeviceControl_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL      pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    ANSC_STATUS                        retStatus = ANSC_STATUS_SUCCESS;

    /* check the parameter name and set the corresponding value */

    if (strcmp(ParamName, "RebootDevice") == 0)
    {
        AnscCopyString(pMyObject->RebootDevice, pString);
        pMyObject->bResetChanged = 1;

        return TRUE;
    }

    if (strcmp(ParamName, "FactoryReset") == 0)
    {
        AnscCopyString(pMyObject->FactoryReset, pString);
        pMyObject->bFactoryResetChanged = 1;

        return TRUE;
    }

    if (strcmp(ParamName, "UserChangedFlags") == 0)
    {
        AnscCopyString(pMyObject->UserChangedFlags, pString);

        retStatus = CosaDmlDcSetUserChangedFlags(NULL, pMyObject->UserChangedFlags);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "DeviceConfigIgnore") == 0)
    {
        AnscCopyString(pMyObject->DeviceConfigIgnore, pString);

        retStatus = CosaDmlDcSetDeviceConfigIgnore(NULL, pMyObject->DeviceConfigIgnore);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "SNMPEnable") == 0)
    {
        AnscCopyString(pMyObject->SNMPEnable, pString);

        retStatus = CosaDmlDcSetSNMPEnable(NULL, pMyObject->SNMPEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "UserOpComponents") == 0)
    {
        pMyObject->UserOpComponents = 0;

        if (strstr(pString, "WebUI")) pMyObject->UserOpComponents |= 0x00000001;
        if (strstr(pString, "Cli")) pMyObject->UserOpComponents   |= 0x00000008;

        return TRUE;
    }

    if (strcmp(ParamName, "HostName") == 0)
    {
        AnscCopyString(pMyObject->HostName, pString);

        retStatus = CosaDmlDcSetHostName(NULL, pMyObject->HostName);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "DomainName") == 0)
    {
        AnscCopyString(pMyObject->DomainName, pString);

        retStatus = CosaDmlDcSetDomainName(NULL, pMyObject->DomainName);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "GuestPassword") == 0)
    {
        AnscCopyString(pMyObject->GuestPassword, pString);

        retStatus = CosaDmlDcSetGuestPassword(NULL, pMyObject->GuestPassword);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "ParentalControlPassword") == 0)
    {
        AnscCopyString(pMyObject->ParentalControlPassword, pString);

        retStatus = CosaDmlDcSetParConPassword(NULL, pMyObject->ParentalControlPassword);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "ParentalControlQuestion") == 0)
    {
        AnscCopyString(pMyObject->ParentalControlQuestion, pString);

        retStatus = CosaDmlDcSetParConQuestion(NULL, pMyObject->ParentalControlQuestion);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (strcmp(ParamName, "ParentalControlAnswer") == 0)
    {
        AnscCopyString(pMyObject->ParentalControlAnswer, pString);

        retStatus = CosaDmlDcSetParConAnswer(NULL, pMyObject->ParentalControlAnswer);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DeviceControl_Validate
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
X_CISCO_COM_DeviceControl_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL      pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    if(pMyObject->WebServerChanged == TRUE)
    {
        if(pMyObject->HTTPSPort == pMyObject->HTTPPort || \
                pMyObject->HTTPSPort == 0 || \
                pMyObject->HTTPPort == 0)
            return FALSE;
            
    }
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_DeviceControl_Commit
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
X_CISCO_COM_DeviceControl_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL      pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    ANSC_STATUS                        retStatus = ANSC_STATUS_SUCCESS;

    if(ifWanRestart == 1 && pMyObject->WanAddrMode == COSA_DML_WanAddrMode_Static) {
        commonSyseventSet("wan-restart", "");
    }
    ifWanRestart = 0;

    if( pMyObject->bResetChanged)
    {
        pMyObject->bResetChanged = 0;
        retStatus = CosaDmlDcSetRebootDevice(NULL, pMyObject->RebootDevice);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;
    }

    if( pMyObject->bFactoryResetChanged)
    {
        pMyObject->bFactoryResetChanged = 0;
        retStatus = CosaDmlDcSetFactoryReset(NULL, pMyObject->FactoryReset);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;
    }

    if (pMyObject->WebServerChanged)
    {
        pMyObject->WebServerChanged = FALSE;
        if (CosaDmlDcSetWebServer(pMyObject->HTTPEnable, pMyObject->HTTPSEnable,
                pMyObject->HTTPPort, pMyObject->HTTPSPort) != ANSC_STATUS_SUCCESS)
            return FALSE;
    }

    CosaDevCtrlReg_SetUserChangedParamsControl((ANSC_HANDLE)pMyObject);
    
    if (pMyObject->bXHSPortChanged) {
        pMyObject->bXHSPortChanged = FALSE;
        //if (CosaDmlDcSetHSEthernetPortEnable(NULL, pMyObject->bXHSPortEnabled) != ANSC_STATUS_SUCCESS)
            return FALSE;
    }
        

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_DeviceControl_Rollback
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
X_CISCO_COM_DeviceControl_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL      pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    CosaDevCtrlReg_GetUserChangedParamsControl((ANSC_HANDLE)pMyObject);

    pMyObject->bResetChanged = 0;
    pMyObject->bFactoryResetChanged = 0;
    pMyObject->WebServerChanged = FALSE;

    return 0;
}

ULONG
LanMngm_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pDevCtrl    = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;

    return AnscSListQueryDepth(&pDevCtrl->LanMngmList);
}

ANSC_HANDLE
LanMngm_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pDevCtrl    = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    pSLinkEntry = AnscQueueGetEntryByIndex((ANSC_HANDLE)&pDevCtrl->LanMngmList, nIndex);
    
    if (pSLinkEntry)
    {
        pLinkObj = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        *pInsNumber = pLinkObj->InstanceNumber;
    }
    
    return pLinkObj;
}

ANSC_HANDLE
LanMngm_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pDevCtrl    = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    PCOSA_DML_LAN_MANAGEMENT        pLanMngm    = NULL;
    BOOL                                      bridgeMode;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return NULL;

    pLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pLinkObj)
        return NULL;
    
    pLanMngm = AnscAllocateMemory(sizeof(COSA_DML_LAN_MANAGEMENT));
    if (!pLanMngm)
    {
        AnscFreeMemory(pLinkObj);
        return NULL;
    }

    pLinkObj->InstanceNumber = pDevCtrl->ulLanMngmNextInsNum;
    pLanMngm->InstanceNumber = pDevCtrl->ulLanMngmNextInsNum;
    pDevCtrl->ulLanMngmNextInsNum++;
    if (pDevCtrl->ulLanMngmNextInsNum == 0)
        pDevCtrl->ulLanMngmNextInsNum = 1;

    _ansc_sprintf(pLanMngm->Alias, "cpe-LanManamgement-%d", pLinkObj->InstanceNumber);
    pLinkObj->hContext      = (ANSC_HANDLE)pLanMngm;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pDevCtrl->LanMngmList, pLinkObj);
    CosaDevCtrlReg_AddLanMngmInfo((ANSC_HANDLE)pDevCtrl, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;

    return pLinkObj;
}


ULONG
LanMngm_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pDevCtrl    = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_LAN_MANAGEMENT        pLanMngm    = (PCOSA_DML_LAN_MANAGEMENT)pLinkObj->hContext;

    if (CosaDmlLanMngm_DelEntry(pLinkObj->InstanceNumber) != ANSC_STATUS_SUCCESS)
        return -1;

    AnscSListPopEntryByLink((PSLIST_HEADER)&pDevCtrl->LanMngmList, &pLinkObj->Linkage);
    if (pLinkObj->bNew)
        CosaDevCtrlReg_DelLanMngmInfo((ANSC_HANDLE)pDevCtrl, (ANSC_HANDLE)pLinkObj);

    AnscFreeMemory(pLanMngm);
    AnscFreeMemory(pLinkObj);

    return 0;
}

BOOL
LanMngm_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_LAN_MANAGEMENT        pLanMngm    = (PCOSA_DML_LAN_MANAGEMENT)pLinkObj->hContext;

    if (strcmp(ParamName, "LanDhcpServer") == 0)
    {
        *pBool = pLanMngm->LanDhcpServer;
        return TRUE;
    }
    if (strcmp(ParamName, "LanNaptEnable") == 0)
    {

        *pBool = pLanMngm->LanNaptEnable;
        return TRUE;
    }
    if (strcmp(ParamName, "LanDhcp125") == 0)
    {
        *pBool = pLanMngm->LanDhcp125;
        return TRUE;
    }
    if (strcmp(ParamName, "LanHnap") == 0)
    {
        *pBool = pLanMngm->LanHnap;
        return TRUE;
    }
    if (strcmp(ParamName, "LanUpnp") == 0)
    {
        *pBool = pLanMngm->LanUpnp;
        return TRUE;
    }

    if (strcmp(ParamName, "LanNaptOnDhcp") == 0)
    {
        *pBool = pLanMngm->LanNaptType;
        return TRUE;
    }


    return FALSE;
}

BOOL
LanMngm_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    return FALSE;
}

BOOL
LanMngm_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_LAN_MANAGEMENT        pLanMngm    = (PCOSA_DML_LAN_MANAGEMENT)pLinkObj->hContext;

    if (strcmp(ParamName, "LanMode") == 0)
    {
        *pUlong = pLanMngm->LanMode;
        return TRUE;
    }
    if (strcmp(ParamName, "LanNetwork") == 0)
    {
        *pUlong = pLanMngm->LanNetwork.Value;
        return TRUE;
    }
    if (strcmp(ParamName, "LanNetworksAllow") == 0)
    {
        *pUlong = pLanMngm->LanNetworksAllow;
        return TRUE;
    }
    if (strcmp(ParamName, "LanSubnetMask") == 0)
    {
        *pUlong = pLanMngm->LanSubnetMask.Value;
        return TRUE;
    }
    if (strcmp(ParamName, "LanIPAddress") == 0)
    {
        *pUlong = pLanMngm->LanIPAddress.Value;
	return TRUE;
    }
    if (strcmp(ParamName, "LanTos") == 0)
    {
        *pUlong = pLanMngm->LanTos;
        return TRUE;
    }

    return FALSE;
}

ULONG
LanMngm_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    return -1;
}

BOOL
LanMngm_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{

    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_LAN_MANAGEMENT        pLanMngm    = (PCOSA_DML_LAN_MANAGEMENT)pLinkObj->hContext;
    BOOL                                      bridgeMode;
    ULONG                                     deviceMode;
    
    if (CosaDmlDcGetDeviceMode(NULL, &deviceMode) != ANSC_STATUS_SUCCESS)
            return FALSE;
    
    if (!(deviceMode-1)) 
        return FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return FALSE;


    if (strcmp(ParamName, "LanDhcpServer") == 0)
    {
        pLanMngm->LanDhcpServer = bValue;
        return TRUE;
    }
    if (strcmp(ParamName, "LanNaptEnable") == 0)
    {
        pLanMngm->LanNaptEnable = bValue;
        return TRUE;
    }
    if (strcmp(ParamName, "LanDhcp125") == 0)
    {
        pLanMngm->LanDhcp125 = bValue;
        return TRUE;
    }
    if (strcmp(ParamName, "LanHnap") == 0)
    {
        pLanMngm->LanHnap = bValue;
        return TRUE;
    }
    if (strcmp(ParamName, "LanUpnp") == 0)
    {
        pLanMngm->LanUpnp = bValue;
        return TRUE;
    }
    if (strcmp(ParamName, "LanNaptOnDhcp") == 0)
    {
        pLanMngm->LanNaptType = bValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
LanMngm_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    )
{
    return FALSE;
}

BOOL
LanMngm_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_LAN_MANAGEMENT        pLanMngm    = (PCOSA_DML_LAN_MANAGEMENT)pLinkObj->hContext;
    BOOL                                      bridgeMode;
    ULONG                                     deviceMode;

    if (CosaDmlDcGetDeviceMode(NULL, &deviceMode) != ANSC_STATUS_SUCCESS)
            return FALSE;
    
    if (!(deviceMode-1)) 
        return FALSE;

    if (strcmp(ParamName, "LanMode") == 0)
    {
        pLanMngm->LanMode = uValuepUlong;
	//RDKB-EMUALTOR
        if(pLanMngm->LanMode == COSA_DML_LanMode_BridgeStatic)
        {
                system("cp /etc/dnsmasq.conf /var/dnsmasq_org.conf");
                system("sh /lib/rdk/BridgeMode.sh");
        }
        else if(pLanMngm->LanMode == COSA_DML_LanMode_Router)
        {
                system("cp /var/dnsmasq_org.conf /etc/dnsmasq.conf");
                system("sh /lib/rdk/RouterMode.sh");
        }
        return TRUE;
    }

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return FALSE;

    if (strcmp(ParamName, "LanNetwork") == 0)
    {
        pLanMngm->LanNetwork.Value = uValuepUlong;
        return TRUE;
    }
    if (strcmp(ParamName, "LanNetworksAllow") == 0)
    {
        pLanMngm->LanNetworksAllow = uValuepUlong;
        return TRUE;
    }
    if (strcmp(ParamName, "LanSubnetMask") == 0)
    {
		CcspTraceWarning(("RDKB_LAN_CONFIG_CHANGED: Setting new LanSubnetMask value  ...\n"));
        pLanMngm->LanSubnetMask.Value = uValuepUlong;
        return TRUE;
    }
    if (strcmp(ParamName, "LanIPAddress") == 0)
    {
		CcspTraceWarning(("RDKB_LAN_CONFIG_CHANGED: Setting new LanIPAddress value  ...\n"));
        pLanMngm->LanIPAddress.Value = uValuepUlong;
        return TRUE;
    }
    if (strcmp(ParamName, "LanTos") == 0)
    {
        pLanMngm->LanTos = uValuepUlong;
        return TRUE;
    }

    	return FALSE;
}

BOOL
LanMngm_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    return FALSE;
}

BOOL
LanMngm_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_LAN_MANAGEMENT        pLanMngm    = (PCOSA_DML_LAN_MANAGEMENT)pLinkObj->hContext;

    /* check subnetmask */
    /* Subnet mask MUST accept ONLY the following IP addresses: */
    /* 255.255.255.0, 255.255.0.0, 255.0.0.0, 255.255.255.128, 255.255.255.252 */
    /*if(pLanMngm->LanSubnetMask.Value != 0xFFFFFF00 &&
       pLanMngm->LanSubnetMask.Value != 0xFFFF0000 &&
       pLanMngm->LanSubnetMask.Value != 0xFF000000 &&
       pLanMngm->LanSubnetMask.Value != 0xFFFFFF80 &&
       pLanMngm->LanSubnetMask.Value != 0xFFFFFFFC )*/ //RDKB-EMU
    if(pLanMngm->LanSubnetMask.Dot[0] != 255 && pLanMngm->LanSubnetMask.Dot[1] != 255 && pLanMngm->LanSubnetMask.Dot[2] != 255 &&
       pLanMngm->LanSubnetMask.Dot[0] != 255 && pLanMngm->LanSubnetMask.Dot[2] != 255 &&
       pLanMngm->LanSubnetMask.Dot[0] != 255 &&
       pLanMngm->LanSubnetMask.Dot[0] != 255 && pLanMngm->LanSubnetMask.Dot[1] != 255 && pLanMngm->LanSubnetMask.Dot[2] != 255 && pLanMngm->LanSubnetMask.Dot[3] != 128 &&
       pLanMngm->LanSubnetMask.Dot[0] != 255 && pLanMngm->LanSubnetMask.Dot[1] != 255 && pLanMngm->LanSubnetMask.Dot[2] != 255 && pLanMngm->LanSubnetMask.Dot[3] != 252 )
    {
		CcspTraceWarning(("RDKB_LAN_CONFIG_CHANGED: Modified LanSubnetMask doesn't meet the conditions,reverting back to old value  ...\n"));
        goto RET_ERR;
    }
    /* check the gateway IP address */
    /* gateway IP address should be private address,*/
    /* range: 10.0.0.0 to 10.255.255.254, 172.16.0.0 to 172.31.255.255, 192.168.0.0 to 192.168.255.255  */
    if(((pLanMngm->LanIPAddress.Value & pLanMngm->LanSubnetMask.Value) == pLanMngm->LanIPAddress.Value) || 
       ((pLanMngm->LanIPAddress.Value | pLanMngm->LanSubnetMask.Value) == 0xFFFFFFFF)){

		CcspTraceWarning(("RDKB_LAN_CONFIG_CHANGED: Modified LanIPAddress doesn't meet the conditions,reverting back to old value  ...\n"));
        goto RET_ERR;
    }else if(pLanMngm->LanIPAddress.Dot[0] == 10 ){
        return TRUE;
    }else if(pLanMngm->LanIPAddress.Dot[0] == 172 && pLanMngm->LanIPAddress.Dot[1] >= 16 && pLanMngm->LanIPAddress.Dot[1] <= 31){
        return TRUE;
    }else if((pLanMngm->LanIPAddress.Value & 0xFFFF0000) == 0xC0A80000){
        return TRUE;
    }else if(pLanMngm->LanIPAddress.Dot[0] == 192 && pLanMngm->LanIPAddress.Dot[1] == 168){
        return TRUE;
    }
RET_ERR:
    CosaDmlLanMngm_GetConf(pLanMngm->InstanceNumber, pLanMngm);
    return FALSE;
}

ULONG
LanMngm_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_LAN_MANAGEMENT        pLanMngm    = (PCOSA_DML_LAN_MANAGEMENT)pLinkObj->hContext;
    PCOSA_DATAMODEL_DEVICECONTROL   pDevCtrl    = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;


    if (pLinkObj->bNew)
    {
        if (CosaDmlLanMngm_AddEntry(pLanMngm) != ANSC_STATUS_SUCCESS)
            return -1;
        CosaDevCtrlReg_DelLanMngmInfo((ANSC_HANDLE)pDevCtrl, (ANSC_HANDLE)pLinkObj);
        pLinkObj->bNew = FALSE;
    }
    else
    {
        if (CosaDmlLanMngm_SetConf(pLanMngm->InstanceNumber, pLanMngm) != ANSC_STATUS_SUCCESS)
        {
            CosaDmlLanMngm_GetConf(pLanMngm->InstanceNumber, pLanMngm);
            return -1;
        }
    }

    return 0;
}

ULONG
LanMngm_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_LAN_MANAGEMENT        pLanMngm    = (PCOSA_DML_LAN_MANAGEMENT)pLinkObj->hContext;
	
    if (CosaDmlLanMngm_GetConf(pLanMngm->InstanceNumber, pLanMngm) != ANSC_STATUS_SUCCESS)
        return -1;
	
    return 0;
}

BOOL
WebAccessLevel_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pBool
    )
{
    /* check the parameter name and return the corresponding value */
    CcspTraceWarning(("-----DeviceControl_GetParamBoolValue,Trying to get parameter '%s'\n", ParamName));

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
WebAccessLevel_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    CcspTraceWarning(("-----WebAccessLevel_GetParamIntValue,Trying to get parameter '%s'\n", ParamName)); 
	
    if (strcmp(ParamName, "HomeUser_Lan_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 1, 1, pInt) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "HomeUser_RfCM_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 1, 2, pInt) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "HomeUser_Mta_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 1, 16, pInt) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "HomeUser_WanRG_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 1, 40, pInt) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "CusAdmin_Lan_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 5, 1, pInt) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "CusAdmin_RfCM_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 5, 2, pInt) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "CusAdmin_Mta_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 5, 16, pInt) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "CusAdmin_WanRG_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 5, 40, pInt) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "AdvUser_Lan_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 10, 1, pInt) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "AdvUser_RfCM_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 10, 2, pInt) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "AdvUser_Mta_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 10, 16, pInt) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "AdvUser_WanRG_Level") == 0)
    {
        return (CosaDmlDcGetWebAccessLevel(NULL, 10, 40, pInt) == ANSC_STATUS_SUCCESS);
    }
    /*CID: 66266 Missing return statement*/
    return FALSE;
	
}

BOOL
WebAccessLevel_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    CcspTraceWarning(("-----DeviceControl_GetParamUlongValue,Trying to get parameter '%s'\n", ParamName));

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

ULONG
WebAccessLevel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    /* check the parameter name and return the corresponding value */
    CcspTraceWarning(("-----DeviceControl_GetParamUlongValue,Trying to get parameter '%s'\n", ParamName));

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

BOOL
WebAccessLevel_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    return FALSE;
}

BOOL
WebAccessLevel_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    CcspTraceWarning(("-----WebAccessLevel_SetParamIntValue,Trying to get parameter '%s'\n", ParamName)); 
	
    if (strcmp(ParamName, "HomeUser_Lan_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 1, 1, iValue) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "HomeUser_RfCM_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 1, 2, iValue) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "HomeUser_Mta_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 1, 16, iValue) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "HomeUser_WanRG_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 1, 40, iValue) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "CusAdmin_Lan_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 5, 1, iValue) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "CusAdmin_RfCM_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 5, 2, iValue) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "CusAdmin_Mta_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 5, 16, iValue) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "CusAdmin_WanRG_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 5, 40, iValue) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "AdvUser_Lan_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 10, 1, iValue) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "AdvUser_RfCM_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 10, 2, iValue) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "AdvUser_Mta_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 10, 16, iValue) == ANSC_STATUS_SUCCESS);
    }

	if (strcmp(ParamName, "AdvUser_WanRG_Level") == 0)
    {
         return (CosaDmlDcSetWebAccessLevel(NULL, 10, 40, iValue) == ANSC_STATUS_SUCCESS);
    }
    /*CID:62013 Missing return statement*/
    return FALSE;
}

BOOL
WebAccessLevel_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    return FALSE;
}

BOOL
WebAccessLevel_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    return FALSE;
}

BOOL
WebAccessLevel_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
WebAccessLevel_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{

    return 0;
}

ULONG
WebAccessLevel_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{

    return 0;
}


