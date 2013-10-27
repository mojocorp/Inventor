TEMPLATE = app
CONFIG -= app_bundle
QT += opengl
DESTDIR = ../../../../build

DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../../../../libInventor/database/include \
               ../../../../libInventor/interaction/include \
               ../../../../libInventor/nodekits/include \
               ../../../../libSoQt/include

LIBS += -L../../../../build -lInventor -lSoQt
