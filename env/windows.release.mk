BUILD_FLAGS := \
	-mwindows

BUILD_DEPENDENCIES := \
	$(BUILD_DEPENDENCIES) \
	$(_BOOST)/lib/libboost_filesystem-mgw7-mt-x64-1_77.dll \
	$(_BOOST)/lib/libboost_system-mgw7-mt-x64-1_77.dll

LINK_LIBRARIES := \
	$(LINK_LIBRARIES) \
	libboost_filesystem-mgw7-mt-x64-1_77 \
	libboost_system-mgw7-mt-x64-1_77
