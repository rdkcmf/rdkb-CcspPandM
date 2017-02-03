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
#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS_ENABLE "dmsb.dhcpv4.server.pool.%d.StaticAddress.%d.Enable"
#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS_ALIAS "dmsb.dhcpv4.server.pool.%d.StaticAddress.%d.Alias"
#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS_CHADDR "dmsb.dhcpv4.server.pool.%d.StaticAddress.%d.Chaddr"
#define PSM_DHCPV4_SERVER_POOL_STATICADDRESS_YIADDR "dmsb.dhcpv4.server.pool.%d.StaticAddress.%d.Yiaddr"

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

#endif
