/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
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
#ifdef FEATURE_SUPPORT_ONBOARD_LOGGING
#include "cosa_onboardlogging_dml.h"
#include "cosa_onboardlogging_internal.h"
#include "ansc_platform.h"
#include "ccsp_trace.h"
#include "cosa_deviceinfo_dml.h"

/***********************************************************************

 APIs for Object:

	OnboardingLogging.

    *  OnboardingLogging_GetParamBoolValue
    *  OnboardingLogging_SetParamBoolValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        OnboardingLogging_GetParamBoolValue
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
OnboardingLogging_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
    PCOSA_DATAMODEL_ONBOARDLOGGING       pMyObject     = (PCOSA_DATAMODEL_ONBOARDLOGGING)g_pCosaBEManager->hOnboardLogging;
    UNREFERENCED_PARAMETER(hInsContext);  
    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pMyObject->bEnable;
        return TRUE;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        OnboardingLogging_SetParamBoolValue
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
OnboardingLogging_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{

    /* check the parameter name and return the corresponding value */
    PCOSA_DATAMODEL_ONBOARDLOGGING       pMyObject     = (PCOSA_DATAMODEL_ONBOARDLOGGING)g_pCosaBEManager->hOnboardLogging;
    ANSC_STATUS  returnStatus = ANSC_STATUS_SUCCESS;

    if (IsBoolSame(hInsContext, ParamName, bValue, OnboardingLogging_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if(bValue == pMyObject->bEnable)
                return TRUE;
        if( bValue )
                returnStatus = EnableOnboardLogging(pMyObject);
        else
                returnStatus = DisableOnboardLogging(pMyObject);

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            CcspTraceInfo(("%s EXIT Error\n", __FUNCTION__));
            return  returnStatus;
        }

        return TRUE;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}
#endif
