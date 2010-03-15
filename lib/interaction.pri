DEPENDPATH += interaction \
              interaction/include \
              interaction/include/Inventor \
              interaction/src/actions \
              interaction/src/draggers \
              interaction/src/manips \
              interaction/src/nodekits \
              interaction/src/nodes \
              interaction/include/Inventor/actions \
              interaction/include/Inventor/draggers \
              interaction/include/Inventor/manips \
              interaction/include/Inventor/nodekits \
              interaction/include/Inventor/nodes \
              interaction/src/draggers/geom

# Input
PUBLIC_HEADERS += interaction/include/Inventor/SoInteraction.h \
           interaction/include/Inventor/SoSceneManager.h \
           interaction/include/Inventor/actions/SoBoxHighlightRenderAction.h \
           interaction/include/Inventor/actions/SoLineHighlightRenderAction.h \
           interaction/include/Inventor/draggers/SoCenterballDragger.h \
           interaction/include/Inventor/draggers/SoDirectionalLightDragger.h \
           interaction/include/Inventor/draggers/SoDragger.h \
           interaction/include/Inventor/draggers/SoDragPointDragger.h \
           interaction/include/Inventor/draggers/SoHandleBoxDragger.h \
           interaction/include/Inventor/draggers/SoJackDragger.h \
           interaction/include/Inventor/draggers/SoPointLightDragger.h \
           interaction/include/Inventor/draggers/SoRotateCylindricalDragger.h \
           interaction/include/Inventor/draggers/SoRotateDiscDragger.h \
           interaction/include/Inventor/draggers/SoRotateSphericalDragger.h \
           interaction/include/Inventor/draggers/SoScale1Dragger.h \
           interaction/include/Inventor/draggers/SoScale2Dragger.h \
           interaction/include/Inventor/draggers/SoScale2UniformDragger.h \
           interaction/include/Inventor/draggers/SoScaleUniformDragger.h \
           interaction/include/Inventor/draggers/SoSpotLightDragger.h \
           interaction/include/Inventor/draggers/SoTabBoxDragger.h \
           interaction/include/Inventor/draggers/SoTabPlaneDragger.h \
           interaction/include/Inventor/draggers/SoTrackballDragger.h \
           interaction/include/Inventor/draggers/SoTransformBoxDragger.h \
           interaction/include/Inventor/draggers/SoTransformerDragger.h \
           interaction/include/Inventor/draggers/SoTranslate1Dragger.h \
           interaction/include/Inventor/draggers/SoTranslate2Dragger.h \
           interaction/include/Inventor/manips/SoCenterballManip.h \
           interaction/include/Inventor/manips/SoDirectionalLightManip.h \
           interaction/include/Inventor/manips/SoHandleBoxManip.h \
           interaction/include/Inventor/manips/SoJackManip.h \
           interaction/include/Inventor/manips/SoPointLightManip.h \
           interaction/include/Inventor/manips/SoSpotLightManip.h \
           interaction/include/Inventor/manips/SoTabBoxManip.h \
           interaction/include/Inventor/manips/SoTrackballManip.h \
           interaction/include/Inventor/manips/SoTransformBoxManip.h \
           interaction/include/Inventor/manips/SoTransformerManip.h \
           interaction/include/Inventor/manips/SoTransformManip.h \
           interaction/include/Inventor/nodekits/SoInteractionKit.h \
           interaction/include/Inventor/nodes/SoAntiSquish.h \
           interaction/include/Inventor/nodes/SoSelection.h \
           interaction/include/Inventor/nodes/SoSurroundScale.h

