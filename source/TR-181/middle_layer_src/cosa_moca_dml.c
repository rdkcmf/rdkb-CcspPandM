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

    module: cosa_moca_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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

#include "cosa_moca_dml.h"
#include "dml_tr181_custom_cfg.h"

#ifdef CONFIG_SYSTEM_MOCA

#if 0
#ifdef AnscTraceWarning
#undef AnscTraceWarning
#define AnscTraceWarning(a) printf("%s:%d> ", __FUNCTION__, __LINE__); printf a
#endif
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

    MoCA.

    *  MoCA_GetParamBoolValue
    *  MoCA_GetParamIntValue
    *  MoCA_GetParamUlongValue
    *  MoCA_GetParamStringValue
    *  MoCA_SetParamBoolValue
    *  MoCA_SetParamIntValue
    *  MoCA_SetParamUlongValue
    *  MoCA_SetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MoCA_GetParamBoolValue
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
MoCA_GetParamBoolValue
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
        MoCA_GetParamIntValue
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
MoCA_GetParamIntValue
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
        MoCA_GetParamUlongValue
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
MoCA_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject     = (PCOSA_DATAMODEL_MOCA    )g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_CFG              pCfg          = &pMyObject->MoCACfg;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "X_CISCO_COM_ProvisioningServerAddressType", TRUE))
    {
        /* collect value */
        *puLong = pCfg->X_CISCO_COM_ProvisioningServerAddressType;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        MoCA_GetParamStringValue
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
MoCA_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject     = (PCOSA_DATAMODEL_MOCA    )g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_CFG              pCfg          = &pMyObject->MoCACfg;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "X_CISCO_COM_ProvisioningFilename", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, (char *)pCfg->X_CISCO_COM_ProvisioningFilename);
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_ProvisioningServerAddress", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, (char *)pCfg->X_CISCO_COM_ProvisioningServerAddress);
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MoCA_SetParamBoolValue
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
MoCA_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;

    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MoCA_SetParamIntValue
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
MoCA_SetParamIntValue
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
        MoCA_SetParamUlongValue
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
MoCA_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{    
    PCOSA_DATAMODEL_MOCA            pMyObject     = (PCOSA_DATAMODEL_MOCA    )g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_CFG              pCfg          = &pMyObject->MoCACfg;
    
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "X_CISCO_COM_ProvisioningServerAddressType", TRUE))
    {
        /* save update to backup */
        pCfg->X_CISCO_COM_ProvisioningServerAddressType = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MoCA_SetParamStringValue
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
MoCA_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject     = (PCOSA_DATAMODEL_MOCA    )g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_CFG              pCfg          = &pMyObject->MoCACfg;
    
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "X_CISCO_COM_ProvisioningFilename", TRUE))
    {
        /* save update to backup */
        AnscCopyString((PCHAR)pCfg->X_CISCO_COM_ProvisioningFilename, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_ProvisioningServerAddress", TRUE))
    {
        /* save update to backup */
        AnscCopyString((PCHAR)pCfg->X_CISCO_COM_ProvisioningServerAddress, pString);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MoCA_Validate
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
MoCA_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject               = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;

    /* We should add some validation code here*/

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        MoCA_Commit
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
MoCA_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject     = (PCOSA_DATAMODEL_MOCA    )g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_CFG              pCfg          = &pMyObject->MoCACfg;
    ANSC_STATUS                     ReturnStatus  = ANSC_STATUS_SUCCESS;

    ReturnStatus = CosaDmlMocaSetCfg((ANSC_HANDLE)NULL, pCfg);
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        MoCA_Rollback
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
MoCA_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject     = (PCOSA_DATAMODEL_MOCA    )g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_CFG              pCfg          = &pMyObject->MoCACfg;

    CosaDmlMocaGetCfg((ANSC_HANDLE)NULL, pCfg);
    
    return 0;
}

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.

    *  Interface1_GetEntryCount
    *  Interface1_GetEntry
    *  Interface1_GetParamBoolValue
    *  Interface1_GetParamIntValue
    *  Interface1_GetParamUlongValue
    *  Interface1_GetParamStringValue
    *  Interface1_SetParamBoolValue
    *  Interface1_SetParamIntValue
    *  Interface1_SetParamUlongValue
    *  Interface1_SetParamStringValue
    *  Interface1_Validate
    *  Interface1_Commit
    *  Interface1_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface1_GetEntryCount
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
Interface1_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject     = (PCOSA_DATAMODEL_MOCA    )g_pCosaBEManager->hMoCA;
    
    return CosaDmlMocaGetNumberOfIfs((ANSC_HANDLE)NULL);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Interface1_GetEntry
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
Interface1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject     = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;

    *pInsNumber  = pMyObject->MoCAIfFullTable[nIndex].MoCAIfFull.Cfg.InstanceNumber;

    return &pMyObject->MoCAIfFullTable[nIndex];
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface1_GetParamBoolValue
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
Interface1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    PCOSA_DATAMODEL_MOCA            pMyObject   = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;

    AnscTraceWarning(("ParamName: %s\n", ParamName));
    CosaDmlMocaIfGetDinfo(NULL, pMoCAIfFull->Cfg.InstanceNumber-1, &pMoCAIfFull->DynamicInfo);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool = pMoCAIfFull->Cfg.bEnabled;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Upstream", TRUE))
    {
        /* collect value */
        *pBool = FALSE ; /*For an Internet Gateway Device, Upstream will be false for all LAN interfaces*/
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxNodes", TRUE))
    {
        /* collect value */
        
        *pBool = pMoCAIfFull->DynamicInfo.MaxNodes;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PreferredNC", TRUE))
    {
        /* collect value */
        *pBool = pMoCAIfFull->Cfg.bPreferredNC;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PrivacyEnabledSetting", TRUE))
    {
        /* collect value */
        *pBool = pMoCAIfFull->Cfg.PrivacyEnabledSetting;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PrivacyEnabled", TRUE))
    {
        /* collect value */
        
        *pBool = pMoCAIfFull->DynamicInfo.PrivacyEnabled;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "QAM256Capable", TRUE))
    {
        /* collect value */
        *pBool = pMoCAIfFull->StaticInfo.QAM256Capable;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_Reset", TRUE))
    {
        /* collect value */
        *pBool = pMoCAIfFull->Cfg.X_CISCO_COM_Reset;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_MixedMode", TRUE))
    {
        /* collect value */
        *pBool = pMoCAIfFull->Cfg.X_CISCO_COM_MixedMode;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_ChannelScanning", TRUE))
    {
        /* collect value */
        *pBool = pMoCAIfFull->Cfg.X_CISCO_COM_ChannelScanning;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_AutoPowerControlEnable", TRUE))
    {
        /* collect value */
        *pBool = pMoCAIfFull->Cfg.X_CISCO_COM_AutoPowerControlEnable;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_EnableTabooBit", TRUE))
    {
        /* collect value */
        *pBool = pMoCAIfFull->Cfg.X_CISCO_COM_EnableTabooBit;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_CycleMaster", TRUE))
    {
        /* collect value */
        *pBool = pMoCAIfFull->StaticInfo.X_CISCO_COM_CycleMaster;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface1_GetParamIntValue
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
Interface1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;

    /* check the parameter name and return the corresponding value */

    AnscTraceWarning(("ParamName: %s: pMoCAIfFull->Cfg.TxPowerLimit: %d\n", ParamName, pMoCAIfFull->Cfg.TxPowerLimit));

    if( AnscEqualString(ParamName, "TxPowerLimit", TRUE))
    {
        /* collect value */
        *pInt = pMoCAIfFull->Cfg.TxPowerLimit;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface1_GetParamUlongValue
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
Interface1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{    
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    PCOSA_DATAMODEL_MOCA            pMyObject   = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;

    AnscTraceWarning(("ParamName: %s\n", ParamName));
    CosaDmlMocaIfGetDinfo(NULL, pMoCAIfFull->Cfg.InstanceNumber-1, &pMoCAIfFull->DynamicInfo);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->DynamicInfo.Status;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "LastChange", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->DynamicInfo.LastChange;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxBitRate", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->StaticInfo.MaxBitRate;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxIngressBW", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->DynamicInfo.MaxIngressBW;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxEgressBW", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->DynamicInfo.MaxEgressBW;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "NetworkCoordinator", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->DynamicInfo.NetworkCoordinator;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "NodeID", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->DynamicInfo.NodeID;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "BackupNC", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->DynamicInfo.BackupNC;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "CurrentOperFreq", TRUE))
    {
        *puLong = pMoCAIfFull->DynamicInfo.CurrentOperFreq;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "LastOperFreq", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->DynamicInfo.LastOperFreq;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PowerCntlPhyTarget", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->Cfg.PowerCntlPhyTarget;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "BeaconPowerLimit", TRUE))
    {
        /* collect value */
        AnscTraceWarning(("pMoCAIfFull->Cfg.BeaconPowerLimit: %d\n", pMoCAIfFull->Cfg.BeaconPowerLimit));

        *puLong = pMoCAIfFull->Cfg.BeaconPowerLimit;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "TxBcastRate", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->DynamicInfo.TxBcastRate;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "TxBcastPowerReduction", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->StaticInfo.TxBcastPowerReduction;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PacketAggregationCapability", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->StaticInfo.PacketAggregationCapability;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AutoPowerControlPhyRate", TRUE))
    {
        /* collect value */

        AnscTraceWarning(("ParamName: %s: pMoCAIfFull->Cfg.AutoPowerControlPhyRate: %d\n", ParamName, 
                          pMoCAIfFull->Cfg.AutoPowerControlPhyRate));
        *puLong = pMoCAIfFull->Cfg.AutoPowerControlPhyRate;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_BestNetworkCoordinatorID", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->DynamicInfo.X_CISCO_COM_BestNetworkCoordinatorID;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_NumberOfConnectedClients", TRUE))
    {
        /* collect value */
        *puLong = pMoCAIfFull->DynamicInfo.X_CISCO_COM_NumberOfConnectedClients;
        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "TxPowerLimit", TRUE))
    {
        AnscTraceWarning(("ParamName: %s: pMoCAIfFull->Cfg.TxPowerLimit: %d\n", ParamName, pMoCAIfFull->Cfg.TxPowerLimit));
        *puLong = pMoCAIfFull->Cfg.TxPowerLimit;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface1_GetParamStringValue
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
Interface1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{    
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    PCOSA_DATAMODEL_MOCA            pMyObject   = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    
    AnscTraceWarning(("ParamName: %s\n", ParamName));
    CosaDmlMocaIfGetDinfo(NULL, pMoCAIfFull->Cfg.InstanceNumber-1, &pMoCAIfFull->DynamicInfo);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMoCAIfFull->Cfg.Alias);
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "Name", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMoCAIfFull->StaticInfo.Name);
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "LowerLayers", TRUE))
    {
        /* collect value */
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "MACAddress", TRUE))
    {
        /* collect value */
        _ansc_sprintf
            (
                pValue,
                "%02X:%02X:%02X:%02X:%02X:%02X",
                pMoCAIfFull->StaticInfo.MacAddress[0],
                pMoCAIfFull->StaticInfo.MacAddress[1],
                pMoCAIfFull->StaticInfo.MacAddress[2],
                pMoCAIfFull->StaticInfo.MacAddress[3],
                pMoCAIfFull->StaticInfo.MacAddress[4],
                pMoCAIfFull->StaticInfo.MacAddress[5]
            );
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "FirmwareVersion", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMoCAIfFull->StaticInfo.FirmwareVersion);
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "HighestVersion", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMoCAIfFull->StaticInfo.HighestVersion);
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "CurrentVersion", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMoCAIfFull->DynamicInfo.CurrentVersion);
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "FreqCapabilityMask", TRUE))
    {
        /* collect value */
        _ansc_sprintf
            (
                pValue,
                "%02X%02X%02X%02X%02X%02X%02X%02X",
                pMoCAIfFull->StaticInfo.FreqCapabilityMask[0],
                pMoCAIfFull->StaticInfo.FreqCapabilityMask[1],
                pMoCAIfFull->StaticInfo.FreqCapabilityMask[2],
                pMoCAIfFull->StaticInfo.FreqCapabilityMask[3],
                pMoCAIfFull->StaticInfo.FreqCapabilityMask[4],
                pMoCAIfFull->StaticInfo.FreqCapabilityMask[5],
                pMoCAIfFull->StaticInfo.FreqCapabilityMask[6],
                pMoCAIfFull->StaticInfo.FreqCapabilityMask[7]
            );
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "FreqCurrentMaskSetting", TRUE))
    {
        AnscCopyString(pValue, (PCHAR)pMoCAIfFull->Cfg.FreqCurrentMaskSetting);
        *pUlSize = AnscSizeOfString((PCHAR)pMoCAIfFull->Cfg.FreqCurrentMaskSetting);
        return 0;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_ChannelScanMask", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, (PCHAR)pMoCAIfFull->Cfg.X_CISCO_COM_ChannelScanMask);
        *pUlSize = AnscSizeOfString((PCHAR)pMoCAIfFull->Cfg.X_CISCO_COM_ChannelScanMask);
        return 0;
    }

    if( AnscEqualString(ParamName, "FreqCurrentMask", TRUE))
    {
        /* collect value */
        _ansc_sprintf
            (
                pValue,
                "%02X%02X%02X%02X%02X%02X%02X%02X",
                pMoCAIfFull->DynamicInfo.FreqCurrentMask[0],
                pMoCAIfFull->DynamicInfo.FreqCurrentMask[1],
                pMoCAIfFull->DynamicInfo.FreqCurrentMask[2],
                pMoCAIfFull->DynamicInfo.FreqCurrentMask[3],
                pMoCAIfFull->DynamicInfo.FreqCurrentMask[4],
                pMoCAIfFull->DynamicInfo.FreqCurrentMask[5],
                pMoCAIfFull->DynamicInfo.FreqCurrentMask[6],
                pMoCAIfFull->DynamicInfo.FreqCurrentMask[7]
            );

        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "KeyPassphrase", TRUE))
    {
        /* When read KeyPassphrase should return an empty string */
        AnscCopyString(pValue, pMoCAIfFull->Cfg.KeyPassphrase);
        *pUlSize = AnscSizeOfString(pMoCAIfFull->Cfg.KeyPassphrase);
        return 0;
    }

    if( AnscEqualString(ParamName, "NetworkTabooMask", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, (PCHAR)pMoCAIfFull->StaticInfo.NetworkTabooMask);
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "NodeTabooMask", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, (PCHAR)pMoCAIfFull->Cfg.NodeTabooMask);
        *pUlSize = AnscSizeOfString((PCHAR)pMoCAIfFull->Cfg.NodeTabooMask);
        return 0;
    }

    if( AnscEqualString(ParamName, "X_CISCO_NetworkCoordinatorMACAddress", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, (PCHAR)pMoCAIfFull->DynamicInfo.X_CISCO_NetworkCoordinatorMACAddress);
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface1_SetParamBoolValue
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
Interface1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;

    AnscTraceWarning(("ParamName: %s bvalue\n", ParamName, bValue));

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pMoCAIfFull->Cfg.bEnabled = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PreferredNC", TRUE))
    {
        /* save update to backup */
        pMoCAIfFull->Cfg.bPreferredNC = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PrivacyEnabledSetting", TRUE))
    {
        /* save update to backup */
        pMoCAIfFull->Cfg.PrivacyEnabledSetting = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_Reset", TRUE))
    {
        /* save update to backup */
        pMoCAIfFull->Cfg.X_CISCO_COM_Reset = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_MixedMode", TRUE))
    {
        /* save update to backup */
        pMoCAIfFull->Cfg.X_CISCO_COM_MixedMode = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_ChannelScanning", TRUE))
    {
        /* save update to backup */
        pMoCAIfFull->Cfg.X_CISCO_COM_ChannelScanning = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_AutoPowerControlEnable", TRUE))
    {
        /* save update to backup */
        pMoCAIfFull->Cfg.X_CISCO_COM_AutoPowerControlEnable = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_EnableTabooBit", TRUE))
    {
        /* save update to backup */
        pMoCAIfFull->Cfg.X_CISCO_COM_EnableTabooBit = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface1_SetParamIntValue
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
Interface1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;

    AnscTraceWarning(("ParamName: %s iValue: %d\n", ParamName, iValue));

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "TxPowerLimit", TRUE))
    {
        /* save update to backup */
        pMoCAIfFull->Cfg.TxPowerLimit = iValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface1_SetParamUlongValue
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
Interface1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{    
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    
    AnscTraceWarning(("ParamName: %s uValue: %d\n", ParamName, uValue));

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "PowerCntlPhyTarget", TRUE))
    {
        /* save update to backup */
        pMoCAIfFull->Cfg.PowerCntlPhyTarget = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "BeaconPowerLimit", TRUE))
    {

        /* save update to backup */
        pMoCAIfFull->Cfg.BeaconPowerLimit = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AutoPowerControlPhyRate", TRUE))
    {
        /* save update to backup */
        pMoCAIfFull->Cfg.AutoPowerControlPhyRate = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface1_SetParamStringValue
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
Interface1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    
    AnscTraceWarning(("ParamName: %s pString: %s\n", ParamName, pString));

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pMoCAIfFull->Cfg.Alias, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "LowerLayers", TRUE))
    {
        /* save update to backup */
        /* Interface is a layer 1 interface. LowerLayers will not be used. */
        return FALSE;
    }

    if( AnscEqualString(ParamName, "FreqCurrentMaskSetting", TRUE))
    {
        /* save update to backup */
        
        AnscCopyString((PCHAR)pMoCAIfFull->Cfg.FreqCurrentMaskSetting, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_ChannelScanMask", TRUE))
    {
        /* save update to backup */
        AnscCopyString((PCHAR)pMoCAIfFull->Cfg.X_CISCO_COM_ChannelScanMask, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "KeyPassphrase", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pMoCAIfFull->Cfg.KeyPassphrase, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "NodeTabooMask", TRUE))
    {
        /* save update to backup */
	AnscCopyString((PCHAR)pMoCAIfFull->Cfg.NodeTabooMask, pString);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface1_Validate
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
Interface1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject               = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_IF_FULL_TABLE    pMoCAIfFullTable        = (PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           keyPassLen              = 0;
    
    AnscTraceWarning(("\n"));

    ulEntryCount = CosaDmlMocaGetNumberOfIfs((ANSC_HANDLE)NULL/*pPoamMoCADm*/);
        
    keyPassLen = AnscSizeOfString(pMoCAIfFullTable->MoCAIfFull.Cfg.KeyPassphrase);
    if(pMoCAIfFullTable->MoCAIfFull.Cfg.PrivacyEnabledSetting == TRUE)
    {
        if( (keyPassLen < 12) || (keyPassLen > 17) )
        {
            AnscCopyString(pReturnParamName, "KeyPassphrase");
            *puLength = AnscSizeOfString("KeyPassphrase");

            return FALSE;
        }
    }

/*
    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        if (
                ((ULONG)pMoCAIfFullTable != (ULONG)&pMyObject->MoCAIfFullTable[ulIndex]) &&
                AnscEqualString
                    (
                        pMoCAIfFullTable->MoCAIfFull.Cfg.Alias,
                        pMyObject->MoCAIfFullTable[ulIndex].MoCAIfFull.Cfg.Alias,
                        TRUE
                    )
            )
        {
            AnscCopyString(pReturnParamName, "Alias");

            *puLength = AnscSizeOfString("Alias");

            return FALSE;
        }
    }
*/

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface1_Commit
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
Interface1_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    PCOSA_DATAMODEL_MOCA            pMyObject   = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    ANSC_STATUS                     ReturnStatus  = ANSC_STATUS_SUCCESS;

    AnscTraceWarning(("\n"));

    ReturnStatus = CosaDmlMocaIfSetCfg((ANSC_HANDLE)NULL, pMoCAIfFull->Cfg.InstanceNumber-1, &pMoCAIfFull->Cfg);
    
    return ReturnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface1_Rollback
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
Interface1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    PCOSA_DATAMODEL_MOCA            pMyObject   = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;

    CosaDmlMocaIfGetCfg((ANSC_HANDLE)NULL, pMoCAIfFull->Cfg.InstanceNumber-1, &pMoCAIfFull->Cfg);

    return 0;
}

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.X_CISCO_COM_PeerTable.{i}.

    *  X_CISCO_COM_PeerTable_GetEntryCount
    *  X_CISCO_COM_PeerTable_GetEntry
    *  X_CISCO_COM_PeerTable_IsUpdated
    *  X_CISCO_COM_PeerTable_Synchronize
    *  X_CISCO_COM_PeerTable_GetParamBoolValue
    *  X_CISCO_COM_PeerTable_GetParamIntValue
    *  X_CISCO_COM_PeerTable_GetParamUlongValue
    *  X_CISCO_COM_PeerTable_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_PeerTable_GetEntryCount
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
X_CISCO_COM_PeerTable_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject       = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_IF_FULL          pIf             = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           InterfaceIndex  = pIf->Cfg.InstanceNumber - 1;
    
    return pMyObject->MoCAIfFullTable[InterfaceIndex].ulMoCAPeerTableCount;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        X_CISCO_COM_PeerTable_GetEntry
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
X_CISCO_COM_PeerTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{   
    PCOSA_DATAMODEL_MOCA            pMyObject       = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_IF_FULL          pIf             = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           InterfaceIndex  = pIf->Cfg.InstanceNumber - 1;

    if ( pMyObject->MoCAIfFullTable[InterfaceIndex].ulMoCAPeerTableCount <= nIndex )
        return NULL;

    *pInsNumber               = nIndex+1;

    return (ANSC_HANDLE)&pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAPeerTable[nIndex];
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_PeerTable_IsUpdated
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
X_CISCO_COM_PeerTable_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    static ULONG last_tick = 0;
    
    if ( !last_tick ) 
    {
        last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - MOCA_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_PeerTable_Synchronize
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
X_CISCO_COM_PeerTable_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject       = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_IF_FULL          pIf             = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           InterfaceIndex  = pIf->Cfg.InstanceNumber - 1;
    PCOSA_DML_MOCA_PEER             pConf           = NULL;
    ULONG                           Count           = 0;
    ANSC_STATUS                     ret             = ANSC_STATUS_SUCCESS;
    
    if ( pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAPeerTable )
    {
        AnscFreeMemory(pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAPeerTable);
        pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAPeerTable       = NULL;
        pMyObject->MoCAIfFullTable[InterfaceIndex].ulMoCAPeerTableCount = 0;
    }

    ret = CosaDmlMocaIfPeerTableGetTable((ANSC_HANDLE)NULL, InterfaceIndex, &pConf,&Count );
    if ( ret != ANSC_STATUS_SUCCESS )
    {
        return ret;
    }

    pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAPeerTable       = pConf;
    pMyObject->MoCAIfFullTable[InterfaceIndex].ulMoCAPeerTableCount = Count;

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_PeerTable_GetParamBoolValue
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
X_CISCO_COM_PeerTable_GetParamBoolValue
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
        X_CISCO_COM_PeerTable_GetParamIntValue
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
X_CISCO_COM_PeerTable_GetParamIntValue
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
        X_CISCO_COM_PeerTable_GetParamUlongValue
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
X_CISCO_COM_PeerTable_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_MOCA_PEER             pConf           = (PCOSA_DML_MOCA_PEER)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "PeerSpeed", TRUE))
    {
        /* collect value */
        *puLong = pConf->PeerSpeed;
        
        return TRUE;
    }
    
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_PeerTable_GetParamStringValue
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
X_CISCO_COM_PeerTable_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

static ANSC_STATUS
_MeshTxNodeTable_Release
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DML_MOCA_IF_FULL_TABLE    pMoCAIfFull     = (PCOSA_DML_MOCA_IF_FULL_TABLE)hThisObject;
    PCOSA_DML_MOCA_MeshTxNode       pMeshTxNode     = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = AnscSListGetFirstEntry(&pMoCAIfFull->MoCAMeshTxNodeTable);
    
    while ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        AnscSListPopEntryByLink(&pMoCAIfFull->MoCAMeshTxNodeTable, &pCosaContext->Linkage);


        pMeshTxNode = (PCOSA_DML_MOCA_MeshTxNode)pCosaContext->hContext;

        if ( AnscSListQueryDepth(&pMeshTxNode->MoCAMeshRxNodeTable) )
        {
            PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = NULL;
            PSINGLE_LINK_ENTRY              pSLinkEntry     = AnscSListGetFirstEntry(&pMeshTxNode->MoCAMeshRxNodeTable);
            
            while ( pSLinkEntry )
            {
                pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
                pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
            
                AnscSListPopEntryByLink(&pMeshTxNode->MoCAMeshRxNodeTable, &pCosaContext->Linkage);
                AnscFreeMemory(pCosaContext->hContext);
                AnscFreeMemory(pCosaContext);
            }
        }
        
        AnscFreeMemory(pCosaContext->hContext);
        AnscFreeMemory(pCosaContext);
    }
    
    AnscSListInitializeHeader(&pMoCAIfFull->MoCAMeshTxNodeTable);
    return returnStatus;
}

static PCOSA_DML_MOCA_MeshTxNode
_MeshTxNodeTable_StoreTxNode
    (
        PSLIST_HEADER               pMoCAMeshTxNodeTable,
        ULONG                       ulInterfaceIndex,
        ULONG                       TxNodeID
    )
{
    ULONG                           instNum         = 0;
    PCOSA_DML_MOCA_MeshTxNode       pMeshTxNode     = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = AnscSListGetFirstEntry(pMoCAMeshTxNodeTable);
    PCOSA_CONTEXT_MOCA_LINK_OBJECT  pMocaLink       = NULL;

    //check duplicate
    while ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        pMeshTxNode = (PCOSA_DML_MOCA_MeshTxNode)pCosaContext->hContext;
        //printf("HOUJIN: %s: Existing InstanceNumber=%u and TxNodeID=%u, New TxNodeID=%u\n",
        //    __func__, pCosaContext->InstanceNumber, pMeshTxNode->TxNodeID, TxNodeID);
        
        if ( instNum < pCosaContext->InstanceNumber )
            instNum = pCosaContext->InstanceNumber;

        if ( pMeshTxNode->TxNodeID == TxNodeID )
            return pMeshTxNode;
    }


    //append entity
    pMeshTxNode = (PCOSA_DML_MOCA_MeshTxNode)AnscAllocateMemory(sizeof(COSA_DML_MOCA_MeshTxNode));
    if ( !pMeshTxNode )
        return NULL;

    AnscSListInitializeHeader(&pMeshTxNode->MoCAMeshRxNodeTable);
    pMeshTxNode->TxNodeID = TxNodeID;

    
    //append context
    pMocaLink = (PCOSA_CONTEXT_MOCA_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_MOCA_LINK_OBJECT));
    if ( !pMocaLink )
    {
        AnscFreeMemory(pMeshTxNode);
        return NULL;
    }

    pMocaLink->InterfaceIndex = ulInterfaceIndex;
    pMocaLink->Index          = instNum++;
    pMocaLink->InstanceNumber = instNum;
    pMocaLink->hContext       = (ANSC_HANDLE)pMeshTxNode;

    //printf("HOUJIN: %s: Created InstanceNumber=%u and TxNodeID=%u\n",
    //    __func__, instNum, TxNodeID);
    CosaSListPushEntryByInsNum(pMoCAMeshTxNodeTable, (PCOSA_CONTEXT_LINK_OBJECT)pMocaLink);
    
    return pMeshTxNode;
}

static PCOSA_DML_MOCA_MeshRxNode
_MeshTxNodeTable_StoreRxNode
    (
        PSLIST_HEADER               pMoCAMeshRxNodeTable,
        ULONG                       ulInterfaceIndex,
        ULONG                       RxNodeID,
        ULONG                       TxRate
    )
{
    ULONG                           instNum         = 0;
    PCOSA_DML_MOCA_MeshRxNode       pMeshRxNode     = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = AnscSListGetFirstEntry(pMoCAMeshRxNodeTable);
    PCOSA_CONTEXT_MOCA_LINK_OBJECT  pMocaLink       = NULL;

    //check duplicate
    while ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        pMeshRxNode = (PCOSA_DML_MOCA_MeshRxNode)pCosaContext->hContext;
        //printf("HOUJIN: %s: Existing InstanceNumber=%u and RxNodeID=%u, New RxNodeID=%u\n",
        //    __func__, pCosaContext->InstanceNumber, pMeshRxNode->RxNodeID, RxNodeID);
        
        if ( instNum < pCosaContext->InstanceNumber )
            instNum = pCosaContext->InstanceNumber;

        if ( pMeshRxNode->RxNodeID == RxNodeID )
            return pMeshRxNode;
    }


    //append entity
    pMeshRxNode = (PCOSA_DML_MOCA_MeshRxNode)AnscAllocateMemory(sizeof(COSA_DML_MOCA_MeshRxNode));
    if ( !pMeshRxNode )
        return NULL;

    pMeshRxNode->RxNodeID = RxNodeID;
    pMeshRxNode->TxRate = TxRate;

    
    //append context
    pMocaLink = (PCOSA_CONTEXT_MOCA_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_MOCA_LINK_OBJECT));
    if ( !pMocaLink )
    {
        AnscFreeMemory(pMeshRxNode);
        return NULL;
    }

    pMocaLink->InterfaceIndex = ulInterfaceIndex;
    pMocaLink->Index          = instNum++;
    pMocaLink->InstanceNumber = instNum;
    pMocaLink->hContext       = (ANSC_HANDLE)pMeshRxNode;

    //printf("HOUJIN: %s: Created InstanceNumber=%u, RxNodeID=%u, TxRate=%u\n",
    //    __func__, instNum, RxNodeID, TxRate);
    CosaSListPushEntryByInsNum(pMoCAMeshRxNodeTable, (PCOSA_CONTEXT_LINK_OBJECT)pMocaLink);
    
    return pMeshRxNode;
}

static ANSC_STATUS
_MeshTxNodeTable_Populate
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulInterfaceIndex,
        const COSA_DML_MOCA_MESH*   pMoCAMesh,
        ULONG                       Count
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DML_MOCA_IF_FULL_TABLE    pMoCAIfFull     = (PCOSA_DML_MOCA_IF_FULL_TABLE)hThisObject;
    PCOSA_DML_MOCA_MeshTxNode       pMeshTxNode     = NULL;
    ULONG                           i               = 0;

    if ( AnscSListQueryDepth(&pMoCAIfFull->MoCAMeshTxNodeTable) )
    {
        _MeshTxNodeTable_Release(hThisObject);
    }

    for ( i = 0; i < Count; ++i )
    {
        pMeshTxNode = _MeshTxNodeTable_StoreTxNode(&pMoCAIfFull->MoCAMeshTxNodeTable, ulInterfaceIndex, pMoCAMesh[i].TxNodeID);
        if ( pMeshTxNode )
        {
            _MeshTxNodeTable_StoreRxNode(&pMeshTxNode->MoCAMeshRxNodeTable, ulInterfaceIndex, pMoCAMesh[i].RxNodeID, pMoCAMesh[i].TxRate);
        }
    }
    
    return returnStatus;
}

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.X_CISCO_COM_Mesh.MeshTxNodeTable.{i}.

    *  MeshTxNodeTable_GetEntryCount
    *  MeshTxNodeTable_GetEntry
    *  MeshTxNodeTable_IsUpdated
    *  MeshTxNodeTable_Synchronize
    *  MeshTxNodeTable_GetParamUlongValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        MeshTxNodeTable_GetEntryCount
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
MeshTxNodeTable_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DML_MOCA_IF_FULL_TABLE pMoCAIfFull = (PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext;
    return AnscSListQueryDepth(&pMoCAIfFull->MoCAMeshTxNodeTable);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        MeshTxNodeTable_GetEntry
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
MeshTxNodeTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{   
    PCOSA_DML_MOCA_IF_FULL_TABLE    pMoCAIfFull     = (PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(&pMoCAIfFull->MoCAMeshTxNodeTable, nIndex);

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

        BOOL
        MeshTxNodeTable_IsUpdated
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
MeshTxNodeTable_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    static ULONG last_tick = 0;
    
    if ( !last_tick ) 
    {
        last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - MOCA_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        MeshTxNodeTable_Synchronize
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
MeshTxNodeTable_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DML_MOCA_IF_FULL_TABLE    pMoCAIfFull     = (PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext;
    PCOSA_DML_MOCA_IF_FULL          pIf             = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           InterfaceIndex  = pIf->Cfg.InstanceNumber - 1;
    PCOSA_DML_MOCA_MESH             pMoCAMesh       = NULL;
    ULONG                           Count           = 0;
    ANSC_STATUS                     ret             = ANSC_STATUS_SUCCESS;

    _MeshTxNodeTable_Release(hInsContext);

    //fetch data
    ret = CosaDmlMocaIfMeshTableGetTable((ANSC_HANDLE)NULL, InterfaceIndex, &pMoCAMesh, &Count);
    if ( ret != ANSC_STATUS_SUCCESS )
    {
        goto FIN;
    }

    _MeshTxNodeTable_Populate(hInsContext, InterfaceIndex, pMoCAMesh, Count);

FIN:
    if ( pMoCAMesh )
        AnscFreeMemory(pMoCAMesh);
    return ret;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MeshTxNodeTable_GetParamUlongValue
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
MeshTxNodeTable_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT pContextLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_MOCA_MeshTxNode pConf = (PCOSA_DML_MOCA_MeshTxNode)pContextLinkObject->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "TxNodeID", TRUE))
    {
        /* collect value */
        *puLong = pConf->TxNodeID;
        
        return TRUE;
    }
    
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.X_CISCO_COM_Mesh.MeshTxNodeTable.{i}.MeshRxNodeTable.{i}.

    *  MeshRxNodeTable_GetEntryCount
    *  MeshRxNodeTable_GetEntry
    *  MeshRxNodeTable_GetParamUlongValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        MeshRxNodeTable_GetEntryCount
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
MeshRxNodeTable_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT pContextLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_MOCA_MeshTxNode pMoCAMeshTxNode = (PCOSA_DML_MOCA_MeshTxNode)pContextLinkObject->hContext;
    return AnscSListQueryDepth(&pMoCAMeshTxNode->MoCAMeshRxNodeTable);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        MeshRxNodeTable_GetEntry
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
MeshRxNodeTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{   
    PCOSA_CONTEXT_LINK_OBJECT   pContextLinkObject  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_MOCA_MeshTxNode   pMoCAMeshTxNode     = (PCOSA_DML_MOCA_MeshTxNode)pContextLinkObject->hContext;
    PCOSA_CONTEXT_LINK_OBJECT   pCosaContext        = NULL;
    PSINGLE_LINK_ENTRY          pSLinkEntry         = NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(&pMoCAMeshTxNode->MoCAMeshRxNodeTable, nIndex);

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

        BOOL
        MeshRxNodeTable_GetParamUlongValue
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
MeshRxNodeTable_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT   pContextLinkObject  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_MOCA_MeshRxNode   pConf               = (PCOSA_DML_MOCA_MeshRxNode)pContextLinkObject->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "RxNodeID", TRUE))
    {
        /* collect value */
        *puLong = pConf->RxNodeID;
        
        return TRUE;
    }
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "TxRate", TRUE))
    {
        /* collect value */
        *puLong = pConf->TxRate;
        
        return TRUE;
    }
    
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.Stats.

    *  Stats2_GetParamBoolValue
    *  Stats2_GetParamIntValue
    *  Stats2_GetParamUlongValue
    *  Stats2_GetParamStringValue
    *  Stats2_SetParamBoolValue
    *  Stats2_SetParamIntValue
    *  Stats2_SetParamUlongValue
    *  Stats2_SetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats2_GetParamBoolValue
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
Stats2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Reset", TRUE))
    {
        /* collect value */
        *pBool = FALSE;
        
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats2_GetParamIntValue
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
Stats2_GetParamIntValue
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
        Stats2_GetParamUlongValue
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
Stats2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    PCOSA_DATAMODEL_MOCA            pMyObject   = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    /*PPOAM_COSAMOCADM_OBJECT         pPoamMoCADm = (PPOAM_COSAMOCADM_OBJECT )pMyObject->pPoamMoCADm;*/
    COSA_DML_MOCA_STATS             Stats;

    CosaDmlMocaIfGetStats((ANSC_HANDLE)NULL/*pPoamMoCADm*/, pMoCAIfFull->Cfg.InstanceNumber, &Stats);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "BytesSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.BytesSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "BytesReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.BytesReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PacketsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.PacketsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.PacketsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ErrorsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.ErrorsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ErrorsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.ErrorsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "UnicastPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "UnicastPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DiscardPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DiscardPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MulticastPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MulticastPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "BroadcastPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "BroadcastPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "UnknownProtoPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.UnknownProtoPacketsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_ExtAggrAverageTx", TRUE))
    {
        /* collect value */
        *puLong = Stats.X_CISCO_COM_ExtAggrAverageTx;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_ExtAggrAverageRx", TRUE))
    {
        /* collect value */
        *puLong = Stats.X_CISCO_COM_ExtAggrAverageRx;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Stats2_GetParamStringValue
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
Stats2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats2_SetParamBoolValue
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
Stats2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Reset", TRUE))
    {
        /* save update to backup */
        if ( bValue == TRUE )
        {
            CosaDmlMocaIfResetStats( NULL, pMoCAIfFull->Cfg.InstanceNumber-1 );
            return TRUE;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats2_SetParamIntValue
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
Stats2_SetParamIntValue
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
        Stats2_SetParamUlongValue
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
Stats2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{    
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats2_SetParamStringValue
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
Stats2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    
    /* check the parameter name and set the corresponding value */


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats2_Validate
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
Stats2_Validate
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
        Stats2_Commit
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
Stats2_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Stats2_Rollback
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
Stats2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.Stats.X_CISCO_COM_ExtCounterTable.{i}.

    *  X_CISCO_COM_ExtCounterTable_GetEntryCount
    *  X_CISCO_COM_ExtCounterTable_GetEntry
    *  X_CISCO_COM_ExtCounterTable_GetParamBoolValue
    *  X_CISCO_COM_ExtCounterTable_GetParamIntValue
    *  X_CISCO_COM_ExtCounterTable_GetParamUlongValue
    *  X_CISCO_COM_ExtCounterTable_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_ExtCounterTable_GetEntryCount
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
X_CISCO_COM_ExtCounterTable_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject       = (PCOSA_DATAMODEL_MOCA    )g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_IF_FULL          pIf             = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           InterfaceIndex  = pIf->Cfg.InstanceNumber - 1;

    return CosaDmlMocaIfExtCounterGetNumber((ANSC_HANDLE)NULL, InterfaceIndex);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        X_CISCO_COM_ExtCounterTable_GetEntry
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
X_CISCO_COM_ExtCounterTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DML_MOCA_IF_FULL          pIf             = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           InterfaceIndex  = 0;
    PCOSA_DML_MOCA_EXTCOUNTER       pExtCounter     = NULL;
    PCOSA_CONTEXT_MOCA_LINK_OBJECT  pMocaLink       = NULL;
    ANSC_STATUS                     ret             = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_MOCA            pMyObject   = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;

    InterfaceIndex  = pIf->Cfg.InstanceNumber - 1;

    /* Get one ExtCounter Entry from backend*/
    pExtCounter = (PCOSA_DML_MOCA_EXTCOUNTER)AnscAllocateMemory( sizeof(COSA_DML_MOCA_EXTCOUNTER) );
    if ( pExtCounter == NULL )
    {
        return NULL;
    }
    
    AnscTraceWarning(("InterfaceIndex: %lu nIndex: %lu pExtCounte: p\n",  InterfaceIndex, nIndex, pExtCounter));
    ret = CosaDmlMocaIfExtCounterGetEntry((ANSC_HANDLE)NULL, InterfaceIndex, nIndex, pExtCounter);
    if ( ret == ANSC_STATUS_FAILURE )
    {
        AnscFreeMemory(pExtCounter);
        return NULL;
    }

    /* Wrapper the entry and save two index because we need them later */
    pMocaLink = (PCOSA_CONTEXT_MOCA_LINK_OBJECT)AnscAllocateMemory( sizeof( COSA_CONTEXT_MOCA_LINK_OBJECT ) );
    if ( pMocaLink == NULL )
    {
        AnscFreeMemory(pExtCounter);
        return pMocaLink;
    }

    pMocaLink->InterfaceIndex = InterfaceIndex;
    pMocaLink->Index          = nIndex;
    pMocaLink->hContext       = (ANSC_HANDLE)pExtCounter;
    *pInsNumber               = nIndex+1;
    
    /* Put into our list */
    CosaSListPushEntryByInsNum(&pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAExtCounterTable, (PCOSA_CONTEXT_LINK_OBJECT)pMocaLink);

    return (ANSC_HANDLE)pMocaLink;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_ExtCounterTable_GetParamBoolValue
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
X_CISCO_COM_ExtCounterTable_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_ExtCounterTable_GetParamIntValue
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
X_CISCO_COM_ExtCounterTable_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_ExtCounterTable_GetParamUlongValue
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
X_CISCO_COM_ExtCounterTable_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{    
    PCOSA_CONTEXT_MOCA_LINK_OBJECT  pMocaLink       = (PCOSA_CONTEXT_MOCA_LINK_OBJECT)hInsContext;
    PCOSA_DML_MOCA_EXTCOUNTER       pConf     = (PCOSA_DML_MOCA_EXTCOUNTER)pMocaLink->hContext;

    AnscTraceWarning(("InterfaceIndex: %lu Index: %lu pConf: p\n",  pMocaLink->InterfaceIndex, pMocaLink->Index, pConf));
    /*this is counter object. the information will be changed from time to time.*/
    CosaDmlMocaIfExtCounterGetEntry((ANSC_HANDLE)NULL, pMocaLink->InterfaceIndex, pMocaLink->Index, pConf);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Type", TRUE))
    {
        /* collect value */
        *puLong = pConf->Type;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Map", TRUE))
    {
        /* collect value */
        *puLong = pConf->Map;
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "Rsrv", TRUE))
    {
        /* collect value */
        *puLong = pConf->Rsrv;
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "Lc", TRUE))
    {
        /* collect value */
        *puLong = pConf->Lc;
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "Adm", TRUE))
    {
        /* collect value */
        *puLong = pConf->Adm;
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "Probe", TRUE))
    {
        /* collect value */
        *puLong = pConf->Probe;
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "Async", TRUE))
    {
        /* collect value */
        *puLong = pConf->Async;
        return TRUE;
    }
    
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_ExtCounterTable_GetParamStringValue
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
X_CISCO_COM_ExtCounterTable_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{    

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.Stats.X_CISCO_COM_ExtAggrCounterTable.{i}.

    *  X_CISCO_COM_ExtAggrCounterTable_GetEntryCount
    *  X_CISCO_COM_ExtAggrCounterTable_GetEntry
    *  X_CISCO_COM_ExtAggrCounterTable_GetParamBoolValue
    *  X_CISCO_COM_ExtAggrCounterTable_GetParamIntValue
    *  X_CISCO_COM_ExtAggrCounterTable_GetParamUlongValue
    *  X_CISCO_COM_ExtAggrCounterTable_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_ExtAggrCounterTable_GetEntryCount
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
X_CISCO_COM_ExtAggrCounterTable_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject       = (PCOSA_DATAMODEL_MOCA    )g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_IF_FULL          pIf             = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           InterfaceIndex  = pIf->Cfg.InstanceNumber - 1;
    
    return CosaDmlMocaIfExtAggrCounterGetNumber((ANSC_HANDLE)NULL, InterfaceIndex);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        X_CISCO_COM_ExtAggrCounterTable_GetEntry
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
X_CISCO_COM_ExtAggrCounterTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DML_MOCA_IF_FULL          pIf             = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           InterfaceIndex  = 0;
    PCOSA_DML_MOCA_EXTAGGRCOUNTER   pExtAggrCounter = NULL;
    PCOSA_CONTEXT_MOCA_LINK_OBJECT  pMocaLink       = NULL;
    ANSC_STATUS                     ret             = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_MOCA            pMyObject   = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;

    InterfaceIndex  = pIf->Cfg.InstanceNumber - 1;

    /* Get one ExtCounter Entry from backend*/
    pExtAggrCounter = (PCOSA_DML_MOCA_EXTAGGRCOUNTER)AnscAllocateMemory( sizeof(COSA_DML_MOCA_EXTAGGRCOUNTER) );
    if ( pExtAggrCounter == NULL )
    {
        return NULL;
    }
    
    ret = CosaDmlMocaIfExtAggrCounterGetEntry((ANSC_HANDLE)NULL, InterfaceIndex, nIndex, pExtAggrCounter);
    if ( ret == ANSC_STATUS_FAILURE )
    {
        AnscFreeMemory(pExtAggrCounter);
        return NULL;
    }

    /* Wrapper the entry and save two index because we need them later */
    pMocaLink = (PCOSA_CONTEXT_MOCA_LINK_OBJECT)AnscAllocateMemory( sizeof( COSA_CONTEXT_MOCA_LINK_OBJECT ) );
    if ( pMocaLink == NULL )
    {
        AnscFreeMemory(pExtAggrCounter);
        return pMocaLink;
    }

    pMocaLink->InterfaceIndex = InterfaceIndex;
    pMocaLink->Index          = nIndex;
    pMocaLink->hContext       = (ANSC_HANDLE)pExtAggrCounter;
    *pInsNumber               = nIndex+1;

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAExtAggrCounterTable, (PCOSA_CONTEXT_LINK_OBJECT)pMocaLink);

    return (ANSC_HANDLE)pMocaLink;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_ExtAggrCounterTable_GetParamBoolValue
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
X_CISCO_COM_ExtAggrCounterTable_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_ExtAggrCounterTable_GetParamIntValue
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
X_CISCO_COM_ExtAggrCounterTable_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_ExtAggrCounterTable_GetParamUlongValue
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
X_CISCO_COM_ExtAggrCounterTable_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{    
    PCOSA_CONTEXT_MOCA_LINK_OBJECT  pMocaLink        = (PCOSA_CONTEXT_MOCA_LINK_OBJECT)hInsContext;
    PCOSA_DML_MOCA_EXTAGGRCOUNTER   pConf  = (PCOSA_DML_MOCA_EXTAGGRCOUNTER)pMocaLink->hContext;

    /*this is counter object. the information will be changed from time to time.*/
    CosaDmlMocaIfExtAggrCounterGetEntry((ANSC_HANDLE)NULL, pMocaLink->InterfaceIndex, pMocaLink->Index, pConf);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Tx", TRUE))
    {
        /* collect value */
        *puLong = pConf->Tx;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Rx", TRUE))
    {
        /* collect value */
        *puLong = pConf->Rx;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_ExtAggrCounterTable_GetParamStringValue
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
X_CISCO_COM_ExtAggrCounterTable_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{    

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}



/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.QoS.

    *  QoS_GetParamBoolValue
    *  QoS_GetParamIntValue
    *  QoS_GetParamUlongValue
    *  QoS_GetParamStringValue
    *  QoS_SetParamBoolValue
    *  QoS_SetParamIntValue
    *  QoS_SetParamUlongValue
    *  QoS_SetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        QoS_GetParamBoolValue
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
QoS_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DML_MOCA_IF_FULL_TABLE    pMoCACfg = (PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "X_CISCO_COM_Enabled", TRUE))
    {
        /* collect value */
        *pBool = pMoCACfg->MoCAIfQos.Enabled;
        
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        QoS_GetParamIntValue
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
QoS_GetParamIntValue
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
        QoS_GetParamUlongValue
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
QoS_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "EgressNumFlows", TRUE))
    {
        /* collect value */
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IngressNumFlows", TRUE))
    {
        /* collect value */
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        QoS_GetParamStringValue
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
QoS_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Qos_SetParamBoolValue
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
Qos_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DML_MOCA_IF_FULL_TABLE    pMoCACfg = (PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "X_CISCO_COM_Enabled", TRUE))
    {
        /* save update to backup */
        pMoCACfg->MoCAIfQos.Enabled = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Qos_SetParamIntValue
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
Qos_SetParamIntValue
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
        Qos_SetParamUlongValue
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
Qos_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{    
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Qos_SetParamStringValue
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
Qos_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = &((PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext)->MoCAIfFull;
    
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Qos_Validate
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
Qos_Validate
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
        Qos_Commit
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
Qos_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DML_MOCA_IF_FULL          pMoCAIfFull = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           ulIndex     = pMoCAIfFull->Cfg.InstanceNumber-1;
    
    PCOSA_DATAMODEL_MOCA            pMyObject     = (PCOSA_DATAMODEL_MOCA    )g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_QOS              pCfg          = &pMyObject->MoCAIfFullTable[ulIndex].MoCAIfQos;
    ANSC_STATUS                     ReturnStatus  = ANSC_STATUS_SUCCESS;

    ReturnStatus = CosaDmlMocaIfSetQos(NULL, ulIndex, pCfg);
    
    return ReturnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Qos_Rollback
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
Qos_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.QoS.FlowStats.{i}.

    *  FlowStats_GetEntryCount
    *  FlowStats_GetEntry
    *  FlowStats_IsUpdated
    *  FlowStats_Synchronize
    *  FlowStats_GetParamUlongValue
    *  FlowStats_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        FlowStats_GetEntryCount
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
FlowStats_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject       = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_IF_FULL          pIf             = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           InterfaceIndex  = pIf->Cfg.InstanceNumber - 1;
    
    return pMyObject->MoCAIfFullTable[InterfaceIndex].ulMoCAFlowTableCount;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        FlowStats_GetEntry
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
FlowStats_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{   
    PCOSA_DATAMODEL_MOCA            pMyObject       = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_IF_FULL          pIf             = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           InterfaceIndex  = pIf->Cfg.InstanceNumber - 1;

    if ( pMyObject->MoCAIfFullTable[InterfaceIndex].ulMoCAFlowTableCount <= nIndex )
        return NULL;

    *pInsNumber               = nIndex+1;
    
    return (ANSC_HANDLE)&pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAFlowTable[nIndex];
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        FlowStats_IsUpdated
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
FlowStats_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    static ULONG last_tick = 0;
    
    if ( !last_tick ) 
    {
        last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - MOCA_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        FlowStats_Synchronize
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
FlowStats_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject       = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_IF_FULL          pIf             = (PCOSA_DML_MOCA_IF_FULL)hInsContext;
    ULONG                           InterfaceIndex  = pIf->Cfg.InstanceNumber - 1;
    PCOSA_DML_MOCA_FLOW             pConf           = NULL;
    ULONG                           Count           = 0;
    ANSC_STATUS                     ret             = ANSC_STATUS_SUCCESS;
    
    if ( pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAFlowTable )
    {
        AnscFreeMemory(pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAFlowTable);
        pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAFlowTable       = NULL;
        pMyObject->MoCAIfFullTable[InterfaceIndex].ulMoCAFlowTableCount = 0;
    }

    ret = CosaDmlMocaIfFlowTableGetTable((ANSC_HANDLE)NULL, InterfaceIndex, &pConf, &Count );
    if ( ret != ANSC_STATUS_SUCCESS )
    {
        return ret;
    }

    pMyObject->MoCAIfFullTable[InterfaceIndex].pMoCAFlowTable       = pConf;
    pMyObject->MoCAIfFullTable[InterfaceIndex].ulMoCAFlowTableCount = Count;

    return ret;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        FlowStats_GetParamUlongValue
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
FlowStats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_MOCA_FLOW             pConf           = (PCOSA_DML_MOCA_FLOW)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "FlowID", TRUE))
    {
        /* collect value */
        *puLong = pConf->FlowID;
        
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "IngressNodeID", TRUE))
    {
        /* collect value */
        *puLong = pConf->IngressNodeID;
        
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "EgressNodeID", TRUE))
    {
        /* collect value */
        *puLong = pConf->EgressNodeID;
        
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "LeaseTimeLeft", TRUE))
    {
        /* collect value */
        *puLong = pConf->FlowTimeLeft;
        
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "FlowPackets", TRUE))
    {
        /* collect value */
        *puLong = pConf->PacketSize;
        
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "MaxRate", TRUE))
    {
        /* collect value */
        *puLong = pConf->PeakDataRate;
        
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "MaxBurstSize", TRUE))
    {
        /* collect value */
        *puLong = pConf->BurstSize;
        
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "FlowTag", TRUE))
    {
        /* collect value */
        *puLong = pConf->FlowTag;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "LeaseTime", TRUE))
    {
        /* collect value */
        *puLong = pConf->LeaseTime;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        FlowStats_GetParamStringValue
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
FlowStats_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_MOCA_FLOW             pConf           = (PCOSA_DML_MOCA_FLOW)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "PacketDA", TRUE))
    {
        /* collect value */
        _ansc_strcpy(pValue, (PCHAR)pConf->DestinationMACAddress );
        *pUlSize = _ansc_strlen(pValue);
        
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.Associated{i}.

    *  AssociatedDevice_GetEntryCount
    *  AssociatedDevice_GetEntry
    *  AssociatedDevice_IsUpdated
    *  AssociatedDevice_Synchronize
    *  AssociatedDevice_GetParamBoolValue
    *  AssociatedDevice_GetParamIntValue
    *  AssociatedDevice_GetParamUlongValue
    *  AssociatedDevice_GetParamStringValue

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
AssociatedDevice_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_MOCA            pMyObject           = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_IF_FULL_TABLE    pMoCAIfFullTable    = (PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext;
    
    return pMoCAIfFullTable->ulMoCAAssocDeviceCount;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        AssociatedDevice_GetEntry
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
AssociatedDevice_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{    
    PCOSA_DATAMODEL_MOCA            pMyObject           = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    PCOSA_DML_MOCA_IF_FULL_TABLE    pMoCAIfFullTable    = (PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext;

    if (nIndex < pMoCAIfFullTable->ulMoCAAssocDeviceCount)
    {
        *pInsNumber  = nIndex + 1;

        return &pMoCAIfFullTable->pMoCAAssocDevice[nIndex];
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        AssociatedDevice_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
static ULONG last_tick;
#define REFRESH_INTERVAL 120
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))

BOOL
AssociatedDevice_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    if ( !last_tick ) 
    {
        last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - MOCA_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        AssociatedDevice_Synchronize
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
AssociatedDevice_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{    
    PCOSA_DATAMODEL_MOCA            pMyObject           = (PCOSA_DATAMODEL_MOCA)g_pCosaBEManager->hMoCA;
    /*PPOAM_COSAMOCADM_OBJECT         pPoamMoCADm         = (PPOAM_COSAMOCADM_OBJECT )pMyObject->pPoamMoCADm;*/
    PCOSA_DML_MOCA_IF_FULL_TABLE    pMoCAIfFullTable    = (PCOSA_DML_MOCA_IF_FULL_TABLE)hInsContext;

    if ( pMoCAIfFullTable->pMoCAAssocDevice)
    {
        AnscFreeMemory(pMoCAIfFullTable->pMoCAAssocDevice);
        
        pMoCAIfFullTable->pMoCAAssocDevice= NULL;
    }    
   
    CosaDmlMocaIfGetAssocDevices
        (
            (ANSC_HANDLE)NULL/*pPoamMoCADm*/, 
            pMoCAIfFullTable->MoCAIfFull.Cfg.InstanceNumber-1, 
            &pMoCAIfFullTable->ulMoCAAssocDeviceCount,
            &pMoCAIfFullTable->pMoCAAssocDevice,
            NULL
        );
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        AssociatedDevice_GetParamBoolValue
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
AssociatedDevice_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DML_MOCA_ASSOC_DEVICE     pMoCAAssocDevice    = (PCOSA_DML_MOCA_ASSOC_DEVICE)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "PreferredNC", TRUE))
    {
        /* collect value */
        *pBool = pMoCAAssocDevice->PreferredNC;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "QAM256Capable", TRUE))
    {
        /* collect value */
        *pBool = pMoCAAssocDevice->QAM256Capable;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Active", TRUE))
    {
        /* collect value */
        *pBool = pMoCAAssocDevice->Active;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        AssociatedDevice_GetParamIntValue
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
AssociatedDevice_GetParamIntValue
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
        AssociatedDevice_GetParamUlongValue
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
AssociatedDevice_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_MOCA_ASSOC_DEVICE     pMoCAAssocDevice    = (PCOSA_DML_MOCA_ASSOC_DEVICE)hInsContext;
 
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "NodeID", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->NodeID;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PHYTxRate", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->PHYTxRate;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PHYRxRate", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->PHYRxRate;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "TxPowerControlReduction", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->TxPowerControlReduction;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "RxPowerLevel", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->RxPowerLevel;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "TxBcastRate", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->TxBcastRate;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "RxBcastPowerLevel", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->RxBcastPowerLevel;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "TxPackets", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->TxPackets;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "RxPackets", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->RxPackets;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "RxErroredAndMissedPackets", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->RxErroredAndMissedPackets;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PacketAggregationCapability", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->PacketAggregationCapability;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "RxSNR", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->RxSNR;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_RxBcastRate", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->X_CISCO_COM_RxBcastRate;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_NumberOfClients", TRUE))
    {
        /* collect value */
        *puLong = pMoCAAssocDevice->X_CISCO_COM_NumberOfClients;
        return TRUE;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        AssociatedDevice_GetParamStringValue
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
AssociatedDevice_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_MOCA_ASSOC_DEVICE     pMoCAAssocDevice    = (PCOSA_DML_MOCA_ASSOC_DEVICE)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "MACAddress", TRUE))
    {
        /* collect value */
        _ansc_sprintf
            (
                pValue,
                "%02X:%02X:%02X:%02X:%02X:%02X",
                pMoCAAssocDevice->MACAddress[0],
                pMoCAAssocDevice->MACAddress[1],
                pMoCAAssocDevice->MACAddress[2],
                pMoCAAssocDevice->MACAddress[3],
                pMoCAAssocDevice->MACAddress[4],
                pMoCAAssocDevice->MACAddress[5]
            );
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "HighestVersion", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMoCAAssocDevice->HighestVersion);
        *pUlSize = AnscSizeOfString(pValue);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */

    return -1;
}

#endif
