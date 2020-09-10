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
#include "ansc_platform.h"

static int ifWanRestart = 0;

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
BOOL
XConf_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    CcspTraceInfo(("Inside %s function \n",__FUNCTION__));

    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject    = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    PCOSA_DML_DEVICECONTROL_XConf    pXConf       = &pMyObject->XConf;

    if( AnscEqualString(ParamName, "xconfCheckNow", TRUE))
    {
       *pBool=FALSE;
        return TRUE;
    }
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
       *pBool = pXConf->Enable;
        return TRUE;
    }
    return FALSE;
}

ULONG
XConf_GetParamStringValue
    (
       ANSC_HANDLE                 hInsContext,
       char*                       ParamName,
       char*                       pValue,
       ULONG*                      pUlSize
    )
{
    CcspTraceInfo(("Inside %s function \n",__FUNCTION__));

    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject    = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    PCOSA_DML_DEVICECONTROL_XConf    pXConf       = &pMyObject->XConf;

   if( AnscEqualString(ParamName, "ServerURL", TRUE))
    {
       AnscCopyString( pValue , pXConf->ServerURL );
       return 0;
    }
    return -1;
}

BOOL
XConf_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
   CcspTraceInfo(("Inside %s function \n",__FUNCTION__));

    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject    = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    PCOSA_DML_DEVICECONTROL_XConf    pXConf       = &pMyObject->XConf;

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        pXConf->Enable = bValue;
        return TRUE;
    }

    int status = 0;

    if( AnscEqualString(ParamName, "xconfCheckNow", TRUE))
    {
                AnscTraceWarning(("Triggering firmware download check from TR181\n"));
               if( TRUE == bValue )
       {
                      //Need to confirm the functionality once the firmware is obtained
                       return TRUE;
                }
     }
 return FALSE;
}

BOOL
XConf_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    CcspTraceInfo(("Inside %s function \n",__FUNCTION__));

    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject    = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    PCOSA_DML_DEVICECONTROL_XConf    pXConf       = &pMyObject->XConf;

    if (AnscEqualString(ParamName, "ServerURL", TRUE))
    {
      int rc=-1, rc2=-1, rc3=-1;
        rc=IPv4_Url_Validate(pString);
        rc2=IPv6_Url_Validate(pString);
        rc3=Fqdn_Url_Validate(pString);
        if(rc==0 || rc2==0 ||  rc3==0)
        {
         AnscCopyString( pXConf->ServerURL , pString );
           return TRUE;
        }
        else
        {
         return FALSE;
        }

    }
    return FALSE;
}

