set(HEADERS
    Background.h
    QuadThing.h
)

set(SOURCES
    QuadThing.c++
    Background.c++
    FlashBackground.c++
)

# intermediate object library to avoid duplicated 'main' symbol.
add_library(qmorf_objects OBJECT ${HEADERS} ${SOURCES})
target_link_libraries(qmorf_objects Inventor)

add_executable(qmorf qmorf.c++)
target_link_libraries(qmorf Inventor InventorXt qmorf_objects)

install(TARGETS qmorf DESTINATION bin/demos)
