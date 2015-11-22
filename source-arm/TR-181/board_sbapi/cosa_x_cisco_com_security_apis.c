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

    module: cosa_x_cisco_com_security_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "cosa_x_cisco_com_security_apis.h"

#ifdef _COSA_SIM_

/*
BOOLEAN                       ApplyFirewallSettings;
BOOLEAN                       CleanLog;
BOOLEAN                       TriggerEmailLog[64];
BOOLEAN                       EmailEnable[64];
CHAR                            FilterWebTraffic[64];
CHAR                            TrafficDetect[64];
CHAR                            FilterLanTraffic[64];
CHAR                            AllowPassthrough[64];
CHAR                            EmailSendTo[64];
CHAR                            EmailServer[64];
CHAR                            EmailUserName[64];
CHAR                            EmailPassword[64];

*/
COSA_DML_SECURITY_CFG g_SecurityConfig = 
{
    FALSE, /*BOOLEAN                         ApplyFirewallSettings;*/
    FALSE, /*BOOLEAN                         CleanLog;*/
    FALSE, /*BOOLEAN                         TriggerEmailLog[64];*/
    FALSE, /*BOOLEAN                         EmailEnable[64];*/
    "Cookies,Java,ActiveX,PopUp,FragmentedIP", /*CHAR                            FilterWebTraffic[64];*/
    "portScan,ipFlood", /*CHAR                            TrafficDetect[64];*/
    "http,https,ICMP,Multicast,P2P,IDENT", /*CHAR                            FilterLanTraffic[64];*/
    "IPsec, PPTP, L2TP, Multicast", /*CHAR                            AllowPassthrough[64];*/
    "USGv2@comcast.com",/*CHAR                            EmailSendTo[64];*/
    "USGv2.mail.comcast.com", /*CHAR                            EmailServer[64];*/
    "username", /*CHAR                            EmailUserName[64];*/
    "password", /*CHAR                            EmailPassword[64];*/
    "xxx@cisco.com", /*CHAR                            EmailFromAddress[64];*/
    FALSE,
    FALSE,
    FALSE,
};


COSA_DML_FIREWALL_CFG2 g_FirewallConfig2 = 
{
    COSA_DML_FIREWALL_LEVEL_Low, 
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE
};


