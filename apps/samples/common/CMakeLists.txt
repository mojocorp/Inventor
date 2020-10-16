set(HEADERS
    Useful.h
    LineManip.h
    InventorLogo.h
)

set(SOURCES
    Useful.c++
    LineManip.c++
    LineManipHilight.c++
)

add_definitions(-D_USE_MATH_DEFINES)
add_library(common STATIC ${HEADERS} ${SOURCES})
target_link_libraries(common PUBLIC Inventor)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(common PROPERTIES FOLDER "apps/samples")