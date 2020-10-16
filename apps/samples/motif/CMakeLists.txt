add_executable(menu menu.c++)
target_link_libraries(menu Inventor InventorXt)
install(TARGETS menu DESTINATION bin/samples)
