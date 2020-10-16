add_executable(vortex vortex.c++)
target_link_libraries(vortex Inventor InventorXt)
install(TARGETS vortex DESTINATION bin/samples)
