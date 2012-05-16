TEMPLATE = app
TARGET   = ivtest
CONFIG  += opengl
CONFIG  -= app_bundle
DESTDIR  = ../build

LIBS += -L../build -lInventor
linux-g++: LIBS += -lGLU

INCLUDEPATH += . ../3rdparty/gtest-1.6.0/include
INCLUDEPATH += ../libInventor/database/include \
               ../libInventor/interaction/include \
               ../libInventor/nodekits/include

SOURCES += main.cpp \
           testVec2f.cpp \
           testBox3f.cpp \
           testCreation.cpp \
           testString.cpp \
           testFile.cpp

LIBS += -L../build -lgtest
