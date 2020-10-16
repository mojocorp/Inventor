add_subdirectory(glad)
add_subdirectory(glu)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "disable shared libs" FORCE)
set(SKIP_INSTALL_ALL ON CACHE BOOL "disable install" FORCE)
add_subdirectory(freetype2)

set_target_properties(glad PROPERTIES FOLDER "3rdparty/glad")
set_target_properties(GLU PROPERTIES FOLDER "3rdparty/GLU")
set_target_properties(freetype PROPERTIES FOLDER "3rdparty/freetype2")
