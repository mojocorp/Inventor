TEMPLATE = lib
TARGET   = Inventor
DESTDIR  = ../build

CONFIG += opengl
CONFIG -= qt flat

DEFINES += GLEW_BUILD GLEW_MX

debug: DEFINES += DEBUG

linux-g++ {
    CONFIG += x11
    INCLUDEPATH += /usr/include/freetype2/
}

win32 {
    DEFINES += INVENTOR_EXPORTS WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS
    
    INCLUDEPATH += ../3rdparty/pthreads-w32 \
                   ../3rdparty/freetype/include \
                   ../3rdparty/libjpeg
    DEFINES += PTW32_STATIC_LIB
    LIBS += -lpthreads-w32 -lWSock32
}

x11 {
    DEFINES += SB_HAS_X11
}

macx {
    LIBS += -framework ApplicationServices

    LIBS += -L/usr/X11/lib/
    INCLUDEPATH += /usr/X11/include/ /usr/X11/include/freetype2
}

DEPENDPATH += ../libimage

INCLUDEPATH += ../libimage


INCLUDEPATH+= database/include \
              interaction/include \
              nodekits/include
              
include(database.pri)
include(interaction.pri)
include(nodekits.pri)

LIBS += -L$$OUT_PWD/../build -limage -ljpeg -lfreetype
