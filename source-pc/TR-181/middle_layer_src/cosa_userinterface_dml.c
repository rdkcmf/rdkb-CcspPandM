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

    module: cosa_userinterface_dml.c

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

#include "cosa_userinterface_dml.h"

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

    UserInterface.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    UserInterface.UserInterface.

    *  UserInterface_GetParamBoolValue
    *  UserInterface_SetParamBoolValue
    *  UserInterface_GetParamUlongValue
    *  UserInterface_SetParamUlongValue
    *  UserInterface_Validate
    *  UserInterface_Commit

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UserInterface_GetParamBoolValue
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
UserInterface_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;
	if (strcmp(ParamName, "PasswordReset") == 0)
    {
        *pBool = FALSE;
        return TRUE;
    }

     if (strcmp(ParamName, "PasswordLockoutEnable") == 0)
    {
        *pBool = pMyObject->UserInterfaceCfg.bPasswordLockoutEnable;
        return TRUE;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
void ResetPassword()
{
	pthread_detach(pthread_self());
	system("dmcli simu setv Device.Users.User.3.X_CISCO_COM_Password string password");
}

BOOL
UserInterface_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;
	if (strcmp(ParamName, "PasswordReset") == 0)
    {
        if(bValue == TRUE)
		{
			pthread_t PwdRst;
			pthread_create(&PwdRst, NULL, &ResetPassword, NULL);
		}
        return TRUE;
    }

    if (strcmp(ParamName, "PasswordLockoutEnable") == 0)
    {
        pMyObject->UserInterfaceCfg.bPasswordLockoutEnable = bValue;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UserInterface_GetParamUlongValue
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
UserInterface_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;

    /* CosaDmlRaGetCfg(NULL, &pMyObject->RaCfg); */

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "PasswordLockoutAttempts") == 0)
    {
        *puLong = pMyObject->UserInterfaceCfg.PasswordLockoutAttempts;
        return TRUE;
    }

    if (strcmp(ParamName, "PasswordLockoutTime") == 0)
    {
        *puLong = pMyObject->UserInterfaceCfg.PasswordLockoutTime;
        return TRUE;
    }
  return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UserInterface_SetParamUlongValue
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
UserInterface_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;

    if (strcmp(ParamName, "PasswordLockoutAttempts") == 0)
    {
        pMyObject->UserInterfaceCfg.PasswordLockoutAttempts = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "PasswordLockoutTime") == 0)
    {
        pMyObject->UserInterfaceCfg.PasswordLockoutTime = uValue;
        return TRUE;
    }
 return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UserInterface_Validate
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
UserInterface_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UserInterface_Commit
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
UserInterface_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;

    CosaDmlUserInterfaceSetCfg(NULL, &pMyObject->UserInterfaceCfg);

    return 0;
}

