TEMPLATE = app
TARGET = IvExplorer
QT += opengl
CONFIG -= app_bundle
DESTDIR = ../../../build

ICON = icon.icns
QMAKE_INFO_PLIST=info.plist
RC_FILE = info.rc

DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../../../libSoQt/include
INCLUDEPATH += ../../../libInventor/database/include \
               ../../../libInventor/interaction/include \
               ../../../libInventor/nodekits/include

RESOURCES = IvExplorer.qrc

LIBS += -L../../../build -lInventor -lSoQt

# Input
HEADERS += MainWindow.h \
           QxTreeModel.h \
           QxTableModel.h \
           QxTreeModelItem.h \
           QxFieldItemDelegate.h \
           QRecentFilesMenu.h

SOURCES += main.cpp \
           MainWindow.cpp \
           QxTreeModel.cpp \
           QxTableModel.cpp \
           QxTreeModelItem.cpp \
           QxFieldItemDelegate.cpp \
           QRecentFilesMenu.cpp
