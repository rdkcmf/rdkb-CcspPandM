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
LDFLAGS += -L$(RDK_FSROOT_PATH)/ti/netdk/src/ti_udhcp
LDFLAGS += -L$(SDK_PATH)/ti/netdk/src/ti_dhcpv6
LDFLAGS += $(ldflags-y) -luipc -lpthread -ldhcp4cApi -lapi_dhcpv4c  -llmapi


LDFLAGS += -lhal_ethsw
LDFLAGS += -lhal_cm
LDFLAGS += -lhal_platform
LDFLAGS += -lhal_mta
LDFLAGS += -lcurl -lm
LDFLAGS += -lticc -lti_sme -lsme
LDFLAGS += -lhal_mso_mgmt

#ifeq ($(CONFIG_CISCO_HOTSPOT), y)
#    CFLAGS += -DCONFIG_CISCO_HOTSPOT
#    INCPATH += $(SDK_PATH)/ti/gw/src
#endif

CFLAGS += -DCONFIG_CISCO_HOTSPOT
INCPATH += $(SDK_PATH)/ti/gw/src

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

ifeq ($(SDK_VERSION), 4.3.0.37)

    # for symbols I2C_TunerHwGet/Send, HAL_FfsSaveCalibrationTableToNVRAM/HAL_FfsRestoreCalibrationTableFromNVRAM
	LDFLAGS += -L$(SDK_PATH)/ti/lib -lall_docsis

	INCPATH += $(SDK_PATH)/ti/docsis/src/common/docsis_mac/docsis_db/include  # x_cisco_com_devicecontrol

else
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

endif

# MOD for SDK 4.3. Intel libraries changed
LDFLAGS += -lcrypto
# MOD END

# for symbols PACM_MSM_GetMsmNumEndPointSetUp and 
# PACM_Prov_GetSwDlRebootFlag/Get|SetMtaDevProvisioningState/SetVoiceReadyForReset/SetMtaDevCfmState/SetSwDlRebootFlag/PopulateErrorOid
# and for symbols SnmpDb_SetEndPointOperatingStatus/SetEsafeDevServiceImpact/GetEndPointAppearsInConfig
ifeq ($(CONFIG_TI_PACM), y) 
  CFLAGS += -DCONFIG_TI_PACM
  LDFLAGS += -lcertlib -lpacm_util -lpacm_mtacontrol_util -lpacm_prov_util 
endif


INCPATH += $(RDK_SOURCE_PATH)/hal/include 
INCPATH += $(RDK_SOURCE_PATH)/CcspLMLite/source
INCPATH += $(RDK_SOURCE_PATH)/CcspPandM/source/Custom
INCPATH += $(RDK_PROJECT_ROOT_PATH)/sdk/arm/fsroot/vgwsdk/ti/netdk/src/ti_udhcp
