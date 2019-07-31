add_executable(tumble tumble.c++ SceneTumble.c++)
target_link_libraries(tumble Inventor InventorXt)

add_executable(viewer viewer.c++ simpleViewer.c++)
target_link_libraries(viewer Inventor InventorXt)