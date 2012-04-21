TEMPLATE = lib
TARGET = glew
DESTDIR  = ../../build
CONFIG += opengl
CONFIG -= qt

DEPENDPATH += . src include/GL
INCLUDEPATH += . include

DEFINES += GLEW_BUILD GLEW_MX

# Input
HEADERS += include/GL/glew.h \
           include/GL/glxew.h \
           include/GL/wglew.h
SOURCES += src/glew.c
