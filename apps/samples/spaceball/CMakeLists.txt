add_executable(spaceball spaceball.c++)
target_link_libraries(spaceball Inventor InventorXt)

add_executable(spballViewer spballViewer.c++)
target_link_libraries(spballViewer Inventor InventorXt)