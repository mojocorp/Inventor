set(SOURCES
    ivnorm.c++
    FindNormals.c++
    Faces.c++ 
    Edges.c++
)
add_executable(ivnorm ${SOURCES})
target_link_libraries(ivnorm Inventor)