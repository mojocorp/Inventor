add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
add_library (BarChart SHARED BarChart.h BarChart.c++)
target_link_libraries(BarChart PUBLIC Inventor)
target_include_directories (BarChart PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(BarChart PROPERTIES FOLDER "apps/nodes")

install(TARGETS BarChart DESTINATION bin/nodes)
