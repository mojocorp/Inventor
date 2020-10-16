set(HEADERS
    Edges.h
    Faces.h
    FindNormals.h
)

set(SOURCES
    ivnorm.c++
    FindNormals.c++
    Faces.c++ 
    Edges.c++
)

if(WIN32)
include_directories(../../compat)
endif()

add_definitions(-D_USE_MATH_DEFINES)
add_executable(ivnorm ${HEADERS} ${SOURCES})
target_link_libraries(ivnorm Inventor)
set_target_properties(ivnorm PROPERTIES FOLDER "apps/tools")

install(TARGETS ivnorm DESTINATION bin/tools)
