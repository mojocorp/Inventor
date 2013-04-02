TEMPLATE = app
TARGET = 
CONFIG -= app_bundle
QT += opengl
DESTDIR = ../../../build

DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../../../libInventor/database/include \
               ../../../libInventor/interaction/include \
               ../../../libInventor/nodekits/include

LIBS += -L../../../build -lInventor

# Input
HEADERS +=
SOURCES += ivToIncludeFile.cpp