PRIVATE_HEADERS += interaction/src/draggers/geom/SoCenterballDraggerGeom.h \
           interaction/src/draggers/geom/SoDirectionalLightDraggerGeom.h \
           interaction/src/draggers/geom/SoDragPointDraggerGeom.h \
           interaction/src/draggers/geom/SoHandleBoxDraggerGeom.h \
           interaction/src/draggers/geom/SoJackDraggerGeom.h \
           interaction/src/draggers/geom/SoPointLightDraggerGeom.h \
           interaction/src/draggers/geom/SoRotateCylindricalDraggerGeom.h \
           interaction/src/draggers/geom/SoRotateDiscDraggerGeom.h \
           interaction/src/draggers/geom/SoRotateSphericalDraggerGeom.h \
           interaction/src/draggers/geom/SoScale1DraggerGeom.h \
           interaction/src/draggers/geom/SoScale2DraggerGeom.h \
           interaction/src/draggers/geom/SoScale2UniformDraggerGeom.h \
           interaction/src/draggers/geom/SoScaleUniformDraggerGeom.h \
           interaction/src/draggers/geom/SoSpotLightDraggerGeom.h \
           interaction/src/draggers/geom/SoTabBoxDraggerGeom.h \
           interaction/src/draggers/geom/SoTabPlaneDraggerGeom.h \
           interaction/src/draggers/geom/SoTrackballDraggerGeom.h \
           interaction/src/draggers/geom/SoTransformBoxDraggerGeom.h \
           interaction/src/draggers/geom/SoTransformerDraggerGeom.h \
           interaction/src/draggers/geom/SoTranslate1DraggerGeom.h \
           interaction/src/draggers/geom/SoTranslate2DraggerGeom.h

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

SOURCES += interaction/src/SoInteraction.cpp \
           interaction/src/SoSceneManager.cpp \
           interaction/src/SoSelection.cpp \
           interaction/src/actions/SoBoxHighlightRenderAction.cpp \
           interaction/src/actions/SoLineHighlightRenderAction.cpp \
           interaction/src/draggers/SoCenterballDragger.cpp \
           interaction/src/draggers/SoDirectionalLightDragger.cpp \
           interaction/src/draggers/SoDragger.cpp \
           interaction/src/draggers/SoDraggerInit.cpp \
           interaction/src/draggers/SoDragPointDragger.cpp \
           interaction/src/draggers/SoHandleBoxDragger.cpp \
           interaction/src/draggers/SoJackDragger.cpp \
           interaction/src/draggers/SoPointLightDragger.cpp \
           interaction/src/draggers/SoRotateCylindricalDragger.cpp \
           interaction/src/draggers/SoRotateDiscDragger.cpp \
           interaction/src/draggers/SoRotateSphericalDragger.cpp \
           interaction/src/draggers/SoScale1Dragger.cpp \
           interaction/src/draggers/SoScale2Dragger.cpp \
           interaction/src/draggers/SoScale2UniformDragger.cpp \
           interaction/src/draggers/SoScaleUniformDragger.cpp \
           interaction/src/draggers/SoSpotLightDragger.cpp \
           interaction/src/draggers/SoTabBoxDragger.cpp \
           interaction/src/draggers/SoTabPlaneDragger.cpp \
           interaction/src/draggers/SoTrackballDragger.cpp \
           interaction/src/draggers/SoTransformBoxDragger.cpp \
           interaction/src/draggers/SoTransformerDragger.cpp \
           interaction/src/draggers/SoTranslate1Dragger.cpp \
           interaction/src/draggers/SoTranslate2Dragger.cpp \
           interaction/src/manips/SoCenterballManip.cpp \
           interaction/src/manips/SoDirectionalLightManip.cpp \
           interaction/src/manips/SoHandleBoxManip.cpp \
           interaction/src/manips/SoJackManip.cpp \
           interaction/src/manips/SoPointLightManip.cpp \
           interaction/src/manips/SoSpotLightManip.cpp \
           interaction/src/manips/SoTabBoxManip.cpp \
           interaction/src/manips/SoTrackballManip.cpp \
           interaction/src/manips/SoTransformerManip.cpp \
           interaction/src/manips/SoTransformManip.cpp \
           interaction/src/manips/SoTransformBoxManip.cpp \
           interaction/src/nodekits/SoInteractionKit.cpp \
           interaction/src/nodes/SoAntiSquish.cpp \
           interaction/src/nodes/SoSurroundScale.cpp
