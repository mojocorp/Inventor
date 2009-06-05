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
HEADERS += image.h
SOURCES += close.c filbuf.c flsbuf.c name.c open.c pix.c rdwr.c rle.c row.c
