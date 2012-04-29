TEMPLATE = app
TARGET   = gtest
CONFIG  += opengl
CONFIG  -= app_bundle
DESTDIR  = ../build

LIBS += -L../build -lInventor
linux-g++: LIBS += -lGLU

INCLUDEPATH += . include
INCLUDEPATH += ../libInventor/database/include \
               ../libInventor/interaction/include \
               ../libInventor/nodekits/include

# Google Test Framework
HEADERS += include/gtest/gtest.h
SOURCES += src/gtest-all.cc

SOURCES += main.cpp \
           testVec2f.cpp \
           testBox3f.cpp \
           testCreation.cpp \
           testString.cpp \
           testFile.cpp

