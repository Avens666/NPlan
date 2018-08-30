#-------------------------------------------------
#
# Project created by QtCreator 2012-02-02T14:12:51
#
#-------------------------------------------------

message(Processing BaseLib.pro...)

TARGET = RenderingEngine
include(../qtproject/platform.conf)


TEMPLATE = lib
CONFIG += staticlib warn_off




DEFINES += NDEBUG

INCLUDEPATH += ../Interface \
                ../Interface/BaseClass \
                ../Interface/RenderingEngine \
                ./Include

HEADERS += Include/RESurface.h \
            Include/RESurfaceFactory.h \
            Include/REWindow.h

SOURCES += \
#        Src/glenv.cpp \
        Src/REEffectHelper.cpp \
        Src/RERect.cpp \
        Src/RESurface.cpp \
        Src/RESurfaceFactory.cpp \
        Src/REWindow.cpp \



