set(HEADERS
    GetVolumeAction.h
)

set(SOURCES
    printVolume.c++
    GetVolumeAction.c++
)

add_executable(printVolume ${HEADERS} ${SOURCES})
target_link_libraries(printVolume Inventor InventorXt)
install(TARGETS printVolume DESTINATION bin/examples/Toolmaker)
