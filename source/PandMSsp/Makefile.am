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

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = CcspPandMSsp

CcspPandMSsp_CPPFLAGS = -I$(top_srcdir)/source/TR-181/board_sbapi -I$(top_srcdir)/source/TR-181/middle_layer_src -I$(top_srcdir)/custom/comcast/source/TR-181/custom_include -I$(top_srcdir)/custom/comcast/source/TR-181/custom_ml -I$(top_srcdir)/custom/comcast -I$(top_srcdir)/source/TR-181/include -I$(top_srcdir)/source/Custom $(CPPFLAGS)

if CCSP_ARCH_ARM
AM_CFLAGS += -D_ANSC_LITTLE_ENDIAN_
CcspPandMSsp_CPPFLAGS += -I$(top_srcdir)/source-arm/TR-181/board_include -I$(top_srcdir)/source-arm/TR-181/board_sbapi
endif

if CCSP_ARCH_MIPS
CcspPandMSsp_CPPFLAGS += -I$(top_srcdir)/source-mips/TR-181/board_include -I$(top_srcdir)/source-mips/TR-181/board_sbapi
endif

if CCSP_ARCH_PC
AM_CFLAGS += -D_ANSC_LITTLE_ENDIAN_
CcspPandMSsp_CPPFLAGS += -I$(top_srcdir)/source-pc/TR-181/board_include -I$(top_srcdir)/source-pc/TR-181/board_sbapi
endif

CcspPandMSsp_CFLAGS = $(SYSTEMD_CFLAGS)
CcspPandMSsp_SOURCES = ssp_messagebus_interface.c ssp_main.c ssp_action.c dm_pack_datamodel.c
CcspPandMSsp_LDFLAGS = -lccsp_common -ldl -rdynamic -lwebconfig_framework -lsecure_wrapper $(SYSTEMD_LDFLAGS) -lsyscfg -lpthread
