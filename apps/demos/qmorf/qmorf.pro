TEMPLATE = app
TARGET = qmorf
CONFIG -= app_bundle
QT += opengl
DESTDIR = ../../../build
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../../../libInventor/database/include \
               ../../../libInventor/interaction/include \
               ../../../libInventor/nodekits/include \
               ../../../libSoQt/include

LIBS += -L../../../build -lInventor -lSoQt

# Input
HEADERS += Background.h QuadThing.h qmorf.h
SOURCES += Background.cpp \
           FlashBackground.cpp \
           qmorf.cpp \
           QuadThing.cpp
