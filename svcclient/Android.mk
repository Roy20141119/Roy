LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:=ZPClient.cpp
LOCAL_SHARED_LIBRARIES:=libutils libbinder
LOCAL_MODULE_TAGS:=optional
LOCAL_MODULE:=libZPClient
LOCAL_PRELINK_MODULE:=false
include $(BUILD_SHARED_LIBRARY)