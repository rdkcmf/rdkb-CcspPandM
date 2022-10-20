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


/**********************************************************************

    module: dslh_dmagnt_exported.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the framework's exported functions
        by the Dslh DataModelAgent object;

        *   CosaGetParamValueUlong
        *   CosaGetParamValueString
        *   CosaGetParamValueBool
        *   CosaGetInstanceNumberByIndex
        *   CosaGetRegistryRootFolder
        *   CosaRegisterCallBackAfterInitDml
        *   CosaCOSARepopulateTable

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        01/06/2011    initial revision.
        01/11/2011    added SLAP related apis.
        03/21/2011    added api to retrieve instance number by index

**********************************************************************/

#include "cosa_apis.h"

#include "plugin_main_apis.h"
#include "cosa_apis_busutil.h"
#include "ansc_platform.h"

extern void * g_pDslhDmlAgent;
extern COSARepopulateTableProc g_COSARepopulateTable;
// This below macro need to enable when ethernet interface 
// tr181 is moved from pandm to Ethagent component.
#ifdef FEATURE_RDKB_WAN_MANAGER
#define ENABLE_ETHERNET_TR181_REMOTE_CALL
#endif

#ifdef ENABLE_ETHERNET_TR181_REMOTE_CALL
extern ANSC_HANDLE bus_handle;
#if defined(_PLATFORM_RASPBERRYPI_)
#define ETHERNET_INTERFACE_OBJECT "Device.Ethernet.X_RDK_Interface."
#else
#define ETHERNET_INTERFACE_OBJECT "Device.Ethernet.Interface"
#endif
#define ETH_COMPONENT_NAME "eRT.com.cisco.spvtg.ccsp.ethagent"
#define ETH_DBUS_PATH "/com/cisco/spvtg/ccsp/ethagent"
static ANSC_STATUS RdkBus_GetParamValues( char *pComponent, char *pBus, char *pParamName, char *pReturnVal );

#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
#define X_RDK_ETHERNET_INTERFACE_OBJECT    "Device.Ethernet.X_RDK_Interface."
#define PPP_INTERFACE_OBJECT               "Device.PPP."
#define PTM_INTERFACE_OBJECT               "Device.PTM."
#define DSL_INTERFACE_OBJECT               "Device.DSL."
#define VLAN_INTERFACE_OBJECT              "Device.X_RDK_Ethernet."
#define VLAN_DBUS_PATH                     "/com/cisco/spvtg/ccsp/vlanmanager"
#define VLAN_COMPONENT_NAME                "eRT.com.cisco.spvtg.ccsp.vlanmanager"
#define VLAN_ETHLINK_NOE_PARAM_NAME        "Device.X_RDK_Ethernet.LinkNumberOfEntries"
#define VLAN_ETHLINK_TABLE_NAME            "Device.X_RDK_Ethernet.Link."
#define VLAN_ETHLINK_TABLE_FORMAT          "Device.X_RDK_Ethernet.Link.%d."
#endif

#endif
/**********************************************************************

    prototype:

        ULONG
        CosaGetParamValueUlong2
            (
                char*                       pParamName
            )

    description:

        This function is called to retrieve a UONG value of a parameter;

    argument:   
            char*                       pParamName
            The full name of the parameter;

    return:     the ULONG value;

**********************************************************************/
ULONG
CosaGetParamValueUlong
    (
        char*                       pParamName
    )
{
#ifdef ENABLE_ETHERNET_TR181_REMOTE_CALL
    char acTmpReturnValue[256] = {0};
    ULONG result = 0;
    if (strstr(pParamName, ETHERNET_INTERFACE_OBJECT))
    {
        if (ANSC_STATUS_FAILURE == RdkBus_GetParamValues(ETH_COMPONENT_NAME, ETH_DBUS_PATH, pParamName, acTmpReturnValue))
        {
            CcspTraceError(("[%s][%d]Failed to get param value\n", __FUNCTION__, __LINE__));
            return 0;
        }
        result = strtoul(acTmpReturnValue, NULL, 10);
        return result;
    }
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
    /* Query VlanManager */
    if (strstr(pParamName, VLAN_INTERFACE_OBJECT))
    {
        if (ANSC_STATUS_FAILURE == RdkBus_GetParamValues(VLAN_COMPONENT_NAME, VLAN_DBUS_PATH, pParamName, acTmpReturnValue))
        {
            CcspTraceError(("[%s][%d]Failed to get param value\n", __FUNCTION__, __LINE__));
            return 0;
        }
        result = strtoul(acTmpReturnValue, NULL, 10);
        return result;
    }
#endif
#endif
    /* we should look up CR to find right component.
            if it's P&M component, we just call the global variable 
            Currently, we suppose all the parameter is from P&M. */

    return g_GetParamValueUlong(g_pDslhDmlAgent, pParamName);
}

