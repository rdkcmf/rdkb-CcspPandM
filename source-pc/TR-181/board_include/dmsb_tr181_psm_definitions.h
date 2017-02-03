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

    File: dmsb_tr181_psm_definitions.h

    For Data Model SBAPI Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        This file defines the PSM parameter names for TR-181
        data model SBAPI integration.

    ---------------------------------------------------------------

    environment:

        Platform Independent, but with multi-LAN support

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        02/28/2013  initial revision.

**********************************************************************/

#ifndef _DMSB_TR181_PSM_DEFINITIONS_
#define _DMSB_TR181_PSM_DEFINITIONS_

/**********************************************************************
                            IP Interface
**********************************************************************/
/*
 *  definitions for PSM parameters, "dmsb.l3net.{i}.*"
 */
#define  DMSB_TR181_PSM_l3net_Root                  "dmsb.l3net."
#define  DMSB_TR181_PSM_l3net_i                     "%d."

#define  DMSB_TR181_PSM_l3net_EthLink               "EthLink"

#define  DMSB_TR181_PSM_l3net_Enable                "Enable"
#define  DMSB_TR181_PSM_l3net_Alias                 "Alias"
#define  DMSB_TR181_PSM_l3net_MaxMTU                "MaxMTU"
#define  DMSB_TR181_PSM_l3net_AutoIPEnable          "AutoIPEnable"
#define  DMSB_TR181_PSM_l3net_IPv4Enable            "IPv4Enable"
#define  DMSB_TR181_PSM_l3net_IPv6Enable            "IPv6Enable"
#define  DMSB_TR181_PSM_l3net_ULAEnable             "ULAEnable"

#define  DMSB_TR181_PSM_l3net_V4InstanceNumber      "V4InstanceNumber"
#define  DMSB_TR181_PSM_l3net_V4Addr                "V4Addr"
#define  DMSB_TR181_PSM_l3net_V4SubnetMask          "V4SubnetMask"
#define  DMSB_TR181_PSM_l3net_V6InstanceNumber      "V6InstanceNumber"
#define  DMSB_TR181_PSM_l3net_V6Addr                "V6Addr"
#define  DMSB_TR181_PSM_l3net_V6AddrPrefix          "V6AddrPrefix"
#define  DMSB_TR181_PSM_l3net_V6PreferredLifetime   "V6PreferredLifetime"
#define  DMSB_TR181_PSM_l3net_V6ValidLifetime       "V6ValidLifetime"
#define  DMSB_TR181_PSM_l3net_V6Anycast             "V6Anycast"

#define  DMSB_TR181_PSM_l3net_V6PrefixEnable            "V6PrefixEnable"
#define  DMSB_TR181_PSM_l3net_V6PrefixAlias             "V6PrefixAlias"
#define  DMSB_TR181_PSM_l3net_V6PrefixPrefix            "V6PrefixPrefix"
#define  DMSB_TR181_PSM_l3net_V6PrefixStaticType        "V6PrefixStaticType"
#define  DMSB_TR181_PSM_l3net_V6PrefixParentPrefix      "V6PrefixParentPrefix"
#define  DMSB_TR181_PSM_l3net_V6PrefixChildPrefixBits   "V6PrefixChildPrefixBits"
#define  DMSB_TR181_PSM_l3net_V6PrefixOnLink            "V6PrefixOnLink"
#define  DMSB_TR181_PSM_l3net_V6PrefixAutonomous        "V6PrefixAutonomous"
#define  DMSB_TR181_PSM_l3net_V6PrefixPreferredLifetime "V6PrefixPreferredLifetime"
#define  DMSB_TR181_PSM_l3net_V6PrefixValidLifetime     "V6PrefixValidLifetime"

#define  DMSB_TR181_PSM_l3net_ArpCacheTimeout           "ArpCacheTimeout"
#define  DMSB_TR181_PSM_l3net_UpnpIgdEnabled            "UPnPIGDEnabled"

