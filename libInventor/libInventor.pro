TEMPLATE = lib
TARGET   = Inventor
DESTDIR  = ../build

# don't use system glu on windows since it's old 1.2 version
!win32: CONFIG += opengl

CONFIG -= qt flat

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
}

DEFINES += GLEW_BUILD

linux-g++ {
    CONFIG += x11
    INCLUDEPATH += /usr/include/freetype2/
    LIBS += -lGLU
}

win32 {
    DEFINES += INVENTOR_EXPORTS WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS
    
    INCLUDEPATH += ../3rdparty/freetype/include \
                   ../3rdparty/libjpeg \
                   ../3rdparty/glu/include

    # required libs for OpenGL
    LIBS += -lopengl32 -lUser32 -lGdi32
    LIBS += -L$$OUT_PWD/../build -lglu
}

x11 {
    DEFINES += SB_HAS_X11
}

macx {
    LIBS += -framework ApplicationServices

    INCLUDEPATH += ../3rdparty/freetype/include
}

DEPENDPATH += ../libimage

INCLUDEPATH += ../libimage

INCLUDEPATH+= database/include \
              interaction/include \
              nodekits/include
              
include(database/database.pri)
include(interaction/interaction.pri)
include(nodekits/nodekits.pri)

LIBS += -L$$OUT_PWD/../build -limage -lfreetype
