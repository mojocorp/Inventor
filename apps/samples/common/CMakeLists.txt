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

add_library(common STATIC ${HEADERS} ${SOURCES})
target_link_libraries(common PUBLIC Inventor)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