/**********************************************************************
                            Ethernet Link
**********************************************************************/
/*
 *  definitions for PSM parameters, "dmsb.EthLink.{i}.*"
 */
#define  DMSB_TR181_PSM_EthLink_Root                "dmsb.EthLink."
#define  DMSB_TR181_PSM_EthLink_i                   "%d."
#define  DMSB_TR181_PSM_EthLink_Enable              "Enable"
#define  DMSB_TR181_PSM_EthLink_Alias               "Alias"
#define  DMSB_TR181_PSM_EthLink_PriorityTagging     "PriorityTagging"
#define  DMSB_TR181_PSM_EthLink_l2net               "l2net"
#define  DMSB_TR181_PSM_EthLink_l2netType           "l2netType"
#define  DMSB_TR181_PSM_EthLink_l2net               "l2net"
#define  DMSB_TR181_PSM_EthLink_l2netType           "l2netType"
#define  DMSB_TR181_PSM_EthLink_LowerLayerType      "LowerLayerType"    /* Can be Layer 1 Interface */
#define  DMSB_TR181_PSM_EthLink_LowerLayerInstNum   "LowerLayerInstanceNumber"
/* Reference COSA_DML_LINK_TYPE_NAME_* for the names of l2net types */

/**********************************************************************
                            Bridge
**********************************************************************/
/*
 *  definitions for PSM parameters, "dmsb.l2net.{i}.*"
 */
#define  DMSB_TR181_PSM_l2net_Root                  "dmsb.l2net."
#define  DMSB_TR181_PSM_l2net_i                     "%d."
#define  DMSB_TR181_PSM_l2net_name                  "Name"

/**********************************************************************
                            MultiLAN
**********************************************************************/
/*
 *  definitions for PSM parameters, "dmsb.X_CISCO_COM_MultiLAN.*"
 */
#define  DMSB_TR181_PSM_MultiLAN_Root                           "dmsb.MultiLAN."
#define  DMSB_TR181_PSM_MultiLAN_PrimaryLan_l3net               "PrimaryLAN_l3net"
#define  DMSB_TR181_PSM_MultiLAN_HomeSecurity_l3net             "HomeSecurity_l3net"
#define  DMSB_TR181_PSM_MultiLAN_PrimaryLan_l2net               "PrimaryLAN_l2net"
#define  DMSB_TR181_PSM_MultiLAN_PrimaryLan_HsPorts             "PrimaryLAN_HsPorts"
#define  DMSB_TR181_PSM_MultiLAN_PrimaryLANDHCPv4ServerPool     "PrimaryLAN_DHCPv4ServerPool"

#define  DMSB_TR181_PSM_MultiLAN_HomeSecurity_l2net             "HomeSecurity_l2net"
#define  DMSB_TR181_PSM_MultiLAN_HomeSecurity_HsPorts           "HomeSecurity_HsPorts"
#define  DMSB_TR181_PSM_MultiLAN_HomeSecurity_DHCPv4ServerPool  "HomeSecurity_DHCPv4ServerPool"
#define  DMSB_TR181_PSM_MultiLAN_HomeSecurity_WiFiRadio         "HomeSecurity_WiFiRadio"
#define  DMSB_TR181_PSM_MultiLAN_HomeSecurity_WiFiSsid          "HomeSecurity_WiFiSsid"
#define  DMSB_TR181_PSM_MultiLAN_HomeSecurity_WiFiAp            "HomeSecurity_WiFiAp"