/***********************************************************************

 APIs for Object:

    UserInterface.RemoteAccess.

    *  RemoteAccess_GetParamBoolValue
    *  RemoteAccess_GetParamIntValue
    *  RemoteAccess_GetParamUlongValue
    *  RemoteAccess_GetParamStringValue
    *  RemoteAccess_SetParamBoolValue
    *  RemoteAccess_SetParamIntValue
    *  RemoteAccess_SetParamUlongValue
    *  RemoteAccess_SetParamStringValue
    *  RemoteAccess_Validate
    *  RemoteAccess_Commit
    *  RemoteAccess_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RemoteAccess_GetParamBoolValue
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
RemoteAccess_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;

    /* CosaDmlRaGetCfg(NULL, &pMyObject->RaCfg); */

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pMyObject->RaCfg.bEnabled;
        return TRUE;
    }

    if (strcmp(ParamName, "UpgradePermission") == 0)
    {
        *pBool = pMyObject->RaCfg.UpgradePermission;
        return TRUE;
    }

    if (strcmp(ParamName, "FromAnyIP") == 0)
    {
        *pBool = pMyObject->RaCfg.bFromAnyIp;
        return TRUE;
    }

    if (strcmp(ParamName, "HttpEnable") == 0)
    {
        *pBool = pMyObject->RaCfg.HttpEnable;
        return TRUE;
    }

    if (strcmp(ParamName, "HttpsEnable") == 0)
    {
        *pBool = pMyObject->RaCfg.HttpsEnable;
        return TRUE;
    }

    if (strcmp(ParamName, "TelnetEnable") == 0)
    {
        *pBool = pMyObject->RaCfg.TelnetEnable;
        return TRUE;
    }

    if (strcmp(ParamName, "SSHEnable") == 0)
    {
        *pBool = pMyObject->RaCfg.SSHEnable;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RemoteAccess_GetParamIntValue
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
RemoteAccess_GetParamIntValue
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
        RemoteAccess_GetParamUlongValue
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
RemoteAccess_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
	
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;

    /* CosaDmlRaGetCfg(NULL, &pMyObject->RaCfg); */

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "StartIp") == 0)
    {
        *puLong = pMyObject->RaCfg.StartIp.Value;
        return TRUE;
    }

    if (strcmp(ParamName, "EndIp") == 0)
    {
        *puLong = pMyObject->RaCfg.EndIp.Value;
        return TRUE;
    }

    if (strcmp(ParamName, "HttpPort") == 0)
    {
        *puLong = pMyObject->RaCfg.HttpPort;
        return TRUE;
    }

    if (strcmp(ParamName, "HttpsPort") == 0)
    {
        *puLong = pMyObject->RaCfg.HttpsPort;
        return TRUE;
    }

    if (strcmp(ParamName, "TelnetPort") == 0)
    {
        *puLong = pMyObject->RaCfg.TelnetPort;
        return TRUE;
    }

    if (strcmp(ParamName, "SSHPort") == 0)
    {
        *puLong = pMyObject->RaCfg.SSHPort;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RemoteAccess_GetParamStringValue
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
RemoteAccess_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;

    if (strcmp(ParamName, "SupportedProtocols") == 0)
    {
        AnscCopyString(pValue, pMyObject->RaCfg.SupportedProtocols);
        return 0;
    }

    if (strcmp(ParamName, "StartIpV6") == 0)
    {
        AnscCopyString(pValue, pMyObject->RaCfg.StartIpV6);
        return 0;
    }

    if (strcmp(ParamName, "EndIpV6") == 0)
    {
        AnscCopyString(pValue, pMyObject->RaCfg.EndIpV6);
        return 0;
    }
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RemoteAccess_SetParamBoolValue
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
RemoteAccess_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{


  
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;



    if (strcmp(ParamName, "Enable") == 0)
    {
        pMyObject->RaCfg.bEnabled = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "UpgradePermission") == 0)
    {
        pMyObject->RaCfg.UpgradePermission = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "FromAnyIP") == 0)
    {
        pMyObject->RaCfg.bFromAnyIp = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "HttpEnable") == 0)
    {
        pMyObject->RaCfg.HttpEnable = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "HttpsEnable") == 0)
    {
        pMyObject->RaCfg.HttpsEnable = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "TelnetEnable") == 0)
    {
        pMyObject->RaCfg.TelnetEnable = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "SSHEnable") == 0)
    {
        pMyObject->RaCfg.SSHEnable = bValue;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RemoteAccess_SetParamIntValue
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
RemoteAccess_SetParamIntValue
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
        RemoteAccess_SetParamUlongValue
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
RemoteAccess_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;
	

    if (strcmp(ParamName, "StartIp") == 0)
    {
        pMyObject->RaCfg.StartIp.Value = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "EndIp") == 0)
    {
        pMyObject->RaCfg.EndIp.Value = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "HttpPort") == 0)
    {
        pMyObject->RaCfg.HttpPort = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "HttpsPort") == 0)
    {
        pMyObject->RaCfg.HttpsPort = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "TelnetPort") == 0)
    {
        pMyObject->RaCfg.TelnetPort = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "SSHPort") == 0)
    {
        pMyObject->RaCfg.SSHPort = uValue;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RemoteAccess_SetParamStringValue
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
RemoteAccess_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    /* check the parameter name and set the corresponding value */
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    if (strcmp(ParamName, "StartIpV6") == 0)
    {
        AnscCopyString(pMyObject->RaCfg.StartIpV6, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "EndIpV6") == 0)
    {
        AnscCopyString(pMyObject->RaCfg.EndIpV6, pString);
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RemoteAccess_Validate
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
RemoteAccess_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RemoteAccess_Commit
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
RemoteAccess_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;

    CosaDmlRaSetCfg(NULL, &pMyObject->RaCfg);
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RemoteAccess_Rollback
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
RemoteAccess_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;

    CosaDmlRaGetCfg(NULL, &pMyObject->RaCfg);
    
    return 0;
}

/* Beautiful cut off */

ULONG
iprange_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;
    
    return AnscSListQueryDepth(&pMyObject->iprangeList);
}

