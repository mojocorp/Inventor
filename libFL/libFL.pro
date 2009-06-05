######################################################################
# qmake -spec macx-g++/linux-g++
######################################################################

TEMPLATE = lib
TARGET   = FLWrapper
DESTDIR  = ../build

DEPENDPATH += . ang src
INCLUDEPATH += . ang src

CONFIG -= qt flat
CONFIG += staticlib

QMAKE_LFLAGS += --whole-archive

DEPENDPATH  += ../3rdparty/freetype/include
INCLUDEPATH += ../3rdparty/freetype/include
LIBS += -L$$OUT_PWD/../build -lfreetype

macx {
    LIBS += -framework OpenGL
}


# Input
HEADERS += ang/fl.h ang/flfreetype.h src/flclient.h
SOURCES += ang/fl.c ang/flclient.c ang/flfreetype.c
