find_package(X11)

# On Mac OS X, GLX is provided as a separate OpenGL implementation, different
# from the standard OpenGL framework which provides support for GLUT and native
# Mac OS X applications.
if (APPLE AND X11_FOUND)
    find_path (X11_GL_INCLUDE_PATH GL/glx.h ${X11_INC_SEARCH_PATH})
    if (NOT X11_GL_INCLUDE_PATH)
        message (ERROR "Could not find GL/glx.h")
        set (X11_FOUND FALSE)
    endif (NOT X11_GL_INCLUDE_PATH)
    include_directories(${X11_GL_INCLUDE_PATH})
endif ()


if(APPLE)
  add_definitions(-DIVPREFIX="/opt/local/")
elseif(UNIX)
  add_definitions(-DIVPREFIX="/usr")
endif()

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    add_definitions(-DDEBUG)
endif ()

if(BUILD_APPS)
    add_subdirectory(converters)
    add_subdirectory(demos)
    add_subdirectory(nodes)
    add_subdirectory(samples)
    add_subdirectory(tools)
endif()
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