ANSC_HANDLE
iprange_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject    = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;
    PSLIST_HEADER                   piprangeHead  = (PSLIST_HEADER)&pMyObject->iprangeList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pLink        = (PSINGLE_LINK_ENTRY       )NULL;

    pLink = AnscSListGetEntryByIndex(piprangeHead, nIndex);

    if ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pCosaContext->InstanceNumber;
    }
    
    return pCosaContext;
}

ANSC_HANDLE
iprange_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject               = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->iprangeList;
    PCOSA_DML_UI_IPRANGE_ENTRY      pEntry                  = (PCOSA_DML_UI_IPRANGE_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY       )NULL;

    pEntry = (PCOSA_DML_UI_IPRANGE_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_UI_IPRANGE_ENTRY));
    if (!pEntry)
    {
        return NULL;
    }

    _ansc_sprintf(pEntry->Alias, "iprange%d", pMyObject->uliprangeNextInsNum);

    /* Update the cache */
    pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pCosaContext)
    {
        AnscFreeMemory(pEntry);

        return NULL;
    }

    pCosaContext->InstanceNumber = pEntry->InstanceNumber = pMyObject->uliprangeNextInsNum;

    pMyObject->uliprangeNextInsNum++;

    if ( pMyObject->uliprangeNextInsNum == 0 )
    {
        pMyObject->uliprangeNextInsNum = 1;
    }

    pCosaContext->hContext        = (ANSC_HANDLE)pEntry;
    pCosaContext->hParentTable    = NULL;
    pCosaContext->bNew            = TRUE;

    CosaSListPushEntryByInsNum(pListHead, pCosaContext);

    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext;
}

ULONG
iprange_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject               = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_UI_IPRANGE_ENTRY      pEntry                  = (PCOSA_DML_UI_IPRANGE_ENTRY)pCosaContext->hContext;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER)&pMyObject->iprangeList;

    if ( !pCosaContext->bNew )
    {
        if ( ANSC_STATUS_SUCCESS != CosaDmlUIIPRangeDelEntry(NULL, pEntry) )
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    AnscSListPopEntryByLink(pListHead, &pCosaContext->Linkage);

    AnscFreeMemory(pEntry);
    AnscFreeMemory(pCosaContext);

    return ANSC_STATUS_SUCCESS;
}

ULONG
iprange_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_UI_IPRANGE_ENTRY      piprange      = (PCOSA_DML_UI_IPRANGE_ENTRY)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "StartIP") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, piprange->StartIP);
        return 0;
    }

    if (strcmp(ParamName, "EndIP") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, piprange->EndIP);
        return 0;
    }

    if (strcmp(ParamName, "Desp") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, piprange->Desp);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

BOOL
iprange_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_UI_IPRANGE_ENTRY      piprange      = (PCOSA_DML_UI_IPRANGE_ENTRY)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "StartIP") == 0)
    {
        /* save update to backup */
         AnscCopyString(piprange->StartIP, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "EndIP") == 0)
    {
        /* save update to backup */
         AnscCopyString(piprange->EndIP, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "Desp") == 0)
    {
        /* save update to backup */
         AnscCopyString(piprange->Desp, pString);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
iprange_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
iprange_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject    = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_UI_IPRANGE_ENTRY      piprange      = (PCOSA_DML_UI_IPRANGE_ENTRY)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlUIIPRangeAddEntry(NULL, piprange);
    }
    else
    {
        CosaDmlUIIPRangeSetEntry(NULL, piprange);
    }

    return 0;

}

ULONG
iprange_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{   
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject    = (PCOSA_DATAMODEL_USERINTERFACE)g_pCosaBEManager->hUserinterface; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_UI_IPRANGE_ENTRY      piprange      = (PCOSA_DML_UI_IPRANGE_ENTRY)pCosaContext->hContext;

    CosaDmlUIIPRangeGetCfg(NULL, piprange);

    return 0; 
}
