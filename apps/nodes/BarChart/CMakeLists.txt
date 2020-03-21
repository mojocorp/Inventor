add_library (BarChart SHARED BarChart.h BarChart.c++)
target_link_libraries(BarChart PUBLIC Inventor)
target_include_directories (BarChart PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
