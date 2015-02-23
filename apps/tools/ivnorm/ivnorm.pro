TEMPLATE = app
TARGET = ivnorm
CONFIG -= app_bundle
QT += opengl
DESTDIR = ../../../build
DEPENDPATH += .

INCLUDEPATH += .
INCLUDEPATH += ../../../libInventor/database/include \
               ../../../libInventor/interaction/include \
               ../../../libInventor/nodekits/include
LIBS += -L../../../build -lInventor -lSoQt

# Input
HEADERS += Edges.h Faces.h FindNormals.h
SOURCES += Edges.cpp Faces.cpp FindNormals.cpp ivnorm.cpp
