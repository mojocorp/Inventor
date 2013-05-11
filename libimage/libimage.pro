######################################################################
# qmake -spec macx-g++
######################################################################

TEMPLATE = lib
TARGET   = image
DESTDIR  = ../build

DEPENDPATH += .
INCLUDEPATH += .

CONFIG -= qt flat
CONFIG += staticlib

# Input
HEADERS += image.h image-sgi.h image-eps.h
SOURCES += image.cpp image-sgilib.c image-epslib.c stb_image.c

