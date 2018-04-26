/************************************************************************************
  If not stated otherwise in this file or this component's Licenses.txt file the
  following copyright and licenses apply:

  Copyright 2018 RDK Management

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
**************************************************************************/

#ifdef CONFIG_CISCO_HOTSPOT
/**************************************************************************

    module: cosa_x_comcast-com_gre_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        zhicheng_qiu@cable.comcast.com

    -------------------------------------------------------------------

    revision:

        05/18/2015    initial revision.

**************************************************************************/


#ifndef  _COSA_X_COMCAST__COM_GRE_DML_H
#define  _COSA_X_COMCAST__COM_GRE_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

ULONG GreTunnel_GetEntryCount (  ANSC_HANDLE hInsContext  );	//
ULONG GreTunnelIf_GetEntryCount (  ANSC_HANDLE hInsContext  );	//

ANSC_HANDLE GreTunnel_GetEntry( ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber );	//
ANSC_HANDLE GreTunnelIf_GetEntry( ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber );	//

BOOL GreTunnel_GetParamBoolValue ( ANSC_HANDLE hInsContext, char*  ParamName, BOOL*  pBool); //
BOOL GreTunnelIf_GetParamBoolValue ( ANSC_HANDLE hInsContext, char*  ParamName, BOOL*  pBool); //

BOOL GreTunnel_GetParamUlongValue ( ANSC_HANDLE hInsContext, char* ParamName, ULONG* pUlong);	//
BOOL GreTunnelIf_GetParamUlongValue ( ANSC_HANDLE hInsContext, char* ParamName, ULONG* pUlong);	//

ULONG GreTunnel_GetParamStringValue ( ANSC_HANDLE  hInsContext, char* ParamName, char* pValue, ULONG* pUlSize );  //
ULONG GreTunnelIf_GetParamStringValue ( ANSC_HANDLE  hInsContext, char* ParamName, char* pValue, ULONG* pUlSize );  //

BOOL GreTunnel_GetParamIntValue ( ANSC_HANDLE hInsContext, char* ParamName, int*  pInt );	//
BOOL GreTunnelIf_GetParamIntValue ( ANSC_HANDLE hInsContext, char* ParamName, int*  pInt );	//

BOOL GreTunnel_SetParamBoolValue ( ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue); //
BOOL GreTunnelIf_SetParamBoolValue ( ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue); //

BOOL GreTunnel_SetParamUlongValue ( ANSC_HANDLE  hInsContext, char* ParamName, ULONG  uValuepUlong); //
BOOL GreTunnelIf_SetParamUlongValue ( ANSC_HANDLE  hInsContext, char* ParamName, ULONG  uValuepUlong); //

BOOL GreTunnel_SetParamStringValue ( ANSC_HANDLE hInsContext, char*  ParamName, char*  strValue ); //
BOOL GreTunnelIf_SetParamStringValue ( ANSC_HANDLE hInsContext, char*  ParamName, char*  strValue ); //

BOOL GreTunnel_SetParamIntValue ( ANSC_HANDLE hInsContext, char* ParamName, int value ); //	
BOOL GreTunnelIf_SetParamIntValue ( ANSC_HANDLE hInsContext, char* ParamName, int value ); //	

BOOL GreTunnel_Validate ( ANSC_HANDLE hInsContext, char* pReturnParamName, ULONG* puLength ); //	
BOOL GreTunnelIf_Validate ( ANSC_HANDLE hInsContext, char* pReturnParamName, ULONG* puLength ); //	

ULONG GreTunnel_Commit ( ANSC_HANDLE hInsContext );	//
ULONG GreTunnelIf_Commit ( ANSC_HANDLE hInsContext );	//

ULONG GreTunnel_Rollback( ANSC_HANDLE hInsContext);	//
ULONG GreTunnelIf_Rollback( ANSC_HANDLE hInsContext); //

BOOL GreTunnelStat_GetParamUlongValue ( ANSC_HANDLE hInsContext, char* ParamName, ULONG* pUlong );	//

#endif
#endif
