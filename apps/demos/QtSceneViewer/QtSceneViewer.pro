TEMPLATE = app
TARGET   = SceneViewer
CONFIG  -= app_bundle
QT  += opengl
DESTDIR = ../../../build

win32 {
    RC_FILE = info.rc
}

mac {
    ICON = icon.icns
    QMAKE_INFO_PLIST=info.plist
}

INCLUDEPATH += ../../../libSoQt/include
INCLUDEPATH += ../../../libInventor/database/include \
               ../../../libInventor/interaction/include \
               ../../../libInventor/nodekits/include

LIBS += -L../../../build -lInventor -lSoQt
linux-g++: LIBS += -lGLU

HEADERS += MainWindow.h \
           QRecentFilesMenu.h

SOURCES += main.cpp \
           MainWindow.cpp \
           QRecentFilesMenu.cpp

RESOURCES += resources.qrc
