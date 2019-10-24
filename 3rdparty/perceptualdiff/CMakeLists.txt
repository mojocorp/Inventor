project(perceptualdiff)

cmake_minimum_required(VERSION 3.1)

set(HEADERS 
    CompareArgs.h
    LPyramid.h
    Metric.h
    RGBAImage.h
)

set(SOURCES
    CompareArgs.cpp
    LPyramid.cpp
    Metric.cpp
    RGBAImage.cpp
)

add_library (pdiff STATIC ${HEADERS} ${SOURCES})
target_include_directories (pdiff PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
