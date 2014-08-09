AM_CFLAGS = -D_ANSC_LINUX
AM_CFLAGS += -D_ANSC_USER
AM_CFLAGS += -D_ANSC_LITTLE_ENDIAN_
AM_LDFLAGS = -L$(top_builddir)/../CcspCommonLibrary/lib
AM_LDFLAGS += -lccsp_common

AM_CPPFLAGS = -Wall -Werror
ACLOCAL_AMFLAGS = -I m4
hardware_platform = i686-linux-gnu
bin_PROGRAMS = CcspPandMSsp
CcspPandMSsp_CPPFLAGS = -I/var/tmp/pc-rdkb/include/dbus-1.0 -I$(top_builddir)/../CcspCommonLibrary/source/ccsp/custom -I$(top_builddir)/../CcspCommonLibrary/source/ccsp/include -I$(top_builddir)/../CcspCommonLibrary/source/debug_api/include -I$(top_builddir)/../CcspCommonLibrary/source/cosa/include -I$(top_builddir)/../CcspCommonLibrary/source/cosa/include/linux -I$(top_builddir)/../CcspCommonLibrary/source/ccsp/components/include -I$(top_builddir)/../CcspCommonLibrary/source/cosa/package/slap/include -I$(top_builddir)/../hal/include -I$(top_builddir)/source/TR-181/board_sbapi -I$(top_builddir)/../CcspCommonLibrary/source/util_api/http/include -I$(top_builddir)/../CcspCommonLibrary/source/util_api/ansc/include -I$(top_builddir)/source/TR-181/middle_layer_src -I$(top_builddir)/../CcspCommonLibrary/source/ccsp/components/common/MessageBusHelper/include -I$(top_builddir)/../CcspCommonLibrary/source/ccsp/components/common/PoamIrepFolder -I$(top_builddir)/source-pc/TR-181/board_include -I$(top_builddir)/custom/ga/source/TR-181/custom_include -I$(top_builddir)/custom/comcast -I$(top_builddir)/source/TR-181/include $(CPPFLAGS)
CcspPandMSsp_SOURCES = ssp_messagebus_interface.c ssp_main.c ssp_action.c
CcspPandMSsp_LDFLAGS =  -L$(top_builddir)/../CcspCommonLibrary/lib -lccsp_common