BOOL
XConf_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
XConf_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject    = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;

    return CosaDmlXConfSetConfig(NULL, &pMyObject->XConf);
}



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
    
    if( AnscEqualString(ParamName, "ErouterEnable", TRUE)) {
        retStatus = CosaDmlDcGetErouterEnabled(NULL,pBool);
        if (retStatus != ANSC_STATUS_SUCCESS) {
            return FALSE;
        }
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MultiHomedBridgingStatus", TRUE))
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

    if (AnscEqualString(ParamName, "WanSecondIPRipAdvertised", TRUE))
    {
        if (CosaDmlDcGetWanSecondIPRipAdvertised(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "ResetDefaultEnable", TRUE))
    {
        if (CosaDmlDcGetResetDefaultEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "ReinitCmMac", TRUE))
    {
		/* Always return false for this parameter while a GET operation.*/
       *pBool = FALSE;
		return TRUE;
    }

    if (AnscEqualString(ParamName, "IGMPProxyEnable", TRUE))
    {
        if (CosaDmlDcGetIGMPProxyEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "DNSProxyEnable", TRUE))
    {
        if (CosaDmlDcGetDNSProxyEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "TelnetEnable", TRUE))
    {
        if (CosaDmlDcGetTelnetEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "SSHEnable", TRUE))
    {
        if (CosaDmlDcGetSSHEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "HNAPEnable", TRUE))
    {
        if (CosaDmlDcGetHNAPEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "EnableStaticNameServer", TRUE))
    {
        if (CosaDmlDcGetEnableStaticNameServer(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "ReleaseWan", TRUE))
    {
        if (CosaDmlDcGetReleaseWan(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "RenewWan", TRUE))
    {
        if (CosaDmlDcGetRenewWan(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "EnableZeroConfig", TRUE))
    {
        if (CosaDmlDcGetEnableZeroConfig(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "HTTPEnable", TRUE))
    {
        if (CosaDmlDcGetHTTPEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "HTTPSEnable", TRUE))
    {
        if (CosaDmlDcGetHTTPSEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "IGMPSnoopingEnable", TRUE))
    {
        if (CosaDmlDcGetIGMPSnoopingEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "EnableMonitorUserChangedParams", TRUE))
    {
        *pBool = pMyObject->EnableMonitorUserChangedParams;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "EnableMsoRemoteMgmt", TRUE))
    {
        if (CosaDmlDcGetMsoRemoteMgmtEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
 
    if (AnscEqualString(ParamName, "EnableCusadminRemoteMgmt", TRUE))
    {
        if (CosaDmlDcGetCusadminRemoteMgmtEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    
    if (AnscEqualString(ParamName, "XHSEthernetPortEnable", TRUE))
    {
        if (CosaDmlDcGetHSEthernetPortEnable(NULL, pBool) != ANSC_STATUS_SUCCESS)
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

    if( AnscEqualString(ParamName, "MultiHomedHSDFlag", TRUE))
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

    if( AnscEqualString(ParamName, "MultiHomedUIPageControl", TRUE))
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

    if (AnscEqualString(ParamName, "WanAddressMode", TRUE))
    {
        if (CosaDmlDcGetWanAddressMode(NULL, &wanAddrMode) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = (ULONG)wanAddrMode;

        return TRUE;
    }
    if (AnscEqualString(ParamName, "WanStaticIPAddress", TRUE))
    {
        if (CosaDmlDcGetWanStaticIPAddress(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "WanStaticSubnetMask", TRUE))
    {
        if (CosaDmlDcGetWanStaticSubnetMask(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "WanStaticGatewayIP", TRUE))
    {
        if (CosaDmlDcGetWanStaticGatewayIP(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "WanSecondIPAddress", TRUE))
    {
        if (CosaDmlDcGetWanSecondIPAddr(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "WanBackupDefaultGateway", TRUE))
    {
        if (CosaDmlDcGetWanBackupDefaultGateway(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "NameServer1", TRUE))
    {
        if (CosaDmlDcGetWanNameServer(NULL, &ipAddr, 1) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "NameServer2", TRUE))
    {
        if (CosaDmlDcGetWanNameServer(NULL, &ipAddr, 2) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }
/*
    if (AnscEqualString(ParamName, "NameServer1", TRUE))
    {
       *puLong = pMyObject->NameServer1.Value;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "NameServer2", TRUE))
    {

       *puLong = pMyObject->NameServer2.Value;
        return TRUE;
    }
    */

    if (AnscEqualString(ParamName, "DeviceMode", TRUE))
    {
        if (CosaDmlDcGetDeviceMode(NULL, &ipAddr) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = ipAddr;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "HTTPPort", TRUE))
    {
        if (CosaDmlDcGetHTTPPort(NULL, puLong) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "HTTPSPort", TRUE))
    {
        if (CosaDmlDcGetHTTPSPort(NULL, puLong) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "ReinitMacThreshold", TRUE))
    {
	*puLong = pMyObject->ReinitMacThreshold;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "WebUITimeout", TRUE))
    {
        if (CosaDmlDcGetWebUITimeout(NULL, puLong) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "PowerSavingModeStatus", TRUE))
    {
        if (CosaDmlDcGetPowerSavingModeStatus(NULL, puLong) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "NoOfGuests", TRUE))
    {
        *puLong = CosaDmlDcGetNoOfGuests();

        return TRUE;
    }

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "MocaHardwareStatus", TRUE))
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
    if( AnscEqualString(ParamName, "RebootDevice", TRUE) )
    {
        /* collect value */
        if (CosaDmlDcGetRebootDevice(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if( AnscEqualString(ParamName, "FactoryReset", TRUE) )
    {
        /* collect value */
        if (CosaDmlDcGetFactoryReset(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if( AnscEqualString(ParamName, "X_CrashPortalURL", TRUE))
    {
      /* collect value */
      if (CosaDmlDcGetX_CrashPortalURL(NULL, pValue) != ANSC_STATUS_SUCCESS)
          return -1;

      return 0;
    }

    if( AnscEqualString(ParamName, "UserChangedFlags", TRUE) )
    {
        /* collect value */
        if (CosaDmlDcGetUserChangedFlags(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if( AnscEqualString(ParamName, "DeviceConfigStatus", TRUE) )
    {
        /* collect value */
        if (CosaDmlDcGetDeviceConfigStatus(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if( AnscEqualString(ParamName, "DeviceConfigIgnore", TRUE) )
    {
        /* collect value */
        if (CosaDmlDcGetDeviceConfigIgnore(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if( AnscEqualString(ParamName, "SNMPEnable", TRUE) )
    {
        /* collect value */
        if (CosaDmlDcGetSNMPEnable(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if( AnscEqualString(ParamName, "UserOpComponents", TRUE))
    {
        int n = 0;
        *pValue = '\0';
        if (pMyObject->UserOpComponents & 0x00000001) n += sprintf(pValue+n, "%s%s", n ? "," : "", "WebUI");
        if (pMyObject->UserOpComponents & 0x00000008) n += sprintf(pValue+n, "%s%s", n ? "," : "", "Cli");

        return 0;
    }

    if( AnscEqualString(ParamName, "UserChangedParams", TRUE))
    {
        return CosaDevCtrlReg_GetUserChangedParams(pMyObject, pValue, pulSize);
    }

    if( AnscEqualString(ParamName, "HostName", TRUE))
    {
        return CosaDmlDcGetWanHostName(NULL, pValue);
    }

    if( AnscEqualString(ParamName, "DomainName", TRUE))
    {
        return CosaDmlDcGetWanDomainName(NULL, pValue);
    }
    
    /*get wan static Domain name (readonly)*/    
    if( AnscEqualString(ParamName, "WanStaticDomainName", TRUE))
    {
        return CosaDmlDcGetWanStaticDomainName(NULL, pValue);
    }

    if( AnscEqualString(ParamName, "GuestPassword", TRUE))
    {
        return CosaDmlDcGetGuestPassword(NULL, pValue);
    }

    if( AnscEqualString(ParamName, "ParentalControlPassword", TRUE))
    {
        return CosaDmlDcGetParConPassword(NULL, pValue);
    }

    if( AnscEqualString(ParamName, "ParentalControlDefaultPwd", TRUE))
    {
        return CosaDmlDcGetDefaultParConPassword(NULL, pValue);
    }

    if( AnscEqualString(ParamName, "ParentalControlQuestion", TRUE))
    {
        return CosaDmlDcGetParConQuestion(NULL, pValue);
    }

    if( AnscEqualString(ParamName, "ParentalControlAnswer", TRUE))
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
    
    if (AnscEqualString(ParamName, "ErouterEnable", TRUE)) {
        
        retStatus = CosaDmlDcSetErouterEnabled(NULL, bValue);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;
     
        return TRUE;
    }

    if (AnscEqualString(ParamName, "WanSecondIPRipAdvertised", TRUE))
    {
        pMyObject->WanSecIPRIPAdv = bValue;

        retStatus = CosaDmlDcSetWanSecondIPRipAdvertised(NULL, pMyObject->WanSecIPRIPAdv);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "ResetDefaultEnable", TRUE))
    {
        pMyObject->ResetDefaultEnable = bValue;

        retStatus = CosaDmlDcSetResetDefaultEnable(NULL, pMyObject->ResetDefaultEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    if (AnscEqualString(ParamName, "ReinitCmMac", TRUE))
    {
        if(bValue == 1)
        {
           	// Call reinit mac function
       	   	if(CosaDmlDcSetReInitCmMac() == 0)
	   		{
           		return TRUE;
	   		}
	   		else
				return FALSE;
        }
        else if(bValue == 0)
        {
           /* Need not have to set and return true from here*/
           return TRUE;
        }
        else
           return FALSE;
    }


    if (AnscEqualString(ParamName, "IGMPProxyEnable", TRUE))
    {
        retStatus = CosaDmlDcSetIGMPProxyEnable(NULL, bValue);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "DNSProxyEnable", TRUE))
    {
        retStatus = CosaDmlDcSetDNSProxyEnable(NULL, bValue);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "TelnetEnable", TRUE))
    {
        pMyObject->TelnetEnable = bValue;

        retStatus = CosaDmlDcSetTelnetEnable(NULL, pMyObject->TelnetEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "SSHEnable", TRUE))
    {
        pMyObject->SSHEnable = bValue;

        retStatus = CosaDmlDcSetSSHEnable(NULL, pMyObject->SSHEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "HNAPEnable", TRUE))
    {
        pMyObject->HNAPEnable = bValue;

        retStatus = CosaDmlDcSetHNAPEnable(NULL, pMyObject->HNAPEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "EnableStaticNameServer", TRUE))
    {
        pMyObject->EnableStaticNameServer = bValue;

        retStatus = CosaDmlDcSetEnableStaticNameServer(NULL, pMyObject->EnableStaticNameServer);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "EnableZeroConfig", TRUE))
    {
        pMyObject->EnableZeroConfig = bValue;
        retStatus = CosaDmlDcSetEnableZeroConfig(NULL, pMyObject->EnableZeroConfig);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "ReleaseWan", TRUE))
    {
        retStatus = CosaDmlDcSetReleaseWan(NULL, pMyObject->ReleaseWan);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "RenewWan", TRUE))
    {
        retStatus = CosaDmlDcSetRenewWan(NULL, pMyObject->RenewWan);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "HTTPEnable", TRUE))
    {
        pMyObject->HTTPEnable = bValue;
        pMyObject->WebServerChanged = TRUE;

        return TRUE;
    }
 
    if (AnscEqualString(ParamName, "HTTPSEnable", TRUE))
    {
        pMyObject->HTTPSEnable = bValue;
        pMyObject->WebServerChanged = TRUE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "IGMPSnoopingEnable", TRUE))
    {
        pMyObject->IGMPSnoopingEnable = bValue;

        retStatus = CosaDmlDcSetIGMPSnoopingEnable(NULL, pMyObject->IGMPSnoopingEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "EnableMonitorUserChangedParams", TRUE))
    {
        pMyObject->EnableMonitorUserChangedParams = bValue;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "EnableMsoRemoteMgmt", TRUE))
    {
        pMyObject->EnableMsoRemoteMgmt = bValue;

        retStatus = CosaDmlDcSetMsoRemoteMgmtEnable(NULL, pMyObject->EnableMsoRemoteMgmt);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "EnableCusadminRemoteMgmt", TRUE))
    {
        pMyObject->EnableCusadminRemoteMgmt = bValue;

        retStatus = CosaDmlDcSetCusadminRemoteMgmtEnable(NULL, pMyObject->EnableCusadminRemoteMgmt);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }
    
    if (AnscEqualString(ParamName, "XHSEthernetPortEnable", TRUE))
    {
//         if (CosaDmlDcSetHSEthernetPortEnable(NULL, bValue) != ANSC_STATUS_SUCCESS)
//             return FALSE;
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
    if( AnscEqualString(ParamName, "MultiHomedHSDFlag", TRUE))
    {
        pMyObject->HsdFlag = uValue;
        pMyObject->Mode = uValue;

        retStatus = CosaDmlDcSetMultiHomedHSDFlag(NULL, pMyObject->HsdFlag);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "MultiHomedUIPageControl", TRUE))
    {
        pMyObject->UIPageControl = uValue;

        retStatus = CosaDmlDcSetMultiHomedUIPageControl(NULL, pMyObject->UIPageControl);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "WanAddressMode", TRUE))
    {
        pMyObject->WanAddrMode = uValue;

        retStatus = CosaDmlDcSetWanAddressMode(NULL, pMyObject->WanAddrMode);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }
    if (AnscEqualString(ParamName, "WanStaticIPAddress", TRUE))
    {
        pMyObject->WanIPAddr.Value = uValue;

        retStatus = CosaDmlDcSetWanStaticIPAddress(NULL, pMyObject->WanIPAddr.Value);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        ifWanRestart = 1;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "WanStaticSubnetMask", TRUE))
    {
        pMyObject->WanSubnetMask.Value = uValue;

        retStatus = CosaDmlDcSetWanStaticSubnetMask(NULL, pMyObject->WanSubnetMask.Value);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        ifWanRestart = 1;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "WanStaticGatewayIP", TRUE))
    {
        pMyObject->WanGateway.Value = uValue;

        retStatus = CosaDmlDcSetWanStaticGatewayIP(NULL, pMyObject->WanGateway.Value);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        ifWanRestart = 1;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "WanSecondIPAddress", TRUE))
    {
        pMyObject->WanSecIPAddr.Value = uValue;

        retStatus = CosaDmlDcSetWanSecondIPAddr(NULL, pMyObject->WanSecIPAddr.Value);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "WanBackupDefaultGateway", TRUE))
    {
        pMyObject->WanBackupGateway.Value = uValue;

        retStatus = CosaDmlDcSetWanBackupDefaultGateway(NULL, pMyObject->WanBackupGateway.Value);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "NameServer1", TRUE))
    {
        pMyObject->NameServer1.Value = uValue;

        retStatus = CosaDmlDcSetWanNameServer(NULL, pMyObject->NameServer1.Value, 1);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "NameServer2", TRUE))
    {
        pMyObject->NameServer2.Value = uValue;

        retStatus = CosaDmlDcSetWanNameServer(NULL, pMyObject->NameServer2.Value, 2);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "DeviceMode", TRUE))
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

    if (AnscEqualString(ParamName, "HTTPPort", TRUE))
    {
        if (IsPortInUse(uValue))
        {
            CcspTraceWarning(("Port already in use\n"));
            return FALSE;
        }
        pMyObject->HTTPPort = uValue;
        pMyObject->WebServerChanged = TRUE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "HTTPSPort", TRUE))
    {
        if (IsPortInUse(uValue))
        {
            CcspTraceWarning(("Port already in use\n"));
            return FALSE;
        }
        pMyObject->HTTPSPort = uValue;
        pMyObject->WebServerChanged = TRUE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "ReinitMacThreshold", TRUE))
    {
	retStatus = CosaDmlDcSetReinitMacThreshold(NULL, uValue);
	if (retStatus != ANSC_STATUS_SUCCESS)
        {    return FALSE;	}
	else
	{	pMyObject->ReinitMacThreshold = uValue;	}

        return TRUE;
    }
 
    if (AnscEqualString(ParamName, "WebUITimeout", TRUE))
    {
        pMyObject->WebUITimeout = uValue;

        retStatus = CosaDmlDcSetWebUITimeout(NULL, pMyObject->WebUITimeout);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "NoOfGuests", TRUE))
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

    if( AnscEqualString(ParamName, "RebootDevice", TRUE))
    {
        AnscCopyString(pMyObject->RebootDevice, pString);
        pMyObject->bResetChanged = 1;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "FactoryReset", TRUE))
    {
        AnscCopyString(pMyObject->FactoryReset, pString);
        pMyObject->bFactoryResetChanged = 1;
     
        return TRUE;
    }

   if( AnscEqualString(ParamName, "X_CrashPortalURL", TRUE))
    {
        int rc1=-1, rc2=-1, rc3=-1;
        rc1=IPv4_Url_Validate(pString);
        rc2=IPv6_Url_Validate(pString);
        rc3=Fqdn_Url_Validate(pString);
        if(rc1==0 || rc2==0 ||  rc3==0)
          AnscCopyString(pMyObject->X_CrashPortalURL, pString);
        else
           return FALSE;

        retStatus = CosaDmlDcSetX_CrashPortalURL(NULL, pMyObject->X_CrashPortalURL);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }


    if( AnscEqualString(ParamName, "UserChangedFlags", TRUE))
    {
        AnscCopyString(pMyObject->UserChangedFlags, pString);

        retStatus = CosaDmlDcSetUserChangedFlags(NULL, pMyObject->UserChangedFlags);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DeviceConfigIgnore", TRUE))
    {
        AnscCopyString(pMyObject->DeviceConfigIgnore, pString);

        retStatus = CosaDmlDcSetDeviceConfigIgnore(NULL, pMyObject->DeviceConfigIgnore);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SNMPEnable", TRUE))
    {
        AnscCopyString(pMyObject->SNMPEnable, pString);

        retStatus = CosaDmlDcSetSNMPEnable(NULL, pMyObject->SNMPEnable);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "UserOpComponents", TRUE))
    {
        pMyObject->UserOpComponents = 0;

        if (strstr(pString, "WebUI")) pMyObject->UserOpComponents |= 0x00000001;
        if (strstr(pString, "Cli")) pMyObject->UserOpComponents   |= 0x00000008;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "HostName", TRUE))
    {
        AnscCopyString(pMyObject->HostName, pString);

        retStatus = CosaDmlDcSetHostName(NULL, pMyObject->HostName);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DomainName", TRUE))
    {
        AnscCopyString(pMyObject->DomainName, pString);

        retStatus = CosaDmlDcSetDomainName(NULL, pMyObject->DomainName);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "GuestPassword", TRUE))
    {
        AnscCopyString(pMyObject->GuestPassword, pString);

        retStatus = CosaDmlDcSetGuestPassword(NULL, pMyObject->GuestPassword);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ParentalControlPassword", TRUE))
    {
        AnscCopyString(pMyObject->ParentalControlPassword, pString);

        retStatus = CosaDmlDcSetParConPassword(NULL, pMyObject->ParentalControlPassword);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ParentalControlQuestion", TRUE))
    {
        AnscCopyString(pMyObject->ParentalControlQuestion, pString);

        retStatus = CosaDmlDcSetParConQuestion(NULL, pMyObject->ParentalControlQuestion);
        if (retStatus != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ParentalControlAnswer", TRUE))
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
    ANSC_STATUS                        retStatus = ANSC_STATUS_SUCCESS;
    if(pMyObject->WebServerChanged == TRUE)
    {
        if(pMyObject->HTTPSPort == pMyObject->HTTPPort || \
                pMyObject->HTTPSPort == 0 || \
                pMyObject->HTTPPort == 0)
            return FALSE;
            
    }
       if (pMyObject->bResetChanged  == 1){
       retStatus =CosaDmlDcCheckRebootDeviceString(NULL, pMyObject->RebootDevice);
        if (retStatus != ANSC_STATUS_SUCCESS){
               CcspTraceInfo(("%s Entered string %s is not a valid one",__FUNCTION__,pMyObject->RebootDevice));
               return FALSE;
       }
     }
       if (pMyObject->bFactoryResetChanged = 1){
       retStatus =CosaDmlDcCheckFactoryResetString(NULL, pMyObject->FactoryReset);
        if (retStatus != ANSC_STATUS_SUCCESS){
               CcspTraceInfo(("%s Entered string %s is not a valid one", __FUNCTION__, pMyObject->FactoryReset));
               return FALSE;
       }
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
        if (CosaDmlDcSetHSEthernetPortEnable(NULL, pMyObject->bXHSPortEnabled) != ANSC_STATUS_SUCCESS)
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
    CosaDmlDcGetHTTPPort(NULL, &pMyObject->HTTPPort);
    CosaDmlDcGetHTTPSPort(NULL, &pMyObject->HTTPSPort);

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

    if (AnscEqualString(ParamName, "LanDhcpServer", TRUE))
    {
        *pBool = pLanMngm->LanDhcpServer;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanNaptEnable", TRUE))
    {

        *pBool = pLanMngm->LanNaptEnable;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanDhcp125", TRUE))
    {
        *pBool = pLanMngm->LanDhcp125;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanHnap", TRUE))
    {
        *pBool = pLanMngm->LanHnap;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanUpnp", TRUE))
    {
        *pBool = pLanMngm->LanUpnp;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "LanNaptOnDhcp", TRUE))
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

    if (AnscEqualString(ParamName, "LanMode", TRUE))
    {
        *pUlong = pLanMngm->LanMode;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanNetwork", TRUE))
    {
        *pUlong = pLanMngm->LanNetwork.Value;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanNetworksAllow", TRUE))
    {
        *pUlong = pLanMngm->LanNetworksAllow;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanSubnetMask", TRUE))
    {
        *pUlong = pLanMngm->LanSubnetMask.Value;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanIPAddress", TRUE))
    {
        *pUlong = pLanMngm->LanIPAddress.Value;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanTos", TRUE))
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


    if (AnscEqualString(ParamName, "LanDhcpServer", TRUE))
    {
        pLanMngm->LanDhcpServer = bValue;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanNaptEnable", TRUE))
    {
        pLanMngm->LanNaptEnable = bValue;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanDhcp125", TRUE))
    {
        pLanMngm->LanDhcp125 = bValue;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanHnap", TRUE))
    {
        pLanMngm->LanHnap = bValue;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanUpnp", TRUE))
    {
        pLanMngm->LanUpnp = bValue;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanNaptOnDhcp", TRUE))
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

    if (AnscEqualString(ParamName, "LanMode", TRUE))
    {
	//RDKB-27656 : Bridge Mode must not set to true using WEBPA & dmcli in ETHWAN mode
        char buf[16] = {0};
        if (syscfg_get(NULL, "eth_wan_enabled", buf, sizeof(buf)) == 0)
        {
            if ( (buf != NULL) && (strcmp(buf,"true") == 0 )  )
            {
		CcspTraceWarning(("Bridge Mode not supported in Ethernet WAN mode\n"));
		return FALSE;
            }
        }
        if(CosaGetParamValueBool("Device.X_RDKCENTRAL-COM_VideoService.Enabled") && uValuepUlong != 3)
        {
            CcspTraceWarning(("LanMode setting to Bridge is not supported when VideoService is ENABLED\n"));
            return FALSE;
        }
	
        /*if((uValuepUlong == 2) && is_mesh_enabled())
        {
            CcspTraceWarning(("BRIDGE_ERROR:Fail to enable Bridge mode when Mesh is on\n"));
            return FALSE;
        }*/

        pLanMngm->LanMode = uValuepUlong;
        CcspTraceWarning(("RDKB_LAN_CONFIG_CHANGED: Setting new LanMode value (bridge-dhcp(1),bridge-static(2),router(3),full-bridge-static(4)) as (%d)...\n",
        		uValuepUlong ));	
        return TRUE;
    }

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
       (TRUE == bridgeMode))
        return FALSE;

    if (AnscEqualString(ParamName, "LanNetwork", TRUE))
    {
        pLanMngm->LanNetwork.Value = uValuepUlong;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanNetworksAllow", TRUE))
    {
        pLanMngm->LanNetworksAllow = uValuepUlong;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanSubnetMask", TRUE))
    {
		CcspTraceWarning(("RDKB_LAN_CONFIG_CHANGED: Setting new LanSubnetMask value  ...\n"));
        pLanMngm->LanSubnetMask.Value = uValuepUlong;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanIPAddress", TRUE))
    {
		CcspTraceWarning(("RDKB_LAN_CONFIG_CHANGED: Setting new LanIPAddress value  ...\n"));
        pLanMngm->LanIPAddress.Value = uValuepUlong;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LanTos", TRUE))
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
    /* 255.255.255.0, 255.255.0.0, 255.0.0.0, 255.255.255.128, 255.255.255.252, 255.240.0.0*/
#if defined(_XB6_PRODUCT_REQ_)  || defined(_PLATFORM_RASPBERRYPI_) || defined (_HUB4_PRODUCT_REQ_) || defined(_PLATFORM_TURRIS_)
    if(pLanMngm->LanSubnetMask.Value != 0x00FFFFFF &&
       pLanMngm->LanSubnetMask.Value != 0x0000FFFF &&
       pLanMngm->LanSubnetMask.Value != 0x000000FF &&  
       pLanMngm->LanSubnetMask.Value != 0x80FFFFFF && 
       pLanMngm->LanSubnetMask.Value != 0xFCFFFFFF &&
       pLanMngm->LanSubnetMask.Value != 0x0000F0FF )
#elif defined(_BCI_FEATURE_REQ)
#ifdef _CBR_PRODUCT_REQ_
    if(pLanMngm->LanSubnetMask.Value != 0x000000FF &&   //8
       pLanMngm->LanSubnetMask.Value != 0x000080FF &&   //9
       pLanMngm->LanSubnetMask.Value != 0x0000C0FF &&   //10
       pLanMngm->LanSubnetMask.Value != 0x0000E0FF &&     //11
       pLanMngm->LanSubnetMask.Value != 0x0000F0FF &&     //12
       pLanMngm->LanSubnetMask.Value != 0x0000F8FF &&   //13
       pLanMngm->LanSubnetMask.Value != 0x0000FCFF &&   //14
       pLanMngm->LanSubnetMask.Value != 0x0000FEFF &&   //15
       pLanMngm->LanSubnetMask.Value != 0x0000FFFF &&   //16
       pLanMngm->LanSubnetMask.Value != 0x0080FFFF &&   //17
       pLanMngm->LanSubnetMask.Value != 0x00C0FFFF &&   //18
       pLanMngm->LanSubnetMask.Value != 0x00E0FFFF &&   //19
       pLanMngm->LanSubnetMask.Value != 0x00F0FFFF &&   //20
       pLanMngm->LanSubnetMask.Value != 0x00F8FFFF &&   //21
       pLanMngm->LanSubnetMask.Value != 0x00FCFFFF &&   //22
       pLanMngm->LanSubnetMask.Value != 0x00FEFFFF &&   //23
       pLanMngm->LanSubnetMask.Value != 0x00FFFFFF &&   //24
       pLanMngm->LanSubnetMask.Value != 0x80FFFFFF &&   //25
       pLanMngm->LanSubnetMask.Value != 0xC0FFFFFF &&   //26
       pLanMngm->LanSubnetMask.Value != 0xE0FFFFFF &&   //27
       pLanMngm->LanSubnetMask.Value != 0xF0FFFFFF &&   //28
       pLanMngm->LanSubnetMask.Value != 0xF8FFFFFF )   //29
#else
    if(pLanMngm->LanSubnetMask.Value != 0xFF000000 &&   //8
       pLanMngm->LanSubnetMask.Value != 0xFF800000 &&   //9
       pLanMngm->LanSubnetMask.Value != 0xFFC00000 &&   //10
       pLanMngm->LanSubnetMask.Value != 0xFFE00000 &&     //11
       pLanMngm->LanSubnetMask.Value != 0xFFF00000 &&     //12
       pLanMngm->LanSubnetMask.Value != 0xFFF80000 &&   //13
       pLanMngm->LanSubnetMask.Value != 0xFFFC0000 &&   //14
       pLanMngm->LanSubnetMask.Value != 0xFFFE0000 &&   //15
       pLanMngm->LanSubnetMask.Value != 0xFFFF0000 &&   //16
       pLanMngm->LanSubnetMask.Value != 0xFFFF8000 &&   //17
       pLanMngm->LanSubnetMask.Value != 0xFFFFC000 &&   //18
       pLanMngm->LanSubnetMask.Value != 0xFFFFE000 &&   //19
       pLanMngm->LanSubnetMask.Value != 0xFFFFF000 &&   //20
       pLanMngm->LanSubnetMask.Value != 0xFFFFF800 &&   //21
       pLanMngm->LanSubnetMask.Value != 0xFFFFFC00 &&   //22
       pLanMngm->LanSubnetMask.Value != 0xFFFFFE00 &&   //23
       pLanMngm->LanSubnetMask.Value != 0xFFFFFF00 &&   //24
       pLanMngm->LanSubnetMask.Value != 0xFFFFFF80 &&   //25
       pLanMngm->LanSubnetMask.Value != 0xFFFFFFC0 &&   //26
       pLanMngm->LanSubnetMask.Value != 0xFFFFFFE0 &&   //27
       pLanMngm->LanSubnetMask.Value != 0xFFFFFFF0 &&   //28
       pLanMngm->LanSubnetMask.Value != 0xFFFFFFF8 &&   //29
       pLanMngm->LanSubnetMask.Value != 0xFFFFFFFC &&   //30
       pLanMngm->LanSubnetMask.Value != 0xFFFFFFFE )   //31
#endif     
#else
    if(pLanMngm->LanSubnetMask.Value != 0xFFFFFF00 &&
       pLanMngm->LanSubnetMask.Value != 0xFFFF0000 &&
       pLanMngm->LanSubnetMask.Value != 0xFF000000 &&
       pLanMngm->LanSubnetMask.Value != 0xFFFFFF80 &&
       pLanMngm->LanSubnetMask.Value != 0xFFFFFFFC )
#endif
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
        if (pLanMngm->LanSubnetMask.Value != 0x00FFFFFF &&
            pLanMngm->LanSubnetMask.Value != 0x0000FFFF &&
            pLanMngm->LanSubnetMask.Value != 0x80FFFFFF &&
            pLanMngm->LanSubnetMask.Value != 0xFCFFFFFF &&
            pLanMngm->LanSubnetMask.Value != 0x0000F0FF )
        {
            CcspTraceWarning(("RDKB_LAN_CONFIG_CHANGED: Invalid Lan IP and subnet mask combination for a private network, reverting back to old value  ...\n"));
            goto RET_ERR;
        }
        return TRUE;
    }
#if defined(_XB6_PRODUCT_REQ_) || defined (_CBR_PRODUCT_REQ_) || defined(_PLATFORM_RASPBERRYPI_) || defined (_HUB4_PRODUCT_REQ_) || defined(_PLATFORM_TURRIS_)
    else if((pLanMngm->LanIPAddress.Value & 0x0000FFFF) == 0x0000A8C0)
#else
   else if((pLanMngm->LanIPAddress.Value & 0xFFFF0000) == 0xC0A80000)
#endif
   {
        if (pLanMngm->LanSubnetMask.Value != 0x00FFFFFF &&
            pLanMngm->LanSubnetMask.Value != 0x0000FFFF &&
            pLanMngm->LanSubnetMask.Value != 0x80FFFFFF &&
            pLanMngm->LanSubnetMask.Value != 0xFCFFFFFF )
        {
            CcspTraceWarning(("RDKB_LAN_CONFIG_CHANGED: Invalid Lan IP and subnet mask combination for a private network, reverting back to old value  ...\n"));
            goto RET_ERR;
        }
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
#if !defined(_COSA_BCM_MIPS_) && !defined(_ENABLE_DSL_SUPPORT_) 
        else {

			char ip[64]={0};
			char sub[64]={0};
			unsigned int iip=pLanMngm->LanIPAddress.Value;
			unsigned int isub=pLanMngm->LanSubnetMask.Value;
			snprintf(ip, sizeof(ip), "%d.%d.%d.%d", (iip >> 24) & 0xFF, (iip >> 16) & 0xFF, (iip >> 8) & 0xFF, 254);
			snprintf(sub, sizeof(sub), "%d.%d.%d.%d", (isub >> 24) & 0xFF, (isub >> 16) & 0xFF, (isub >> 8) & 0xFF, (isub ) & 0xFF);
CcspTraceWarning( ("--------- LanMngm_Commit CosaDmlDcResetBr0 %s,%s>>\n", ip, sub));				
			CosaDmlDcResetBr0(ip, sub);
CcspTraceWarning( ("--------- LanMngm_Commit CosaDmlDcResetBr0 <<\n"));		
		}
#endif
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
	
    if (AnscEqualString(ParamName, "HomeUser_Lan_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 1, 1, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "HomeUser_RfCM_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 1, 2, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "HomeUser_Mta_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 1, 16, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "HomeUser_WanRG_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 1, 40, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "CusAdmin_Lan_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 5, 1, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "CusAdmin_RfCM_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 5, 2, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "CusAdmin_Mta_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 5, 16, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "CusAdmin_WanRG_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 5, 40, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "AdvUser_Lan_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 10, 1, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "AdvUser_RfCM_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 10, 2, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "AdvUser_Mta_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 10, 16, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "AdvUser_WanRG_Level", TRUE))
    {
        if (CosaDmlDcGetWebAccessLevel(NULL, 10, 40, pInt) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }
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
	
    if (AnscEqualString(ParamName, "HomeUser_Lan_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 1, 1, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "HomeUser_RfCM_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 1, 2, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "HomeUser_Mta_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 1, 16, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "HomeUser_WanRG_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 1, 40, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "CusAdmin_Lan_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 5, 1, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "CusAdmin_RfCM_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 5, 2, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "CusAdmin_Mta_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 5, 16, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "CusAdmin_WanRG_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 5, 40, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "AdvUser_Lan_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 10, 1, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "AdvUser_RfCM_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 10, 2, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "AdvUser_Mta_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 10, 16, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

	if (AnscEqualString(ParamName, "AdvUser_WanRG_Level", TRUE))
    {
        if (CosaDmlDcSetWebAccessLevel(NULL, 10, 40, iValue) != ANSC_STATUS_SUCCESS)
            return FALSE;

        return TRUE;
    }

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

BOOL
WebPA_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       bValue
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    PCOSA_DML_WEBPA_CFG2 pWebPACfg = &pMyObject->WebPAConfig;
  if( AnscEqualString(ParamName, "Enable", TRUE))
     {
        *bValue=pWebPACfg->Enable;
        return TRUE;
     }
  return FALSE;
}

ULONG
WebPA_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pUlSize
 )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    PCOSA_DML_WEBPA_CFG2 pWebPACfg = &pMyObject->WebPAConfig;
  if( AnscEqualString(ParamName, "ServerURL", TRUE))
    {
       AnscCopyString(pValue,pWebPACfg->ServerURL);
       return 0;
    }
  return -1;
}

BOOL
WebPA_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    PCOSA_DML_WEBPA_CFG2 pWebPACfg = &pMyObject->WebPAConfig;
    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
       pWebPACfg->Enable = bValue;
        return TRUE;
    }
  return FALSE;
}

BOOL
WebPA_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
  PCOSA_DATAMODEL_DEVICECONTROL   pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
  PCOSA_DML_WEBPA_CFG2 pWebPACfg = &pMyObject->WebPAConfig;
  if (AnscEqualString(ParamName, "ServerURL", TRUE))
    {
       int rc=-1,rc2=-1,rc3=-1;
       rc=IPv4_Url_Validate(pString);
       rc2=IPv6_Url_Validate(pString);
       rc3=Fqdn_Url_Validate(pString);
       if(rc==0 || rc2==0 || rc3==0)
        {
          AnscCopyString(pWebPACfg->ServerURL,pString);
          return TRUE;
        }
       else
        {
         return FALSE;
        }
    }
  return FALSE;

}

BOOL
WebPA_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
WebPA_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL        pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    return CosaDmlWebPASetConfig2(NULL, &pMyObject->WebPAConfig);
}