/********************************************************************
    Bridge and Bridge Port Definition
*********************************************************************/
#define DMSB_L2_TYPE_BRIDGE               "Bridge"
#define DMSB_HIDEN_LAN_INF                "l2sd0-t"
#define DMSB_BRG_NAME_PREFIX              "br%d"
#define DMSB_DELIM                        " "
#define DMSB_TAGGING_DELIM                "-t"
#define DMSB_L2_INFO_PREFIX               "dmsb."
#define DMSB_L2_PREFIX                    "dmsb.l2net."
#define DMSB_L2_INST_PREFIX               "dmsb.l2net.%u"
//#define DMSB_ATOM_L2_INST_PREFIX          "dmsb.atom.l2net.%ul"
#define DMSB_L2_DEVICE_INST_PREFIX        "dmsb.l2net.%u.Device.%s"
//#define DMSB_ATOM_L2_DEVICE_INST_PREFIX   "dmsb.atom.l2net.%ul.Device.%s"
#define DMSB_L2_BPORT_PREFIX              "dmsb.l2net.%u.Port."
//$HL 7/8/2013
#define DMSB_L2_BVLAN_PREFIX              "dmsb.l2net.%u.Vlan."
#define DMSB_L2_BPORT_INST_PREFIX         "dmsb.l2net.%u.Port.%u"
#define DMSB_L2_BVLAN_INST_PREFIX         "dmsb.l2net.%u.Vlan.%u"
#define DMSB_L2_INFO_CONCAT(b)            DMSB_L2_INFO_PREFIX "." b  
#define DMSB_L2_CONCAT(b)                 DMSB_L2_INST_PREFIX "." b
//#define DMSB_ATOM_L2_CONCAT(b)            DMSB_ATOM_L2_INST_PREFIX "." b
#define DMSB_L2_DEVICE_CONCAT(b)          DMSB_L2_DEVICE_INST_PREFIX "." b 
//#define DMSB_ATOM_L2_DEVICE_CONCAT(b)     DMSB_ATOM_L2_DEVICE_INST_PREFIX "." b 
#define DMSB_L2_BPORT_CONCAT(b)           DMSB_L2_BPORT_INST_PREFIX "." b 
#define DMSB_L2_BVLAN_CONCAT(b)           DMSB_L2_BVLAN_INST_PREFIX "." b 


/**********************************************************************
                            DHCPv4 Server Pool
**********************************************************************/
#define PSM_DHCPV4_SERVER_POOL "dmsb.dhcpv4.server.pool."
#define PSM_DHCPV4_SERVER_POOL_i "%d."

#define PSM_DHCPV4_SERVER_ENABLE "dmsb.dhcpv4.server.Enable" 
#define PSM_DHCPV4_SERVER_POOL_ALLOWDELETE "dmsb.dhcpv4.server.pool.%d.AllowDelete"
#define PSM_DHCPV4_SERVER_POOL_ENABLE "dmsb.dhcpv4.server.pool.%d.Enable"
#define PSM_DHCPV4_SERVER_POOL_INTERFACE "dmsb.dhcpv4.server.pool.%d.IPInterface"
#define PSM_DHCPV4_SERVER_POOL_MINADDRESS "dmsb.dhcpv4.server.pool.%d.MinAddress"
#define PSM_DHCPV4_SERVER_POOL_MAXADDRESS "dmsb.dhcpv4.server.pool.%d.MaxAddress"
#define PSM_DHCPV4_SERVER_POOL_SUBNETMASK "dmsb.dhcpv4.server.pool.%d.SubnetMask"
#define PSM_DHCPV4_SERVER_POOL_DNSSERVERS "dmsb.dhcpv4.server.pool.%d.DNSServers"
#define PSM_DHCPV4_SERVER_POOL_DOMAINNAME "dmsb.dhcpv4.server.pool.%d.DomainName"
#define PSM_DHCPV4_SERVER_POOL_IPROUTERS "dmsb.dhcpv4.server.pool.%d.IPRouters"
#define PSM_DHCPV4_SERVER_POOL_LEASETIME "dmsb.dhcpv4.server.pool.%d.LeaseTime"
#define PSM_DHCPV4_SERVER_POOL_ALIAS "dmsb.dhcpv4.server.pool.%d.Alias"
#define PSM_DHCPV4_SERVER_POOL_ORDER "dmsb.dhcpv4.server.pool.%d.Order"
#define PSM_DHCPV4_SERVER_POOL_TIMEOFFSET "dmsb.dhcpv4.server.pool.%d.TimeOffset"    