COSA_SECURITY_IA_POLICY2 g_SecurityInternetAccess[10] = 
{
    {1, TRUE, FALSE, FALSE, "Policy1", TRUE, {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,"00:00", "06:00"}, 2, {{0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, {0x02, 0x02, 0x02, 0x02, 0x02, 0x02}},
    2, {{192, 168, 1, 66}, {192, 168, 1, 67}}, 2, {{{192, 168, 1, 10}, {192, 168, 1, 12}}, {{192, 168, 1, 20}, {192, 168, 1, 22}}}, 2, {{0, "abc", "www.abc.com"}, {0, "abc", "www.def.com"}}, 2, {{0, "", "keyword1"}, {0, "", "keyword2"}},
    2, {{1, "app1", TRUE, COSA_DML_IA_APP_PROTO_Tcp, 1000, 1010}, {2, "app2", TRUE, COSA_DML_IA_APP_PROTO_Udp, 2000, 2020}} },

    {2, TRUE, TRUE, TRUE, "Policy2", TRUE, {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, "02:00", "08:00"}, 2, {{0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, {0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    2, {{192, 168, 1, 100}, {192, 168, 1, 101}}, 2, {{{192, 168, 1, 110}, {192, 168, 1, 112}}, {{192, 168, 1, 220}, {192, 168, 1, 222}}}, 2, {{0, "abc", "www.ghi.com"}, {0, "abc", "www.jkl.com"}}, 2, {{0, "", "keyword3"}, {0, "", "keyword4"}},
    2, {{1, "app3", TRUE, COSA_DML_IA_APP_PROTO_Tcp, 10000, 10101}, {2, "app4", TRUE, COSA_DML_IA_APP_PROTO_Udp, 20000, 20200}} }
};

COSA_DML_IA_LOG_ENTRY g_SecurityLog[2] = 
{
    {"2011-01-01T00:00:00Z", "Deny", {192, 168, 1, 23}, "tom", {192, 168, 1, 24}, 11, "dummy1"},
    {"2011-01-02T00:00:00Z", "Deny", {192, 168, 2, 66}, "tom", {192, 168, 1, 67}, 12, "dummy2"}
};

ULONG g_NumOfPolicies = 2;
ULONG g_NumOfLogEntries = 2;


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
            );

        Description:
	        This is the initialization routine for XYZ backend.

        Arguments:      ANSC_HANDLE                 hDml
                        Opaque handle from DM adapter. 
        

	                    PANSC_HANDLE                phContext		
                        Opaque handle passed back from backend, 
                        needed by CosaDmlNatXyz() routines.

        Return:         Status of operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlSecurityGetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_SECURITY_CFG     pCfg
            );

    Description:

        Backend implementation to get Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_SECURITY_CFG     pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlSecurityGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_SECURITY_CFG      pCfg
    )
{

    pCfg->ApplyFirewallSettings  = g_SecurityConfig.ApplyFirewallSettings;
    pCfg->CleanLog               = g_SecurityConfig.CleanLog;
    pCfg->TriggerEmailLog        = g_SecurityConfig.TriggerEmailLog;
    pCfg->EmailEnable            = g_SecurityConfig.EmailEnable;
    pCfg->EmailParentalControlBreach = g_SecurityConfig.EmailParentalControlBreach;
    pCfg->EmailAlertsOrWarnings = g_SecurityConfig.EmailAlertsOrWarnings;
    _ansc_strcpy(pCfg->FilterWebTraffic, g_SecurityConfig.FilterWebTraffic);
    _ansc_strcpy(pCfg->TrafficDetect, g_SecurityConfig.TrafficDetect);
    _ansc_strcpy(pCfg->FilterLanTraffic, g_SecurityConfig.FilterLanTraffic);
    _ansc_strcpy(pCfg->AllowPassthrough, g_SecurityConfig.AllowPassthrough);
    _ansc_strcpy(pCfg->EmailSendTo, g_SecurityConfig.EmailSendTo);
    _ansc_strcpy(pCfg->EmailServer, g_SecurityConfig.EmailServer);
    _ansc_strcpy(pCfg->EmailUserName, g_SecurityConfig.EmailUserName);
    _ansc_strcpy(pCfg->EmailPassword, g_SecurityConfig.EmailPassword);
    _ansc_strcpy(pCfg->EmailFromAddress, g_SecurityConfig.EmailFromAddress);
    pCfg->EmailFirewallBreach   = g_SecurityConfig.EmailFirewallBreach;

#if 1
        AnscTraceWarning(("pCfg->ApplyFirewallSettings :%s\n", pCfg->ApplyFirewallSettings?"TRUE":"FALSE"));
        AnscTraceWarning(("pCfg->CleanLog :%s\n", pCfg->CleanLog?"TRUE":"FALSE"));
        AnscTraceWarning(("pCfg->TriggerEmailLog :%s\n", pCfg->TriggerEmailLog?"TRUE":"FALSE"));
        AnscTraceWarning(("pCfg->EmailEnable :%s\n", pCfg->EmailEnable?"TRUE":"FALSE"));
        
        AnscTraceWarning(("pCfg->FilterWebTraffic :%s\n", pCfg->FilterWebTraffic));
        AnscTraceWarning(("pCfg->TrafficDetect :%s\n", pCfg->TrafficDetect));
        AnscTraceWarning(("pCfg->FilterLanTraffic :%s\n", pCfg->FilterLanTraffic));
        AnscTraceWarning(("pCfg->AllowPassthrough :%s\n", pCfg->AllowPassthrough));
        AnscTraceWarning(("pCfg->EmailSendTo :%s\n", pCfg->EmailSendTo));
        AnscTraceWarning(("pCfg->EmailServer :%s\n", pCfg->EmailServer));
        AnscTraceWarning(("pCfg->EmailUserName :%s\n", pCfg->EmailUserName));
        AnscTraceWarning(("pCfg->EmailPassword :%s\n", pCfg->EmailPassword));
#endif

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlSecuritySetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_SECURITY_CFG      pCfg
            );

    Description:

        Backend implementation to set Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_SECURITY_CFG   pCfg
                  Pointer of the configuration to be set.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlSecuritySetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_SECURITY_CFG      pCfg
    )
{
    /* Debug information */
#if 1
    AnscTraceWarning(("pCfg->ApplyFirewallSettings :%s\n", pCfg->ApplyFirewallSettings?"TRUE":"FALSE"));
    AnscTraceWarning(("pCfg->CleanLog :%s\n", pCfg->CleanLog?"TRUE":"FALSE"));
    AnscTraceWarning(("pCfg->TriggerEmailLog :%s\n", pCfg->TriggerEmailLog?"TRUE":"FALSE"));
    AnscTraceWarning(("pCfg->EmailEnable :%s\n", pCfg->EmailEnable?"TRUE":"FALSE"));
    
    AnscTraceWarning(("pCfg->FilterWebTraffic :%s\n", pCfg->FilterWebTraffic));
    AnscTraceWarning(("pCfg->TrafficDetect :%s\n", pCfg->TrafficDetect));
    AnscTraceWarning(("pCfg->FilterLanTraffic :%s\n", pCfg->FilterLanTraffic));
    AnscTraceWarning(("pCfg->AllowPassthrough :%s\n", pCfg->AllowPassthrough));
    AnscTraceWarning(("pCfg->EmailSendTo :%s\n", pCfg->EmailSendTo));
    AnscTraceWarning(("pCfg->EmailServer :%s\n", pCfg->EmailServer));
    AnscTraceWarning(("pCfg->EmailUserName :%s\n", pCfg->EmailUserName));
    AnscTraceWarning(("pCfg->EmailPassword :%s\n", pCfg->EmailPassword));
#endif

    /*Apply to backend. */

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlFirewallGetConfig2
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_FIREWALL_CFG2     pCfg
            );

    Description:

        Backend implementation to get Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_FIREWALL_CFG2     pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlFirewallGetConfig2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG2     pCfg
    )
{
    pCfg->FirewallLevel                   = g_FirewallConfig2.FirewallLevel;
    pCfg->FilterAnonymousInternetRequests = g_FirewallConfig2.FilterAnonymousInternetRequests;
    pCfg->FilterIDENT                     = g_FirewallConfig2.FilterIDENT;
    pCfg->FilterMulticast                 = g_FirewallConfig2.FilterMulticast;
    pCfg->FilterNATRedirection            = g_FirewallConfig2.FilterNATRedirection;
    pCfg->IPSecPassthrough                = g_FirewallConfig2.IPSecPassthrough;
    pCfg->L2TPPassthrough                 = g_FirewallConfig2.L2TPPassthrough;
    pCfg->PPTPPassthrough                 = g_FirewallConfig2.PPTPPassthrough;
    pCfg->WebBlockActiveX                 = g_FirewallConfig2.WebBlockActiveX;
    pCfg->WebBlockCookies                 = g_FirewallConfig2.WebBlockCookies;
    pCfg->WebBlockJava                    = g_FirewallConfig2.WebBlockJava;
    pCfg->WebBlockProxy                   = g_FirewallConfig2.WebBlockProxy;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlFirewallSetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_FIREWALL_CFG      pCfg
            );

    Description:

        Backend implementation to set Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_FIREWALL_CFG      pCfg
                  Pointer of the configuration to be set.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlFirewallSetConfig2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG2     pCfg
    )
{
    g_FirewallConfig2.FirewallLevel                   = pCfg->FirewallLevel;
    g_FirewallConfig2.FilterAnonymousInternetRequests = pCfg->FilterAnonymousInternetRequests;
    g_FirewallConfig2.FilterIDENT                     = pCfg->FilterIDENT;
    g_FirewallConfig2.FilterMulticast                 = pCfg->FilterMulticast;
    g_FirewallConfig2.FilterNATRedirection            = pCfg->FilterNATRedirection;
    g_FirewallConfig2.IPSecPassthrough                = pCfg->IPSecPassthrough;
    g_FirewallConfig2.L2TPPassthrough                 = pCfg->L2TPPassthrough;
    g_FirewallConfig2.PPTPPassthrough                 = pCfg->PPTPPassthrough;
    g_FirewallConfig2.WebBlockActiveX                 = pCfg->WebBlockActiveX;
    g_FirewallConfig2.WebBlockCookies                 = pCfg->WebBlockCookies;
    g_FirewallConfig2.WebBlockJava                    = pCfg->WebBlockJava;
    g_FirewallConfig2.WebBlockProxy                   = pCfg->WebBlockProxy;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaGetNumberOfPolicies
            (
                ANSC_HANDLE                 hContext
            );

    Description:

        Backend implementation to set Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.


    Return:       The number of the Access Policies.

**********************************************************************/
ULONG
CosaDmlIaGetNumberOfPolicies
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_NumOfPolicies;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaGetPolicy
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_IA_POLICY         pEntry
            );

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index number of the IA policy.

                  PCOSA_DML_IA_POLICY         pEntry
                  Policy struct to be filled in.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaGetPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY         pEntry
    )
{
    pEntry->InstanceNumber = g_SecurityInternetAccess[ulIndex].ulInstanceNumber;
    pEntry->bEnabled = g_SecurityInternetAccess[ulIndex].bEnabled;
    pEntry->EnableLanHostMACAddresses = g_SecurityInternetAccess[ulIndex].EnableLanHostMACAddresses;
    pEntry->AllowLanHostMACAddresses = g_SecurityInternetAccess[ulIndex].AllowLanHostMACAddresses;
    AnscCopyString(pEntry->Alias, g_SecurityInternetAccess[ulIndex].Name);

    pEntry->LanHost.bUseLanHosts = g_SecurityInternetAccess[ulIndex].bUseLanHosts;
    pEntry->LanHost.MacCount     = g_SecurityInternetAccess[ulIndex].ulNumOfMac;
    pEntry->LanHost.IpCount      = g_SecurityInternetAccess[ulIndex].ulNumOfIp;
    pEntry->LanHost.IprCount      = g_SecurityInternetAccess[ulIndex].ulNumOfIp;
    AnscCopyMemory(pEntry->LanHost.MacList, g_SecurityInternetAccess[ulIndex].MacList, sizeof(COSA_DML_IA_LH_MAC) * pEntry->LanHost.MacCount);
    AnscCopyMemory(pEntry->LanHost.IpList,  g_SecurityInternetAccess[ulIndex].IPList , sizeof(COSA_DML_IA_LH_IP ) * pEntry->LanHost.IpCount );
    AnscCopyMemory(pEntry->LanHost.IprList, g_SecurityInternetAccess[ulIndex].IPRList ,sizeof(COSA_DML_IA_LH_IPR) * pEntry->LanHost.IprCount);

/*
    pEntry->ulNumberOfApplications = g_SecurityInternetAccess[ulIndex].ulNumOfApp;
    pEntry->ulNumberOfKeywords     = g_SecurityInternetAccess[ulIndex].ulNumOfKeyword;
    pEntry->ulNumberOfURLs         = g_SecurityInternetAccess[ulIndex].ulNumOfUrl;
*/

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaSetPolicyValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            )

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index number of the IA policy.

                  ULONG                       ulInstanceNumber
                  Instance number to set back.

                  char*                       pAlias
                  Alias to set back.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaSetPolicyValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaGetPolicyByName
            (
                ANSC_HANDLE                 hContext,
                char*                       pName,
                PCOSA_DML_IA_POLICY         pEntry
            );

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index number of the IA policy.

                  PCOSA_DML_IA_POLICY         pEntry
                  Policy struct to be filled in.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaGetPolicyByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInsNum,
        PCOSA_DML_IA_POLICY         pEntry
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulInsNum )
        {
            pEntry->bEnabled = g_SecurityInternetAccess[i].bEnabled;
            pEntry->EnableLanHostMACAddresses = g_SecurityInternetAccess[i].EnableLanHostMACAddresses;
            pEntry->AllowLanHostMACAddresses = g_SecurityInternetAccess[i].AllowLanHostMACAddresses;
            AnscCopyString(pEntry->Alias, g_SecurityInternetAccess[i].Name);

            pEntry->LanHost.bUseLanHosts = g_SecurityInternetAccess[i].bUseLanHosts;
            pEntry->LanHost.MacCount     = g_SecurityInternetAccess[i].ulNumOfMac;
            pEntry->LanHost.IpCount      = g_SecurityInternetAccess[i].ulNumOfIp;
            pEntry->LanHost.IprCount      = g_SecurityInternetAccess[i].ulNumOfIp;
            AnscCopyMemory(pEntry->LanHost.MacList, g_SecurityInternetAccess[i].MacList, sizeof(COSA_DML_IA_LH_MAC) * pEntry->LanHost.MacCount);
            AnscCopyMemory(pEntry->LanHost.IpList,  g_SecurityInternetAccess[i].IPList , sizeof(COSA_DML_IA_LH_IP ) * pEntry->LanHost.IpCount );
            AnscCopyMemory(pEntry->LanHost.IprList, g_SecurityInternetAccess[i].IPRList ,sizeof(COSA_DML_IA_LH_IPR) * pEntry->LanHost.IprCount);

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaAddPolicy
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_IA_POLICY         pEntry
            );

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_IA_POLICY         pEntry
                  Policy to be added.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaAddPolicy
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IA_POLICY         pEntry
    )
{
    g_SecurityInternetAccess[g_NumOfPolicies].bEnabled = pEntry->bEnabled;
    g_SecurityInternetAccess[g_NumOfPolicies].EnableLanHostMACAddresses = pEntry->EnableLanHostMACAddresses;
    g_SecurityInternetAccess[g_NumOfPolicies].AllowLanHostMACAddresses = pEntry->AllowLanHostMACAddresses;
    AnscCopyString(g_SecurityInternetAccess[g_NumOfPolicies].Name, pEntry->Alias);

    g_SecurityInternetAccess[g_NumOfPolicies].bUseLanHosts = pEntry->LanHost.bUseLanHosts;
    g_SecurityInternetAccess[g_NumOfPolicies].ulNumOfMac   = pEntry->LanHost.MacCount;
    g_SecurityInternetAccess[g_NumOfPolicies].ulNumOfIp    = pEntry->LanHost.IpCount;
    g_SecurityInternetAccess[g_NumOfPolicies].ulNumOfIp    = pEntry->LanHost.IprCount;
    AnscCopyMemory(g_SecurityInternetAccess[g_NumOfPolicies].MacList, pEntry->LanHost.MacList, sizeof(COSA_DML_IA_LH_MAC) * pEntry->LanHost.MacCount);
    AnscCopyMemory(g_SecurityInternetAccess[g_NumOfPolicies].IPList,  pEntry->LanHost.IpList,  sizeof(COSA_DML_IA_LH_IP ) * pEntry->LanHost.IpCount );
    AnscCopyMemory(g_SecurityInternetAccess[g_NumOfPolicies].IPRList, pEntry->LanHost.IprList, sizeof(COSA_DML_IA_LH_IPR) * pEntry->LanHost.IprCount);

    g_NumOfPolicies++;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaDelPolicy
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNum
            );

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be added.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaDelPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    ULONG                           i = 0;
    ULONG                           j = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulInstanceNumber )
        {
            for ( j = i; j < g_NumOfPolicies; j++ )
            {
                AnscCopyMemory
                    (
                        &g_SecurityInternetAccess[j], 
                        &g_SecurityInternetAccess[j + 1], 
                        sizeof(COSA_SECURITY_IA_POLICY2)
                    );
            }

            g_NumOfPolicies--;

            break;
        }
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaSetPolicy
            (
                ANSC_HANDLE                 hContext,
                char*                       pName
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be set.

                  PCOSA_DML_IA_POLICY         pEntry
                  The new policy to be set.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaSetPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY         pEntry
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulInstanceNumber )
        {
            g_SecurityInternetAccess[i].bEnabled = pEntry->bEnabled;
            g_SecurityInternetAccess[i].EnableLanHostMACAddresses = pEntry->EnableLanHostMACAddresses;
            g_SecurityInternetAccess[i].AllowLanHostMACAddresses = pEntry->AllowLanHostMACAddresses;
            AnscCopyString(g_SecurityInternetAccess[i].Name, pEntry->Alias);

            g_SecurityInternetAccess[i].bUseLanHosts = pEntry->LanHost.bUseLanHosts;
            g_SecurityInternetAccess[i].ulNumOfMac   = pEntry->LanHost.MacCount;
            g_SecurityInternetAccess[i].ulNumOfIp    = pEntry->LanHost.IpCount;
            g_SecurityInternetAccess[i].ulNumOfIp    = pEntry->LanHost.IprCount;
            AnscCopyMemory(g_SecurityInternetAccess[i].MacList, pEntry->LanHost.MacList, sizeof(COSA_DML_IA_LH_MAC) * pEntry->LanHost.MacCount);
            AnscCopyMemory(g_SecurityInternetAccess[i].IPList,  pEntry->LanHost.IpList,  sizeof(COSA_DML_IA_LH_IP ) * pEntry->LanHost.IpCount );
            AnscCopyMemory(g_SecurityInternetAccess[i].IPRList, pEntry->LanHost.IprList, sizeof(COSA_DML_IA_LH_IPR) * pEntry->LanHost.IprCount);

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaGetPolicySchedule
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber,
                PCOSA_DML_IA_POLICY_SCH     pEntry
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be set.

                  PCOSA_DML_IA_POLICY         pEntry
                  The new policy to be set.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaGetPolicySchedule
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY_SCH     pEntry
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulInstanceNumber )
        {
            *pEntry = g_SecurityInternetAccess[i].Schedule;

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaGetPolicySchedule
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber,
                PCOSA_DML_IA_POLICY_SCH     pEntry
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be set.

                  PCOSA_DML_IA_POLICY         pEntry
                  The new policy to be set.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaSetPolicySchedule
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY_SCH     pEntry
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulInstanceNumber )
        {
            g_SecurityInternetAccess[i].Schedule = *pEntry;

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetNumberOfUrls
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be set.

                  PCOSA_DML_IA_POLICY         pEntry
                  The new policy to be set.


    Return:       The number of the Access Policies.

**********************************************************************/

ULONG
CosaDmlIaPolicyGetNumberOfUrls
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            return g_SecurityInternetAccess[i].ulNumOfUrl;
        }
    }

    return 0;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetUrl
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                PCOSA_DML_IA_POLICY_URL     pUrl
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of policy to be set.

                  ULONG                       ulIndex
                  Index number of the IP range entry

                  PCOSA_DML_IA_LH_IPR         pEntry
                  The IP range entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyGetUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_URL     pUrl
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            AnscCopyString(pUrl->Alias, g_SecurityInternetAccess[i].URLList[ulIndex].Alias);
            AnscCopyString(pUrl->Url,   g_SecurityInternetAccess[i].URLList[ulIndex].Url  );

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetUrlByInsNum
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulURLInstanceNumber,
                PCOSA_DML_IA_POLICY_URL     pUrl
            )

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  The name of policy to be set.

                  ULONG                       ulURLInstanceNumber
                  Instance number of the blocked url entry

                  PCOSA_DML_IA_POLICY_URL     pUrl
                  The blocked url entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyGetUrlByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulURLInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl
    )
{
    ULONG                           i = 0;
    ULONG                           j = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            for ( j = 0; j < g_SecurityInternetAccess[i].ulNumOfUrl; j++ )
            {
                if ( g_SecurityInternetAccess[i].URLList[j].InstanceNumber == ulURLInstanceNumber )
                {
                    AnscCopyString(pUrl->Alias, g_SecurityInternetAccess[i].URLList[j].Alias);
                    AnscCopyString(pUrl->Url,   g_SecurityInternetAccess[i].URLList[j].Url  );
                }
            }

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetUrlValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            )

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index number of the IA policy.

                  ULONG                       ulInstanceNumber
                  Instance number to set back.

                  char*                       pAlias
                  Alias to set back.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicySetUrlValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyAddUrl
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_URL     pUrl
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  The Instance Number of the policy to be set.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be added


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyAddUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl
    )
{
    ULONG                           i       = 0;
    ULONG                           ulIndex = g_SecurityInternetAccess[i].ulNumOfUrl;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            g_SecurityInternetAccess[i].URLList[ulIndex].InstanceNumber = pUrl->InstanceNumber;
            AnscCopyString(g_SecurityInternetAccess[i].URLList[ulIndex].Alias, pUrl->Alias);
            AnscCopyString(g_SecurityInternetAccess[i].URLList[ulIndex].Url, pUrl->Url);

            g_SecurityInternetAccess[i].ulNumOfUrl++;

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyDelUrl
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_URL     pUrl
            )

    Description:

        Backend implementation to delete the IP range entry.

    Arguments:    ANSC_HANLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of the corresponding policy.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be deleted.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicyDelUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl        /* Identified by InstanceNumber */
    )
{
    PCOSA_SECURITY_IA_POLICY2       pPolicy = NULL;
    ULONG                           i       = 0;
    ULONG                           j       = 0;
    ULONG                           k       = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            pPolicy = &g_SecurityInternetAccess[i];

            for ( j = 0; j < pPolicy->ulNumOfUrl; j++ )
            {
                if ( pPolicy->URLList[j].InstanceNumber == pUrl->InstanceNumber )
                {
                    for ( k = j; k < pPolicy->ulNumOfUrl; k++ )
                    {
                        AnscCopyMemory
                            (
                                &pPolicy->URLList[k], 
                                &pPolicy->URLList[k + 1] ,
                                sizeof(COSA_DML_IA_POLICY_URL)
                            );
                    }

                    g_SecurityInternetAccess[i].ulNumOfUrl--;

                    return ANSC_STATUS_SUCCESS;
                }
            }
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetUrl
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_URL     pUrl
            );

    Description:

        Backend implementation to update the IP range entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of the corresponding policy.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be updated.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicySetUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl        /* Identified by InstanceNumber */
    )
{
    ULONG                           i       = 0;
    ULONG                           j       = 0;
    PCOSA_SECURITY_IA_POLICY2       pPolicy = NULL;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            pPolicy = &g_SecurityInternetAccess[i];

            for ( j = 0; j < pPolicy->ulNumOfUrl; j++ )
            {
                if ( pPolicy->URLList[j].InstanceNumber == pUrl->InstanceNumber )
                {
                    AnscCopyString(pPolicy->URLList[j].Alias, pUrl->Alias);
                    AnscCopyString(pPolicy->URLList[j].Url, pUrl->Url);

                    return ANSC_STATUS_SUCCESS;
                }
            }
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlIaPolicyGetNumberOfKeywords
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber
            );


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  The Instance Number of policy.

    Return:       The number of the Access Policies.

**********************************************************************/

