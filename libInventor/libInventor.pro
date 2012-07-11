TEMPLATE = lib
TARGET   = Inventor
DESTDIR  = ../build

# don't use system glu on windows since it's old 1.2 version
!win32: CONFIG += opengl

CONFIG -= qt flat

DEFINES += GLEW_MX GLEW_STATIC

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
}

linux-g++ {
    CONFIG += x11
    INCLUDEPATH += /usr/include/freetype2/
}

win32 {
    RC_FILE = version.rc

    DEFINES += INVENTOR_EXPORTS WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS
    
    INCLUDEPATH += ../3rdparty/pthreads-w32 \
                   ../3rdparty/freetype/include \
                   ../3rdparty/libjpeg \
                   ../3rdparty/glu-1.3.0/include

    DEFINES += PTW32_STATIC_LIB
    LIBS += -lpthreads-w32 -lWSock32

    # required libs for OpenGL
    LIBS += -lopengl32 -lUser32 -lGdi32
    LIBS += -L$$OUT_PWD/../build -lglu
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
INCLUDEPATH += ../3rdparty/glew-1.7.0/include

INCLUDEPATH+= database/include \
              interaction/include \
              nodekits/include
              
include(database/database.pri)
include(interaction/interaction.pri)
include(nodekits/nodekits.pri)

LIBS += -L$$OUT_PWD/../build -limage -ljpeg -lfreetype -lGLEW
