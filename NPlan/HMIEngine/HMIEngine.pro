#-------------------------------------------------
#
# Project created by QtCreator 2012-02-02T14:12:51
#
#-------------------------------------------------

message(Processing BaseLib.pro...)

TARGET = HMIEngine
include(../qtproject/platform.conf)


TEMPLATE = lib
CONFIG += staticlib warn_off
#CONFIG -= qt

DEFINES += NDEBUG

INCLUDEPATH += ../Interface \
                ../Interface/BaseClass \
                ../Interface/RenderingEngine \
				../Interface/HMIEngine \
                ../TINYXML/Include \
                ./Include

SOURCES += \
        ../Interface/BaseClass/CharSet.cpp \
		../Interface/BaseClass/Conversion.cpp \
		../Interface/BaseClass/error.cpp \
		../Interface/BaseClass/FilePath.cpp \
		../Interface/BaseClass/KBaseLib.cpp \
		../Interface/BaseClass/KBitHandler.cpp \
		../Interface/BaseClass/KComplexFile.cpp \
		../Interface/BaseClass/KFile.cpp \
		../Interface/BaseClass/KGlobalFunction.cpp \
		../Interface/BaseClass/KNFile.cpp \
		../Interface/BaseClass/KNRect.cpp \
		../Interface/BaseClass/KPackedFile.cpp \
		../Interface/BaseClass/KProfile.cpp \
		../Interface/BaseClass/KTime.cpp \
		../Interface/BaseClass/PolygonFunc.cpp \
		Src\AnimationThread.cpp \
		Src\DataSync.cpp \
		Src\Easing.cpp \
		Src\K9PatchImageDrawable.cpp \
		Src\KBitmapTextDrawable.cpp \
		Src\KButtonView.cpp \
		Src\KCheckboxView.cpp \
		Src\KColorDrawable.cpp \
		Src\KConfigManager.cpp \
		Src\KDrawable.cpp \
		Src\KEasingCurve.cpp \
		Src\KEditView.cpp \
		Src\KFontManager.cpp \
		Src\KGalleryView.cpp \
		Src\KGestureDetector.cpp \
		Src\KHWRjt.cpp \
		Src\KHWRjt_android.cpp \
		Src\KImageDrawable.cpp \
		Src\KKoteiMessage.cpp \
		Src\KLayer.cpp \
		Src\KListView.cpp \
		Src\KMenuView.cpp \
		Src\KMoveableView.cpp \
		Src\KProcessView.cpp \
		Src\KScrollView.cpp \
		Src\KSlideView.cpp \
		Src\KStaticTextView.cpp \
		Src\KStringManager.cpp \
		Src\KSurfaceManager.cpp \
		Src\KSvgAnalyzing.cpp \
		Src\KSwitchButtonView.cpp \
		Src\KTextMultiLineDrawable.cpp \
		Src\KTextView.cpp \
		Src\KTimer.cpp \
		Src\KTipView.cpp \
		Src\KViewGroup.cpp \
		Src\KScreen.cpp \
		Src\KShapeDrawable.cpp \
		Src\KTextDrawable.cpp \
		Src\KView.cpp \
		Src\main.cpp \
		Src\mainforlinux.cpp \
		Src\mainforwindows.cpp \
		Src\message.cpp \
		Src\messageforlinux.cpp \
        Src\messageforwindows.cpp \
        Src\NUIAnimation.cpp



		