ULONG
CosaDmlIaPolicyGetNumberOfKeywords
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            return g_SecurityInternetAccess[i].ulNumOfKeyword;
        }
    }

    return 0;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetKeyword
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                PCOSA_DML_IA_POLICY_KEYWORD pKeyword
            );


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber,
                  The Instance Number of policy.

                  ULONG                       ulIndex
                  Index number of the blocked keyword entry

                  PCOSA_DML_IA_POLICY_KEYWORD pKeyword
                  The blocked keyword entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyGetKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber, ulPolicyInstanceNumber )
        {
            pKeyword->InstanceNumber = g_SecurityInternetAccess[i].KeywordList[ulIndex].InstanceNumber;

            AnscCopyString(pKeyword->Alias, g_SecurityInternetAccess[i].KeywordList[ulIndex].Alias);
            AnscCopyString(pKeyword->Keyword, g_SecurityInternetAccess[i].KeywordList[ulIndex].Keyword);

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetKeywordByInsNum
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulKeywordInstanceNumber,
                PCOSA_DML_IA_POLICY_KEYWORD pKeyword
            )

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  The name of policy to be set.

                  ULONG                       ulKeywordInstanceNumber
                  Instance number of the blocked keyword entry

                  PCOSA_DML_IA_POLICY_KEYWORD pKeyword
                  The blocked keyword entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicyGetKeywordByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulKeywordInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    )
{
    ULONG                           i = 0;
    ULONG                           j = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            for ( j = 0; j < g_SecurityInternetAccess[i].ulNumOfKeyword; j++ )
            {
                if ( g_SecurityInternetAccess[i].KeywordList[j].InstanceNumber == ulKeywordInstanceNumber )
                {
                    AnscCopyString(pKeyword->Alias,   g_SecurityInternetAccess[i].KeywordList[j].Alias);
                    AnscCopyString(pKeyword->Keyword, g_SecurityInternetAccess[i].KeywordList[j].Keyword);
                }
            }

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetKeywordValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            )

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  Instance number of the IA policy.

                  ULONG                       ulIndex
                  Index of keyword entry.

                  ULONG                       ulInstanceNumber
                  Instance number to set back.

                  char*                       pAlias
                  Alias to set back.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicySetKeywordValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    return ANSC_STATUS_SUCCESS;
}



/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyAddKeyword
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_KEYWORD pKeyword
            );


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of policy to be set.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be added


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyAddKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    )
{
    ULONG                           i       = 0;
    ULONG                           ulIndex = g_SecurityInternetAccess[i].ulNumOfKeyword;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            AnscCopyMemory(&g_SecurityInternetAccess[i].KeywordList[ulIndex], pKeyword, sizeof(COSA_DML_IA_POLICY_KEYWORD));

            g_SecurityInternetAccess[i].ulNumOfKeyword++;

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyDelKeyword
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_KEYWORD pKeyword
            )


    Description:

        Backend implementation to delete the IP range entry.

    Arguments:    ANSC_HANLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of the corresponding policy.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be deleted.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicyDelKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    )
{
    PCOSA_SECURITY_IA_POLICY2       pPolicy = NULL;
    ULONG                           i       = 0;
    ULONG                           j       = 0;
    ULONG                           k       = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            pPolicy = &g_SecurityInternetAccess[i];

            for ( j = 0; j < pPolicy->ulNumOfKeyword; j++ )
            {
                if ( pPolicy->KeywordList[j].InstanceNumber == pKeyword->InstanceNumber )
                {
                    for ( k = j; k < pPolicy->ulNumOfKeyword; k++ )
                    {
                        AnscCopyMemory
                            (
                                &pPolicy->KeywordList[k], 
                                &pPolicy->KeywordList[k + 1],
                                sizeof(COSA_DML_IA_POLICY_KEYWORD)
                            );
                    }

                    g_SecurityInternetAccess[i].ulNumOfKeyword--;

                    return ANSC_STATUS_SUCCESS;
                }
            }
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetKeyword
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_KEYWORD pKeyword
            );


    Description:

        Backend implementation to update the IP range entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber,
                  The Instance Number of the corresponding policy.

                  PCOSA_DML_IA_POLICY_KEYWORD pKeyword
                  Blocked keyword entry to be updated.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicySetKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    )
{
    ULONG                           i       = 0;
    ULONG                           j       = 0;
    PCOSA_SECURITY_IA_POLICY2       pPolicy = NULL;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            pPolicy = &g_SecurityInternetAccess[i];

            for ( j = 0; j < pPolicy->ulNumOfKeyword; j++ )
            {
                if ( pPolicy->KeywordList[j].InstanceNumber == pKeyword->InstanceNumber )
                {
                    AnscCopyMemory(&pPolicy->KeywordList[j], pKeyword, sizeof(COSA_DML_IA_POLICY_KEYWORD));

                    return ANSC_STATUS_SUCCESS;
                }
            }
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlIaPolicyGetNumberOfApps
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber
            );


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be set.

                  PCOSA_DML_IA_POLICY         pEntry
                  The new policy to be set.


    Return:       The number of the Access Policies.

**********************************************************************/

