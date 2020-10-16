add_executable(constrainManip constrainManip.c++)
target_link_libraries(constrainManip Inventor InventorXt)
install(TARGETS constrainManip DESTINATION bin/samples)
