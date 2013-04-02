TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../../../libInventor/database/include \
               ../../../libInventor/interaction/include \
               ../../../libInventor/nodekits/include
LIBS += -L../../../build -lInventor

# Input
HEADERS += SoAsciiTextV2.h \
           SoEnvironmentV1.h \
           SoFontStyleV2.h \
           SoIndexedTriangleMeshV1.h \
           SoLODV2.h \
           SoPackedColorV2.h \
           SoPickStyleV1.h \
           SoShapeHintsV1.h \
           SoText2V2.h \
           SoText3V2.h \
           SoTextureCoordinatePlaneV1.h \
           SoVertexPropertyV2.h \
           SoWWWAnchorV2.h \
           SoWWWInlineV2.h \
           Util.h
SOURCES += downgradeToV1.cpp \
           downgradeToV2.cpp \
           main.cpp \
           SoAsciiTextV2.cpp \
           SoEnvironmentV1.cpp \
           SoFontStyleV2.cpp \
           SoIndexedTriangleMeshV1.cpp \
           SoLODV2.cpp \
           SoPackedColorV2.cpp \
           SoPickStyleV1.cpp \
           SoShapeHintsV1.cpp \
           SoText2V2.cpp \
           SoText3V2.cpp \
           SoTextureCoordinatePlaneV1.cpp \
           SoVertexPropertyV2.cpp \
           SoWWWAnchorV2.cpp \
           SoWWWInlineV2.cpp
