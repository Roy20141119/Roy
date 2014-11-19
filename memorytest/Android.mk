LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:=memory.cpp
LOCAL_SHARED_LIBRARIES:=libutils
LOCAL_MODULE_TAGS:=optional
LOCAL_MODULE:=memory
include $(BUILD_EXECUTABLE)