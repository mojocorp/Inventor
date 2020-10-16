add_executable(twoDisplay example.c++)
target_link_libraries(twoDisplay Inventor InventorXt)
install(TARGETS twoDisplay DESTINATION bin/samples)