/**********************************************************************

    prototype:

        int
        CosaGetParamValueString
            (
                char*                       pParamName,
                char*                       pBuffer,
                PULONG                      pulSize
            )

    description:

        This function is called to retrieve a string value of a parameter;

    argument:   
            char*                       pParamName
            The full name of the parameter;

            char*                       pBuffer,
            The buffer for the value;

            PULONG                      pulSize
            The buffer of size;

    return:     0 = SUCCESS; -1 = FAILURE; 1 = NEW_SIZE;

**********************************************************************/
int
CosaGetParamValueString
    (
        char*                       pParamName,
        char*                       pBuffer,
        PULONG                      pulSize
    )
{
#ifdef ENABLE_ETHERNET_TR181_REMOTE_CALL
    char acTmpReturnValue[1024] = {0};
    if (strstr(pParamName, ETHERNET_INTERFACE_OBJECT))
    {
        if (ANSC_STATUS_FAILURE == RdkBus_GetParamValues(ETH_COMPONENT_NAME, ETH_DBUS_PATH, pParamName, acTmpReturnValue))
        {
            CcspTraceError(("[%s][%d]Failed to get param value\n", __FUNCTION__, __LINE__));
            return -1;
        }
        strncpy(pBuffer, acTmpReturnValue, strlen(acTmpReturnValue));
        *pulSize = strlen(acTmpReturnValue) + 1;
        return 1;
    }
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
    /* Added possible LowerLayer Interafces */
    if(strstr(pParamName, PPP_INTERFACE_OBJECT) || strstr(pParamName, VLAN_INTERFACE_OBJECT) ||
        strstr(pParamName, PTM_INTERFACE_OBJECT) || strstr(pParamName, DSL_INTERFACE_OBJECT) ||
        strstr(pParamName,X_RDK_ETHERNET_INTERFACE_OBJECT))
    {
        if (ANSC_STATUS_FAILURE == RdkBus_GetParamValueFromAnyComp(pParamName, acTmpReturnValue))
        {
            CcspTraceError(("[%s][%d]Failed to get param value\n", __FUNCTION__, __LINE__));
            return -1;
        }
        strncpy(pBuffer, acTmpReturnValue, strlen(acTmpReturnValue)+1);
        *pulSize = strlen(acTmpReturnValue) + 1;
        return 0;
    }
#endif
#endif
    /* we should look up CR to find right component.
            if it's P&M component, we just call the global variable 
            Currently, we suppose all the parameter is from P&M. */


    return g_GetParamValueString(g_pDslhDmlAgent, pParamName, pBuffer, pulSize);

}

/**********************************************************************

    prototype:

        BOOL
        CosaGetParamValueBool
            (
                char*                       pParamName
            )

    description:

        This function is called to retrieve a UONG value of a parameter;

    argument:   
            char*                       pParamName
            The full name of the parameter;

    return:     the BOOL value;

**********************************************************************/
BOOL
CosaGetParamValueBool
    (
        char*                       pParamName
    )
{
#ifdef ENABLE_ETHERNET_TR181_REMOTE_CALL
    char acTmpReturnValue[256] = {0};
    if (strstr(pParamName, ETHERNET_INTERFACE_OBJECT))
    {
        if (ANSC_STATUS_FAILURE == RdkBus_GetParamValues(ETH_COMPONENT_NAME, ETH_DBUS_PATH, pParamName, acTmpReturnValue))
        {
            CcspTraceError(("[%s][%d]Failed to get param value\n", __FUNCTION__, __LINE__));
            return FALSE;
        }
        if (!(strcmp(acTmpReturnValue, "true")))
            return TRUE;
        else
            return FALSE;
    }
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
    /* Added possible LowerLayer Interafces */
    if(strstr(pParamName, PPP_INTERFACE_OBJECT) || strstr(pParamName, VLAN_INTERFACE_OBJECT) || 
        strstr(pParamName, PTM_INTERFACE_OBJECT) || strstr(pParamName, DSL_INTERFACE_OBJECT) || 
        strstr(pParamName,X_RDK_ETHERNET_INTERFACE_OBJECT))
    {
        if (ANSC_STATUS_FAILURE == RdkBus_GetParamValueFromAnyComp(pParamName, acTmpReturnValue))
        {
            CcspTraceError(("[%s][%d]Failed to get param value\n", __FUNCTION__, __LINE__));
            return FALSE;
        }
        if (!(strcmp(acTmpReturnValue, "true")))
            return TRUE;
        else
            return FALSE;
    }
#endif
#endif
    /* we should look up CR to find right component.
            if it's P&M component, we just call the global variable 
            Currently, we suppose all the parameter is from P&M. */

    return g_GetParamValueBool(g_pDslhDmlAgent, pParamName);
}

