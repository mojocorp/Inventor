find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
find_package(Motif REQUIRED)

# On Mac OS X, GLX is provided as a separate OpenGL implementation, different
# from the standard OpenGL framework which provides support for GLUT and native
# Mac OS X applications.
if (APPLE AND X11_FOUND)
    find_path (X11_GL_INCLUDE_PATH GL/glx.h ${X11_INC_SEARCH_PATH})
    if (NOT X11_GL_INCLUDE_PATH)
        message (ERROR "Could not find GL/glx.h")
    endif (NOT X11_GL_INCLUDE_PATH)
    include_directories(${X11_GL_INCLUDE_PATH})
endif ()

set(PUBLIC_HEADERS
    include/Inventor/Xt/SoXt.h
    include/Inventor/Xt/SoXtClipboard.h
    include/Inventor/Xt/SoXtComponent.h
    include/Inventor/Xt/SoXtCursors.h
    include/Inventor/Xt/SoXtDirectionalLightEditor.h
    include/Inventor/Xt/SoXtGLWidget.h
    include/Inventor/Xt/SoXtIcons.h
    include/Inventor/Xt/SoXtLightSliderSet.h
    include/Inventor/Xt/SoXtMaterialEditor.h
    include/Inventor/Xt/SoXtMaterialList.h
    include/Inventor/Xt/SoXtMaterialSliderSet.h
    include/Inventor/Xt/SoXtPrintDialog.h
    include/Inventor/Xt/SoXtRenderArea.h
    include/Inventor/Xt/SoXtResource.h
    include/Inventor/Xt/SoXtSliderSet.h
    include/Inventor/Xt/SoXtSliderSetBase.h
    include/Inventor/Xt/SoXtTransformSliderSet.h
    include/Inventor/Xt/devices/SoXtDevice.h
    include/Inventor/Xt/devices/SoXtInputFocus.h
    include/Inventor/Xt/devices/SoXtKeyboard.h
    include/Inventor/Xt/devices/SoXtMouse.h
    include/Inventor/Xt/devices/SoXtSpaceball.h
    include/Inventor/Xt/viewers/SoXtConstrainedViewer.h
    include/Inventor/Xt/viewers/SoXtExaminerViewer.h
    include/Inventor/Xt/viewers/SoXtFlyViewer.h
    include/Inventor/Xt/viewers/SoXtFullViewer.h
    include/Inventor/Xt/viewers/SoXtPlaneViewer.h
    include/Inventor/Xt/viewers/SoXtViewer.h
    include/Inventor/Xt/viewers/SoXtWalkViewer.h
)
set(PRIVATE_HEADERS
    include/SoGLwDrawA.h
    include/SoGLwDrawAP.h
    include/SoGLwMDrawA.h
    include/SoGLwMDrawAP.h
    include/SoXtFloatText.h
    include/SoXtImportInterest.h
    include/SoXtMMSliderMax.h
    include/SoXtMMSliderMin.h
    include/SoXtMinMaxSlider.h
    include/SoXtMultiSlider.h
    include/SoXtMultiSliders.h
    include/SoXtSliderControlPanel.h
    include/SoXtSliderModule.h
    include/SoXtSliderModules.h
    include/SoXtSliderTool.h
    include/_SoXtColorEditor.h
    include/_SoXtColorPatch.h
    include/_SoXtColorSlider.h
    include/_SoXtColorWheel.h
    include/_SoXtSlider.h
    include/_SoXtUIRegion.h
)

set(SOXT_SOURCES
    src/mixedMode/SoFloatCallbackList.h
    src/sgi_visual.h
    src/viewers/Sgm/ThumbWheel.h
    src/viewers/Sgm/ThumbWheelP.h
    src/viewers/SoXtBitmapButton.h
    src/SoGLwDAUtil.c
    src/SoGLwDrawA.c
    src/SoGLwMDrawA.c
    src/sgi_visual.c
    src/viewers/Sgm/ThumbWheel.c
    src/SoXt.c++
    src/SoXtClipboard.c++
    src/SoXtComponent.c++
    src/SoXtGLWidget.c++
    src/SoXtImportInterest.c++
    src/SoXtPrintDlog.c++
    src/SoXtRendrArea.c++
    src/SoXtRsrc.c++
    src/devices/SoXtFocus.c++
    src/devices/SoXtKeybd.c++
    src/devices/SoXtMouse.c++
    src/devices/SoXtSpball.c++
    src/lightEditor/SoXtDirLitEd.c++
    src/mixedMode/SoFloatCBList.c++
    src/mixedMode/SoXtColEd.c++
    src/mixedMode/SoXtColPatch.c++
    src/mixedMode/SoXtColSlider.c++
    src/mixedMode/SoXtColWheel.c++
    src/mixedMode/SoXtSlider.c++
    src/mixedMode/SoXtUIRegion.c++
    src/motif/SoXtCntlPnl.c++
    src/motif/SoXtFltTxt.c++
    src/motif/SoXtLtSldrSet.c++
    src/motif/SoXtMMSldrMax.c++
    src/motif/SoXtMMSldrMin.c++
    src/motif/SoXtMinMax.c++
    src/motif/SoXtMltSldr.c++
    src/motif/SoXtMltSldrs.c++
    src/motif/SoXtMtlSSet.c++
    src/motif/SoXtSSetBase.c++
    src/motif/SoXtSldrMdl.c++
    src/motif/SoXtSldrMdls.c++
    src/motif/SoXtSldrSet.c++
    src/motif/SoXtSldrTl.c++
    src/motif/SoXtTfSldrSet.c++
    src/mtlEdit/SoXtMtlEdit.c++
    src/mtlEdit/SoXtMtlList.c++
    src/viewers/SoXtBitmapButton.c++
    src/viewers/SoXtConstVwr.c++
    src/viewers/SoXtCursors.c++
    src/viewers/SoXtExamVwr.c++
    src/viewers/SoXtFlyVwr.c++
    src/viewers/SoXtFullVwr.c++
    src/viewers/SoXtIcons.c++
    src/viewers/SoXtPlaneVwr.c++
    src/viewers/SoXtViewer.c++
    src/viewers/SoXtWalkVwr.c++
)

if(APPLE)
  add_definitions(-DIVPREFIX="/opt/local/")
elseif(UNIX)
  add_definitions(-DIVPREFIX="/usr")
endif()

# for "Sgm/ThumbWheelP.h"
include_directories(src/viewers)

add_library (InventorXt SHARED ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOXT_SOURCES})
target_link_libraries(InventorXt PUBLIC Inventor OpenGL::GL ${MOTIF_LIBRARIES} ${X11_LIBRARIES} ${X11_Xt_LIB} ${X11_Xi_LIB} ${X11_GL_LIB} PRIVATE GLU)
target_include_directories (InventorXt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${MOTIF_INCLUDE_DIR})

install(TARGETS InventorXt CONFIGURATIONS Debug DESTINATION lib/InventorDebug)
install(TARGETS InventorXt CONFIGURATIONS Release DESTINATION lib)

foreach ( file ${PUBLIC_HEADERS} )
    get_filename_component( dir ${file} DIRECTORY )
    string(REPLACE "include/" "" dir "${dir}")
    install( FILES ${file} DESTINATION include/${dir} )
endforeach()