#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS "dmsb.dhcpv4.server.pool.%d.StaticAddress."
#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS_ENABLE "dmsb.dhcpv4.server.pool.1.StaticAddress.%d.Enable"
#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS_ALIAS "dmsb.dhcpv4.server.pool.1.StaticAddress.%d.Alias"
#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS_CHADDR "dmsb.dhcpv4.server.pool.1.StaticAddress.%d.Chaddr"
#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS_YIADDR "dmsb.dhcpv4.server.pool.1.StaticAddress.%d.Yiaddr"
#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS_X_CISCO_COM_DEVICENAME "dmsb.dhcpv4.server.pool.1.StaticAddress.%d.X_CISCO_COM_DeviceName"
#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS_X_CISCO_COM_COMMENT "dmsb.dhcpv4.server.pool.1.StaticAddress.%d.X_CISCO_COM_Comment"
#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS_INSTANCENUMBER "dmsb.dhcpv4.server.pool.1.StaticAddress.InstanceNumber"

#define PSM_DHCPV4_SERVER_POOL_OPTION "dmsb.dhcpv4.server.pool.%d.Option."
#define PSM_DHCPV4_SERVER_POOL_OPTION_ENABLE "dmsb.dhcpv4.server.pool.%d.Option.%d.Enable"
#define PSM_DHCPV4_SERVER_POOL_OPTION_ALIAS "dmsb.dhcpv4.server.pool.%d.Option.%d.Alias"
#define PSM_DHCPV4_SERVER_POOL_OPTION_TAG "dmsb.dhcpv4.server.pool.%d.Option.%d.Tag"
#define PSM_DHCPV4_SERVER_POOL_OPTION_VALUE "dmsb.dhcpv4.server.pool.%d.Option.%d.Value"

/* Following are not supported by middle-layer of DHCPV4
"dmsb.dhcpv4.server.pool.%d.vendorclassID" // not supported
"dmsb.dhcpv4.server.pool.%d.vandorclassIDexclude"  // not supported
"dmsb.dhcpv4.server.pool.%d.vendorclassIDmode"  // not supported
"dmsb.dhcpv4.server.pool.%d.clientID"  // not supported
"dmsb.dhcpv4.server.pool.%d.clientIDexclude"  // not supported
"dmsb.dhcpv4.server.pool.%d.userclassID"  // not supported
"dmsb.dhcpv4.server.pool.%d.userclassIDExclude"  // not supported
"dmsb.dhcpv4.server.pool.%d.chaddr"  // not supported
"dmsb.dhcpv4.server.pool.%d.chaddrmask"  // not supported
"dmsb.dhcpv4.server.pool.%d.chaddrexclude"  // not supported
"dmsb.dhcpv4.server.pool.%d.reservedaddresses"  // not supported

#define PSM_DHCPV4_SERVER_POOL_CLIENT_ALIAS "dmsb.dhcpv4.server.pool.%d.client.%d.alias"
*/

/**********************************************************************
                            File Transfer
**********************************************************************/
#define  DMSB_TR181_PSM_ft_Root                  "dmsb.filetransfer."
#define  DMSB_TR181_PSM_ft_ServerAddress         "Serveraddress"
#define  DMSB_TR181_PSM_ft_Protocol              "Protocol"
#define  DMSB_TR181_PSM_ft_FileName              "Filename"

/**********************************************************************
                            True Static IP
**********************************************************************/

