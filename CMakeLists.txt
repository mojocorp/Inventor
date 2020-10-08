cmake_minimum_required(VERSION 3.11.4)
project(Inventor)

find_package(X11)

option(BUILD_DOC "Build documentation" ON)
if(NOT WIN32)
    option(BUILD_APPS "Build applications" ON)
    option(BUILD_EXAMPLES "Build examples" ON)
endif()

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# necessary for linux, no effect on other platforms.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Extract version information from git
find_package(Git REQUIRED)
execute_process(
  COMMAND ${GIT_EXECUTABLE} describe --always --tags
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE INVENTOR_VERSION
  ERROR_QUIET
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "VERSION: ${INVENTOR_VERSION}")

add_definitions(-DINVENTOR_VERSION="${INVENTOR_VERSION}")
add_definitions(-DGL_SILENCE_DEPRECATION)

add_subdirectory(3rdparty)
add_subdirectory(tools)
add_subdirectory(lib)
if (X11_FOUND)
  add_subdirectory(libSoXt)
endif()
add_subdirectory(data)
if (BUILD_DOC)
  add_subdirectory(doc)
endif()
add_subdirectory(apps)

