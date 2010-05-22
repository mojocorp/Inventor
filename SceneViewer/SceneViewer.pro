TEMPLATE = app
TARGET   = SceneViewer
CONFIG  -= app_bundle
QT  += opengl
DESTDIR = ../build

win32 {
    RC_FILE = info.rc
}

mac {
    ICON = icon.icns
    QMAKE_INFO_PLIST=info.plist
}

INCLUDEPATH += ../libSoQt/include
INCLUDEPATH += ../lib/database/include \
               ../lib/interaction/include \
               ../lib/nodekits/include

LIBS += -L../build -lInventor -lSoQt

HEADERS += MainWindow.h \
           QRecentFilesMenu.h

SOURCES += main.cpp \
           MainWindow.cpp \
           QRecentFilesMenu.cpp

RESOURCES += resources.qrc
