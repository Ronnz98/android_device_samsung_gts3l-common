LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libsecril_shim
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := libsecril_shim.cpp

LOCAL_VENDOR_MODULE := true
LOCAL_MULTILIB := both

LOCAL_SHARED_LIBRARIES := \
    libutils \
    libcutils \
    liblog \
    libbinder

include $(BUILD_SHARED_LIBRARY)
