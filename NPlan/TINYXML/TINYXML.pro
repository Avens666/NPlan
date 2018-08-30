#-------------------------------------------------
#
# Project created by QtCreator 2012-02-02T14:12:51
#
#-------------------------------------------------

message(Processing BaseLib.pro...)
TARGET = TINYXML
include(../qtproject/platform.conf)


TEMPLATE = lib
CONFIG += staticlib warn_off
CONFIG -= qt

DEFINES += NDEBUG

INCLUDEPATH += ../Interface \
                ../Interface/BaseClass \
                ../Interface/RenderingEngine \
                ./Include

SOURCES += \
        Src/tinystr.cpp \
        Src/tinyxml.cpp \
        Src/tinyxmlerror.cpp \
        Src/tinyxmlparser.cpp




