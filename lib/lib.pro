TEMPLATE = lib
TARGET   = Inventor
DESTDIR  = ../build

CONFIG += opengl
CONFIG -= qt flat

linux-g++ {
    CONFIG += x11
}

win32 {
    DEFINES += INVENTOR_EXPORTS WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS
    INCLUDEPATH += ../3rdparty/win-iconv
    LIBS += -lWs2_32
}

x11 {
    DEFINES += SB_HAS_X11
}

DEPENDPATH += ../3rdparty/freetype \
              ../3rdparty/libjpeg \
              ../libFL \
              ../libimage

INCLUDEPATH += ../libimage

INCLUDEPATH+= database/include \
              interaction/include \
              nodekits/include

include(database.pri)
include(interaction.pri)
include(nodekits.pri)

LIBS += -L$$OUT_PWD/../build -lFLWrapper -limage -ljpeg -lfreetype -liconv
