##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2015 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
AM_CFLAGS = -D_ANSC_LINUX
AM_CFLAGS += -D_ANSC_USER
AM_CFLAGS += -D_ANSC_LITTLE_ENDIAN_
AM_LDFLAGS = -lccsp_common

AM_CPPFLAGS = -Wall -Werror
ACLOCAL_AMFLAGS = -I m4
hardware_platform = i686-linux-gnu

noinst_LTLIBRARIES = libCcspPandM_middle_layer_src.la
libCcspPandM_middle_layer_src_la_CPPFLAGS = -I$(top_srcdir)/source/TR-181/board_sbapi -I$(top_srcdir)/source/TR-181/middle_layer_src -I$(top_srcdir)/source-pc/TR-181/board_include -I$(top_srcdir)/custom/comcast/source/TR-181/custom_include -I$(top_srcdir)/custom/comcast/source/TR-181/custom_ml -I$(top_srcdir)/custom/comcast -I$(top_srcdir)/source/TR-181/include -I$(top_srcdir)/source-pc/TR-181/board_sbapi -I$(top_srcdir)/source/Custom -I$(top_srcdir)/source-arm/TR-181/board_ml/ $(CPPFLAGS)
libCcspPandM_middle_layer_src_la_SOURCES = cosa_ethernet_dml.c cosa_neighdisc_internal.c cosa_neighdisc_dml.c cosa_ppp_internal.c cosa_apis_util.c cosa_interfacestack_internal.c cosa_users_dml.c cosa_upnp_dml.c cosa_x_cisco_com_ddns_dml.c cosa_dhcpv4_dml.c cosa_routing_dml.c cosa_ra_dml.c plugin_main.c cosa_hosts_dml.c cosa_apis_busutil.c cosa_x_cisco_com_ddns_internal.c cosa_nat_dml.c cosa_x_cisco_com_diagnostics_dml.c cosa_dns_dml.c plugin_main_apis.c cosa_ip_internal.c cosa_x_cisco_com_rlog_dml.c cosa_upnp_internal.c cosa_ipv6rd_dml.c cosa_x_cisco_com_security_internal.c cosa_userinterface_dml.c cosa_x_cisco_com_diagnostics_internal.c cosa_bridging_dml.c cosa_routing_internal.c cosa_bridging_internal.c cosa_x_cisco_com_mld_internal.c cosa_time_internal.c cosa_moca_dml.c cosa_nat_internal.c cosa_hosts_internal.c cosa_dhcpv6_dml.c cosa_x_cisco_com_mld_dml.c cosa_ip_dml.c cosa_ethernet_internal.c cosa_dns_internal.c cosa_com_cisco_spvtg_ccsp_pam_dml.c cosa_x_cisco_com_security_dml.c cosa_ppp_dml.c cosa_ipv6rd_internal.c cosa_dhcpv6_internal.c cosa_ra_internal.c cosa_x_cisco_com_rlog_internal.c cosa_gatewayinfo_dml.c cosa_deviceinfo_internal.c cosa_time_dml.c cosa_x_cisco_com_multilan_dml.c cosa_x_cisco_com_devicecontrol_dml.c cosa_x_cisco_com_devicecontrol_internal.c cosa_deviceinfo_dml.c cosa_dhcpv4_internal.c cosa_moca_internal.c cosa_users_internal.c cosa_firewall_internal.c cosa_firewall_dml.c cosa_interfacestack_dml.c cosa_userinterface_internal.c cosa_x_comcast_com_parentalcontrol_internal.c cosa_nat_dml_custom.c cosa_x_comcast_com_parentalcontrol_dml.c
libCcspPandM_middle_layer_src_la_LDFLAGS = -lccsp_common
