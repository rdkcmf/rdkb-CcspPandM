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
#######################################################################
#   Copyright [2014] [Cisco Systems, Inc.]
# 
#   Licensed under the Apache License, Version 2.0 (the \"License\");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
# 
#       http://www.apache.org/licenses/LICENSE-2.0
# 
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an \"AS IS\" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#######################################################################

#
#   component list
#

PNM_BUILD_TR_181=1
PNM_BUILD_TR_135=0
PNM_BUILD_TR_098=0
PNM_BUILD_Xcalibur=0

#
#   platform specific customization
#

include $(SDK_PATH)/.config

LDFLAGS += -L$(SDK_PATH)/ti/netdk/src/uipc
LDFLAGS += -L$(SDK_PATH)/ti/netdk/src/ti_dhcpv6
LDFLAGS += $(ldflags-y) -luipc -lpthread -lapi_dhcpv4c -llmapi

LDFLAGS += -lhal_ethsw
LDFLAGS += -lhal_platform
LDFLAGS += -lhal_mta
LDFLAGS += -lcurl -lm
LDFLAGS += -lticc -lti_sme -lsme

ifeq ($(CONFIG_CISCO_HOTSPOT), y)
    CFLAGS += -DCONFIG_CISCO_HOTSPOT
    INCPATH += $(SDK_PATH)/ti/gw/src
endif

ifeq ($(CONFIG_TI_BBU), y)
  LDFLAGS += -lbbu
  CFLAGS += -DCONFIG_TI_BBU
endif

# MOCA
ifeq ($(CONFIG_SYSTEM_MOCA), y)
    MOCA_LDFLAGS += -lhal_moca
    #MOCA_LDFLAGS += -lmoca_api
    CFLAGS += -DCONFIG_SYSTEM_MOCA
    LDFLAGS += $(MOCA_LDFLAGS)
endif

# UTOPIA
# UTOPIA_INC_DIR = -I$(CCSP_ROOT_DIR)/Utopia/source/utapi/lib -I$(CCSP_ROOT_DIR)/Utopia/source/utctx/lib
# CFLAGS += $(UTOPIA_INC_DIR)
UTOPIA_LDFLAGS = -L$(SDK_PATH)/ti/lib -lutapi -lutctx -lsyscfg -lsysevent -lulog
LDFLAGS += $(UTOPIA_LDFLAGS)

ifeq ($(CONFIG_VENDOR_CUSTOMER_COMCAST), y)
    CFLAGS += -DCONFIG_VENDOR_CUSTOMER_COMCAST
endif

# SDK_VERSION comes from intel_usg
ifeq ($(SDK_VERSION),)
    SDK_VERSION = 4.2
endif

sdk_version := v$(SDK_VERSION)

