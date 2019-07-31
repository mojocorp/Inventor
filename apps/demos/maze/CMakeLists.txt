add_definitions(-DNO_AUDIO)
add_executable(maze maze.c++)
target_link_libraries(maze Inventor InventorXt)