ULONG
CosaDmlIaPolicyGetNumberOfApps
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            return g_SecurityInternetAccess[i].ulNumOfApp;
        }
    }

    return 0;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetApp
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                PCOSA_DML_IA_POLICY_APP     pApp
            );


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of policy to be set.

                  ULONG                       ulIndex
                  Index number of the IP range entry

                  PCOSA_DML_IA_LH_IPR         pEntry
                  The IP range entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyGetApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_APP     pApp
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            AnscCopyString(pApp->Alias, g_SecurityInternetAccess[i].AppList[ulIndex].Alias);
            pApp->InstanceNumber  = g_SecurityInternetAccess[i].AppList[ulIndex].InstanceNumber;
            pApp->IsWellKnown     = g_SecurityInternetAccess[i].AppList[ulIndex].IsWellKnown;
            pApp->Protocol        = g_SecurityInternetAccess[i].AppList[ulIndex].Protocol;
            pApp->StartPortNumber = g_SecurityInternetAccess[i].AppList[ulIndex].StartPortNumber;
            pApp->EndPortNumber   = g_SecurityInternetAccess[i].AppList[ulIndex].EndPortNumber;

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetAppByInsNum
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulAppInstanceNumber,
                PCOSA_DML_IA_POLICY_APP     pApp
            )

    Description:

        Backend implementation to get App entry by instance number.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  The instance number of policy to be get.

                  ULONG                       ulAppInstanceNumber
                  Instance number of the blocked app entry

                  PCOSA_DML_IA_POLICY_APP     pApp
                  The blocked app entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyGetAppByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulAppInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    )
{
    ULONG                           i = 0;
    ULONG                           j = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            for ( j = 0; j < g_SecurityInternetAccess[i].ulNumOfApp; j++ )
            {
                if ( g_SecurityInternetAccess[i].AppList[j].InstanceNumber == ulAppInstanceNumber )
                {
                    AnscCopyMemory(pApp, &g_SecurityInternetAccess[i].AppList[j], sizeof(COSA_DML_IA_POLICY_APP));

                    return ANSC_STATUS_SUCCESS;
                }
            }   
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetKeywordValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            )

    Description:

        Backend implementation to set back instance number and alias.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  Instance number of the IA policy.

                  ULONG                       ulIndex
                  Index of keyword entry.

                  ULONG                       ulInstanceNumber
                  Instance number to set back.

                  char*                       pAlias
                  Alias to set back.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicySetAppValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyAddApp
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_APP     pApp
            )


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of policy to be set.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be added


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyAddApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    )
{
    ULONG                           i       = 0;
    ULONG                           ulIndex = g_SecurityInternetAccess[i].ulNumOfApp;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber, ulPolicyInstanceNumber, TRUE )
        {
            AnscCopyString(g_SecurityInternetAccess[i].AppList[ulIndex].Alias, pApp->Alias);
            g_SecurityInternetAccess[i].AppList[ulIndex].IsWellKnown     = pApp->IsWellKnown;
            g_SecurityInternetAccess[i].AppList[ulIndex].Protocol        = pApp->Protocol;
            g_SecurityInternetAccess[i].AppList[ulIndex].StartPortNumber = pApp->StartPortNumber;
            g_SecurityInternetAccess[i].AppList[ulIndex].EndPortNumber   = pApp->EndPortNumber;

            g_SecurityInternetAccess[i].ulNumOfApp++;

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyDelBlockedApp
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_APP     pApp
            );


    Description:

        Backend implementation to delete the IP range entry.

    Arguments:    ANSC_HANLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of the corresponding policy.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be deleted.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicyDelBlockedApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    )
{
    PCOSA_SECURITY_IA_POLICY2       pPolicy = NULL;
    ULONG                           i       = 0;
    ULONG                           j       = 0;
    ULONG                           k       = 0;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            pPolicy = &g_SecurityInternetAccess[i];

            for ( j = 0; j < pPolicy->ulNumOfApp; j++ )
            {
                if ( pPolicy->AppList[j].InstanceNumber == pApp->InstanceNumber )
                {
                    for ( k = j; k < pPolicy->ulNumOfApp; k++ )
                    {
                        AnscCopyString(pPolicy->AppList[k].Alias, pPolicy->AppList[k + 1].Alias);
                        pPolicy->AppList[k].InstanceNumber  = pPolicy->AppList[k + 1].InstanceNumber;
                        pPolicy->AppList[k].IsWellKnown     = pPolicy->AppList[k + 1].IsWellKnown;
                        pPolicy->AppList[k].Protocol        = pPolicy->AppList[k + 1].Protocol;
                        pPolicy->AppList[k].StartPortNumber = pPolicy->AppList[k + 1].StartPortNumber;
                        pPolicy->AppList[k].EndPortNumber   = pPolicy->AppList[k + 1].EndPortNumber;
                    }

                    pPolicy->ulNumOfApp--;

                    return ANSC_STATUS_SUCCESS;
                }
            }
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetBlockedApp
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_APP     pApp
            );

    Description:

        Backend implementation to update the IP range entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of the corresponding policy.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be updated.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicySetBlockedApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    )
{
    ULONG                           i       = 0;
    ULONG                           j       = 0;
    PCOSA_SECURITY_IA_POLICY2       pPolicy = NULL;

    for ( i = 0; i < g_NumOfPolicies; i++ )
    {
        if ( g_SecurityInternetAccess[i].ulInstanceNumber == ulPolicyInstanceNumber )
        {
            pPolicy = &g_SecurityInternetAccess[i];

            for ( j = 0; j < pPolicy->ulNumOfApp; j++ )
            {
                if ( pPolicy->AppList[j].InstanceNumber == pApp->InstanceNumber )
                {
                    AnscCopyString(pPolicy->AppList[j].Alias, pApp->Alias);
                    pPolicy->AppList[j].InstanceNumber  = pApp->InstanceNumber;
                    pPolicy->AppList[j].IsWellKnown     = pApp->IsWellKnown;
                    pPolicy->AppList[j].Protocol        = pApp->Protocol;
                    pPolicy->AppList[j].StartPortNumber = pApp->StartPortNumber;
                    pPolicy->AppList[j].EndPortNumber   = pApp->EndPortNumber;

                    return ANSC_STATUS_SUCCESS;
                }
            }
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        PCOSA_DML_IA_LOG_ENTRY
        CosaDmlIaGetLogEntries
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    Description:

        Backend implementation to update the IP range entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PULONG                      pulCount
                  Log entry count


    Return:       Log entries.

**********************************************************************/
PCOSA_DML_IA_LOG_ENTRY
CosaDmlIaGetLogEntries
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    PCOSA_DML_IA_LOG_ENTRY          pLogBlock = (PCOSA_DML_IA_LOG_ENTRY)NULL;

    pLogBlock = (PCOSA_DML_IA_LOG_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_IA_LOG_ENTRY) * g_NumOfLogEntries);

    if ( !pLogBlock )
    {
        *pulCount = 0;

        return NULL;
    }

    AnscCopyMemory(pLogBlock, g_SecurityLog, sizeof(COSA_DML_IA_LOG_ENTRY) * g_NumOfLogEntries);

    *pulCount = g_NumOfLogEntries;

    return pLogBlock;
}

void CosaDmlIaRemove()
{
}

#elif (_COSA_INTEL_USG_ARM_ || _COSA_DRG_TPG_)

#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include "syscfg/syscfg.h"
#include "utapi_security.h"
#include <time.h>

#include "cosa_drg_common.h"

int g_iap_num;
iap_entry_t * g_iaps ;
int safe_strcpy(char * dst, char * src, int dst_size);

static void print_iap();
#ifdef _COSA_INTEL_USG_ARM_
static int be_struct_2_middle_layer(iap_entry_t * p_in, PCOSA_DML_IA_POLICY p_out)
{
    int i = 0;

    if (!p_in || !p_out)
        return -1;

    p_out->InstanceNumber = p_in->tr_inst_num;
    safe_strcpy(p_out->Alias, p_in->policyname, sizeof(p_out->Alias));
     
    p_out->bEnabled = p_in->enabled;

    p_out->LanHost.bUseLanHosts = p_in->lanhosts_set;
    p_out->LanHost.MacCount     = p_in->lanhosts.mac_count;
    p_out->LanHost.IpCount      = p_in->lanhosts.ip_count;
    p_out->LanHost.IprCount     = p_in->lanhosts.iprange_count;    

    printf("sizeof(iap_entry_t) %d \n", sizeof(iap_entry_t)); 

    for (i=0; i<p_out->LanHost.MacCount && i<COSA_DML_IA_LH_MAX_MAC; i++)
    {
        int  tmp[6] = {0};
        int  j = 0;
        char * p = p_in->lanhosts.maclist;
        
        _ansc_sscanf(p, "%02x:%02x:%02x:%02x:%02x:%02x",
                     &tmp[0],&tmp[1],&tmp[2],&tmp[3],&tmp[4],&tmp[5]);

        for (j=0; j<6; j++)
            p_out->LanHost.MacList[i].Mac[j] = tmp[j];

        p += MACADDR_SZ;

    }

    for (i=0; i<p_out->LanHost.IpCount && i<COSA_DML_IA_LH_MAX_IP; i++)
    {
        /*iplist is the last octect of ip address*/
        char * p = p_in->lanhosts.iplist;
        int  tmp = 0;

        p_out->LanHost.IpList[i].Ip.Value = CosaUtilGetIfAddr("brlan0");
        
        _ansc_sscanf(p, "%d",&tmp);
        p_out->LanHost.IpList[i].Ip.Dot[3] = tmp;

        p += IPADDR_SZ;
    }

    for (i=0; i<p_out->LanHost.IprCount && i<COSA_DML_IA_LH_MAX_IP_RANGE; i++)
    {
        /*start_ip/end_ip is the last octect of ip address*/
        p_out->LanHost.IprList[i].StartIp.Value = p_out->LanHost.IprList[i].EndIp.Value = CosaUtilGetIfAddr("brlan0");
        p_out->LanHost.IprList[i].StartIp.Dot[3] = p_in->lanhosts.iprangelist[i].start_ip;
        p_out->LanHost.IprList[i].EndIp.Dot[3] = p_in->lanhosts.iprangelist[i].end_ip;
    }

    return 0;
}


static int middle_layer_2_be_struct(PCOSA_DML_IA_POLICY p_in, iap_entry_t * p_out, BOOL * p_alias_changed)
{
    int i = 0;

    p_out->tr_inst_num = p_in->InstanceNumber;

    if (AnscEqualString(p_out->policyname, p_in->Alias, TRUE))
    {
        *p_alias_changed = FALSE;
    }
    else 
    {
        *p_alias_changed = TRUE;
        safe_strcpy(p_out->policyname, p_in->Alias, sizeof(p_out->policyname));
    }
    
    p_out->enabled = p_in->bEnabled;

    p_out->lanhosts_set = p_in->LanHost.bUseLanHosts;

    p_out->lanhosts.mac_count = p_in->LanHost.MacCount;
    /*this native alloc is Utopia style*/
    p_out->lanhosts.maclist   = realloc(p_out->lanhosts.maclist, MACADDR_SZ * p_out->lanhosts.mac_count);
    if (p_out->lanhosts.maclist)
    {
        char * p = p_out->lanhosts.maclist;
        
        memset(p_out->lanhosts.maclist, 0, MACADDR_SZ * p_out->lanhosts.mac_count);
        for (i=0; i<p_out->lanhosts.mac_count; i++)
        {
            _ansc_sprintf(p, "%02x:%02x:%02x:%02x:%02x:%02x",
                          p_in->LanHost.MacList[i].Mac[0],p_in->LanHost.MacList[i].Mac[1],
                          p_in->LanHost.MacList[i].Mac[2],p_in->LanHost.MacList[i].Mac[3],
                          p_in->LanHost.MacList[i].Mac[4],p_in->LanHost.MacList[i].Mac[5]);

            p += MACADDR_SZ;
        }
    }

    p_out->lanhosts.ip_count = p_in->LanHost.IpCount;
    /*this native alloc is Utopia style*/
    p_out->lanhosts.iplist   = realloc(p_out->lanhosts.iplist, IPADDR_SZ * p_out->lanhosts.ip_count);
    if (p_out->lanhosts.iplist)
    {
        memset(p_out->lanhosts.iplist, 0, IPADDR_SZ * p_out->lanhosts.ip_count);

        char * p = p_out->lanhosts.iplist;
        for (i=0; i<p_out->lanhosts.ip_count; i++)
        {
            /*there is a gap between firewall.c and definition of lanhosts.iplist, firewall.c only want the last octect*/
            _ansc_sprintf(p, "%d", p_in->LanHost.IpList[i].Ip.Dot[3]);
            p+= IPADDR_SZ;
        }
            
    }

    p_out->lanhosts.iprange_count = p_in->LanHost.IprCount;
    /*this native alloc is Utopia style*/
    p_out->lanhosts.iprangelist   = realloc(p_out->lanhosts.iprangelist, sizeof(iprange_t) * p_out->lanhosts.iprange_count);
    if (p_out->lanhosts.iprangelist)
    {
        memset(p_out->lanhosts.iprangelist, 0, sizeof(iprange_t) * p_out->lanhosts.iprange_count);
        for (i=0; i<p_out->lanhosts.iprange_count; i++)
        {
            p_out->lanhosts.iprangelist[i].start_ip = p_in->LanHost.IprList[i].StartIp.Dot[3];
            p_out->lanhosts.iprangelist[i].end_ip   = p_in->LanHost.IprList[i].EndIp.Dot[3];
        }
    }


    return 0;
}

static iap_entry_t * find_ia_policy(ULONG index)
{
    int i = 0;

    for (i=0; i<g_iap_num; i++)
    {
        if (g_iaps[i].tr_inst_num == index)
            return (g_iaps + i);
    }
    
    return NULL;
}

static void free_iap_entries(iap_entry_t * p_in, int count)
{
    int i = 0;
    
    if (p_in)
    {
        for (i=0; i<count; i++ )
        {
            Utopia_FreeInternetAccessPolicy(p_in+i);
        }

        free(p_in);
    }
    return;
}

static int refresh_be_buf(iap_entry_t ** pp_in, int * p_count)
{
    UtopiaContext ctx ;
    
    free_iap_entries(*pp_in, *p_count);

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    if (Utopia_GetInternetAccessPolicy(&ctx, p_count, pp_in)) 
    {
        Utopia_Free(&ctx, 0);
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_Free(&ctx, 0);
    
    return 0;
}
#endif


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlFirewallGetConfig2
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_FIREWALL_CFG2     pCfg
            );

    Description:

        Backend implementation to get Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_FIREWALL_CFG2     pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

#define MAX_LEN 16

ANSC_STATUS
CosaDmlFirewallGetConfig2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG2     pCfg
    )
{
    UtopiaContext ctx;
    firewall_t    fw;

    char value[MAX_LEN] = {'\0'};

    if ( !pCfg )
        return ANSC_STATUS_FAILURE;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_SECURITY: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    Utopia_GetFirewallSettings (&ctx, &fw);
    pCfg->FilterIdent = (fw.filter_ident == true) ? TRUE : FALSE;
    pCfg->FilterMulticast = (fw.filter_multicast == true) ? TRUE : FALSE;
    pCfg->FilterP2P = (fw.filter_p2p_from_wan == true) ? TRUE : FALSE;
    pCfg->FilterHTTP = (fw.filter_http_from_wan == true) ? TRUE : FALSE;
    pCfg->FilterHTTPs = (fw.filter_http_from_wan == true) ? TRUE : FALSE;
    pCfg->FilterAnonymousInternetRequests = (fw.filter_anon_req == true) ? TRUE : FALSE;

    pCfg->FilterIdentV6 = (fw.filter_ident_v6 == true) ? TRUE : FALSE;
    pCfg->FilterMulticastV6 = (fw.filter_multicast_v6 == true) ? TRUE : FALSE;
    pCfg->FilterP2PV6 = (fw.filter_p2p_from_wan_v6 == true) ? TRUE : FALSE;
    pCfg->FilterHTTPV6 = (fw.filter_http_from_wan_v6 == true) ? TRUE : FALSE;
    pCfg->FilterHTTPsV6 = (fw.filter_http_from_wan_v6 == true) ? TRUE : FALSE;
    pCfg->FilterAnonymousInternetRequestsV6 = (fw.filter_anon_req_v6 == true) ? TRUE : FALSE;

    pCfg->TrueStaticIpEnable = (fw.true_static_ip_enable == true) ? TRUE : FALSE;
    pCfg->TrueStaticIpEnableV6 = (fw.true_static_ip_enable_v6 == true) ? TRUE : FALSE;
    pCfg->SmartPktDectionEnable = (fw.smart_pkt_dection_enable == true) ? TRUE : FALSE;
    pCfg->SmartPktDectionEnableV6 = (fw.smart_pkt_dection_enable_v6 == true) ? TRUE : FALSE;
    pCfg->WanPingEnable = (fw.wan_ping_enable == true) ? TRUE : FALSE;
    pCfg->WanPingEnableV6 = (fw.wan_ping_enable_v6 == true) ? TRUE : FALSE;

    value[0] = '\0';
    Utopia_Get(&ctx, UtopiaValue_Firewall_Level, value, sizeof(value));

    if(!_ansc_strcmp(value, "High"))
        pCfg->FirewallLevel = COSA_DML_FIREWALL_LEVEL_High;
    else if(!_ansc_strcmp(value, "Low")) 
        pCfg->FirewallLevel = COSA_DML_FIREWALL_LEVEL_Low;
    else if(!_ansc_strcmp(value,"Medium"))
        pCfg->FirewallLevel = COSA_DML_FIREWALL_LEVEL_Medium;
    else if(!_ansc_strcmp(value, "Custom"))
        pCfg->FirewallLevel = COSA_DML_FIREWALL_LEVEL_Custom;
    else 
        pCfg->FirewallLevel = COSA_DML_FIREWALL_LEVEL_None;

    value[0] = '\0';
    Utopia_Get(&ctx, UtopiaValue_Firewall_LevelV6, value, sizeof(value));

    if(!_ansc_strcmp(value, "High"))
        pCfg->FirewallLevelV6 = COSA_DML_FIREWALL_LEVEL_High;
    else if(!_ansc_strcmp(value, "Low")) 
        pCfg->FirewallLevelV6 = COSA_DML_FIREWALL_LEVEL_Low;
    else if(!_ansc_strcmp(value,"Medium"))
        pCfg->FirewallLevelV6 = COSA_DML_FIREWALL_LEVEL_Medium;
    else if(!_ansc_strcmp(value, "Custom"))
        pCfg->FirewallLevelV6 = COSA_DML_FIREWALL_LEVEL_Custom;
    else 
        pCfg->FirewallLevelV6 = COSA_DML_FIREWALL_LEVEL_None;

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlFirewallSetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_FIREWALL_CFG      pCfg
            );

    Description:

        Backend implementation to set Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_FIREWALL_CFG      pCfg
                  Pointer of the configuration to be set.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlFirewallSetConfig2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG2     pCfg
    )
{
    int rc = -1;
    UtopiaContext ctx;
    firewall_t fw;

    if ( !pCfg )
        return ANSC_STATUS_FAILURE;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_SECURITY: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    fw.filter_ident = (pCfg->FilterIdent == FALSE) ? 0 : 1;
    fw.filter_multicast = (pCfg->FilterMulticast == FALSE) ? 0 : 1;
    fw.filter_anon_req = (pCfg->FilterAnonymousInternetRequests == FALSE) ? 0 : 1;
    fw.filter_p2p_from_wan = (pCfg->FilterP2P == FALSE) ? 0 : 1;
    fw.filter_http_from_wan = (pCfg->FilterHTTP == FALSE) ? 0 : 1;

    fw.filter_ident_v6 = (pCfg->FilterIdentV6 == FALSE) ? 0 : 1;
    fw.filter_multicast_v6 = (pCfg->FilterMulticastV6 == FALSE) ? 0 : 1;
    fw.filter_anon_req_v6 = (pCfg->FilterAnonymousInternetRequestsV6 == FALSE) ? 0 : 1;
    fw.filter_p2p_from_wan_v6 = (pCfg->FilterP2PV6 == FALSE) ? 0 : 1;
    fw.filter_http_from_wan_v6 = (pCfg->FilterHTTPV6 == FALSE) ? 0 : 1;

    fw.true_static_ip_enable = (pCfg->TrueStaticIpEnable == FALSE) ? 0 : 1;
    fw.true_static_ip_enable_v6 = (pCfg->TrueStaticIpEnableV6 == FALSE) ? 0 : 1;
    fw.smart_pkt_dection_enable = (pCfg->SmartPktDectionEnable == FALSE) ? 0 : 1;
    fw.smart_pkt_dection_enable_v6 = (pCfg->SmartPktDectionEnableV6 == FALSE) ? 0 : 1;
    fw.wan_ping_enable = (pCfg->WanPingEnable == FALSE) ? 0 : 1;
    fw.wan_ping_enable_v6 = (pCfg->WanPingEnableV6 == FALSE) ? 0 : 1;

    switch(pCfg->FirewallLevel)
    {
        case 1:
            Utopia_Set(&ctx, UtopiaValue_Firewall_Level, "High");
            break;
        case 2:
            Utopia_Set(&ctx, UtopiaValue_Firewall_Level, "Medium");
            break;
        case 3:
            Utopia_Set(&ctx, UtopiaValue_Firewall_Level, "Low");
            break;
        case 4:
            Utopia_Set(&ctx, UtopiaValue_Firewall_Level, "Custom");
            break;
        default:
            Utopia_Set(&ctx, UtopiaValue_Firewall_Level, "None");
            break;
    }

    switch(pCfg->FirewallLevelV6)
    {
        case 1:
            Utopia_Set(&ctx, UtopiaValue_Firewall_LevelV6, "High");
            break;
        case 2:
            Utopia_Set(&ctx, UtopiaValue_Firewall_LevelV6, "Medium");
            break;
        case 3:
            Utopia_Set(&ctx, UtopiaValue_Firewall_LevelV6, "Low");
            break;
        case 4:
            Utopia_Set(&ctx, UtopiaValue_Firewall_LevelV6, "Custom");
            break;
        default:
            Utopia_Set(&ctx, UtopiaValue_Firewall_LevelV6, "None");
            break;
    }

    //fw.spi_protection = pCfg->FirewallLevel != COSA_DML_FIREWALL_LEVEL_None ? 1 : 0;
    fw.spi_protection = 1;

    rc = Utopia_SetFirewallSettings(&ctx, fw);

    Utopia_Free(&ctx, !rc);

    if(rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaGetNumberOfPolicies
            (
                ANSC_HANDLE                 hContext
            );

    Description:

        Backend implementation to set Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.


    Return:       The number of the Access Policies.

**********************************************************************/
ULONG
CosaDmlIaGetNumberOfPolicies
    (
        ANSC_HANDLE                 hContext
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    return g_iap_num;
#endif
    return 0;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaGetPolicy
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_IA_POLICY         pEntry
            );

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index number of the IA policy.

                  PCOSA_DML_IA_POLICY         pEntry
                  Policy struct to be filled in.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaGetPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY         pEntry
    )
{
#ifdef _COSA_INTEL_USG_ARM_

    be_struct_2_middle_layer(&g_iaps[ulIndex], pEntry);

    return ANSC_STATUS_SUCCESS;
    
#endif
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaSetPolicyValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            )

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index number of the IA policy.

                  ULONG                       ulInstanceNumber
                  Instance number to set back.

                  char*                       pAlias
                  Alias to set back.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaSetPolicyValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    BOOL  alias_changed = 0;
    UtopiaContext ctx ;
    iap_entry_t * p_iap = NULL;
    char  old_name[128] = {0};
    
    g_iaps[ulIndex].tr_inst_num = ulInstanceNumber;
    
    if (!AnscEqualString(g_iaps[ulIndex].policyname, pAlias, TRUE))
    {
        alias_changed = TRUE;
        safe_strcpy(old_name, g_iaps[ulIndex].policyname, sizeof(old_name));
        safe_strcpy(g_iaps[ulIndex].policyname, pAlias, sizeof(g_iaps[ulIndex].policyname));
    }

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    /*alias will be copied to backend policyname, and Utopia_AddInternetAccessPolicy will use policyname to find iap_entry, if alias changes,
     we can't use Utopia_AddInternetAccessPolicy, but has to edit the corresponding iap_entry_t*/
    if (!alias_changed)
    {
        if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
            Utopia_Free(&ctx, 1);
        else 
            Utopia_Free(&ctx, 0);
    }
    else
    {
        iap_entry_t oldiap;
        int         old_index = 0; 
        if (!Utopia_FindInternetAccessPolicy(&ctx, old_name, &oldiap, &old_index))
        {
            /*success*/
            if (!Utopia_EditInternetAccessPolicy (&ctx, old_index, p_iap))
                Utopia_Free(&ctx, 1);
            else 
                Utopia_Free(&ctx, 0);
        }
        else 
        {
            Utopia_Free(&ctx, 0);
        }
        
    }
#endif
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaGetPolicyByName
            (
                ANSC_HANDLE                 hContext,
                char*                       pName,
                PCOSA_DML_IA_POLICY         pEntry
            );

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index number of the IA policy.

                  PCOSA_DML_IA_POLICY         pEntry
                  Policy struct to be filled in.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaGetPolicyByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInsNum,
        PCOSA_DML_IA_POLICY         pEntry
    )
{
    /*for rollback*/
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;

    for (i=0; i<g_iap_num; i++)
    {
        if (g_iaps[i].tr_inst_num == ulInsNum)
        {
            be_struct_2_middle_layer(&g_iaps[i], pEntry);
            return ANSC_STATUS_SUCCESS;
        }
    }
    return ANSC_STATUS_CANT_FIND;
    
#endif    

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaAddPolicy
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_IA_POLICY         pEntry
            );

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_IA_POLICY         pEntry
                  Policy to be added.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaAddPolicy
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IA_POLICY         pEntry
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    iap_entry_t iap = {0};
    UtopiaContext ctx ;
    BOOL          alias_changed = 0;

    middle_layer_2_be_struct(pEntry, &iap, &alias_changed);

    /*PCOSA_DML_IA_POLICY is a samller struct than iap_entry_t, fill in the default values*/
    iap.allow_access = 1;

    iap.tod.day = 127;
    strcpy(iap.tod.start_time,"00:00");
    strcpy(iap.tod.stop_time,"23:59");
    iap.tod.all_day = TRUE;


    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    if (!Utopia_AddInternetAccessPolicy (&ctx, &iap))
        Utopia_Free(&ctx, 1);
    else
        Utopia_Free(&ctx, 0);


    refresh_be_buf(&g_iaps, &g_iap_num);

