add_executable(lod details.c++)
target_link_libraries(lod Inventor InventorXt)
install(TARGETS lod DESTINATION bin/samples)
