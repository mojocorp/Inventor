TEMPLATE = app
TARGET = drop
CONFIG -= app_bundle
DESTDIR = ../../../build
QT  += opengl

DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../../../libInventor/database/include \
               ../../../libInventor/interaction/include \
               ../../../libInventor/nodekits/include \
               ../../../libSoQt/include
LIBS += -L../../../build -lInventor -lSoQt
# Input
HEADERS += MotifHelp.h \
           TsBlock.h \
           TsField.h \
           TsPiece.h \
           TsPieces.h \
           TsViewer.h
SOURCES += drop.cpp \
           TsBlock.cpp \
           TsEvents.cpp \
           TsField.cpp \
           TsPiece.cpp \
           TsPieces.cpp \
           TsRemoval.cpp \
           TsTrickle.cpp \
           TsViewer.cpp