#endif

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaDelPolicy
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNum
            );

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be added.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaDelPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNum
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    UtopiaContext ctx;
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
    iap_entry_t * p_iap = NULL;

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    for (i=0; i<g_iap_num; i++)
    {
        if (g_iaps[i].tr_inst_num == ulInstanceNum)
        {
            ret = Utopia_DeleteInternetAccessPolicy(&ctx, g_iaps[i].policyname);
            p_iap = g_iaps + i;
            break;
        }
    }
    
    if (!ret)
        Utopia_Free(&ctx, 1);   
    else 
        Utopia_Free(&ctx, 0);   

    refresh_be_buf(&g_iaps, &g_iap_num);

    return ret;
#endif
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaSetPolicy
            (
                ANSC_HANDLE                 hContext,
                char*                       pName
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be set.

                  PCOSA_DML_IA_POLICY         pEntry
                  The new policy to be set.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaSetPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY         pEntry
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    iap_entry_t * p_iap = NULL;
    int i = 0;
    UtopiaContext ctx ;    
    BOOL          alias_changed = 0;
    char          old_name[128] = {0};

    p_iap = find_ia_policy(ulInstanceNumber);

    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;
    
    safe_strcpy(old_name, p_iap->policyname, sizeof(old_name));
    middle_layer_2_be_struct(pEntry, p_iap, &alias_changed);

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    /*alias will be copied to backend policyname, and Utopia_AddInternetAccessPolicy will use policyname to find iap_entry, if alias changes,
     we can't use Utopia_AddInternetAccessPolicy, but has to edit the corresponding iap_entry_t*/
    if (!alias_changed)
    {
        if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
            Utopia_Free(&ctx, 1);
        else 
            Utopia_Free(&ctx, 0);
    }
    else
    {
        iap_entry_t oldiap;
        int         old_index = 0; 
        if (!Utopia_FindInternetAccessPolicy(&ctx, old_name, &oldiap, &old_index))
        {
            /*success*/
            if (!Utopia_EditInternetAccessPolicy (&ctx, old_index, p_iap))
                Utopia_Free(&ctx, 1);
            else 
                Utopia_Free(&ctx, 0);
        }
        else 
        {
            Utopia_Free(&ctx, 0);
        }
        
    }

    return ANSC_STATUS_SUCCESS;
#endif

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaGetPolicySchedule
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber,
                PCOSA_DML_IA_POLICY_SCH     pEntry
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be set.

                  PCOSA_DML_IA_POLICY         pEntry
                  The new policy to be set.


    Return:       The number of the Access Policies.

**********************************************************************/

#ifdef _COSA_INTEL_USG_ARM_
void  be_struct_2_ml_schedule(iap_entry_t * p_iap, PCOSA_DML_IA_POLICY_SCH pEntry)
{
    if (!p_iap || !pEntry)
        return;

    memset(pEntry, 0, sizeof(*pEntry));

    pEntry->AllowAccess = p_iap->allow_access;

    pEntry->IncludeSunday     = (p_iap->tod.day&DAY_SUN) ? TRUE:FALSE ;
    pEntry->IncludeMonday     = (p_iap->tod.day&DAY_MON) ? TRUE:FALSE ;
    pEntry->IncludeTuesday    = (p_iap->tod.day&DAY_TUE) ? TRUE:FALSE ;
    pEntry->IncludeWednesday  = (p_iap->tod.day&DAY_WED) ? TRUE:FALSE ;
    pEntry->IncludeThursday   = (p_iap->tod.day&DAY_THU) ? TRUE:FALSE ;
    pEntry->IncludeFriday     = (p_iap->tod.day&DAY_FRI) ? TRUE:FALSE ;
    pEntry->IncludeSaturday   = (p_iap->tod.day&DAY_SAT) ? TRUE:FALSE ;    

    pEntry->AllDay            = (p_iap->tod.all_day)     ? TRUE:FALSE;
    safe_strcpy(pEntry->StartTime, p_iap->tod.start_time, sizeof(pEntry->StartTime));
    safe_strcpy(pEntry->EndTime, p_iap->tod.stop_time, sizeof(pEntry->EndTime));

    return;
}

void  ml_schedule_2_be_struct(PCOSA_DML_IA_POLICY_SCH pEntry, iap_entry_t * p_iap)
{
    if (!pEntry || !p_iap)
        return;

    p_iap->allow_access = pEntry->AllowAccess;

    p_iap->tod.day = 0;
    if (pEntry->IncludeSunday)    p_iap->tod.day |= DAY_SUN;
    if (pEntry->IncludeMonday)    p_iap->tod.day |= DAY_MON;
    if (pEntry->IncludeTuesday)   p_iap->tod.day |= DAY_TUE;
    if (pEntry->IncludeWednesday) p_iap->tod.day |= DAY_WED;
    if (pEntry->IncludeThursday)  p_iap->tod.day |= DAY_THU;
    if (pEntry->IncludeFriday)    p_iap->tod.day |= DAY_FRI;
    if (pEntry->IncludeSaturday)  p_iap->tod.day |= DAY_SAT;

    /*if all_day is true, the start_time&stop_time doesn't take effect*/
    p_iap->tod.all_day = (pEntry->AllDay) ? TRUE:FALSE;

    safe_strcpy(p_iap->tod.start_time, pEntry->StartTime, sizeof(p_iap->tod.start_time));
    safe_strcpy(p_iap->tod.stop_time, pEntry->EndTime, sizeof(p_iap->tod.stop_time));

    return;
}
#endif

ANSC_STATUS
CosaDmlIaGetPolicySchedule
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY_SCH     pEntry
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    iap_entry_t * p_iap = NULL;
    int i = 0;

    p_iap = find_ia_policy(ulInstanceNumber);

    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    be_struct_2_ml_schedule(p_iap, pEntry);

    return ANSC_STATUS_SUCCESS;
#endif

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaGetPolicySchedule
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber,
                PCOSA_DML_IA_POLICY_SCH     pEntry
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be set.

                  PCOSA_DML_IA_POLICY         pEntry
                  The new policy to be set.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaSetPolicySchedule
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY_SCH     pEntry
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int ret = 0;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;
    
    p_iap = find_ia_policy(ulInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;
    
    ml_schedule_2_be_struct(pEntry, p_iap);

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
        Utopia_Free(&ctx, 1);
    else 
        Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;

#endif
    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetNumberOfUrls
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be set.

                  PCOSA_DML_IA_POLICY         pEntry
                  The new policy to be set.


    Return:       The number of the Access Policies.

**********************************************************************/

ULONG
CosaDmlIaPolicyGetNumberOfUrls
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    iap_entry_t * p_iap = NULL;
    
    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return 0;
    
    return p_iap->block.url_count;
#endif

    return 0;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetUrl
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                PCOSA_DML_IA_POLICY_URL     pUrl
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of policy to be set.

                  ULONG                       ulIndex
                  Index number of the IP range entry

                  PCOSA_DML_IA_LH_IPR         pEntry
                  The IP range entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyGetUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_URL     pUrl
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    char * p_url = NULL;
    iap_entry_t * p_iap = NULL;
    
    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    if (p_iap->block.url_count <= ulIndex )
        return ANSC_STATUS_CANT_FIND;
    
    pUrl->InstanceNumber = p_iap->block.url_tr_inst_num[ulIndex];
    safe_strcpy(pUrl->Alias, p_iap->block.url_tr_alias, sizeof(pUrl->Alias));

    p_url = p_iap->block.url_list + URL_SZ*ulIndex;
    safe_strcpy(pUrl->Url, p_url, sizeof(pUrl->Url));

    return ANSC_STATUS_SUCCESS;
#endif
    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetUrlByInsNum
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulURLInstanceNumber,
                PCOSA_DML_IA_POLICY_URL     pUrl
            )

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  The name of policy to be set.

                  ULONG                       ulURLInstanceNumber
                  Instance number of the blocked url entry

                  PCOSA_DML_IA_POLICY_URL     pUrl
                  The blocked url entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/
#define B p_iap->block
ANSC_STATUS
CosaDmlIaPolicyGetUrlByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulURLInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl
    )
{
/*for rollback*/
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    char * p_url = NULL;
    iap_entry_t * p_iap = NULL;
    
    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    for (i=0; i<p_iap->block.url_count; i++)
    {
        if (p_iap->block.url_tr_inst_num[i] == ulURLInstanceNumber)
            break;
    }

    if (i == p_iap->block.url_count)
        return ANSC_STATUS_CANT_FIND;

    /*now i stores the index of URL in block.url_list*/
    p_url = p_iap->block.url_list + URL_SZ*i;

    if (p_iap->block.url_count <= ulURLInstanceNumber )
        return ANSC_STATUS_CANT_FIND;

    pUrl->InstanceNumber = p_iap->block.url_tr_inst_num[i];
    safe_strcpy(pUrl->Alias, p_iap->block.url_tr_alias + i*TR_ALIAS_SZ, sizeof(pUrl->Alias));

    safe_strcpy(pUrl->Url, p_url, sizeof(pUrl->Url));

    return ANSC_STATUS_SUCCESS;
    
#endif

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetUrlValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            )

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index number of the IA policy.

                  ULONG                       ulInstanceNumber
                  Instance number to set back.

                  char*                       pAlias
                  Alias to set back.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicySetUrlValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
/*for internal.c to adjust alias/instNum*/
    
#ifdef _COSA_INTEL_USG_ARM_
    /*the two instance number are all sane, no need to validate*/
    int i = 0;
    char * p_url = NULL;
    iap_entry_t * p_iap = NULL;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    p_iap->block.url_tr_inst_num[ulIndex] = ulInstanceNumber;
    safe_strcpy(p_iap->block.url_tr_alias + TR_ALIAS_SZ*ulIndex, pAlias, TR_ALIAS_SZ);

    if (TRUE) 
    {
        UtopiaContext ctx ;

        if(!Utopia_Init(&ctx))
            return ANSC_STATUS_FAILURE;

        if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
            Utopia_Free(&ctx, 1);
        else 
            Utopia_Free(&ctx, 0);
    }