/**********************************************************************

    prototype:

        ULONG
        CosaGetInstanceNumberByIndex
            (
                char*                      pObjName,
                ULONG                      ulIndex
            );

    description:

        This function is called to retrieve the instance number specified by index;

    argument:   char*                      pObjName,
                The full object name;

                ULONG                      ulIndex
                The index specified;
            
    return:     the instance number;

**********************************************************************/
ULONG
CosaGetInstanceNumberByIndex
    (
        char*                      pObjName,
        ULONG                      ulIndex
    )
{
    /* we should look up CR to find right component.
            if it's P&M component, we just call the global variable 
            Currently, we suppose all the parameter is from P&M. */


    return g_GetInstanceNumberByIndex(g_pDslhDmlAgent, pObjName, ulIndex);
}


/**********************************************************************

    prototype:

        void *
        CosaGetRegistryRootFolder
           (
            )

    description:

        This function is called to retrieve RootFolder;

    argument:
            char*                       pParamName
            The full name of the parameter;

    return:     the ULONG value;

**********************************************************************/
void *
CosaGetRegistryRootFolder
    (
    )
{
    return g_GetRegistryRootFolder(g_pDslhDmlAgent);
}

/**********************************************************************

    prototype:

        void *
        CosaRegisterCallBackAfterInitDml
           (
              void * handle
            )

    description:

        This function is called to register callback aftert InitDataModel;

    argument:
            char*                       pParamName
            The full name of the parameter;

    return:     the ULONG value;

**********************************************************************/
void *
CosaRegisterCallBackAfterInitDml
    (
        void * callback
    )
{
    return g_RegisterCallBackAfterInitDml(g_pDslhDmlAgent, callback);
}

/**********************************************************************

    prototype:

        BOOL
        CosaCOSARepopulateTable
           (
              char * objName
            )

    description:

        This function is called to Repopulate object

    argument:
            char*                       objName

    return:     the BOOL value

**********************************************************************/

BOOL
CosaCOSARepopulateTable
    (
        char * objName
    )
{
    return g_COSARepopulateTable(g_pDslhDmlAgent, objName);
}

#if  defined(ENABLE_ETHERNET_TR181_REMOTE_CALL)
static ANSC_STATUS RdkBus_GetParamValues( char *pComponent, char *pBus, char *pParamName, char *pReturnVal )
{
    parameterValStruct_t   **retVal = NULL ;
    char                   *ParamName[1];
    int                    ret               = 0,
                           nval;

    //Assign address for get parameter name
    ParamName[0] = pParamName;

    ret = CcspBaseIf_getParameterValues(
                                    bus_handle,
                                    pComponent,
                                    pBus,
                                    ParamName,
                                    1,
                                    &nval,
                                    &retVal);

    //Copy the value
    if( CCSP_SUCCESS == ret )
    {
        if( NULL != retVal[0]->parameterValue )
        {
            memcpy( pReturnVal, retVal[0]->parameterValue, strlen( retVal[0]->parameterValue ) + 1 );
        }

        if( retVal )
        {
            free_parameterValStruct_t (bus_handle, nval, retVal);
        }

        return ANSC_STATUS_SUCCESS;
    }

    if( retVal )
    {
       free_parameterValStruct_t (bus_handle, nval, retVal);
    }

    return ANSC_STATUS_FAILURE;
}

#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
ANSC_STATUS RdkBus_GetParamNames( char *pComponent, char *pBus, char *pParamName, char a2cReturnVal[][256], int *pReturnSize )
{
    //CCSP_MESSAGE_BUS_INFO  *bus_info         = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    parameterInfoStruct_t  **retInfo;
    int                    ret               = 0,
                           nval;

    ret = CcspBaseIf_getParameterNames(
            bus_handle,
            pComponent,
            pBus,
            pParamName,
            1,
            &nval,
            &retInfo);

    //Copy the value
    if( CCSP_SUCCESS == ret )
    {
        int iLoopCount;

        *pReturnSize = nval;

        for( iLoopCount = 0; iLoopCount < nval; iLoopCount++ )
        {
            if( NULL != retInfo[iLoopCount]->parameterName )
            {
                //CcspTraceInfo(("%s parameterName[%d,%s]\n",__FUNCTION__,iLoopCount,retInfo[iLoopCount]->parameterName));
                snprintf( a2cReturnVal[iLoopCount], strlen( retInfo[iLoopCount]->parameterName ) + 1, "%s", retInfo[iLoopCount]->parameterName );
            }
        }

        if( retInfo )
        {
            free_parameterInfoStruct_t(bus_handle, nval, retInfo);
        }

        return ANSC_STATUS_SUCCESS;
    }

    if( retInfo )
    {
        free_parameterInfoStruct_t(bus_handle, nval, retInfo);
    }

    return ANSC_STATUS_FAILURE;

}

