set(HEADERS
    MyDropSite.h
    MyFileRead.h
    MyMaterialPalette.h
    MySimpleMaterialEditor.h
    MyTextureEditor.h
    MyThumbWheel.h
)

set(SOURCES
    MyDropSite.c++
    MyFileRead.c++
    MyMtlPal.c++
    MySimpleMtlEditor.c++
    MyTextureEd.c++
    MyThumbWhl.c++
)

include_directories (../../../libSoXt/src/mixedMode/)
add_library (InventorWidget STATIC ${HEADERS} ${SOURCES})
target_link_libraries(InventorWidget PUBLIC Inventor InventorXt)
target_include_directories (InventorWidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(componentTest componentTest.c++)
target_link_libraries (componentTest InventorWidget Inventor InventorXt)
install(TARGETS componentTest DESTINATION bin/samples)
