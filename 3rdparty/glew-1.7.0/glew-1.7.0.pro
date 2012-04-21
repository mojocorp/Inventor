TEMPLATE = lib
TARGET = glew
DESTDIR  = ../../build
CONFIG += opengl static
CONFIG -= qt

DEPENDPATH += . src include/GL
INCLUDEPATH += . include

DEFINES += GLEW_MX GLEW_STATIC
#DEFINES += GLEW_BUILD

# Input
HEADERS += include/GL/glew.h \
           include/GL/glxew.h \
           include/GL/wglew.h
SOURCES += src/glew.c
