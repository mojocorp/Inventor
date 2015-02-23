TEMPLATE = app
TARGET = ivAddVP
CONFIG -= app_bundle
QT += opengl
DESTDIR = ../../../build
DEPENDPATH += .

INCLUDEPATH += ../../../libInventor/database/include \
               ../../../libInventor/interaction/include \
               ../../../libInventor/nodekits/include
INCLUDEPATH += .

LIBS += -L../../../build -lInventor -lSoQt

# Input
SOURCES += ivAddVP.cpp
