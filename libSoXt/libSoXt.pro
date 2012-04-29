TEMPLATE = lib
TARGET = SoXt

QT += opengl

DESTDIR    = ../build
DEFINES += "IVPREFIX="
DEFINES += SB_HAS_X11

INCLUDEPATH += ../libInventor/database/include \
               ../libInventor/interaction/include \
               ../libInventor/nodekits/include

LIBS += -L../build -lInventor
# At link time, use the library order  -lXm -lXt -lX11. (http://www.rahul.net/kenton/faqs/Motif-FAQ.html#302)
LIBS += -L/usr/X11R6/lib -lXm -lXt -lX11 -lXi

INCLUDEPATH += /usr/X11/include/ /usr/OpenMotif/include/

DEPENDPATH += . \
              include \
              src \
              src/mixedMode \
              src/viewers \
              include/Inventor/Xt \
              src/viewers/Sgm \
              include/Inventor/Xt/devices \
              include/Inventor/Xt/viewers
INCLUDEPATH += . src include src/viewers/

# Input
HEADERS += include/_SoXtColorEditor.h \
           include/_SoXtColorPatch.h \
           include/_SoXtColorSlider.h \
           include/_SoXtColorWheel.h \
           include/_SoXtSlider.h \
           include/_SoXtUIRegion.h \
           include/SoGLwDrawA.h \
           include/SoGLwDrawAP.h \
           include/SoGLwMDrawA.h \
           include/SoGLwMDrawAP.h \
           include/SoXtFloatText.h \
           include/SoXtImportInterest.h \
           include/SoXtMinMaxSlider.h \
           include/SoXtMMSliderMax.h \
           include/SoXtMMSliderMin.h \
           include/SoXtMultiSlider.h \
           include/SoXtMultiSliders.h \
           include/SoXtSliderControlPanel.h \
           include/SoXtSliderModule.h \
           include/SoXtSliderModules.h \
           include/SoXtSliderTool.h \
           src/sgi_visual.h \
           src/mixedMode/SoFloatCallbackList.h \
           src/viewers/SoXtBitmapButton.h \
           include/Inventor/Xt/SoXt.h \
           include/Inventor/Xt/SoXtClipboard.h \
           include/Inventor/Xt/SoXtComponent.h \
           include/Inventor/Xt/SoXtCursors.h \
           include/Inventor/Xt/SoXtDirectionalLightEditor.h \
           include/Inventor/Xt/SoXtGLWidget.h \
           include/Inventor/Xt/SoXtIcons.h \
           include/Inventor/Xt/SoXtLightSliderSet.h \
           include/Inventor/Xt/SoXtMaterialEditor.h \
           include/Inventor/Xt/SoXtMaterialList.h \
           include/Inventor/Xt/SoXtMaterialSliderSet.h \
           include/Inventor/Xt/SoXtPrintDialog.h \
           include/Inventor/Xt/SoXtRenderArea.h \
           include/Inventor/Xt/SoXtResource.h \
           include/Inventor/Xt/SoXtSliderSet.h \
           include/Inventor/Xt/SoXtSliderSetBase.h \
           include/Inventor/Xt/SoXtTransformSliderSet.h \
           src/viewers/Sgm/ThumbWheel.h \
           src/viewers/Sgm/ThumbWheelP.h \
           include/Inventor/Xt/devices/SoXtDevice.h \
           include/Inventor/Xt/devices/SoXtInputFocus.h \
           include/Inventor/Xt/devices/SoXtKeyboard.h \
           include/Inventor/Xt/devices/SoXtMouse.h \
           include/Inventor/Xt/devices/SoXtSpaceball.h \
           include/Inventor/Xt/viewers/SoXtConstrainedViewer.h \
           include/Inventor/Xt/viewers/SoXtExaminerViewer.h \
           include/Inventor/Xt/viewers/SoXtFlyViewer.h \
           include/Inventor/Xt/viewers/SoXtFullViewer.h \
           include/Inventor/Xt/viewers/SoXtPlaneViewer.h \
           include/Inventor/Xt/viewers/SoXtViewer.h \
           include/Inventor/Xt/viewers/SoXtWalkViewer.h

SOURCES += src/sgi_visual.c \
           src/SoGLwDAUtil.c \
           src/SoGLwDrawA.c \
           src/SoGLwMDrawA.c \
           src/viewers/Sgm/ThumbWheel.c \
           src/devices/SoXtFocus.c++ \
           src/devices/SoXtKeybd.c++ \
           src/devices/SoXtMouse.c++ \
           src/devices/SoXtSpball.c++ \
           src/lightEditor/SoXtDirLitEd.c++ \
           src/mixedMode/SoFloatCBList.c++ \
           src/mixedMode/SoXtColEd.c++ \
           src/mixedMode/SoXtColPatch.c++ \
           src/mixedMode/SoXtColSlider.c++ \
           src/mixedMode/SoXtColWheel.c++ \
           src/mixedMode/SoXtSlider.c++ \
           src/mixedMode/SoXtUIRegion.c++ \
           src/motif/SoXtCntlPnl.c++ \
           src/motif/SoXtFltTxt.c++ \
           src/motif/SoXtLtSldrSet.c++ \
           src/motif/SoXtMinMax.c++ \
           src/motif/SoXtMltSldr.c++ \
           src/motif/SoXtMltSldrs.c++ \
           src/motif/SoXtMMSldrMax.c++ \
           src/motif/SoXtMMSldrMin.c++ \
           src/motif/SoXtMtlSSet.c++ \
           src/motif/SoXtSldrMdl.c++ \
           src/motif/SoXtSldrMdls.c++ \
           src/motif/SoXtSldrSet.c++ \
           src/motif/SoXtSldrTl.c++ \
           src/motif/SoXtSSetBase.c++ \
           src/motif/SoXtTfSldrSet.c++ \
           src/mtlEdit/SoXtMtlEdit.c++ \
           src/mtlEdit/SoXtMtlList.c++ \
           src/SoXt.c++ \
           src/SoXtClipboard.c++ \
           src/SoXtComponent.c++ \
           src/SoXtGLWidget.c++ \
           src/SoXtImportInterest.c++ \
           src/SoXtPrintDlog.c++ \
           src/SoXtRendrArea.c++ \
           src/SoXtRsrc.c++ \
           src/viewers/SoXtBitmapButton.c++ \
           src/viewers/SoXtConstVwr.c++ \
           src/viewers/SoXtCursors.c++ \
           src/viewers/SoXtExamVwr.c++ \
           src/viewers/SoXtFlyVwr.c++ \
           src/viewers/SoXtFullVwr.c++ \
           src/viewers/SoXtIcons.c++ \
           src/viewers/SoXtPlaneVwr.c++ \
           src/viewers/SoXtViewer.c++ \
           src/viewers/SoXtWalkVwr.c++