#endif
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyAddUrl
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_URL     pUrl
            );

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  The Instance Number of the policy to be set.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be added


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyAddUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    char * p_url = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    B.url_count++;

    B.url_tr_inst_num = realloc(B.url_tr_inst_num, B.url_count*sizeof(int));
    if (B.url_tr_inst_num) 
        B.url_tr_inst_num[B.url_count - 1] = pUrl->InstanceNumber;

    B.url_tr_alias = realloc(B.url_tr_alias, B.url_count*TR_ALIAS_SZ);
    if (B.url_tr_alias)
    {
        char * p = B.url_tr_alias+(B.url_count-1)*TR_ALIAS_SZ;
        safe_strcpy(p, pUrl->Alias, TR_ALIAS_SZ);
    }
    
    B.url_list = realloc(B.url_list, B.url_count*URL_SZ);
    if (B.url_list)
    {
        p_url = B.url_list + URL_SZ*(B.url_count-1);
        safe_strcpy(p_url, pUrl->Url, URL_SZ);
    }

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
        Utopia_Free(&ctx, 1);
    else 
        Utopia_Free(&ctx, 0);
    
    return ANSC_STATUS_SUCCESS;
    
#endif

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyDelUrl
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_URL     pUrl
            )

    Description:

        Backend implementation to delete the IP range entry.

    Arguments:    ANSC_HANLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of the corresponding policy.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be deleted.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicyDelUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl        /* Identified by InstanceNumber */
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    int j = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    for (i=0; i<B.url_count; i++)
    {
        if (B.url_tr_inst_num[i] == pUrl->InstanceNumber)
            break;
    }

    if (i == B.url_count)
        return ANSC_STATUS_CANT_FIND;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    /*now i stores index for block.url_list*/
    /*here we saved strength not to realloc block.url_list, just move the back portion url data ahead.*/
    for (j= i; j<B.url_count-1; j++)
    {
        B.url_tr_inst_num[j] = B.url_tr_inst_num[j+1];

        p = &B.url_tr_alias[TR_ALIAS_SZ*j];
        memset(p, 0, TR_ALIAS_SZ);
        memcpy(p, p+TR_ALIAS_SZ, TR_ALIAS_SZ);

        p = &B.url_list[URL_SZ*j];
        memset(p, 0, URL_SZ);
        memcpy(p, p+URL_SZ, URL_SZ);
    }

    B.url_count--;


    if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
        Utopia_Free(&ctx, 1);
    else 
        Utopia_Free(&ctx, 0);
    
    return ANSC_STATUS_SUCCESS;


#endif
    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetUrl
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_URL     pUrl
            );

    Description:

        Backend implementation to update the IP range entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of the corresponding policy.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be updated.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicySetUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl        /* Identified by InstanceNumber */
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    for (i=0; i<p_iap->block.url_count; i++)
    {
        if (pUrl->InstanceNumber == p_iap->block.url_tr_inst_num[i])
            break;
    }
    
    if (i == p_iap->block.url_count)
        return ANSC_STATUS_CANT_FIND;        

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    /*now i strore index of url_list*/
    p = B.url_tr_alias + TR_ALIAS_SZ*i;
    if (!AnscEqualString(p, pUrl->Alias, TRUE))
        safe_strcpy(p, pUrl->Alias, TR_ALIAS_SZ);
    
    p = B.url_list + URL_SZ*i;
    if (!AnscEqualString(p, pUrl->Url, TRUE))
        safe_strcpy(p, pUrl->Url, URL_SZ);

    if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
        Utopia_Free(&ctx, 1);
    else 
        Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
#endif

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlIaPolicyGetNumberOfKeywords
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber
            );


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  The Instance Number of policy.

    Return:       The number of the Access Policies.

**********************************************************************/

ULONG
CosaDmlIaPolicyGetNumberOfKeywords
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return 0;

    return B.keyword_count;
    
#endif
    return 0;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetKeyword
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                PCOSA_DML_IA_POLICY_KEYWORD pKeyword
            );


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber,
                  The Instance Number of policy.

                  ULONG                       ulIndex
                  Index number of the blocked keyword entry

                  PCOSA_DML_IA_POLICY_KEYWORD pKeyword
                  The blocked keyword entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyGetKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    memset(pKeyword, 0, sizeof(*pKeyword));
    
    pKeyword->InstanceNumber = B.keyword_tr_inst_num[ulIndex];

    p = B.keyword_tr_alias + TR_ALIAS_SZ*ulIndex;
    safe_strcpy(pKeyword->Alias, p, sizeof(pKeyword->Alias));

    p = B.keyword_list + IAP_KEYWORD_SZ*ulIndex;
    safe_strcpy(pKeyword->Keyword, p, sizeof(pKeyword->Keyword));

    return ANSC_STATUS_SUCCESS;
    
#endif
    
    
    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetKeywordByInsNum
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulKeywordInstanceNumber,
                PCOSA_DML_IA_POLICY_KEYWORD pKeyword
            )

    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  The name of policy to be set.

                  ULONG                       ulKeywordInstanceNumber
                  Instance number of the blocked keyword entry

                  PCOSA_DML_IA_POLICY_KEYWORD pKeyword
                  The blocked keyword entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicyGetKeywordByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulKeywordInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    )
{
/*for rollback*/
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    for (i=0; i<B.keyword_count; i++)
    {
        if (B.keyword_tr_inst_num[i] == ulKeywordInstanceNumber)
            break;
    }

    if (i == B.keyword_count) 
        return ANSC_STATUS_CANT_FIND;

    /*now i stores the index for block.keyword*/
    memset(pKeyword, 0, sizeof(*pKeyword));
    
    p = B.keyword_tr_alias + TR_ALIAS_SZ*i;
    safe_strcpy(pKeyword->Alias, p, sizeof(pKeyword->Alias));

    p = B.keyword_list + IAP_KEYWORD_SZ*i;
    safe_strcpy(pKeyword->Keyword, p, sizeof(pKeyword->Keyword));

    return ANSC_STATUS_SUCCESS;
    
#endif

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetKeywordValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            )

    Description:

        Backend implementation to get Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  Instance number of the IA policy.

                  ULONG                       ulIndex
                  Index of keyword entry.

                  ULONG                       ulInstanceNumber
                  Instance number to set back.

                  char*                       pAlias
                  Alias to set back.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicySetKeywordValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
/*for inernal.c to generate alias/instNum*/
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    B.keyword_tr_inst_num[ulIndex] = ulInstanceNumber;
    
    p = B.keyword_tr_alias + TR_ALIAS_SZ*ulIndex;
    safe_strcpy(p, pAlias, TR_ALIAS_SZ);

    if (TRUE) 
    {
        UtopiaContext ctx ;

        if(!Utopia_Init(&ctx))
            return ANSC_STATUS_FAILURE;

        if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
            Utopia_Free(&ctx, 1);
        else 
            Utopia_Free(&ctx, 0);
    }
#endif

    
    return ANSC_STATUS_SUCCESS;
}



/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyAddKeyword
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_KEYWORD pKeyword
            );


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of policy to be set.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be added


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyAddKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);

    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    B.keyword_count++;

    B.keyword_tr_inst_num = realloc(B.keyword_tr_inst_num, B.keyword_count*sizeof(int));
    if (B.keyword_tr_inst_num)
        B.keyword_tr_inst_num[B.keyword_count-1] = pKeyword->InstanceNumber;

    B.keyword_tr_alias = realloc(B.keyword_tr_alias, B.keyword_count*TR_ALIAS_SZ);
    if (B.keyword_tr_alias) 
    {
        p = B.keyword_tr_alias + (B.keyword_count-1)*TR_ALIAS_SZ;
        safe_strcpy(p, pKeyword->Alias, TR_ALIAS_SZ);
    }
    
    B.keyword_list = realloc(B.keyword_list, B.keyword_count*IAP_KEYWORD_SZ);
    if (B.keyword_list)
    {
        p = B.keyword_list + (B.keyword_count-1)*IAP_KEYWORD_SZ;
        safe_strcpy(p, pKeyword->Keyword, IAP_KEYWORD_SZ);
    }

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
        Utopia_Free(&ctx, 1);
    else 
        Utopia_Free(&ctx, 0);
    

    return ANSC_STATUS_SUCCESS;
    

#endif

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyDelKeyword
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_KEYWORD pKeyword
            )


    Description:

        Backend implementation to delete the IP range entry.

    Arguments:    ANSC_HANLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of the corresponding policy.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be deleted.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicyDelKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    int j = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);

    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    for (i=0; i<B.keyword_count; i++)
    {
        if (B.keyword_tr_inst_num[i] == pKeyword->InstanceNumber)
            break;
    }

    if (i == B.keyword_count)
        return ANSC_STATUS_CANT_FIND;

    /*now i stores the index for block.keyword_list*/
    /*we don't realloc here to shrink the BE buffer size, just move data ahead.*/
    for (j=i; j<B.keyword_count-1; j++)
    {
        B.keyword_tr_inst_num[j] = B.keyword_tr_inst_num[j+1];

        p = B.keyword_tr_alias + TR_ALIAS_SZ*j;
        memset(p, 0, TR_ALIAS_SZ);
        memcpy(p, p+TR_ALIAS_SZ, TR_ALIAS_SZ);

        p = B.keyword_list + IAP_KEYWORD_SZ*j;
        memset(p, 0, IAP_KEYWORD_SZ);
        memcpy(p, p+IAP_KEYWORD_SZ, TR_ALIAS_SZ);
    }

    B.keyword_count--;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
        Utopia_Free(&ctx, 1);
    else 
        Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;


#endif

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetKeyword
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_KEYWORD pKeyword
            );


    Description:

        Backend implementation to update the IP range entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber,
                  The Instance Number of the corresponding policy.

                  PCOSA_DML_IA_POLICY_KEYWORD pKeyword
                  Blocked keyword entry to be updated.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicySetKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    int j = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);

    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    for (i=0; i<B.keyword_count; i++)
    {
        if (B.keyword_tr_inst_num[i] == pKeyword->InstanceNumber)
            break;
    }

    if (i == B.keyword_count)
        return ANSC_STATUS_CANT_FIND;

    /*now i stores the index for block.keyword_list*/
    p = B.keyword_list + IAP_KEYWORD_SZ*i;
    safe_strcpy(p, pKeyword->Keyword, IAP_KEYWORD_SZ);
    
    p = B.keyword_tr_alias + TR_ALIAS_SZ*i;
    safe_strcpy(p, pKeyword->Alias, TR_ALIAS_SZ);

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
        Utopia_Free(&ctx, 1);
    else 
        Utopia_Free(&ctx, 0);
    
    return ANSC_STATUS_SUCCESS;
    

#endif
    
    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlIaPolicyGetNumberOfApps
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber
            );


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pName
                  The name of policy to be set.

                  PCOSA_DML_IA_POLICY         pEntry
                  The new policy to be set.


    Return:       The number of the Access Policies.

**********************************************************************/
#ifdef _COSA_INTEL_USG_ARM_
BOOL isWellKnownService (const char *name)
{
    int i = 0;
    char *_NetworkServices[] = 

        { "ftp", 
          "telnet",
          "ssh",
          "sftp",
          "domain",
          "finger",
          "tftp",
          "www",
          "http-alt",
          "https",
          "pop3",
          "smtp",
          "nntp",
          "ntp",
          "nameserver",
          "netstat",
          "netbios-nm",
          "sunrpc",
          "snmp",
          "irc",
          "imap2",
          "ldap",
          "isakmp",
          "rtsp",
          0 };


    while (_NetworkServices[i]) {
        if (0 == strcasecmp(name, _NetworkServices[i])) {
            return TRUE;
        }
        i++;   
    }
    return FALSE;
};

void middle_layer_2_be_app(PCOSA_DML_IA_POLICY_APP pApp, appentry_t * p_app )
{
    if (!pApp || !p_app)
        return;
    
    memset(p_app, 0, sizeof(appentry_t));

    /*Alias is the p_app->name*/
    safe_strcpy(p_app->name, pApp->Alias, sizeof(p_app->name));

    p_app->wellknown  = pApp->IsWellKnown = isWellKnownService(p_app->name);
    p_app->proto      = pApp->Protocol - 1;
    p_app->port.start = pApp->StartPortNumber;
    p_app->port.end   = pApp->EndPortNumber;
    
    return;
}

void be_block_app_2_middle_layer(blockentry_t * p_b, PCOSA_DML_IA_POLICY_APP pApp, ULONG ulIndex)
{
    if (!p_b || !pApp)
        return;

    pApp->InstanceNumber = p_b->app_tr_inst_num[ulIndex];

    safe_strcpy(pApp->Alias, p_b->app_list[ulIndex].name, sizeof(pApp->Alias));
    
    pApp->IsWellKnown = p_b->app_list[ulIndex].wellknown = isWellKnownService(pApp->Alias);

    if (!pApp->IsWellKnown)
    {
        /*the enum sequence is the same, but the initial values are different with 1 */
        pApp->Protocol    = p_b->app_list[ulIndex].proto+1;

        pApp->StartPortNumber = p_b->app_list[ulIndex].port.start;
        pApp->EndPortNumber   = p_b->app_list[ulIndex].port.end;
    }
    else 
    {
        /*for wellknown services, backend doesn't care about the following fields, hard code them*/
        pApp->Protocol        =     COSA_DML_IA_APP_PROTO_Both;

        pApp->StartPortNumber = pApp->EndPortNumber = 0;
    }



    return;
    
}
#endif

