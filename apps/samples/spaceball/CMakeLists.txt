add_executable(spaceball spaceball.c++)
target_link_libraries(spaceball Inventor InventorXt)
install(TARGETS spaceball DESTINATION bin/samples)

add_executable(spballViewer spballViewer.c++)
target_link_libraries(spballViewer Inventor InventorXt)
install(TARGETS spballViewer DESTINATION bin/samples)
