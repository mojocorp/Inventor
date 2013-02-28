TEMPLATE = app
TARGET = XtSceneViewer
CONFIG  -= app_bundle
QT += opengl
DESTDIR    = ../../../build
DEFINES += "IVPREFIX="
DEPENDPATH += .
INCLUDEPATH += . \
               ../../../libInventor/database/include/ \
               ../../../libInventor/interaction/include/ \
               ../../../libInventor/nodekits/include/ \
               ../../../libSoXt/include

LIBS += -L../../../build -lInventor -lSoXt
LIBS += -L/usr/X11R6/lib -lXm -lXt -lX11 -lXi
INCLUDEPATH += /usr/X11/include/ /usr/OpenMotif/include/

# Input
HEADERS += SoSceneMenu.h \
           SoSceneViewer.h \
           SvManipList.h
SOURCES += SceneViewer.cpp \
           SoSceneViewer.cpp \
           SvManipList.cpp \
           ../../samples/widgets/MyFileRead.cpp \
           ../../samples/widgets/MyDropSite.cpp