ANSC_STATUS PAM_GetInterfaceInstanceFromVlanmanager(char *pIfName, PUCHAR *pMatchedLowerLayer)
{

    if( ( NULL == pIfName ))
    {
        CcspTraceError(("%s Invalid Buffer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }


    char acTmpReturnValue[256] = { 0 },
         a2cTmpTableParams[10][256] = { 0 };
    INT  iLoopCount,
         iTotalNoofEntries;

    if ( ANSC_STATUS_FAILURE == RdkBus_GetParamValues( VLAN_COMPONENT_NAME, VLAN_DBUS_PATH, VLAN_ETHLINK_NOE_PARAM_NAME, acTmpReturnValue ) )
    {
        CcspTraceError(("%s %d Failed to get param value\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    //Total count
    iTotalNoofEntries = atoi( acTmpReturnValue );
    CcspTraceInfo(("%s %d - TotalNoofEntries:%d\n", __FUNCTION__, __LINE__, iTotalNoofEntries));

    if( 0 >= iTotalNoofEntries )
    {
        return ANSC_STATUS_SUCCESS;
    }


    //Get table names
    iTotalNoofEntries = 0;
    if ( ANSC_STATUS_FAILURE == RdkBus_GetParamNames( VLAN_COMPONENT_NAME, VLAN_DBUS_PATH, VLAN_ETHLINK_TABLE_NAME, a2cTmpTableParams , &iTotalNoofEntries ))
    {
        CcspTraceError(("%s %d Failed to get param value\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    //Traverse from loop
    for ( iLoopCount = 0; iLoopCount < iTotalNoofEntries; iLoopCount++ )
    {
        char acTmpQueryParam[256] = { 0 };

        //Query
        snprintf( acTmpQueryParam, sizeof(acTmpQueryParam ), "%sName", a2cTmpTableParams[ iLoopCount ] );

        memset( acTmpReturnValue, 0, sizeof( acTmpReturnValue ) );
        if ( ANSC_STATUS_FAILURE == RdkBus_GetParamValues( VLAN_COMPONENT_NAME, VLAN_DBUS_PATH, acTmpQueryParam, acTmpReturnValue ) )
        {
            CcspTraceError(("%s %d Failed to get param value\n", __FUNCTION__, __LINE__));
            continue;
        }

        //Compare ifname
        if( 0 == strncmp(acTmpReturnValue, pIfName, 256) )
        {
            char  tmpTableParam[256] = { 0 };

            //Copy table param
            snprintf( tmpTableParam, sizeof(tmpTableParam), "%s", a2cTmpTableParams[ iLoopCount ] );
            *pMatchedLowerLayer =  (PUCHAR)AnscCloneString(tmpTableParam);
            CcspTraceInfo(("%s %d pMatchedLowerLayer %s\n", __FUNCTION__, __LINE__,*pMatchedLowerLayer));
            break;
        }
    }
    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS RdkBus_GetParamValueFromAnyComp( char * pQuery, char *pValue)
{
    if ((pQuery == NULL) || (pValue == NULL))
    {
        CcspTraceError (("%s %d: invalid args..\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }


    int ret ;
    int size = 0;
    char dst_pathname_cr[256] = {0};
    componentStruct_t ** ppComponents = NULL;

    snprintf(dst_pathname_cr, sizeof(dst_pathname_cr) - 1, "eRT.%s", CCSP_DBUS_INTERFACE_CR);

    // Get the component name and dbus path which has the data model
    ret = CcspBaseIf_discComponentSupportingNamespace
        (
         bus_handle,
         dst_pathname_cr,
         pQuery,
         "",
         &ppComponents,
         &size
        );

    if ((ret != CCSP_SUCCESS) || (size <= 0))
    {
        CcspTraceError (("%s %d: CcspBaseIf_discComponentSupportingNamespace() call failed\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    // query the data model from the component
    CcspTraceInfo (("%s %d: quering dm:%s from component:%s of dbuspath:%s\n",
                __FUNCTION__, __LINE__, pQuery, ppComponents[0]->componentName, ppComponents[0]->dbusPath));
    if (RdkBus_GetParamValues (ppComponents[0]->componentName, ppComponents[0]->dbusPath, pQuery, pValue) != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError (("%s %d: CcspBaseIf_discComponentSupportingNamespace() call failed\n", __FUNCTION__, __LINE__));
        free_componentStruct_t(bus_handle, size, ppComponents);
        return ANSC_STATUS_FAILURE;
    }

    free_componentStruct_t(bus_handle, size, ppComponents);
    CcspTraceInfo (("%s %d: dm:%s got value %s\n", __FUNCTION__, __LINE__, pQuery, pValue));

    return ANSC_STATUS_SUCCESS;
}
#endif
#endif
