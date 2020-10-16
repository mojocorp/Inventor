add_executable(ivperf ivperf.c++)
target_link_libraries(ivperf Inventor InventorXt BarChart)
install(TARGETS ivperf DESTINATION bin/tools)
