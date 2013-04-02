TEMPLATE = lib
TARGET = 
CONFIG -= app_bundle
QT += opengl

DESTDIR = ../../../build

LIBS += -L../../../build -lInventor

DEPENDPATH += .
INCLUDEPATH += . \
               ../../../libInventor/database/include/ \
               ../../../libInventor/interaction/include/ \
               ../../../libInventor/nodekits/include/
# Input
HEADERS += BarChart.h
SOURCES += BarChart.cpp