#define  DMSB_TR181_PSM_Tsip_Root                "dmsb.truestaticip."
#define  DMSB_TR181_PSM_Tsip_Enable              "Enable"
#define  DMSB_TR181_PSM_Tsip_IpAddress           "Ipaddress"
#define  DMSB_TR181_PSM_Tsip_SubnetMask          "Subnetmask"
#define  DMSB_TR181_PSM_Tsip_Gateway             "Gateway"
#define  DMSB_TR181_PSM_Tsip_EncryptKey          "Encryptkey"

#define  DMSB_TR181_PSM_Tsip_Asn_Root            "dmsb.truestaticip.Asn."
#define  DMSB_TR181_PSM_Tsip_Asn_i               "%d."
#define  DMSB_TR181_PSM_Tsip_Asn_Alias           "Alias"
#define  DMSB_TR181_PSM_Tsip_Asn_Enable          "Enable"
#define  DMSB_TR181_PSM_Tsip_Asn_IpAddress       "Ipaddress"
#define  DMSB_TR181_PSM_Tsip_Asn_SubnetMask      "Subnetmask"

/**********************************************************************
                            Product-Specific DeviceInfo
**********************************************************************/
#define  DMSB_TR181_PSM_DeviceInfo_Root             "dmsb.device.deviceinfo."
#define  DMSB_TR181_PSM_DeviceInfo_ProductClass     "ProductClass"
#define  DMSB_TR181_PSM_DeviceInfo_ManufacturerOUI  "ManufacturerOUI"

/******************************************************************************
                        Firewall IPv4
*******************************************************************************/
#define PSM_FW_FIREWALL_LEVEL "dmsb.X_CISCO_COM_Security.Firewall.FirewallLevel"

/**********************************************************************
                            Port Mapping
**********************************************************************/
#define  PSM_MAPPING_Enable "dmsb.nat.PortMapping.%d.enable"
#define  PSM_MAPPING_Status "dmsb.nat.PortMapping.%d.status"
#define  PSM_MAPPING_Alias "dmsb.nat.PortMapping.%d.alias"
#define  PSM_MAPPING_AllInterfaces "dmsb.nat.PortMapping.%d.allInterfaces"
#define  PSM_MAPPING_LeaseDuration "dmsb.nat.PortMapping.%d.leaseDuration"
#define  PSM_MAPPING_ExternalPortEndRange "dmsb.nat.PortMapping.%d.externalPortEndRange"
#define  PSM_MAPPING_ExternalPort "dmsb.nat.PortMapping.%d.externalPort"
#define  PSM_MAPPING_InternalPort "dmsb.nat.PortMapping.%d.internalPort"
#define  PSM_MAPPING_Protocol "dmsb.nat.PortMapping.%d.protocol"
#define  PSM_MAPPING_InternalClient "dmsb.nat.PortMapping.%d.internalClient"
#define  PSM_MAPPING_Description "dmsb.nat.PortMapping.%d.description"


/**********************************************************************
                            Port Triggering
**********************************************************************/
#define  PSM_TRIGGERING_Enable "dmsb.nat.X_CISCO_COM_PortTriggers.trigger.%d.enable"
#define  PSM_TRIGGERING_TriggerProtocol "dmsb.nat.X_CISCO_COM_PortTriggers.trigger.%d.triggerProtocol"
#define  PSM_TRIGGERING_TriggerPortStart "dmsb.nat.X_CISCO_COM_PortTriggers.trigger.%d.triggerPortStart"
#define  PSM_TRIGGERING_TriggerPortEnd "dmsb.nat.X_CISCO_COM_PortTriggers.trigger.%d.triggerPortEnd"
#define  PSM_TRIGGERING_ForwardProtocol "dmsb.nat.X_CISCO_COM_PortTriggers.trigger.%d.forwardProtocol"
#define  PSM_TRIGGERING_ForwardPortStart "dmsb.nat.X_CISCO_COM_PortTriggers.trigger.%d.forwardPortStart"
#define  PSM_TRIGGERING_ForwardPortEnd "dmsb.nat.X_CISCO_COM_PortTriggers.trigger.%d.forwardPortEnd"
#define  PSM_TRIGGERING_Description "dmsb.nat.X_CISCO_COM_PortTriggers.trigger.%d.description"

