TEMPLATE = lib
TARGET   = Inventor
DESTDIR  = ../build

CONFIG += opengl
CONFIG -= qt flat

debug: DEFINES += DEBUG

linux-g++ {
    CONFIG += x11
}

win32 {
    DEFINES += INVENTOR_EXPORTS WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS
    
    INCLUDEPATH += ../3rdparty/pthreads-w32
    DEFINES += PTW32_STATIC_LIB
    LIBS += -lpthreads-w32 -lWSock32
}

x11 {
    DEFINES += SB_HAS_X11
}

macx {
    LIBS += -framework ApplicationServices
}

DEPENDPATH += ../3rdparty/freetype \
              ../3rdparty/libjpeg \
              ../libFL \
              ../libimage

INCLUDEPATH += ../libimage \
               ../3rdparty/freetype/include

INCLUDEPATH+= database/include \
              interaction/include \
              nodekits/include

DEFINES += GLEW_STATIC GLEW_MX
              
include(database.pri)
include(interaction.pri)
include(nodekits.pri)

LIBS += -L$$OUT_PWD/../build -lFLWrapper -limage -ljpeg -lfreetype 
