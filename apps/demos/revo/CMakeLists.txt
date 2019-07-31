set(SOURCES
    revo.c++
    RevClass.c++
    profile.c++
)

add_executable(revo ${SOURCES})
target_link_libraries(revo Inventor InventorXt common)