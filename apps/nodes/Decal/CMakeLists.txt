add_library (Decal SHARED Decal.h Decal.c++)
target_link_libraries(Decal PUBLIC Inventor)
target_include_directories (Decal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
