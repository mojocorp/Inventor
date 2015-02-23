TEMPLATE = app
TARGET = ivinfo
CONFIG -= app_bundle
QT += opengl
DESTDIR = ../../../build
DEPENDPATH += .

INCLUDEPATH += ../../../libInventor/database/include \
               ../../../libInventor/interaction/include \
               ../../../libInventor/nodekits/include
LIBS += -L../../../build -lInventor -lSoQt

# Input
SOURCES += ivinfo.cpp
