find_package(X11 REQUIRED)

set(HEADERS
    MyColorEditor.h
    MyColorPatch.h
    MyColorSlider.h
    MyColorWheel.h
    MyDropSite.h
    MyFileRead.h
    MyFloatCallbackList.h
    MyMaterialPalette.h
    MySimpleMaterialEditor.h
    MySlider.h
    MyTextureEditor.h
    MyThumbWheel.h
    MyUIRegion.h
)

set(SOURCES
    MyColEd.c++
    MyColPatch.c++
    MyColSlider.c++
    MyColWheel.c++
    MyDropSite.c++
    MyFileRead.c++
    MyFloatCBList.c++
    MyMtlPal.c++
    MySimpleMtlEditor.c++
    MySlider.c++
    MyTextureEd.c++
    MyThumbWhl.c++
    MyUIRegion.c++
)

# Some really ugly rules to grab the source from the tree:
set(HF
    MyUIRegion.h
    MyColorEditor.h
    MyColorPatch.h
    MyColorSlider.h
    MyColorWheel.h
    MySlider.h 
)

set(CF
    MyUIRegion.c++
    MyColEd.c++
    MyColPatch.c++
    MyColSlider.c++
    MyColWheel.c++
    MySlider.c++
)

set(C2F
    MyFloatCBList.c++
    MyFloatCallbackList.h 
)

foreach(file ${HF})
    string(REPLACE "My" "_SoXt" src "../../../libSoXt/include/${file}")
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          COMMAND sed -f ./nameScript ${src} > ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          VERBATIM
          DEPENDS ${src}
          COMMENT "Creating ${file}"
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

foreach(file ${CF})
    string(REPLACE "My" "SoXt" src "../../../libSoXt/src/mixedMode/${file}")
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          COMMAND sed -f ./nameScript ${src} > ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          VERBATIM
          DEPENDS ${src}
          COMMENT "Creating ${file}"
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

foreach(file ${C2F})
    string(REPLACE "My" "So" src "../../../libSoXt/src/mixedMode/${file}")
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          COMMAND sed -f ./nameScript ${src} > ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          VERBATIM
          DEPENDS ${src}
          COMMENT "Creating ${file}"
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

include_directories (${X11_INCLUDE_DIR})
add_library (InventorWidget STATIC ${HEADERS} ${SOURCES})
target_link_libraries(InventorWidget PUBLIC Inventor InventorXt)
target_include_directories (InventorWidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(componentTest componentTest.c++)
target_link_libraries (componentTest InventorWidget Inventor InventorXt ${X11_LIBRARIES})