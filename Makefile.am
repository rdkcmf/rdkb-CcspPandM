SUBDIRS = source-pc source

lib_LTLIBRARIES=libtr181.la
libtr181_la_LDFLAGS=
libtr181_la_SOURCES=
libtr181_la_DEPENDENCIES= \
	source-pc/TR-181/board_sbapi/libCcspPandM_board_sbapi.la \
	source/TR-181/middle_layer_src/libCcspPandM_middle_layer_src.la \
	source/TR-181/integration_src.shared/libCcspPandM_integration_src_shared.la \
	source/PandMSsp/libCcspPandM_PandMSsp.la

libtr181_la_LIBADD=$(libtr181_la_DEPENDENCIES)
