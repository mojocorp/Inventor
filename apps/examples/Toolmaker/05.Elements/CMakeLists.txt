set(HEADERS
    TemperatureElement.h
)

set(SOURCES
    TemperatureElement.c++
)

add_library(elements_objects OBJECT ${HEADERS} ${SOURCES})
target_link_libraries(elements_objects Inventor)