/*******************************************************************************
                            RemoteManagement 
********************************************************************************/
#define PSM_RM_HTTPPORT "dmsb.UserInterface.X_CISCO_COM_RemoteAccess.HttpPort"
#define PSM_RM_HTTPENABLE "dmsb.UserInterface.X_CISCO_COM_RemoteAccess.HttpEnable"
#define PSM_RM_HTTPSPORT "dmsb.UserInterface.X_CISCO_COM_RemoteAccess.HttpsPort"
#define PSM_RM_HTTPSENABLE "dmsb.UserInterface.X_CISCO_COM_RemoteAccess.HttpsEnable"
#define PSM_RM_STARTIP "dmsb.UserInterface.X_CISCO_COM_RemoteAccess.StartIp"
#define PSM_RM_ENDIP "dmsb.UserInterface.X_CISCO_COM_RemoteAccess.EndIp"
#define PSM_RM_FROMANYIP "dmsb.UserInterface.X_CISCO_COM_RemoteAccess.FromAnyIP"

/*******************************************************************************
                            	DMZ
********************************************************************************/

#define PSM_DMZ_ENABLE "dmsb.NAT.X_CISCO_COM_DMZ.Enable"
#define PSM_DMZ_INTERNALIP "dmsb.NAT.X_CISCO_COM_DMZ.InternalIP"

/*******************************************************************************
                        Parental Control
********************************************************************************/
//Managed Sites
#define PSM_PC_MS_ENABLE "dmsb.X_Comcast_com_ParentalControl.ManagedSites.Enable"
#define PSM_PC_MS_BLOCKED_SITES_NUMBER_OF_ENTRIES "dmsb.X_Comcast_com_ParentalControl.ManagedSites.BlockedSiteNumberOfEntries"
#define PSM_PC_MS_TRUSTED_USER_NUMBER_OF_ENTRIES "dmsb.X_Comcast_com_ParentalControl.ManagedSites.TrustedUserNumberOfEntries"
#define PSM_PC_MS_BLOCKMETHOD "dmsb.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.%d.BlockMethod"
#define PSM_PC_MS_SITE "dmsb.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.%d.Site"
#define PSM_PC_MS_ALWAYSBLOCK "dmsb.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.%d.AlwaysBlock"
#define PSM_PC_MS_STARTTIME "dmsb.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.%d.StartTime"
#define PSM_PC_MS_ENDTIME "dmsb.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.%d.EndTime"
#define PSM_PC_MS_BLOCKDAYS "dmsb.X_Comcast_com_ParentalControl.ManagedSites.BlockedSite.%d.BlockDays"
#define PSM_PC_MS_INSTANCENUMBER "dmsb.X_Comcast_com_ParentalControl.ManagedSites.InstanceNumber"
#define PSM_PC_MS_TRUSTED_ENABLE "dmsb.X_Comcast_com_ParentalControl.ManagedSites.TrustedUser.%d.Trusted"
#define PSM_PC_MS_TRUSTED_IPADDRESS "dmsb.X_Comcast_com_ParentalControl.ManagedSites.TrustedUser.%d.IPAddress"
#define PSM_PC_MS_TRUSTED_IPADDRESS_TYPE "dmsb.X_Comcast_com_ParentalControl.ManagedSites.TrustedUser.%d.IPAddressType"
#define PSM_PC_MS_TRUSTED_HOST_DESCRIPTION "dmsb.X_Comcast_com_ParentalControl.ManagedSites.TrustedUser.%d.HostDescription"
#define PSM_PC_MS_TRUSTED_INSTANCENUMBER "dmsb.X_Comcast_com_ParentalControl.ManagedSites.TrustedUser.InstanceNumber"

