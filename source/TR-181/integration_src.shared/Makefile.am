AM_CFLAGS = -D_ANSC_LINUX
AM_CFLAGS += -D_ANSC_USER
AM_CFLAGS += -D_ANSC_LITTLE_ENDIAN_
AM_CFLAGS += -D_COSA_SIM_
AM_LDFLAGS = -lccsp_common

AM_CPPFLAGS = -Wall -Werror
ACLOCAL_AMFLAGS = -I m4
hardware_platform = i686-linux-gnu

noinst_LTLIBRARIES = libCcspPandM_integration_src_shared.la
libCcspPandM_integration_src_shared_la_CPPFLAGS = -I$(top_srcdir)/source/TR-181/board_sbapi -I$(top_srcdir)/source/TR-181/middle_layer_src -I$(top_srcdir)/source-pc/TR-181/board_include -I$(top_srcdir)/custom/ga/source/TR-181/custom_include -I$(top_srcdir)/custom/comcast -I$(top_srcdir)/source/TR-181/include $(CPPFLAGS)
libCcspPandM_integration_src_shared_la_SOURCES = cosa_ethernet_apis_multilan.c cosa_ip_apis_multilan.c cosa_x_cisco_com_multilan_apis.c
libCcspPandM_integration_src_shared_la_LDFLAGS = -lccsp_common
