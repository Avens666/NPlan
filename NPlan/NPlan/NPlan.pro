#-------------------------------------------------
#
# Project created by QtCreator 2012-02-02T14:12:51
#
#-------------------------------------------------

message(Processing BaseLib.pro...)

TARGET = NPlan

include(../qtproject/platform.conf)


TEMPLATE = app
CONFIG += warn_off

contains(DEFINES, QT_HMI):{
QT += core gui
}
else:{
CONFIG -= qt
}


DEFINES += NDEBUG

INCLUDEPATH += ../Interface \
                ../Interface/BaseClass \
                ../Interface/RenderingEngine \
				../Interface/HMIEngine \
                ../TINYXML/Include \                
                ./file \
                ./manager \
                ./view \
                ./ \


HEADERS  += file/NGlobleResourcePool.h  \
			file/NGlobleTaskBoard.h  \
			file/NProjectBaseFile.h  \
			file/NProjectBaseInfo.h  \
			file/NProjectCalendar.h  \
			file/NProjectData.h  \
			file/NProjectDataStruct.h  \
			file/NProjectEvent.h  \
			file/NProjectExcelData.h  \
			file/NProjectMileStone.h  \
			file/NProjectMPPXMLData.h  \
			file/NProjectObject.h  \
			file/NProjectResActivity.h  \
			file/NProjectResGroup.h  \
			file/NProjectResource.h  \
			file/NProjectResourcePool.h  \
			file/NProjectTask.h  \
			file/NProjectTaskBoard.h  \
			file/NProjectTaskBoardItem.h  \
			file/NProjectTaskCard.h  \
			file/NProjectWarning.h  \
			file/NProjectXLSXMLData.h  \
			file/NProjectXMLData.h  \
			file/utility.h  \
			file/XExcel.h  \
			manager/KConfigManager.h  \
			manager/KProjectManager.h  \
			manager/KStringManager.h  \
			NPlan.h  \
			view/Calendar.h  \
			view/MilestoneEditView.h  \
			view/NAboutView.h  \
			view/NBottomMenuView.h  \
			view/NBottomView.h  \
			view/NCalendarDayView.h  \
			view/NCalendarPage.h  \
			view/NDragStaffView.h  \
			view/NGridView.h  \
			view/NMainView.h  \
			view/NMemberSourceView.h  \
			view/NMenu.h  \
			view/NPlanAppView.h  \
			view/NProjectPage.h  \
			view/NProjectResourceView.h  \
			view/NProjectView.h  \
			view/NPublicSourceView.h  \
			view/NScaleSilderView.h  \
			view/NSilderCursorView.h  \
			view/NSlideView.h  \
			view/NStaffView.h  \
			view/NStoryBoardPage.h  \
			view/NTaskListItem.h  \
			view/NTaskSourceView.h  \
			view/NTimeBarPage.h  \
			view/NSettingView.h  \
			view/TaskBarView.h  \
			view/TaskView.h  \
			view/TimeBarView.h  \
			view/TimeLine.h  \
            qtmainwindow.h


SOURCES += \
        file/NGlobleResourcePool.cpp \
        file/NGlobleTaskBoard.cpp \
        file/NProjectBaseFile.cpp \
        file/NProjectBaseInfo.cpp \
        file/NProjectCalendar.cpp \
        file/NProjectData.cpp \
        file/NProjectEvent.cpp \
        file/NProjectExcelData.cpp \
        file/NProjectMileStone.cpp \
        file/NProjectMPPXMLData.cpp \
        file/NProjectObject.cpp \
        file/NProjectResActivity.cpp \
        file/NProjectResGroup.cpp \
        file/NProjectResource.cpp \
        file/NprojectResourcePool.cpp \
        file/NProjectTask.cpp \
        file/NProjectTaskBoard.cpp \
        file/NProjectTaskBoardItem.cpp \
        file/NProjectTaskCard.cpp \
        file/NProjectWarning.cpp \
        file/NProjectXLSXMLData.cpp \
        file/NProjectXMLData.cpp \
        file/utility.cpp \
        file/XExcel.cpp \
        KMD5.cpp \
        manager/KProjectManager.cpp \
        manager/KStringManager.cpp \
        NPlan.cpp \
		NPlan_android.cpp \
		NPlan_ios.cpp \
		NPlan_qt.cpp \
		NPlan_win32.cpp \
#		UserCountSuggestion.cpp \
        view/Calendar.cpp \
        view/KListViewEx.cpp \
        view/MilestoneEditView.cpp \
        view/NAboutView.cpp \
        view/NBottomMenuView.cpp \
        view/NBottomView.cpp \
        view/NCalendarDayView.cpp \
        view/NCalendarPage.cpp \
        view/NDragStaffView.cpp \
        view/NGridView.cpp \
        view/NMainView.cpp \
        view/NMemberSourceView.cpp \
        view/NMenu.cpp \
        view/NMessageBoxView.cpp \
        view/NMindMapView.cpp \
        view/NNormalSilderView.cpp \
        view/NPlanAppView.cpp \
        view/NProjectEditPage.cpp \
        view/NProjectPage.cpp \
        view/NProjectResourceView.cpp \
        view/NProjectView.cpp \
        view/NPromptView.cpp \
        view/NPublicSourceView.cpp \
        view/NResourceGroupView.cpp \
        view/NScaleSilderView.cpp \
        view/NSilderCursorView.cpp \
        view/NSlideView.cpp \
        view/NStaffView.cpp \
        view/NStoryBoardPage.cpp \
        view/NSwicthImagePage.cpp \
        view/NTaskListItem.cpp \
        view/NTaskSourceView.cpp \
        view/NTimeBarPage.cpp \
        view/NSettingView.cpp \
        view/TaskBarView.cpp \
        view/TaskView.cpp \
        view/TimeBarView.cpp \
        view/TimeLine.cpp \
		qtmainwindow.cpp \
		qtmain.cpp 
		
		

		


LIBS += -L$${DESTDIR}  \

DESTDIR = ../qtproject/bin/$${BUILD_CONFIG}
	
CONFIG(debug, debug|release){
    TARGET = $${TARGET}_debug
    message(Processing TARGET_debug...)
}
	
