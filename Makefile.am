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
SUBDIRS = source-pc source

lib_LTLIBRARIES=libtr181.la
libtr181_la_LDFLAGS=
libtr181_la_SOURCES=
libtr181_la_DEPENDENCIES= \
	source-pc/TR-181/board_sbapi/libCcspPandM_board_sbapi.la \
	source/TR-181/middle_layer_src/libCcspPandM_middle_layer_src.la \
	source/TR-181/integration_src.shared/libCcspPandM_integration_src_shared.la

libtr181_la_LIBADD=$(libtr181_la_DEPENDENCIES)
