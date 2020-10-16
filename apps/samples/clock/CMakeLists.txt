add_executable(ivclock ivclock.c++)
target_link_libraries(ivclock Inventor InventorXt)
install(TARGETS ivclock DESTINATION bin/samples)
