project(Glad)

find_package(OpenGL REQUIRED)

set(GLAD_HEADERS
    include/glad/gl.h
    include/KHR/khrplatform.h
)

set(GLAD_SOURCES
    src/gl.c
)

if(WIN32)
    set(GLAD_HEADERS ${GLAD_HEADERS} include/glad/wgl.h)
    set(GLAD_SOURCES ${GLAD_SOURCES} src/wgl.c)
endif()

if(UNIX)
    find_package(X11 REQUIRED)
    set(GLAD_HEADERS ${GLAD_HEADERS} include/glad/glx.h)
    set(GLAD_SOURCES ${GLAD_SOURCES} src/glx.c)
    include_directories(${X11_INCLUDE_DIR} ${X11_GL_INCLUDE_PATH})
endif()

add_library(glad STATIC ${GLAD_HEADERS} ${GLAD_SOURCES})
target_link_libraries(glad PRIVATE OpenGL::GL)
target_include_directories(glad PUBLIC include/)

