set(HEADERS
    LinkClasses.h
    LinkEngines.h
)

set(SOURCES
    linkages.c++
    LinkClasses.c++
    LinkEngines.c++
)

add_executable(linkages ${HEADERS} ${SOURCES})
target_link_libraries(linkages Inventor InventorXt)
