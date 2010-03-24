######################################################################
# qmake -spec macx-g++
######################################################################

TEMPLATE = lib
TARGET   = image
DESTDIR  = ../build

DEPENDPATH += . \
              ../3rdparty/libjpeg \

INCLUDEPATH += . \
               ../3rdparty/libjpeg

CONFIG -= qt flat
CONFIG += staticlib

# Input
HEADERS += image.h image-sgi.h image-gif.h image-eps.h
SOURCES += image.cpp image-sgilib.c image-giflib.c image-epslib.c

LIBS += -L$$OUT_PWD/../build -ljpeg
