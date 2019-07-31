find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

include_directories(src)

set(HEADERS
    ang/fl.h
    ang/flfreetype.h
    src/flclient.h
)

set(SOURCES
    ang/fl.c
    ang/flclient.c
    ang/flfreetype.c
)

add_library (FLWrapper STATIC ${HEADERS} ${SOURCES})
target_link_libraries(FLWrapper ${FREETYPE_LIBRARIES} ${OPENGL_LIBRARY} )
target_include_directories (FLWrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
