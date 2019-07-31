cmake_minimum_required(VERSION 3.11.4)
project(Inventor)

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
message( STATUS "VERSION: ${INVENTOR_VERSION}")

if (NOT WIN32 AND NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

add_definitions(-DINVENTOR_VERSION="${INVENTOR_VERSION}")

if(APPLE)
  add_definitions(-DIVPREFIX="/opt/local/")
elseif(UNIX)
  add_definitions(-DIVPREFIX="/usr")
endif()


add_subdirectory(tools)
add_subdirectory(libFL)
add_subdirectory(libimage)
add_subdirectory(lib)
add_subdirectory(libSoXt)
add_subdirectory(data)
add_subdirectory(doc)
add_subdirectory(apps)
