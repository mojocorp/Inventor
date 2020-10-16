set(HEADERS
    RevClass.h
)

set(SOURCES
    revo.c++
    RevClass.c++
    profile.c++
)

add_executable(revo ${HEADERS} ${SOURCES})
target_link_libraries(revo Inventor InventorXt common)
install(TARGETS revo DESTINATION bin/demos)
