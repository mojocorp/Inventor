add_library (Decal SHARED Decal.h Decal.c++)
target_link_libraries(Decal PUBLIC Inventor)
target_include_directories (Decal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(Decal PROPERTIES FOLDER "apps/nodes")

install(TARGETS Decal DESTINATION bin/nodes)
