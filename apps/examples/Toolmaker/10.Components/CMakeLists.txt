add_executable(tumble tumble.c++ SceneTumble.c++ SceneTumble.h)
target_link_libraries(tumble Inventor InventorXt)
install(TARGETS tumble DESTINATION bin/examples/Toolmaker)

add_executable(viewer viewer.c++ simpleViewer.c++ simpleViewer.h)
target_link_libraries(viewer Inventor InventorXt)
install(TARGETS viewer DESTINATION bin/examples/Toolmaker)