ULONG
CosaDmlIaPolicyGetNumberOfApps
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    int j = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return 0;

    return B.app_count;

#endif

    return 0;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetApp
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                PCOSA_DML_IA_POLICY_APP     pApp
            );


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of policy to be set.

                  ULONG                       ulIndex
                  Index number of the IP range entry

                  PCOSA_DML_IA_LH_IPR         pEntry
                  The IP range entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyGetApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_APP     pApp
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    int j = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    be_block_app_2_middle_layer(&p_iap->block, pApp, ulIndex);

    return ANSC_STATUS_SUCCESS;
#endif


    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyGetAppByInsNum
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulAppInstanceNumber,
                PCOSA_DML_IA_POLICY_APP     pApp
            )

    Description:

        Backend implementation to get App entry by instance number.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  The instance number of policy to be get.

                  ULONG                       ulAppInstanceNumber
                  Instance number of the blocked app entry

                  PCOSA_DML_IA_POLICY_APP     pApp
                  The blocked app entry to get.


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyGetAppByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulAppInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    )
{
/*for rollback*/
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    int j = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;
    
    for (i=0; i<B.app_count; i++)
    {
        if (ulAppInstanceNumber == B.app_tr_inst_num[i])
            break;
    }

    if (i == B.app_count)
        return ANSC_STATUS_CANT_FIND;

    /*now i stores the index of block.app_list*/
    be_block_app_2_middle_layer(&p_iap->block, pApp, i);
    
    return ANSC_STATUS_SUCCESS;    
#endif
    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetKeywordValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            )

    Description:

        Backend implementation to set back instance number and alias.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulPolicyInstanceNumber
                  Instance number of the IA policy.

                  ULONG                       ulIndex
                  Index of keyword entry.

                  ULONG                       ulInstanceNumber
                  Instance number to set back.

                  char*                       pAlias
                  Alias to set back.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicySetAppValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
/*for middle layer generated alias/instNum*/
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    int j = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    B.app_tr_inst_num[ulIndex] = ulInstanceNumber;

    p = B.app_list[ulIndex].name;
    safe_strcpy(p, pAlias, sizeof(B.app_list[ulIndex].name));


    if (TRUE) 
    {
        UtopiaContext ctx ;

        if(!Utopia_Init(&ctx))
            return ANSC_STATUS_FAILURE;

        if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
            Utopia_Free(&ctx, 1);
        else 
            Utopia_Free(&ctx, 0);
    }


#endif

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyAddApp
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_APP     pApp
            )


    Description:

        Backend implementation to set Internet Access Policy.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of policy to be set.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be added


    Return:       The number of the Access Policies.

**********************************************************************/
ANSC_STATUS
CosaDmlIaPolicyAddApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    int j = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;    

    p_iap = find_ia_policy(ulPolicyInstanceNumber);

    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    B.app_count++;

    B.app_tr_inst_num = realloc(B.app_tr_inst_num, B.app_count*sizeof(int));
    if (B.app_tr_inst_num)
        B.app_tr_inst_num[B.app_count-1] = pApp->InstanceNumber;

    B.app_list = realloc(B.app_list, B.app_count*sizeof(appentry_t));
    if (B.app_list)
    {
        appentry_t * p_app;

        p_app = B.app_list + (B.app_count-1);

        middle_layer_2_be_app(pApp, p_app);
        
        B.app_tr_inst_num[B.app_count-1] = pApp->InstanceNumber;
    }

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
        Utopia_Free(&ctx, 1);
    else 
        Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;

#endif

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicyDelBlockedApp
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_APP     pApp
            );


    Description:

        Backend implementation to delete the IP range entry.

    Arguments:    ANSC_HANLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of the corresponding policy.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be deleted.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicyDelBlockedApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    int j = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;    

    p_iap = find_ia_policy(ulPolicyInstanceNumber);
    
    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    for (i=0; i<B.app_count; i++)
    {
        if (pApp->InstanceNumber == B.app_tr_inst_num[i])
            break;
    }

    if (i == B.app_count)
        return ANSC_STATUS_CANT_FIND;

    /*now i stores the index of block.app_list, I don't take strength to shrink the size of block.app_xxx,
     just move the back portion ahead*/
    for (j=i; j<B.app_count-1; j++)
    {
        B.app_tr_inst_num[j] = B.app_tr_inst_num[j+1];

        memcpy(B.app_list + j, B.app_list + j+1, sizeof(appentry_t));
    }

    B.app_count--;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
        Utopia_Free(&ctx, 1);
    else 
        Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
    
#endif

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaPolicySetBlockedApp
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulPolicyInstanceNumber,
                PCOSA_DML_IA_POLICY_APP     pApp
            );

    Description:

        Backend implementation to update the IP range entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  char*                       pPolicyName
                  The name of the corresponding policy.

                  PCOSA_DML_IA_LH_IPR         pIpr
                  IP range entry to be updated.


    Return:       The number of the Access Policies.

**********************************************************************/

ANSC_STATUS
CosaDmlIaPolicySetBlockedApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    )
{
#ifdef _COSA_INTEL_USG_ARM_
    int i = 0;
    int j = 0;
    char * p = NULL;
    iap_entry_t * p_iap = NULL;
    UtopiaContext ctx ;    
    appentry_t *  p_app = NULL;

    p_iap = find_ia_policy(ulPolicyInstanceNumber);

    if (!p_iap)
        return ANSC_STATUS_CANT_FIND;

    for (i=0; i<B.app_count; i++)
    {
        if (pApp->InstanceNumber == B.app_tr_inst_num[i])
            break;
    }

    if (i == B.app_count)
        return ANSC_STATUS_CANT_FIND;

    /*i stores index for block.app_list*/
    p = B.app_list[i].name;
    safe_strcpy(p, pApp->Alias, sizeof(B.app_list[i].name));

    p_app = B.app_list + i;

    middle_layer_2_be_app(pApp, p_app);

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    if (!Utopia_AddInternetAccessPolicy (&ctx, p_iap))
        Utopia_Free(&ctx, 1);
    else 
        Utopia_Free(&ctx, 0);
    
    return ANSC_STATUS_SUCCESS;

    
#endif

    return ANSC_STATUS_CANT_FIND;
}

#undef B


#define FORMAT_STRING_COUNT 4
#define FIREWALL_LOG_DIR "/tmp/098ujsadf_firewall"
#define GEN_CURRENT_LOG_CMD "/fss/gw/usr/sbin/GenFWLog "
//#define GEN_CURRENT_LOG_CMD "/var/GenFWLog -nz"
#define MERGED_FW_LOG_FILE "/nvram/log/mergeLog.txt"
#define SORT_FW_LOG_FILE "/nvram/log/sortLog.txt"
#define MAX_LOG_ENTRY_COUNT 500
const char format[4][20] = { "Count=\"", "Time=\"", "Action=\"", "Desp=\""};

int _memcpy_2_chr(char *d, char *s, size_t count, char c){
    char *end = strchr(s, c);
    if(end == NULL || end -s == 0 || end -s >= count)
        return 0;
    memcpy(d, s, end - s);
    d[end-s] = '\0';
    return (end-s);
}

int _get_value(char **line, int index, char *val, size_t val_size){
    int count;
    if((*line = strstr(*line, format[index])) == NULL){
        return -1;
    }
    *line += strlen(format[index]);
    count = _memcpy_2_chr(val, *line, val_size, '\"'); 
    *line += count;
    return count;
}


int anlz_line(char *line, PCOSA_DML_IA_LOG_ENTRY entry){
    int i;
    char *pos = line;
    char tmp[20]={0};

    memset(entry, 0, sizeof(COSA_DML_IA_LOG_ENTRY)); 
    if( 0 >= _get_value(&pos, 0, tmp, sizeof(tmp)) || \
        0 >= _get_value(&pos, 1, entry->OccuranceTime, sizeof(entry->OccuranceTime)) || \
        0 >= _get_value(&pos, 2, entry->Action, sizeof(entry->Action))  || \
        0 >= _get_value(&pos, 3, entry->Description, sizeof(entry->Description))){
        return -1;
    }
	//No of attempts
	entry->Count = atoi(tmp);
    return 0;
}

void get_log_entry(char* fName, PCOSA_DML_IA_LOG_ENTRY *entry, unsigned long *count){
    FILE *fp;
    char *line = NULL;
    int c;
    size_t len;
    int line_num = 0;
    if((fp = fopen(fName, "r")) == NULL)
        return;
    
    while( (c = fgetc(fp)) != EOF){
        if( c == '\n')
            line_num++;
    }
    fseek(fp, 0, SEEK_SET);

    if(line_num > 0 ){
//        *entry = (PCOSA_DML_IA_LOG_ENTRY)AnscReallocMemory(*entry, sizeof(COSA_DML_IA_LOG_ENTRY) * (*count), sizeof(COSA_DML_IA_LOG_ENTRY) * (*count + line_num));
        *entry = (PCOSA_DML_IA_LOG_ENTRY)realloc(*entry, sizeof(COSA_DML_IA_LOG_ENTRY) * (*count + line_num + 4));

        if(*entry == NULL){
            *count = 0;
            fclose(fp);
            return;
        }
    }else{
        return;
    }
    
    while(-1 != getline(&line, &len, fp)){
        if(0 == anlz_line(line, *entry + *count))
            (*count)++;        
        if(line){
            free(line);
            line = NULL;
        }
    }
    fclose(fp);
    if(*count == 0){
        AnscFreeMemory(entry);
        *entry = NULL;
    }
    return;
}

static PCOSA_DML_IA_LOG_ENTRY _get_log(ULONG *count){
    struct dirent *ptr;    
    DIR *dir;
    PCOSA_DML_IA_LOG_ENTRY entry = NULL;
    char str[128];

    *count = 0;

    dir = opendir(FIREWALL_LOG_DIR);
    if(dir == NULL)
        return NULL;

    while(( ptr = readdir(dir)) != NULL){
        if(ptr->d_name[0] == '.')
            continue;
        //sprintf(fName, "%s/%s", FIREWALL_LOG_DIR,ptr->d_name);

        memset(str, 0, sizeof(str));
        sprintf(str, "cat %s/%s >> %s", FIREWALL_LOG_DIR,ptr->d_name,MERGED_FW_LOG_FILE);
        system(str);
    }

    /* Sort the logs in descending order of timestamp*/
    memset(str, 0, sizeof(str));
    sprintf(str, "grep Time %s | sort -r -k2,5 > %s", MERGED_FW_LOG_FILE, SORT_FW_LOG_FILE);
    system(str);
        /* Check log time format */
		/* Old timestemp not include year, 
 		 * add year to support it */
#define FWLOG_SUPPORT_OLD_TIME_FORMAT
#ifdef FWLOG_SUPPORT_OLD_TIME_FORMAT
#define FW_LOG_TIME_OLD_FORMAT_LEN 15
        ULONG tmp = *count;
        char year[6];
        struct tm *ptime;
        time_t t;

        t=time(NULL);
        ptime=localtime(&t);
        year[0] = '\0';
#endif
        get_log_entry(SORT_FW_LOG_FILE, &entry, count);

#ifdef FWLOG_SUPPORT_OLD_TIME_FORMAT
        for(;tmp < *count; tmp++)
        {
            if(strlen(entry[tmp].OccuranceTime) == FW_LOG_TIME_OLD_FORMAT_LEN){
                if(year[0] == '\0'){
                    /* this is a temp log file, Get current year */
                    if(strstr(year, "9999") != NULL)
                    {
                        sprintf(year, " %04d", ptime->tm_year + 1900); 
                    }else
                        sprintf(year, " %c%c%c%c", ptr->d_name[0], ptr->d_name[1], ptr->d_name[2], ptr->d_name[3]);
                }
                strncat(entry[tmp].OccuranceTime, year, 5);
            }   
        }
#endif
    memset(str, 0, sizeof(str));
    sprintf(str, "rm -rf %s %s", MERGED_FW_LOG_FILE, SORT_FW_LOG_FILE);  
    system(str);

    closedir(dir);
    return entry;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        PCOSA_DML_IA_LOG_ENTRY
        CosaDmlIaGetLogEntries
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    Description:

        Backend implementation to update the IP range entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PULONG                      pulCount
                  Log entry count


    Return:       Log entries.

**********************************************************************/
PCOSA_DML_IA_LOG_ENTRY
CosaDmlIaGetLogEntries
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    char fw_log_path[50];
    char temp[512];
    static int first_flg = 1;

    PCOSA_DML_IA_LOG_ENTRY pConf = NULL;
    *pulCount = 0;

    /* Don't get the log when initializing */
    if(first_flg == 1){
        first_flg = 0;
        return NULL;
    } 

    fw_log_path[0] = '\0';
#define FWLOG_SUPPORT_OLD_LOCATION
#ifdef FWLOG_SUPPORT_OLD_LOCATION
    if((!commonSyseventGet("FW_LOG_FILE_PATH_V2", fw_log_path, sizeof(fw_log_path))) &&\
        (fw_log_path[0] == '\0')){
#endif
        syscfg_init();
        syscfg_get(NULL, "FW_LOG_FILE_PATH", fw_log_path, sizeof(fw_log_path));
        if(fw_log_path[0] == '\0'){
            return NULL;
        }
#ifdef FWLOG_SUPPORT_OLD_LOCATION
    }
#endif
        
    /* Generate current log message */
    system(GEN_CURRENT_LOG_CMD);

    sprintf(temp, "mkdir -p %s", FIREWALL_LOG_DIR);
    //printf("%s\n", temp);
    system(temp);

    sprintf(temp, "log_handle.sh uncompress_fwlog %s", FIREWALL_LOG_DIR);
    //printf("%s\n", temp);
    system(temp);

    sprintf(temp, "cp %s/???????? %s 2> /dev/null", fw_log_path, FIREWALL_LOG_DIR);
//printf("%s\n", temp);
    system(temp);

printf("%d\n",__LINE__);
    /* get all log information */
    pConf = _get_log(pulCount);

//printf("%d pulCount %d \n",__LINE__, *pulCount);
    sprintf(temp, "rm -r %s", FIREWALL_LOG_DIR);
    system(temp);
    return pConf;
}
static PCOSA_DML_IA_LOG_ENTRY pFWLogBuf = NULL;
static int FWLogNum = 0;
static ULONG FWLogLastTick;

