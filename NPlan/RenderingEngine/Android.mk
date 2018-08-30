LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cpp
LOCAL_MODULE    := RenderingEngine

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Include \
					$(LOCAL_PATH)/../Interface/ \
					$(LOCAL_PATH)/../Interface/RenderingEngine \
					$(LOCAL_PATH)/../Interface/BaseClass \
					$(LOCAL_PATH)/../android/skia/include \
					$(LOCAL_PATH)/../android/skia/include/config \
					$(LOCAL_PATH)/../android/skia/include/core \
					$(LOCAL_PATH)/../android/skia/include/utils \
					$(LOCAL_PATH)/../android/skia/include/effects \
					$(LOCAL_PATH)/../android/skia/include/images \
					$(LOCAL_PATH)/../android/skia/include/lazy \
					$(LOCAL_PATH)/../android/boost/include/boost-1_53  
			
 
					
#$(info $(LOCAL_C_INCLUDES))
					
LOCAL_CFLAGS := -D_UNICODE	-DUNICODE	-DANDROID_NDK -DKOTEI_LINUX -DNDEBUG -DSK_BUILD_FOR_ANDROID -fsigned-char -fpermissive -w

BASELIB_SOURCE := Src


#navi engine code
LOCAL_SRC_FILES += 	 $(BASELIB_SOURCE)/REEffectHelper.cpp \
					 $(BASELIB_SOURCE)/RERect.cpp \
					 $(BASELIB_SOURCE)/RESurface.cpp \
					 $(BASELIB_SOURCE)/RESurfaceFactory.cpp \
					 $(BASELIB_SOURCE)/REWindow.cpp 

 


include $(BUILD_STATIC_LIBRARY)