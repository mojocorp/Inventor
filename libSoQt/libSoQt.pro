TEMPLATE = lib
TARGET = SoQt
QT += opengl

DESTDIR    = ../build

LIBS += -L../build -lInventor

win32 {
  DEFINES += SOQT_EXPORTS
}

INCLUDEPATH += include
INCLUDEPATH += ../lib/database/include \
               ../lib/interaction/include \
               ../lib/nodekits/include

HEADERS += include/Inventor/Qt/SoQt.h \
           include/Inventor/Qt/SoQtIcons.h \
           include/Inventor/Qt/SoQtThumbWheel.h \
           include/Inventor/Qt/SoQtComponent.h \
           include/Inventor/Qt/SoQtGLWidget.h \
           include/Inventor/Qt/SoQtRenderArea.h \
           include/Inventor/Qt/SoQtCursors.h \
           include/Inventor/Qt/devices/SoQtDevice.h \
           include/Inventor/Qt/devices/SoQtKeyboard.h \
           include/Inventor/Qt/devices/SoQtMouse.h \
           include/Inventor/Qt/viewers/SoQtViewer.h \
           include/Inventor/Qt/viewers/SoQtFullViewer.h \
           include/Inventor/Qt/viewers/SoQtExaminerViewer.h \
           include/Inventor/Qt/viewers/SoQtConstrainedViewer.h \
           include/Inventor/Qt/viewers/SoQtFlyViewer.h \
           include/Inventor/Qt/viewers/SoQtWalkViewer.h \
           include/Inventor/Qt/viewers/SoQtPlaneViewer.h \
           include/Inventor/Qt/viewers/SoQtPreferenceDialog.h


SOURCES += src/SoQt.cpp \
           src/SoQtIcons.cpp \
           src/SoQtThumbWheel.cpp \
           src/SoQtComponent.cpp \
           src/SoQtGLWidget.cpp \
           src/SoQtRenderArea.cpp \
           src/SoQtCursors.cpp \
           src/devices/SoQtMouse.cpp \
           src/devices/SoQtKeyboard.cpp \
           src/viewers/SoQtViewer.cpp \
           src/viewers/SoQtFullViewer.cpp \
           src/viewers/SoQtExaminerViewer.cpp \
           src/viewers/SoQtConstrainedViewer.cpp \
           src/viewers/SoQtFlyViewer.cpp \
           src/viewers/SoQtWalkViewer.cpp \
           src/viewers/SoQtPlaneViewer.cpp \
           src/viewers/SoQtPreferenceDialog.cpp

RESOURCES = resources.qrc

FORMS += \
    src/viewers/SoQtPreferenceDialog.ui