//Managed Services
#define PSM_PC_MSE_ENABLE "dmsb.X_Comcast_com_ParentalControl.ManagedServices.Enable"
#define PSM_PC_MSE_SERVICE_NUMBER_OF_ENTRIES "dmsb.X_Comcast_com_ParentalControl.ManagedServices.ServiceNumberOfEntries"
#define PSM_PC_MSE_TRUSTED_USER_NUMBER_OF_ENTRIES "dmsb.X_Comcast_com_ParentalControl.ManagedServices.TrustedUserNumberOfEntries"
#define PSM_PC_MSE_DESCRIPTION "dmsb.X_Comcast_com_ParentalControl.ManagedServices.Service.%d.Description"
#define PSM_PC_MSE_PROTOCOL "dmsb.X_Comcast_com_ParentalControl.ManagedServices.Service.%d.Protocol"
#define PSM_PC_MSE_STARTPORT "dmsb.X_Comcast_com_ParentalControl.ManagedServices.Service.%d.StartPort"
#define PSM_PC_MSE_ENDPORT "dmsb.X_Comcast_com_ParentalControl.ManagedServices.Service.%d.EndPort"
#define PSM_PC_MSE_ALWAYSBLOCK "dmsb.X_Comcast_com_ParentalControl.ManagedServices.Service.%d.AlwaysBlock"
#define PSM_PC_MSE_STARTTIME "dmsb.X_Comcast_com_ParentalControl.ManagedServices.Service.%d.StartTime"
#define PSM_PC_MSE_ENDTIME "dmsb.X_Comcast_com_ParentalControl.ManagedServices.Service.%d.EndTime"
#define PSM_PC_MSE_BLOCKDAYS "dmsb.X_Comcast_com_ParentalControl.ManagedServices.Service.%d.BlockDays"
#define PSM_PC_MSE_INSTANCENUMBER "dmsb.X_Comcast_com_ParentalControl.ManagedServices.InstanceNumber"
#define PSM_PC_MSE_TRUSTED_ENABLE "dmsb.X_Comcast_com_ParentalControl.ManagedServices.TrustedUser.%d.Trusted"
#define PSM_PC_MSE_TRUSTED_IPADDRESS "dmsb.X_Comcast_com_ParentalControl.ManagedServices.TrustedUser.%d.IPAddress"
#define PSM_PC_MSE_TRUSTED_IPADDRESS_TYPE "dmsb.X_Comcast_com_ParentalControl.ManagedServices.TrustedUser.%d.IPAddressType"
#define PSM_PC_MSE_TRUSTED_HOST_DESCRIPTION "dmsb.X_Comcast_com_ParentalControl.ManagedServices.TrustedUser.%d.HostDescription"
#define PSM_PC_MSE_TRUSTED_INSTANCENUMBER "dmsb.X_Comcast_com_ParentalControl.ManagedServices.TrustedUser.InstanceNumber"

//Managed Devices
#define PSM_PC_MD_ENABLE "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.Enable"
#define PSM_PC_MD_ALLOWALL "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.AllowAll"
#define PSM_PC_MD_DEVICE_NUMBER_OF_ENTRIES "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.DeviceNumberOfEntries"
#define PSM_PC_MD_TYPE "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.Device.%d.Type"
#define PSM_PC_MD_DESCRIPTION "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.Device.%d.Description"
#define PSM_PC_MD_MACADDRESS "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.Device.%d.MACAddress"
#define PSM_PC_MD_ALWAYSBLOCK "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.Device.%d.AlwaysBlock"
#define PSM_PC_MD_STARTTIME "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.Device.%d.StartTime"
#define PSM_PC_MD_ENDTIME "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.Device.%d.EndTime"
#define PSM_PC_MD_BLOCKDAYS "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.Device.%d.BlockDays"
#define PSM_PC_MD_INSTANCENUMBER "dmsb.X_Comcast_com_ParentalControl.ManagedDevices.InstanceNumber"

#endif
