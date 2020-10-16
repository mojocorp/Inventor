set(HEADERS
    PlayClass.h
    box.h
    mazes.h
)

set(SOURCES
    maze.c++
)

add_definitions(-DNO_AUDIO)
add_executable(maze ${HEADERS} ${SOURCES})
target_link_libraries(maze Inventor InventorXt)
install(TARGETS maze DESTINATION bin/demos)
