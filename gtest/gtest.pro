TEMPLATE = app
TARGET   = gtest
CONFIG  -= app_bundle
DESTDIR  = ../build

LIBS += -L../build -lInventor

INCLUDEPATH += . include
INCLUDEPATH += ../lib/database/include \
               ../lib/interaction/include \
               ../lib/nodekits/include

# Google Test Framework
HEADERS += include/gtest/gtest.h
SOURCES += src/gtest-all.cc

SOURCES += main.cpp \
           testVec2f.cpp \
           testBox3f.cpp \
           testCreation.cpp \
           testString.cpp

