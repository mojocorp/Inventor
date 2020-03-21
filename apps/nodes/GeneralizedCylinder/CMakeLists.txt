set(SOURCES
    GeneralizedCylinder.c++
    Triangulator.c++
    NurbMaker.c++
)

add_library(GeneralizedCylinder SHARED ${SOURCES})
target_link_libraries(GeneralizedCylinder PUBLIC Inventor)
target_include_directories (GeneralizedCylinder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
