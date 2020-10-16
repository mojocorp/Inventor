set(HEADERS
    Glow.h
    Pyramid.h
    Alternate.h
)

set(SOURCES
    newNodes.c++
    Glow.c++
    Pyramid.c++
    Alternate.c++
)

add_executable(newNodes ${HEADERS} ${SOURCES})
target_link_libraries(newNodes Inventor InventorXt)
install(TARGETS newNodes DESTINATION bin/examples/Toolmaker)
