LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cpp
LOCAL_MODULE    := NPlan

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../Interface/ \
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
					$(LOCAL_PATH)/../TINYXML/Include \
					$(LOCAL_PATH)/../NPlan/file \
					$(LOCAL_PATH)/../NPlan/manager \
					$(LOCAL_PATH)/../NPlan/view \
					$(LOCAL_PATH)/../NPlan/
	 
			
 
					
#$(info $(LOCAL_C_INCLUDES))
					
LOCAL_CFLAGS := -D_UNICODE	-DUNICODE	-DANDROID_NDK -DKOTEI_LINUX -DNDEBUG -DSIGSLOT_USE_POSIX_THREADS \
					-DSK_BUILD_FOR_ANDROID \
				-fsigned-char -fpermissive -w



#baseclass
LOCAL_SRC_FILES += 	        file/NGlobleResourcePool.cpp  \
		file/NGlobleTaskBoard.cpp  \
		file/NProjectBaseFile.cpp  \
		file/NProjectBaseInfo.cpp  \
		file/NProjectCalendar.cpp  \
		file/NProjectData.cpp  \
		file/NProjectEvent.cpp  \
		file/NProjectExcelData.cpp  \
		file/NProjectMileStone.cpp  \
		file/NProjectMPPXMLData.cpp  \
		file/NProjectObject.cpp  \
		file/NProjectResActivity.cpp  \
		file/NProjectResGroup.cpp  \
		file/NProjectResource.cpp  \
		file/NprojectResourcePool.cpp  \
		file/NProjectTask.cpp  \
		file/NProjectTaskBoard.cpp  \
		file/NProjectTaskBoardItem.cpp  \
		file/NProjectTaskCard.cpp  \
		file/NProjectWarning.cpp  \
		file/NProjectXLSXMLData.cpp  \
		file/NProjectXMLData.cpp  \
		file/utility.cpp  \
		file/XExcel.cpp  \
		view/Calendar.cpp  \
		view/KListViewEx.cpp  \
		view/MilestoneEditView.cpp  \
		view/NAboutView.cpp  \
		view/NBottomMenuView.cpp  \
		view/NBottomView.cpp  \
		view/NCalendarDayView.cpp  \
		view/NCalendarPage.cpp  \
		view/NDragStaffView.cpp  \
		view/NGridView.cpp  \
		view/NMainView.cpp  \
		view/NMemberSourceView.cpp  \
		view/NMenu.cpp  \
		view/NPlanAppView.cpp  \
		view/NProjectPage.cpp  \
		view/NProjectResourceView.cpp  \
		view/NProjectView.cpp  \
		view/NPublicSourceView.cpp  \
		view/NScaleSilderView.cpp  \
		view/NSilderCursorView.cpp  \
		view/NSlideView.cpp  \
		view/NStaffView.cpp  \
		view/NStoryBoardPage.cpp  \
		view/NTaskListItem.cpp  \
		view/NTaskSourceView.cpp  \
		view/NTimeBarPage.cpp  \
		view/NSettingView.cpp  \
		view/TaskBarView.cpp  \
		view/TaskView.cpp  \
		view/TimeBarView.cpp  \
		view/TimeLine.cpp  \
		view/NNormalSilderView.cpp \
		view/NMessageBoxView.cpp \
		manager/KConfigManager.cpp  \
		manager/KProjectManager.cpp  \
		manager/KStringManager.cpp  \
		NPlan.cpp  \
		NPlan_android.cpp   
 
		
					
					
					
LOCAL_CPPFLAGS += -fexceptions -frtti	


include $(BUILD_STATIC_LIBRARY)