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
if CCSP_ARCH_ARM
OPT_SRCDIR = source-arm custom
endif

if CCSP_ARCH_PC
OPT_SRCDIR = source-pc
endif

SUBDIRS = $(OPT_SRCDIR) source

lib_LTLIBRARIES=libtr181.la
libtr181_la_SOURCES=
libtr181_la_LDFLAGS=
if CCSP_ARCH_ARM
EXTRA_DEPENDENCIES= \
	source-arm/TR-181/board_ml/libCcspPandM_board_ml.la \
	source-arm/TR-181/board_sbapi/libCcspPandM_board_sbapi.la \
	custom/comcast/source/TR-181/custom_sbapi/libCcspPandM_comcast_sbapi.la \
	custom/comcast/source/TR-181/custom_ml/libCcspPandM_comcast_ml.la
libtr181_la_LDFLAGS += -lhal_moca -lapi_dhcpv4c -lhal_msomgmt -lhal_platform -lhal_ethsw -llmapi -lccsp_common -lcm_mgnt -lcurl
endif

if CCSP_ARCH_PC
EXTRA_DEPENDENCIES= \
	source-pc/TR-181/board_ml/libCcspPandM_board_ml.la \
	source-pc/TR-181/board_sbapi/libCcspPandM_board_sbapi.la
endif

libtr181_la_DEPENDENCIES= \
	$(EXTRA_DEPENDENCIES) \
	source/TR-181/integration_src.shared/libCcspPandM_integration_src_shared.la \
	source/TR-181/middle_layer_src/libCcspPandM_middle_layer_src.la

libtr181_la_LIBADD=$(libtr181_la_DEPENDENCIES)
#libtr181_la_LDFLAGS+=-Wl,--no-undefined -Wl,--no-allow-shlib-undefined
