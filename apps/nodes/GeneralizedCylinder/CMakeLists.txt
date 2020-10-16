set(HEADERS
    GeneralizedCylinder.h
    NurbMaker.h
    Triangulator.h
)

set(SOURCES
    GeneralizedCylinder.c++
    Triangulator.c++
    NurbMaker.c++
)

add_definitions(-D_USE_MATH_DEFINES)
add_library(GeneralizedCylinder SHARED ${HEADERS} ${SOURCES})
target_link_libraries(GeneralizedCylinder PUBLIC Inventor)
target_include_directories (GeneralizedCylinder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(GeneralizedCylinder PROPERTIES FOLDER "apps/nodes")

install(TARGETS GeneralizedCylinder DESTINATION bin/nodes)
