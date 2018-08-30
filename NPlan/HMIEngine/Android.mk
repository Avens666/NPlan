LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cpp
LOCAL_MODULE    := HMIEngine

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Include \
					$(LOCAL_PATH)/../Interface/ \
					$(LOCAL_PATH)/../Interface/RenderingEngine \
					$(LOCAL_PATH)/../Interface/BaseClass \
					$(LOCAL_PATH)/../Interface/HMIEngine/ \
					$(LOCAL_PATH)/../android/skia/include \
					$(LOCAL_PATH)/../android/skia/include/config \
					$(LOCAL_PATH)/../android/skia/include/core \
					$(LOCAL_PATH)/../android/skia/include/utils \
					$(LOCAL_PATH)/../android/skia/include/effects \
					$(LOCAL_PATH)/../android/skia/include/images \
					$(LOCAL_PATH)/../android/skia/include/lazy \
					$(LOCAL_PATH)/../android/boost/include/boost-1_53  \
					$(LOCAL_PATH)/../TINYXML/include
			
 
					
#$(info $(LOCAL_C_INCLUDES))
					
LOCAL_CFLAGS := -D_UNICODE	-DUNICODE	-DANDROID_NDK -DKOTEI_LINUX -DNDEBUG -DSIGSLOT_USE_POSIX_THREADS \
					-DSK_BUILD_FOR_ANDROID \
				-fsigned-char -fpermissive -w

BASELIB_SOURCE := ../Interface/BaseClass
HMILIB_SOURCE := Src

#baseclass
LOCAL_SRC_FILES += 	 $(BASELIB_SOURCE)/CharSet.cpp \
					 $(BASELIB_SOURCE)/Conversion.cpp \
					 $(BASELIB_SOURCE)/error.cpp \
					 $(BASELIB_SOURCE)/FilePath.cpp \
					 $(BASELIB_SOURCE)/KBaseLib.cpp \
					 $(BASELIB_SOURCE)/KBitHandler.cpp \
					 $(BASELIB_SOURCE)/KComplexFile.cpp \
					 $(BASELIB_SOURCE)/KFile.cpp \
					 $(BASELIB_SOURCE)/KGlobalFunction.cpp \
					 $(BASELIB_SOURCE)/KNFile.cpp \
					 $(BASELIB_SOURCE)/KNRect.cpp \
					 $(BASELIB_SOURCE)/KPackedFile.cpp \
					 $(BASELIB_SOURCE)/KProfile.cpp \
					 $(BASELIB_SOURCE)/KTime.cpp \
					 $(BASELIB_SOURCE)/PolygonFunc.cpp
		


	 
LOCAL_SRC_FILES += 	$(HMILIB_SOURCE)/AnimationThread.cpp \
        $(HMILIB_SOURCE)/Easing.cpp \
        $(HMILIB_SOURCE)/KEasingCurve.cpp \
        $(HMILIB_SOURCE)/KTimer.cpp \
        $(HMILIB_SOURCE)/NUIAnimation.cpp \
		$(HMILIB_SOURCE)/K9PatchImageDrawable.cpp \
		$(HMILIB_SOURCE)/KBitmapTextDrawable.cpp \
		$(HMILIB_SOURCE)/KColorDrawable.cpp \
		$(HMILIB_SOURCE)/KDrawable.cpp \
		$(HMILIB_SOURCE)/KImageDrawable.cpp \
		$(HMILIB_SOURCE)/KShapeDrawable.cpp \
		$(HMILIB_SOURCE)/KTextDrawable.cpp \
		$(HMILIB_SOURCE)/KTextMultiLineDrawable.cpp \
		$(HMILIB_SOURCE)/KFontManager.cpp \
		$(HMILIB_SOURCE)/KSurfaceManager.cpp \
		$(HMILIB_SOURCE)/mainforwindows.cpp \
		$(HMILIB_SOURCE)/mainforlinux.cpp \
		$(HMILIB_SOURCE)/main.cpp \
		$(HMILIB_SOURCE)/message.cpp \
		$(HMILIB_SOURCE)/messageforlinux.cpp \
		$(HMILIB_SOURCE)/DataSync.cpp \
		$(HMILIB_SOURCE)/KLayer.cpp \
		$(HMILIB_SOURCE)/KScreen.cpp \
		$(HMILIB_SOURCE)/KView.cpp \
		$(HMILIB_SOURCE)/KViewGroup.cpp \
		$(HMILIB_SOURCE)/KButtonView.cpp \
		$(HMILIB_SOURCE)/KCheckboxView.cpp \
		$(HMILIB_SOURCE)/KEditView.cpp \
		$(HMILIB_SOURCE)/KGalleryView.cpp \
		$(HMILIB_SOURCE)/KListView.cpp \
		$(HMILIB_SOURCE)/KMenuView.cpp \
		$(HMILIB_SOURCE)/KMoveableView.cpp \
		$(HMILIB_SOURCE)/KProcessView.cpp \
		$(HMILIB_SOURCE)/KScrollView.cpp \
		$(HMILIB_SOURCE)/KSlideView.cpp \
		$(HMILIB_SOURCE)/KSwitchButtonView.cpp \
		$(HMILIB_SOURCE)/KTextView.cpp \
		$(HMILIB_SOURCE)/KTipView.cpp \
		$(HMILIB_SOURCE)/KSvgAnalyzing.cpp \
		$(HMILIB_SOURCE)/KKoteiMessage.cpp  \
		$(HMILIB_SOURCE)/KGestureDetector.cpp
					
					
					
					
LOCAL_CPPFLAGS += -fexceptions -frtti	


include $(BUILD_STATIC_LIBRARY)