BUILD_DEPENDENCIES := \
	$(BUILD_DEPENDENCIES) \
	$(_BOOST)/lib/libboost_filesystem-mgw7-mt-d-x64-1_77.dll \
	$(_BOOST)/lib/libboost_system-mgw7-mt-d-x64-1_77.dll

LINK_LIBRARIES := \
	$(LINK_LIBRARIES) \
	sfml-graphics-d \
	sfml-audio-d \
	sfml-network-d \
	sfml-window-d \
	sfml-system-d \
	libboost_filesystem-mgw7-mt-d-x64-1_77 \
	libboost_system-mgw7-mt-d-x64-1_77