#define REFRESH_INTERVAL 30
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))

static int __is_updated(ULONG *last_tick)
{
    if ( !(*last_tick) ) 
    {
        *last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( *last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        *last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        PCOSA_DML_IA_LOG_ENTRY
        CosaDmlIaGetAllLogEntries
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    Description:

        Backend implementation to update the IP range entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PULONG                      pulCount
                  Log entry count


    Return:       Log entries.

**********************************************************************/
ANSC_STATUS
CosaDmlIaGetALLLogEntries
    (
        char*                          pValue,
        ULONG*                         pUlSize
    )
{
    char fw_log_path[50];
    char temp[512];
    static int first_flg = 1;
    int i;
    size_t tmpsize=0;
    char str[128];

    /* Don't get the log when initializing */
    if(__is_updated(&FWLogLastTick)){
        fw_log_path[0] = '\0';
#define FWLOG_SUPPORT_OLD_LOCATION
#ifdef FWLOG_SUPPORT_OLD_LOCATION
        if((!commonSyseventGet("FW_LOG_FILE_PATH_V2", fw_log_path, sizeof(fw_log_path))) &&\
            (fw_log_path[0] == '\0')){
#endif
            syscfg_init();
            syscfg_get(NULL, "FW_LOG_FILE_PATH", fw_log_path, sizeof(fw_log_path));
            if(fw_log_path[0] == '\0'){
                return NULL;
            }
#ifdef FWLOG_SUPPORT_OLD_LOCATION
        }
#endif
        if(pFWLogBuf != NULL){
            AnscFreeMemory(pFWLogBuf);
            FWLogNum = 0;
            pFWLogBuf = NULL;
        }

        /* Generate current log message */
        system(GEN_CURRENT_LOG_CMD);

        sprintf(temp, "mkdir -p %s", FIREWALL_LOG_DIR);
        //printf("%s\n", temp);
        system(temp);

        sprintf(temp, "log_handle.sh uncompress_fwlog %s", FIREWALL_LOG_DIR);
        //printf("%s\n", temp);
        system(temp);

        sprintf(temp, "cp %s/???????? %s 2> /dev/null", fw_log_path, FIREWALL_LOG_DIR);
    //printf("%s\n", temp);
        system(temp);

        /* get all log information */
        pFWLogBuf = _get_log(&FWLogNum);
    //printf("%d pulCount %d \n",__LINE__, *pulCount);
        sprintf(temp, "rm -r %s", FIREWALL_LOG_DIR);
        system(temp);
    }
    if(FWLogNum == 0){
        pValue[0] = '\0';
        return ANSC_STATUS_SUCCESS;
    } 
    if(*pUlSize < (FWLogNum * (20 + sizeof(COSA_DML_IA_LOG_ENTRY))))
    {
        *pUlSize = (FWLogNum + 20) *(20 + sizeof(COSA_DML_IA_LOG_ENTRY));
        return ANSC_STATUS_FAILURE;
    }else{
        for(i=0; (i < FWLogNum) && (*pUlSize > tmpsize + 1); i++){
           tmpsize += snprintf(pValue + tmpsize, *pUlSize - 1 - tmpsize , "\n%d\n%s\n%s\n%s\n%s\n%s\n%s", pFWLogBuf[i].Count, NULL, NULL, NULL, pFWLogBuf[i].Action, pFWLogBuf[i].OccuranceTime,pFWLogBuf[i].Description);
        }
        pValue[tmpsize] = '\0';
        return ANSC_STATUS_SUCCESS;
    }
}

void CosaDmlIaRemove()
{
#ifdef _COSA_INTEL_USG_ARM_
    free_iap_entries(g_iaps, g_iap_num);    
#endif
}

#endif

#ifdef _COSA_INTEL_USG_ARM_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#define CFG_FILE_PATH "/etc/ssmtp.conf"
#define MSG_FILE_PATH "/var/ssmtp_msg"
#define EMAIL_LOG_ID "[Email Notification]"

static int gen_ssmtp_cfg_file(const char *serverAddr, const char *userName, const char *passWord)
{
    FILE *fp;

    if((fp = fopen(CFG_FILE_PATH, "w")) == NULL)
    {
        fprintf(stderr, EMAIL_LOG_ID"Open cfg file failed.\n");
        return -1;
    }

    fprintf(fp, "mailhub=%s\n", serverAddr);
    fprintf(fp, "AuthUser=%s\n", userName);
    fprintf(fp, "AuthPass=%s\n", passWord);
    fprintf(fp, "AuthMethod=LOGIN\n");
    fprintf(fp, "FromLineOverride=YES\n");

    fclose(fp);

    return 0;
}

static int gen_ssmtp_msg_file(const char *to, const char *from, const char *subject, const char *body)
{
    FILE *fp;

    if((fp = fopen(MSG_FILE_PATH, "w")) == NULL)
    {
        fprintf(stderr, EMAIL_LOG_ID"Open message file failed.\n");
        return -1;
    }

    fprintf(fp, "To: %s\n", to);
    fprintf(fp, "From: %s\n", from);
    //fprintf(fp, "Subject: %s\n", subject);
    //fprintf(fp, "\n%s\n", body);

    fclose(fp);
 
    return 0;
}

#define MAX_LINE_LEN 256

static int ssmtp_send(const char *msgFilePath, const char *subject, const char *attachmentPath)
{
    char buf[MAX_LINE_LEN] = "";
    char recipient[MAX_LINE_LEN] = "";
    char attachmentPathCopy[MAX_LINE_LEN] = "";

    FILE *fp;

    if((fp = fopen(msgFilePath, "r")) == NULL)
    {
        fprintf(stderr, EMAIL_LOG_ID"Open message file failed.\n");
        return -1;
    }
    
    //copy recipient address from msg file
    fgets(buf, sizeof(buf), fp);
    strncpy(recipient, buf + strlen("To: "), sizeof(recipient));

    strncpy(attachmentPathCopy, attachmentPath, sizeof(attachmentPathCopy));

    snprintf(buf, sizeof(buf), "(((cat %s; echo 'Subject: %s'; echo; echo; uuencode %s %s) | ssmtp %s) && rm %s) &", msgFilePath, subject, attachmentPath, basename(attachmentPathCopy), recipient, attachmentPath);
    system(buf);

    return 0;
}

#define MAX_PATH_LEN 256

static int prepare_firewall_log(char *logFilePath)
{
    char buf[MAX_LINE_LEN] = "";
    time_t curTime; 
    struct tm *localTime; 
    char *logTypeName = "USGv2_Firewall_Logs";

    curTime=time(NULL); 
    localTime = localtime(&curTime); 
    
    system("grep 'UTOPIA: FW' /var/log/kernel > /var/fwlog");

    snprintf(logFilePath, MAX_PATH_LEN, "/var/%s_%d_%d_%d_%d_%d_%d.txt", logTypeName, localTime->tm_year+1900, localTime->tm_mon+1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec); 
    
    //compress attachment if necessary
    //snprintf(buf, sizeof(buf), "tar cvf %s /var/fwlog", logFilePath);
    snprintf(buf, sizeof(buf), "cp /var/fwlog %s && rm -rf /var/fwlog", logFilePath);

    system(buf);
    
    return 0;
}

static int gen_email_files(const email_notification_t *emailSetting)
{
    int ret;
    FILE *fp;
    char *emailSubject = "USGv2 Firewall Logs";
 
    remove("/var/.email_ready");

    if(emailSetting->bParentalControlBreach) 
    {
        fp = fopen("/var/.email_pclog", "w");
        fclose(fp);
    }   
    else
        remove("/var/.email_pclog");

    if(emailSetting->bAlertsWarnings)
    {
        fp = fopen("/var/.email_awlog", "w");
        fclose(fp);
    }
    else
        remove("/var/.email_awlog");

    if(emailSetting->bFirewallBreach)
    {
        fp = fopen("/var/.email_fwlog", "w");
        fclose(fp);
    }
    else
        remove("/var/.email_fwlog");

    if(emailSetting->bSendLogs)
    {
        fp = fopen("/var/.email_alllog", "w");
        fclose(fp);
    }
    else
        remove("/var/.email_alllog");
    
    if(gen_ssmtp_cfg_file(emailSetting->server, emailSetting->from_addr, emailSetting->password) != 0)
    {
        printf("Failed to generate ssmtp config file.\n");
        return -1;
    }
    
    //As Comcasts's required, use username as the signature in mail body
    if(gen_ssmtp_msg_file(emailSetting->send_to, emailSetting->from_addr, emailSubject, emailSetting->username) != 0)
    {
        printf("Failed to generate ssmtp message file.\n");
        return -1;
    }

    fp = fopen("/var/.email_ready", "w");
    fclose(fp);

    return 0;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlSecurityGetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_SECURITY_CFG     pCfg
            );

    Description:

        Backend implementation to get Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_SECURITY_CFG     pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlSecurityGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_SECURITY_CFG      pCfg
    )
{
    int rc = -1;
    UtopiaContext ctx;
    email_notification_t emailSetting;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    if((rc = Utopia_GetEmailNotificationSetting(&ctx, &emailSetting) == 0))
    {
        pCfg->EmailEnable = emailSetting.bEnabled;
        pCfg->EmailParentalControlBreach = emailSetting.bParentalControlBreach;
        pCfg->EmailAlertsOrWarnings = emailSetting.bAlertsWarnings;
        pCfg->EmailFirewallBreach = emailSetting.bFirewallBreach;
        pCfg->EmailSendLogs = emailSetting.bSendLogs;
        pCfg->TriggerEmailLog = FALSE; //always return FALSE

        _ansc_strcpy(pCfg->EmailSendTo, emailSetting.send_to);
        _ansc_strcpy(pCfg->EmailServer, emailSetting.server);
        _ansc_strcpy(pCfg->EmailUserName, emailSetting.username);
        _ansc_strcpy(pCfg->EmailPassword, emailSetting.password);
        _ansc_strcpy(pCfg->EmailFromAddress, emailSetting.from_addr);
    }

    Utopia_Free(&ctx, 0);
    
    if(rc != 0)
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlSecuritySetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_SECURITY_CFG      pCfg
            );

    Description:

        Backend implementation to set Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_SECURITY_CFG   pCfg
                  Pointer of the configuration to be set.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlSecuritySetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_SECURITY_CFG      pCfg
    )
{
    int rc = -1;
    int ret;
    UtopiaContext ctx;
    email_notification_t emailSetting, defaultEmailSetting;
    FILE *fp;
    char logFilePath[MAX_PATH_LEN] = "";
    
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    emailSetting.bEnabled = pCfg->EmailEnable;
    emailSetting.bParentalControlBreach = pCfg->EmailParentalControlBreach;
    emailSetting.bAlertsWarnings = pCfg->EmailAlertsOrWarnings;
    emailSetting.bFirewallBreach = pCfg->EmailFirewallBreach;
    emailSetting.bSendLogs= pCfg->EmailSendLogs;

    _ansc_strcpy(emailSetting.send_to, pCfg->EmailSendTo);
    _ansc_strcpy(emailSetting.server, pCfg->EmailServer);
    _ansc_strcpy(emailSetting.username, pCfg->EmailUserName);
    _ansc_strcpy(emailSetting.password, pCfg->EmailPassword);

    if(!_ansc_strcmp(pCfg->EmailFromAddress, ""))
    {
        rc = Utopia_GetEmailNotificationSetting(&ctx, &defaultEmailSetting);
        if(!rc)
            _ansc_strcpy(emailSetting.from_addr, defaultEmailSetting.from_addr);
    }
    else
        _ansc_strcpy(emailSetting.from_addr, pCfg->EmailFromAddress);

    rc = Utopia_SetEmailNotificationSetting(&ctx, &emailSetting);
    
    if(!rc)
        ret = gen_email_files(&emailSetting);

    //These 2 params are used by snmp. No GUI mapping.
    if(pCfg->EmailEnable && pCfg->TriggerEmailLog)
    {
        prepare_firewall_log(logFilePath);
        ssmtp_send(MSG_FILE_PATH, "USGv2 Firewall Logs", logFilePath);
    }

    Utopia_Free(&ctx, !rc);
    
    if(rc != 0 || ret != 0)
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}



/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIaInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
            );

        Description:
	        This is the initialization routine for XYZ backend.

        Arguments:      ANSC_HANDLE                 hDml
                        Opaque handle from DM adapter. 
        

	                    PANSC_HANDLE                phContext		
                        Opaque handle passed back from backend, 
                        needed by CosaDmlNatXyz() routines.

        Return:         Status of operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    int rc = -1, ret = -1;
    FILE *fp;
    UtopiaContext ctx;
    email_notification_t emailSetting;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_GetEmailNotificationSetting(&ctx, &emailSetting);

    Utopia_Free(&ctx, 0);

    if(!rc)
        ret = gen_email_files(&emailSetting);
    
    if(rc != 0 || ret != 0)
        return ANSC_STATUS_FAILURE;
    else 
        return ANSC_STATUS_SUCCESS;
}
#endif