ifneq (,$(findstring v4.2,$(sdk_version)))
    # intel SDK v4.2
    $(info *** Go with version 4.2 SDK branch ***)

    # not sure of the way too many dependencies on docsis, so all libraies and path are included here RTian 06/06/2014

    TI_API_PREFIX    :=$(SDK_PATH)/ti
    DOCSIS_API_PREFIX := $(TI_API_PREFIX)/docsis

    include $(SDK_PATH)/ti/lib/docsis_ext_interface.make

    INCPATH += $(shell  find $(DOCSIS_API_PREFIX)/src  -name 'include' )

	CM_LDFLAGS += -L$(TI_API_PREFIX)/lib
	CM_LDFLAGS += -lcm_mgnt
	CM_LDFLAGS += -ldschannellistfreqdb
	CM_LDFLAGS += -lgetnextfreq
	CM_LDFLAGS += -lprimary_ds_freq_override_db
	CM_LDFLAGS += -lus_db
	CM_LDFLAGS += -ldocsis_shared_dbs 
	CM_LDFLAGS += -lutils_docsis 
	CM_LDFLAGS += -lticc -lqos_internal_db
	CM_LDFLAGS += -lnvramstorage
	CM_LDFLAGS += -lfccfg
	CM_LDFLAGS += -lti_sme
	CM_LDFLAGS += -lsme
	CM_LDFLAGS += -lchdb_rx 
	CM_LDFLAGS += -lhal_mxl_hrcls_srv
	CM_LDFLAGS += -lhal_tuner_api
	CM_LDFLAGS += -lhal_dsid
	CM_LDFLAGS += -lhal_ucd
	CM_LDFLAGS += -lhalphs
	CM_LDFLAGS += -lhal_ffs_calibration
	CM_LDFLAGS += -lhal_sync
	CM_LDFLAGS += -lhal_reg_access
	CM_LDFLAGS += -lhalcounter
	CM_LDFLAGS += -lhal_mdd
	CM_LDFLAGS += -lhal_ds_channel_control
	CM_LDFLAGS += -lhal_ds_fw_if
	CM_LDFLAGS += -lhal_us_channel_control
	CM_LDFLAGS += -lhal_filter
	CM_LDFLAGS += -lhalqos
	CM_LDFLAGS += -lhal_global
	CM_LDFLAGS += -lhal_phy
	CM_LDFLAGS += -lhal_fw_download
	CM_LDFLAGS += -lhal_ds_calibration
	CM_LDFLAGS += -lhal_oob
	CM_LDFLAGS += -lhal_i2c_if
	CM_LDFLAGS += -lhal_ucd_forwarding
	CM_LDFLAGS += -lhal_mpeg
	CM_LDFLAGS += -lhal_FST_srv
	CM_LDFLAGS += -lhalbpi
	CM_LDFLAGS += -lhal_us_calibration
	CM_LDFLAGS += -lhalrng
	CM_LDFLAGS += -lhal_db
	CM_LDFLAGS += -lcmd_mbox
	CM_LDFLAGS += -ldpp_config
	CM_LDFLAGS += -lmask_lib
	CM_LDFLAGS += -lpompt
	CM_LDFLAGS += -lcmstatus
	CM_LDFLAGS += -lm
	CM_LDFLAGS += -lnvramstorage
	CM_LDFLAGS += -lutils_docsis
	CM_LDFLAGS += -ldocsis_shared_dbs
	CM_LDFLAGS += -lqos_internal_db
	CM_LDFLAGS += -lfccfg
	CM_LDFLAGS += -lticc
	CM_LDFLAGS += -lti_sme
	CM_LDFLAGS += -lsme

    LDFLAGS += $(CM_LDFLAGS)

else
    # v4.3 and above
    $(info *** Go with version 4.2 SDK branch ***)

    # not sure of the way too many dependencies on docsis, so all libraies and path are included here RTian 06/06/2014

    TI_API_PREFIX    :=$(SDK_PATH)/ti
    DOCSIS_API_PREFIX := $(TI_API_PREFIX)/docsis

    include $(SDK_PATH)/ti/lib/docsis_ext_interface.make

    INCPATH += $(shell  find $(DOCSIS_API_PREFIX)/src  -name 'include' )

    CM_LDFLAGS += -L$(TI_API_PREFIX)/lib
# MOD for SDK 4.3. Intel libraries changed
    CM_LDFLAGS += -lall_docsis
    CM_LDFLAGS += -lgetnextfreq
    CM_LDFLAGS += -lticc # -lqos_internal_db
    CM_LDFLAGS += -lti_sme
    CM_LDFLAGS += -lsme
# MOD END

    LDFLAGS += $(CM_LDFLAGS)

    # MOD for SDK 4.3. Intel libraries changed
    LDFLAGS += -lcrypto
    # MOD END

endif

# for symbols PACM_MSM_GetMsmNumEndPointSetUp and 
# PACM_Prov_GetSwDlRebootFlag/Get|SetMtaDevProvisioningState/SetVoiceReadyForReset/SetMtaDevCfmState/SetSwDlRebootFlag/PopulateErrorOid
# and for symbols SnmpDb_SetEndPointOperatingStatus/SetEsafeDevServiceImpact/GetEndPointAppearsInConfig
ifeq ($(CONFIG_TI_PACM), y) 
  CFLAGS += -DCONFIG_TI_PACM
  LDFLAGS += -lkerb -lcertlib -lpacm_util -lpacm_mtacontrol_util -lpacm_sec_util -lpacm_prov_util -lpacm_snmp_util -lpacm_voim
endif

