include_directories(../../nodes/BarChart)
add_executable(ivperf ivperf.c++)
target_link_libraries(ivperf Inventor InventorXt BarChart)