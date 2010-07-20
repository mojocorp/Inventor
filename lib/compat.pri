DEPENDPATH += compat/include/Inventor/actions \
              compat/include/Inventor/devices \
              compat/include/Inventor/elements \
              compat/include/Inventor/events \
              compat/include/Inventor/misc \
              compat/include/Inventor/nodes \
              compat/include/Inventor/projection \
              compat/include/Inventor/sys \
              compat/include/Inventor/components\stereo \
              compat/include/Inventor/Gui\viewers \
              compat/include/Inventor/Qt\stereo

# Input
HEADERS += compat/include/Inventor/SoCompat.h \
           compat/include/Inventor/actions/SoDistributeAction.h \
           compat/include/Inventor/actions/SoGetPrimitiveCountAction.h \
           compat/include/Inventor/actions/SoNurbsBREPAction.h \
           compat/include/Inventor/actions/SoToVRML2Action.h \
           compat/include/Inventor/actions/SoToVRMLAction.h \
           compat/include/Inventor/devices/SoGLContext.h \
           compat/include/Inventor/elements/SoLogicalViewportElement.h \
           compat/include/Inventor/elements/SoModifyViewVolumeElement.h \
           compat/include/Inventor/elements/SoProjectionElement.h \
           compat/include/Inventor/elements/SoTextureUnitElement.h \
           compat/include/Inventor/misc/SoMemoryObject.h \
           compat/include/Inventor/nodes/SoBackground.h \
           compat/include/Inventor/nodes/SoGradientBackground.h \
           compat/include/Inventor/nodes/SoImage.h \
           compat/include/Inventor/nodes/SoImageBackground.h \
           compat/include/Inventor/nodes/SoProjection.h \
           compat/include/Inventor/nodes/SoTextureUnit.h \
           compat/include/Inventor/nodes/SoShaderProgram.h \
           compat/include/Inventor/nodes/SoShaderObject.h \
           compat/include/Inventor/nodes/SoVertexShader.h \
           compat/include/Inventor/nodes/SoFragmentShader.h \
           compat/include/Inventor/nodes/SoGeometryShader.h \
           compat/include/Inventor/nodes/SoShaderParameter.h \
           compat/include/Inventor/projection/SbProjection.h \
           compat/include/Inventor/projection/SbProjectionCoordinates.h \
           compat/include/Inventor/sys/glew.h \
           compat/include/Inventor/sys/port.h \
           compat/include/Inventor/sys/SoGL.h \
           compat/include/Inventor/sys/SoGLU.h \
           compat/include/Inventor/sys/wglew.h \
           compat/include/Inventor/components/stereo/SoBaseStereo.h \
           compat/include/Inventor/Gui/viewers/SoGuiExaminerViewer.h \
           compat/include/Inventor/Qt/stereo/SoQtStereoDialog.h \
           compat/include/Inventor/Qt/stereo/SoQtAnaglyphStereo.h \
           compat/include/Inventor/Qt/stereo/SoQtHalfScreenStereo.h \
           compat/include/Inventor/Qt/stereo/SoQtInterlacedStereo.h \
           compat/include/Inventor/Qt/stereo/SoQtRawStereo.h

SOURCES += compat/src/SoCompat.cpp \
           compat/src/elements/SoModifyViewVolumeElement.cpp \
           compat/src/elements/SoLogicalViewportElement.cpp \
           compat/src/elements/SoProjectionElement.cpp \
           compat/src/elements/SoTextureUnitElement.cpp \
           compat/src/nodes/SoBackground.cpp \
           compat/src/nodes/SoGradientBackground.cpp \
           compat/src/nodes/SoImageBackground.cpp \
           compat/src/nodes/SoProjection.cpp \
           compat/src/nodes/SoTextureUnit.cpp \
           compat/src/Qt/stereo/SoQtStereoDialog.cpp \
           compat/src/Qt/stereo/SoQtRawStereo.cpp \
           compat/src/Qt/stereo/SoQtAnaglyphStereo.cpp \
           compat/src/Qt/stereo/SoQtHalfScreenStereo.cpp \
           compat/src/Qt/stereo/SoQtInterlacedStereo.cpp \
           compat/src/components/stereo/SoBaseStereo.cpp \
           compat/src/actions/SoDistributeAction.cpp \
           compat/src/actions/SoGetPrimitiveCountAction.cpp \
           compat/src/actions/SoNurbsBREPAction.cpp \
           compat/src/actions/SoToVRML2Action.cpp \
           compat/src/actions/SoToVRMLAction.cpp \
           compat/src/misc/SoMemoryObject.cpp

