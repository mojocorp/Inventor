TEMPLATE = app
TARGET   = ivtest
CONFIG  += opengl console
CONFIG  -= app_bundle
DESTDIR  = ../build

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
           testBox3f.cpp \
           testCreation.cpp \
           testName.cpp \
           testString.cpp \
           testEndian.cpp \
           testFile.cpp

LIBS += -L../build -lgtest
