######################################################################
# Automatically generated by qmake (2.01a) Sat Apr 23 22:07:53 2011
######################################################################

TEMPLATE = app
TARGET = SceneViewerXt
CONFIG  -= app_bundle
QT += opengl
DESTDIR    = ../../../build
DEFINES += "IVPREFIX="
DEPENDPATH += .
INCLUDEPATH += . \
               ../../../lib/database/include/ \
               ../../../lib/interaction/include/ \
               ../../../lib/nodekits/include/ \
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
