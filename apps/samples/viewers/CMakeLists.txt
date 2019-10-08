find_package(X11 REQUIRED)

include_directories(${X11_INCLUDE_DIR} "../../../libSoXt/src/viewers")
add_executable(viewerTest viewerTest.c++)
target_link_libraries(viewerTest Inventor InventorXt ${X11_LIBRARIES})

