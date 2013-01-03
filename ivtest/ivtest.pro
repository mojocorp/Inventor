TEMPLATE = app
TARGET   = ivtest
CONFIG  += opengl console
CONFIG  -= app_bundle
DESTDIR  = ../build
DEFINES += GTEST_LINKED_AS_SHARED_LIBRARY

LIBS += -L../build -lInventor
linux-g++: LIBS += -lGLU

INCLUDEPATH += . pdiff ../3rdparty/gtest-1.6.0/include
INCLUDEPATH += ../libInventor/database/include \
               ../libInventor/interaction/include \
               ../libInventor/nodekits/include

HEADERS += pdiff/CompareArgs.h \
           pdiff/LPyramid.h \
           pdiff/Metric.h \
           pdiff/RGBAImage.h

SOURCES += pdiff/CompareArgs.cpp \
           pdiff/LPyramid.cpp \
           pdiff/Metric.cpp \
           pdiff/RGBAImage.cpp

HEADERS += utils.h
SOURCES += main.cpp \
           utils.cpp \
           testUtils.cpp \
           testVec2f.cpp \
           testVec3f.cpp \
           testColor.cpp \
           testBox3f.cpp \
           testIO.cpp \
           testCreation.cpp \
           testMatrix.cpp \
           testName.cpp \
           testString.cpp \
           testEndian.cpp \
           testFile.cpp \
           testFields.cpp \
           testOffscreen.cpp \
           testModels.cpp \
           testImage.cpp

LIBS += -L../build -lgtest
