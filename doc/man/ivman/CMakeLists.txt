find_package(X11)

# On Mac OS X, GLX is provided as a separate OpenGL implementation, different
# from the standard OpenGL framework which provides support for GLUT and native
# Mac OS X applications.
if (X11_FOUND)
        if (APPLE)
                find_path (X11_GL_INCLUDE_PATH GL/glx.h ${X11_INC_SEARCH_PATH})
                if (NOT X11_GL_INCLUDE_PATH)
                        message (ERROR "Could not find GL/glx.h")
                        set (X11_FOUND FALSE)
                endif (NOT X11_GL_INCLUDE_PATH)
                set (X11_INCLUDE_DIR ${X11_INCLUDE_DIR} ${X11_GL_INCLUDE_PATH})

                find_library (X11_GL_LIB GL ${X11_LIB_SEARCH_PATH})
                if (NOT X11_GL_LIB)
                        message (ERROR "Could not find libGL.dylib")
                        set (X11_FOUND FALSE)
                endif (NOT X11_GL_LIB)

                find_library (X11_GLU_LIB GLU ${X11_LIB_SEARCH_PATH})
                if (NOT X11_GLU_LIB)
                        message (WARNING "Could not find libGLU.dylib")
                        set (X11_FOUND FALSE)
                endif (NOT X11_GLU_LIB)
        else ()
                set (X11_INCLUDE_DIR ${X11_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
                set (X11_GL_LIB ${OPENGL_gl_LIBRARY})
                set (X11_GLU_LIB ${OPENGL_glu_LIBRARY})
        endif ()
        include_directories (${X11_INCLUDE_DIR})
endif (X11_FOUND)

add_executable(ivman Main.c++ ClassDef.c++ Man.c++ Reader.c++ Writer.c++ BuildIssues.c++)
target_link_libraries(ivman Inventor ${X11_GL_LIB})

